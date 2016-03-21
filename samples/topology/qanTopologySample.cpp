/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the Qanava software.
//
// \file	qanStyleSample.cpp
// \author	benoit@qanava.org
// \date	2016 02 09
//-----------------------------------------------------------------------------

// STD headers
#include <cstring>   // std::memcpy on g++5.2 ...

// QuickProperties headers
#include "../../QuickProperties/src/QuickProperties2.h"

// QuickQanava headers
#include "../../src/QuickQanava.h"
#include "../../src/qanProtoSerializer.h"

// Qt headers
#include <QGuiApplication>

// Topology sample headers
#include "./qanTopologySample.h"
#include "./qanImgNode.h"
#include "./topology.pb.h"


#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace qan;

//-----------------------------------------------------------------------------
MainView::MainView( ) :
    QQuickView( ),
    _serializer{ new qan::ProtoSerializer() }
{
    QScopedPointer< qps::AbstractTranslator > translator{ new qps::AbstractTranslator() };
    QuickProperties::initialize( engine(), translator.data() );
    QuickQanava::initialize();
    qmlRegisterType< qan::ImgNode >( "QuickQanava", 2, 0, "AbstractImgNode");
    qmlRegisterType< qan::ImageItem >( "QuickQanava", 2, 0, "ImageItem");

    _serializer->registerNodeOutFunctor( "qan::ImgNode",
                                         []( google::protobuf::Any* anyNodes,
                                         const qan::ProtoSerializer::WeakNode& weakNode,
                                         const qan::ProtoSerializer::ObjectIdMap& objectIdMap  ) {
            std::shared_ptr< qan::ImgNode > imageNode = std::static_pointer_cast< qan::ImgNode >( weakNode.lock() );
            if ( !imageNode )
                return;
            qan::pb::QanImgNode pbImgNode;
            if ( pbImgNode.mutable_base() != nullptr )
                qan::ProtoSerializer::serializeGTpoNodeOut( weakNode,
                                                            *pbImgNode.mutable_base(),
                                                            objectIdMap ); // Serialize base gtpo.pb.GTpoNode
            pbImgNode.set_img_data_size( -1 );
            QBuffer qBuffer{};
            qBuffer.open( QIODevice::WriteOnly );
            if ( imageNode->getImage().save( &qBuffer, "PNG") ) {
                pbImgNode.set_img_data_size( qBuffer.data().size() );
                std::string imageData( qBuffer.data().size(), ' ' );
                std::memcpy( (void*)imageData.data(), qBuffer.data().data(), qBuffer.data().size() );
                pbImgNode.set_img_data( imageData );
            }
            anyNodes->PackFrom( pbImgNode );
            std::cout << "Sleeping for 1s..." << std::endl;
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        } );
    _serializer->registerNodeInFunctor( []( const google::protobuf::Any& anyNode,
                                            qan::ProtoSerializer::Graph& graph,
                                            qan::ProtoSerializer::IdObjectMap& idObjectMap  ) -> qan::ProtoSerializer::WeakNode {
            qan::ProtoSerializer::WeakNode node;
            if ( anyNode.Is< qan::pb::QanImgNode >() ) {
                qan::pb::QanImgNode pbImgNode;
                if ( anyNode.UnpackTo( &pbImgNode ) ) {
                    node = graph.createNode( std::string( "qan::ImgNode" ) );
                    qan::ProtoSerializer::SharedNode ownedNode = node.lock();
                    if ( ownedNode ) {    // Feed the newly created node with PB node data
                        qan::ProtoSerializer::serializeGTpoNodeIn( pbImgNode.base(), node, idObjectMap ); // Serialize base gtpo.pb.GTpoNode
                        int imageDataSize = pbImgNode.img_data_size();
                        QImage image;
                        qan::ImgNode* imgNode = static_cast< qan::ImgNode* >( ownedNode.get() );
                        if ( imgNode != nullptr &&
                             imageDataSize > 0 &&
                             image.loadFromData( (const uchar*)pbImgNode.img_data().data(), imageDataSize, "PNG" ) ) {
                            imgNode->setImage( image );
                        } else
                            std::cerr << "Error while loading image: imageDataSize=" << imageDataSize << std::endl;
                    }
                } else
                    std::cerr << "Protocol Buffer Error while unpacking a qan::pb::QanImgNode" << std::endl;
            }
            return node;
        } );
    engine()->rootContext( )->setContextProperty( "qanSerializer", _serializer.data() );

    setSource( QUrl( "qrc:/main.qml" ) );
    qan::Graph* g = qobject_cast< qan::Graph* >( rootObject( )->findChild< QQuickItem* >( "graph" ) );
    Q_ASSERT( g != nullptr );
}
//-----------------------------------------------------------------------------

int	main( int argc, char** argv )
{
    QGuiApplication app(argc, argv);

    MainView mainView;
    mainView.setResizeMode( QQuickView::SizeRootObjectToView );
    mainView.resize( 1024, 700 );
    mainView.show( );

    return app.exec( );
}

