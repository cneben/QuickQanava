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
// This file is a part of the QuickQanava software library.
//
// \file	qanProtoSerializer.cpp
// \author	benoit@qanava.org
// \date	2016 02 11
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "qpsSerializer.h"

// QuickQanava headers
#include "./qanProtoSerializer.h"

#ifdef _MSC_VER
#pragma warning(disable: 4100)  // Disable unreferenced formal parameter warning for Protocol Buffer generated code
#endif
#include "./quickqanava.pb.h"

// STD headers
#include <fstream>

namespace qan { // ::qan

/* QML Serialization Interface *///--------------------------------------------
void    ProtoSerializer::saveGraphTo( qan::Graph* graph, QString fileName, qan::ProgressNotifier* progress )
{
    if ( fileName.isEmpty() ) {
        qDebug() << "qan::ProtoSerializer::saveGraphTo(): Error: file name is invalid.";
        return;
    }
    if ( graph == nullptr ) {
        qDebug() << "qan::ProtoSerializer::saveGraphTo(" << fileName << "): Error: graph is nullptr.";
        return;
    }
    QUrl url( fileName );
    if ( url.isValid() )
        fileName = url.toLocalFile();
    qDebug() << "qan::ProtoSerializer::saveGraphTo(): fileName=" << fileName;

    std::ofstream os( fileName.toStdString(), std::ios::out | std::ios::trunc | std::ios::binary );
    if ( !os ) {
        std::cerr << "gtpo::ProtoSerializer::serializeOut(): Error: Can't open output stream " << fileName.toStdString() << std::endl;
        return;
    }
    qan::pb::StyleManager pbStyleManager;
    generateObjectIdMap( *graph ); // Force generation of internal graph primitive / ID map before serializing style manager
    saveStyleManager( *graph, graph->styleManager(), pbStyleManager, getObjectIdMap() );

    gtpo::IProgressNotifier* gtpoProgress = progress;
    gtpo::ProgressNotifier voidProgressNotifier;
    if ( gtpoProgress == nullptr )
        gtpoProgress = &voidProgressNotifier;
    progress->beginProgress();
    serializeOut( *graph, os, *gtpoProgress, &pbStyleManager );
    progress->endProgress();
    if ( os.is_open() )
        os.close();
}

void    ProtoSerializer::loadGraphFrom( QString fileName, qan::Graph* graph, qan::ProgressNotifier* progress )
{
    if ( fileName.isEmpty() ) {
        qDebug() << "qan::ProtoSerializer::saveGraphTo(): Error: file name is invalid.";
        return;
    }
    if ( graph == nullptr ) {
        qDebug() << "qan::ProtoSerializer::loadGraphFrom(" << fileName << "): Error: graph is nullptr.";
        return;
    }
    QUrl url( fileName );
    if ( url.isValid() )
        fileName = url.toLocalFile();

    std::ifstream is( fileName.toStdString(), std::ios::in | std::ios::binary );
    if ( !is ) {
        std::cerr << "gtpo::ProtoSerializer::serializeIn(): Error: Can't open input stream " << fileName.toStdString() << std::endl;
        return;
    }
    qan::pb::StyleManager pbStyleManager;
    gtpo::IProgressNotifier* gtpoProgress = progress;
    gtpo::ProgressNotifier voidProgressNotifier;
    if ( gtpoProgress == nullptr )
        gtpoProgress = &voidProgressNotifier;
    progress->beginProgress();
    serializeIn( is, *graph, *gtpoProgress, &pbStyleManager );
    loadStyleManager( pbStyleManager, *graph->getStyleManager(), getIdObjectMap() );
    if ( is.is_open() )
        is.close();
}

auto    ProtoSerializer::saveStyleManager( const qan::Graph& graph,
                                           const qan::StyleManager& styleManager,
                                           qan::pb::StyleManager& pbStyleManager,
                                           const ObjectIdMap& objectIdMap ) -> void
{
    std::unordered_map< qan::Style*, int > styleObjectIdMap;
    try {
        for ( auto& node: graph.getNodes() ) {   // Mapping styles to the node where they are applied
            int nodeId = objectIdMap.at( node.get() );
            styleObjectIdMap.insert( std::make_pair( node->getStyle(), nodeId ) );
        }
        for ( auto& edge: graph.getEdges() ) {   // Mapping styles to the node where they are applied
            int edgeId = objectIdMap.at( edge.get() );
            styleObjectIdMap.insert( std::make_pair( edge->getStyle(), edgeId ) );
        }
    } catch ( std::out_of_range ) {
        std::cerr << "qan::ProtoSerializer::saveStyleManager(): Warning: Out of range access in objectIdMap.";
    }

    qDebug() << "ProtoSerializer::saveStyleManager(): Saving " << styleManager.size() << " styles...";
    pbStyleManager.set_style_count( styleManager.size() );
    int styleId = 0;
    std::unordered_map< qan::Style*, int > styleIdMap;
    for ( auto style : styleManager ) {
        qan::Style* qanStyle = qobject_cast< qan::Style* >( style );
        int id = ++styleId;
        styleIdMap.insert( std::make_pair( qanStyle, id ) );
        if ( qanStyle != nullptr ) {
            qan::pb::Style* pbStyle = pbStyleManager.add_styles( );
            pbStyle->set_id( id );
            pbStyle->set_meta_target( qanStyle->getMetaTarget().toStdString() );
            pbStyle->set_name( qanStyle->getName().toStdString() );
            pbStyle->set_target( qanStyle->getTarget().toStdString() );

            if ( style->inherits( "qan::NodeStyle" ) ) {
                auto nodes = styleObjectIdMap.equal_range( qanStyle );
                for ( auto nodeIter = nodes.first; nodeIter != nodes.second; ++nodeIter )
                    pbStyle->add_node_ids( nodeIter->second );
            } else if ( style->inherits( "qan::EdgeStyle" ) ) {
                auto edges = styleObjectIdMap.equal_range( qanStyle );
                for ( auto edgeIter = edges.first; edgeIter != edges.second; ++edgeIter ) {
                    pbStyle->add_edge_ids( edgeIter->second );
                    std::cerr << "Saving edge Id:" << edgeIter->second << std::endl;
                }
            }
            qps::pb::Properties* pbProperties = pbStyle->mutable_properties();
            qps::ProtoSerializer::serializeOut( *qanStyle, *pbProperties );
        }
    }

    // Serializing default node styles
    std::map< std::string, int > defaultNodeStyle;
    for ( auto nodeStyleIter = styleManager.getDefaultNodeStyles().cbegin();
          nodeStyleIter != styleManager.getDefaultNodeStyles().end();
          ++nodeStyleIter ) {
        std::string target = nodeStyleIter.key().toStdString();
        int styleId = -1;
        try {
            styleId = styleIdMap.at( nodeStyleIter.value() );
        } catch ( std::out_of_range ) { }
        defaultNodeStyle.insert( std::make_pair( target, styleId ) );
    }
    pbStyleManager.mutable_default_node_styles()->insert( defaultNodeStyle.begin(), defaultNodeStyle.end() );

    // Serializing default edge styles
    std::map< std::string, int > defaultEdgeStyle;
    for ( auto edgeStyleIter = styleManager.getDefaultEdgeStyles().cbegin();
          edgeStyleIter != styleManager.getDefaultEdgeStyles().end();
          ++edgeStyleIter ) {
        std::string target = edgeStyleIter.key().toStdString();
        int styleId = -1;
        try {
            styleId = styleIdMap.at( edgeStyleIter.value() );
        } catch ( std::out_of_range ) { }
        defaultEdgeStyle.insert( std::make_pair( target, styleId ) );
    }
    pbStyleManager.mutable_default_edge_styles()->insert( defaultEdgeStyle.begin(), defaultEdgeStyle.end() );
}

auto    ProtoSerializer::loadStyleManager( const qan::pb::StyleManager& pbStyleManager,
                                           qan::StyleManager& styleManager,
                                           const IdObjectMap& idObjectMap ) -> void
{
    std::unordered_map< int, qan::Style* > idStyleMap;
    for ( auto pbStyle : pbStyleManager.styles() ) {
        qan::Style* style = nullptr;
        std::string styleName = pbStyle.name();
        if ( pbStyle.meta_target() == "qan::Node" ) {
            style = styleManager.createNodeStyle( QString::fromStdString( styleName ),
                                                  "qan::Node" );
            if ( style != nullptr ) {
                idStyleMap.insert( std::make_pair( pbStyle.id(), style ) );
                qan::NodeStyle* nodeStyle = static_cast< qan::NodeStyle* >( style );
                for ( auto nodeId : pbStyle.node_ids() ) {
                    try {
                        void* object = idObjectMap.at( nodeId );
                        qan::Node* node = reinterpret_cast< qan::Node* >( object );
                        if ( node != nullptr )
                            node->setStyle( nodeStyle );
                    } catch ( std::out_of_range ) { }
                }
            }
        }
        else if ( pbStyle.meta_target() == "qan::Edge" ) {
            style = styleManager.createEdgeStyle( QString::fromStdString( styleName ),
                                                  "qan::Edge" );
            if ( style != nullptr ) {
                idStyleMap.insert( std::make_pair( pbStyle.id(), style ) );
                qan::EdgeStyle* edgeStyle = static_cast< qan::EdgeStyle* >( style );
                for ( auto edgeId : pbStyle.edge_ids() ) {
                    try {
                        std::cerr << "edgeId=" << edgeId << std::endl;
                        void* object = idObjectMap.at( edgeId );
                        qan::Edge* edge = reinterpret_cast< qan::Edge* >( object );
                        if ( edge != nullptr )
                            edge->setStyle( edgeStyle );
                    } catch ( std::out_of_range ) { std::cerr << "ProtoSerializer::loadStyleManager(): Error: Invalid edge ID." << std::endl; }
                }
            }
        }
        if ( style != nullptr )
            qps::ProtoSerializer::serializeIn( pbStyle.properties(), *style );
    }

    // Serializing default node styles
    for ( auto defaultNodeIter : pbStyleManager.default_node_styles() ) {
        std::string targetName = defaultNodeIter.first;
        int styleId = defaultNodeIter.second;
        qan::NodeStyle* style = nullptr;
        try {
            style = qobject_cast< qan::NodeStyle* >( idStyleMap.at( styleId ) );
        } catch ( std::out_of_range ) { }
        if ( style != nullptr )
            styleManager.setDefaultNodeStyle( QString::fromStdString( targetName ), style );
    }
    for ( auto defaultEdgeIter : pbStyleManager.default_edge_styles() ) {
        std::string targetName = defaultEdgeIter.first;
        int styleId = defaultEdgeIter.second;
        qan::EdgeStyle* style = nullptr;
        try {
            style = qobject_cast< qan::EdgeStyle* >( idStyleMap.at( styleId ) );
        } catch ( std::out_of_range ) { }
        if ( style != nullptr )
            styleManager.setDefaultEdgeStyle( QString::fromStdString( targetName ), style );
    }
}
//-----------------------------------------------------------------------------

} // ::qan

