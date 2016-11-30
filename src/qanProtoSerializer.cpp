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
// \author	benoit@destrat.io
// \date	2016 02 11
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "qpsPbSerializer.h"

// QuickQanava headers
#include "./qanProtoSerializer.h"

#ifdef _MSC_VER
#pragma warning(disable: 4100)  // Disable unreferenced formal parameter warning for Protocol Buffer generated code
#endif
#include "./quickqanava.pb.h"

// STD headers
#include <fstream>

namespace qan { // ::qan


/* Qan ProtoSerializer Object Management *///----------------------------------
ProtoSerializer::ProtoSerializer( QObject* parent )  :
    QObject( parent ),
    _gtpoSerializer( "qan::Node", "qan::Edge", "qan::Group" )
{
    _gtpoSerializer.registerNodeOutFunctor( "qan::Node",
                                            [=]( google::protobuf::Any* anyNodes,
                                                const qan::ProtoSerializer::WeakNode& weakNode,
                                                const qan::ProtoSerializer::ObjectIdMap& objectIdMap  ) {
        std::shared_ptr< qan::Node > qanNode = std::static_pointer_cast< qan::Node >( weakNode.lock() );
        if ( !qanNode )
            return;
        qan::pb::Node pbQanNode;
        serializeQanNodeOut( weakNode, pbQanNode, objectIdMap );
        anyNodes->PackFrom( pbQanNode );
    } );

    _gtpoSerializer.registerNodeInFunctor( [=]( const google::protobuf::Any& anyNode,
                                              qan::ProtoSerializer::Graph& graph,
                                              qan::ProtoSerializer::IdObjectMap& idObjectMap  ) -> qan::ProtoSerializer::WeakNode {
        qan::ProtoSerializer::WeakNode weakNode;
        if ( anyNode.Is< qan::pb::Node >() ) {
            try {
                qan::pb::Node pbQanNode;
                if ( !anyNode.UnpackTo( &pbQanNode ) )
                    throw std::runtime_error( "Error while unpacking protobuf qan::pb::Node" );
                weakNode = graph.createNode( "qan::Node" );
                qan::Node* qanNode = qobject_cast< qan::Node* >( weakNode.lock().get() );
                if ( qanNode == nullptr )
                    throw std::runtime_error( "Creation of a qan::Node fails." );
                serializeQanNodeIn( pbQanNode, weakNode, idObjectMap );
            } catch ( std::exception e ) { std::cerr << "hlg::ProtoSerializer: hlg::Node in-functor error: " << e.what() << std::endl; }
        }
        return weakNode;
    } );
}
//-----------------------------------------------------------------------------

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

    gtpo::IProgressNotifier* gtpoProgress = static_cast< gtpo::IProgressNotifier* >( progress );
    gtpo::ProgressNotifier voidProgressNotifier;
    if ( gtpoProgress == nullptr )
        gtpoProgress = &voidProgressNotifier;
    gtpoProgress->beginProgress();
    try {
        qan::pb::Graph pbGraph;
        serializeOut( *graph, pbGraph, *gtpoProgress );
        if ( !pbGraph.SerializeToOstream( &os) )
            std::cerr << "qan::ProtoSerializer::saveGraphTo(): Protocol Buffer Error while writing to output stream." << std::endl;
    } catch ( ... ) { std::cerr << "gtpo::ProtoSerializer::saveGraphTo(): Protocol Buffer Error while writing to output stream." << std::endl; }

    gtpoProgress->endProgress();
    if ( os.is_open() )
        os.close();
}

auto    ProtoSerializer::serializeOut( const qan::Graph& graph,
                                       qan::pb::Graph& pbGraph,
                                       gtpo::IProgressNotifier& progressNotifier ) -> void
{
    const qan::StyleManager* styleManager = graph.getStyleManager();
    if ( styleManager == nullptr || pbGraph.mutable_style_manager() == nullptr ) {
        std::cerr << "qan::ProtoSerializer::serializeOut(): Error, style manager is nullptr." << std::endl;
        return;
    }
    _gtpoSerializer.generateObjectIdMap( graph ); // Force generation of internal graph primitive / ID map before serializing style manager
    serializeStyleManagerOut( graph, *styleManager, *pbGraph.mutable_style_manager() );
    gtpo::pb::Graph* pbGtpoGraph = pbGraph.mutable_graph();
    if ( pbGtpoGraph != nullptr )
        _gtpoSerializer.serializeOut( graph, *pbGtpoGraph, progressNotifier );
}

void    ProtoSerializer::loadGraphFrom( QString fileName, qan::Graph* graph, qan::ProgressNotifier* progress )
{
    if ( fileName.isEmpty() ) {
        qDebug() << "qan::ProtoSerializer::loadGraphFrom(): Error: file name is invalid.";
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
        std::cerr << "qan::ProtoSerializer::serializeIn(): Error: Can't open input stream " << fileName.toStdString() << std::endl;
        return;
    }
    gtpo::IProgressNotifier* gtpoProgress = static_cast< gtpo::IProgressNotifier* >( progress );
    gtpo::ProgressNotifier voidProgressNotifier;
    if ( gtpoProgress == nullptr )
        gtpoProgress = &voidProgressNotifier;
    gtpoProgress->beginProgress();
    try {
        qan::pb::Graph pbGraph;
        if ( !pbGraph.ParseFromIstream( &is ) )
            throw std::runtime_error( "qan::ProtoSerializer::loadGraphFrom(): Error: Protocol Buffer QAN graph in serialization fails.");
        serializeIn( pbGraph, *graph, *gtpoProgress );
    } catch ( ... ) {
        std::cerr << "qan::ProtoSerializer::loadGraphFrom(): Error: Protocol Buffer QAN graph in serialization fails." << std::endl;
    }
    gtpoProgress->endProgress();
    if ( is.is_open() )
        is.close();
}

auto    ProtoSerializer::serializeIn( const qan::pb::Graph& pbGraph,
                                      qan::Graph& graph,
                                      gtpo::IProgressNotifier& progress ) -> void
{
    progress.beginProgress();
    try {
        qan::StyleManager* styleManager = graph.getStyleManager();
        if ( styleManager == nullptr ) {
            std::cerr << "qan::ProtoSerializer::serializeIn(): Error, style manager is nullptr." << std::endl;
            return;
        }
        _gtpoSerializer.serializeIn( pbGraph.graph(), graph, progress );
        serializeStyleManagerIn( pbGraph.style_manager(), *styleManager );  // Note 20160404: Warning, gtpoSerializer.serializeIn() clear the objectIdMap, style manager should be serialized _after_ topology
    } catch ( ... ) { std::cerr << "qan::ProtoSerializer::serializeIn(): Error while serializing in graph." << std::endl; }
    progress.endProgress();
}

auto    ProtoSerializer::serializeQanNodeOut( const WeakNode& weakNode, qan::pb::Node& pbQanNode, const ObjectIdMap& objectIdMap ) -> void
{
    if ( pbQanNode.mutable_base() != nullptr )
        gtpo::ProtoSerializer< qan::Config >::serializeGTpoNodeOut( weakNode,
                                                                    *pbQanNode.mutable_base(),
                                                                    objectIdMap ); // Serialize base gtpo.pb.GTpoNode
    std::shared_ptr< qan::Node > qanNode = std::static_pointer_cast< qan::Node >( weakNode.lock() );
    /*if ( qanNode != nullptr && qanNode->getStyle() != nullptr ) {
        int styleId = -1;
        try {
            styleId = objectIdMap.at( qanNode->getStyle() );
        } catch ( const std::out_of_range& ) { }
          catch ( ... ) { }
        pbQanNode.set_style_id( styleId );  // Default to -1 if there is a style mapping error
    }*/
}

auto    ProtoSerializer::serializeQanNodeIn(  const qan::pb::Node& pbQanNode, WeakNode& weakNode, IdObjectMap& idObjectMap ) -> void
{
    qan::Node* qanNode = qobject_cast< qan::Node* >( weakNode.lock().get() );
    if ( qanNode != nullptr ) {
        gtpo::ProtoSerializer< qan::Config >::serializeGTpoNodeIn( pbQanNode.base(), weakNode, idObjectMap ); // Serialize base gtpo.pb.GTpoNode

        // Note 20160404: This code should not find any style since style manager is serialized _after_ topology, kept
        // if a node is serialized in from the network...
        /*void* styleObject{ nullptr };
        try {
            if ( pbQanNode.style_id() != -1 )
                styleObject = idObjectMap.at( pbQanNode.style_id() );
        }   catch ( const std::out_of_range& ) { styleObject = nullptr; }
            catch ( ... ) { styleObject = nullptr; }

        if ( styleObject != nullptr ) {
            qan::NodeStyle* nodeStyle = reinterpret_cast< qan::NodeStyle* >( styleObject );
            qanNode->setStyle( nodeStyle );
        }*/
    }
}

auto    ProtoSerializer::serializeStyleManagerOut( const qan::Graph& graph,
                                                   const qan::StyleManager& styleManager,
                                                   qan::pb::StyleManager& pbStyleManager ) -> void
{
    for ( const auto style : styleManager )
        _gtpoSerializer.addObjectId( style );
    const ObjectIdMap& objectIdMap = _gtpoSerializer.getObjectIdMap();

    // Map styles to nodes ids
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

    pbStyleManager.set_style_count( styleManager.size() );
    for ( auto style : styleManager ) {
        qan::Style* qanStyle = qobject_cast< qan::Style* >( style );
        int styleId = -1;
        try {
            styleId = objectIdMap.at( style );
        } catch ( std::out_of_range ) { }
        if ( qanStyle != nullptr ) {
            qan::pb::Style* pbStyle = pbStyleManager.add_styles( );
            pbStyle->set_id( styleId );
            pbStyle->set_meta_target( qanStyle->getMetaTarget().toStdString() );
            pbStyle->set_name( qanStyle->getName().toStdString() );
            pbStyle->set_target( qanStyle->getTarget().toStdString() );

            if ( style->inherits( "qan::NodeStyle" ) ) {
                auto nodes = styleObjectIdMap.equal_range( qanStyle );
                for ( auto nodeIter = nodes.first; nodeIter != nodes.second; ++nodeIter ) {
                    pbStyle->add_node_ids( nodeIter->second );
                }
            } else if ( style->inherits( "qan::EdgeStyle" ) ) {
                auto edges = styleObjectIdMap.equal_range( qanStyle );
                for ( auto edgeIter = edges.first; edgeIter != edges.second; ++edgeIter ) {
                    pbStyle->add_edge_ids( edgeIter->second );
                }
            }
            qps::pb::QtObject* pbProperties = pbStyle->mutable_properties();
            qps::PbSerializer::serializeOut( *qanStyle, *pbProperties );
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
            styleId = objectIdMap.at( nodeStyleIter.value() );
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
            styleId = objectIdMap.at( edgeStyleIter.value() );
        } catch ( std::out_of_range ) { }
        defaultEdgeStyle.insert( std::make_pair( target, styleId ) );
    }
    pbStyleManager.mutable_default_edge_styles()->insert( defaultEdgeStyle.begin(), defaultEdgeStyle.end() );
}

auto    ProtoSerializer::serializeStyleManagerIn( const qan::pb::StyleManager& pbStyleManager,
                                                  qan::StyleManager& styleManager ) -> void
{
    IdObjectMap& idObjectMap = _gtpoSerializer.getIdObjectMap();
    for ( auto pbStyle : pbStyleManager.styles() ) {
        qan::Style* style = nullptr;
        std::string styleName = pbStyle.name();
        style = styleManager.findStyleByName( QString::fromStdString( styleName ) );    // Note 20160404: Existing styles are overwritten (for exemple default styles...)
        if ( style == nullptr ) {
            if ( pbStyle.meta_target() == "qan::Node" ) {
                style = styleManager.createNodeStyle( QString::fromStdString( styleName ),
                                                      "qan::Node" );
                if ( style != nullptr ) {
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
        }
        if ( style != nullptr ) {
            idObjectMap.insert( std::make_pair( pbStyle.id(), style ) );
            qps::PbSerializer::serializeIn( pbStyle.properties(), *style );
        }
    }

    // Serializing default node styles
    for ( auto defaultNodeIter : pbStyleManager.default_node_styles() ) {
        std::string targetName = defaultNodeIter.first;
        int styleId = defaultNodeIter.second;
        qan::NodeStyle* style = nullptr;
        try {
            style = reinterpret_cast< qan::NodeStyle* >( idObjectMap.at( styleId ) );
            if ( style != nullptr )
                styleManager.setDefaultNodeStyle( QString::fromStdString( targetName ), style );
        } catch ( std::out_of_range ) { }
    }
    for ( auto defaultEdgeIter : pbStyleManager.default_edge_styles() ) {
        std::string targetName = defaultEdgeIter.first;
        int styleId = defaultEdgeIter.second;
        qan::EdgeStyle* style = nullptr;
        try {
            style = reinterpret_cast< qan::EdgeStyle* >( idObjectMap.at( styleId ) );
            if ( style != nullptr )
                styleManager.setDefaultEdgeStyle( QString::fromStdString( targetName ), style );
        } catch ( std::out_of_range ) { }
    }
}
//-----------------------------------------------------------------------------

} // ::qan

