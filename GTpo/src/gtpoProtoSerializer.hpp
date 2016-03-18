/*
    The MIT License (MIT)

    Copyright (c) 2016 Benoit AUTHEMAN

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the GTpo software library.
//
// \file	gtpoProtoSerializer.cpp
// \author	benoit@qanava.org
// \date	2016 02 10
//-----------------------------------------------------------------------------

#ifdef GTPO_HAVE_PROTOCOL_BUFFER

#include "gtpo.pb.h"

namespace gtpo { // ::gtpo

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::initProtocolBuffer() -> void
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
}

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::shutDownProtocolBuffer() -> void
{
    google::protobuf::ShutdownProtobufLibrary();
}

template < class GraphConfig, class Notifier >
ProtoSerializer< GraphConfig, Notifier >::ProtoSerializer( std::string nodeDefaultName ,
                                                           std::string edgeDefaultName ) :
    gtpo::Serializer< GraphConfig, Notifier >( ),
    _nodeDefaultName( nodeDefaultName ),
    _edgeDefaultName( edgeDefaultName )
{
    registerNodeOutFunctor( getNodeDefaultName(),
                            [=]( google::protobuf::Any* anyNodes,
                                const WeakNode& weakNode,
                                const ObjectIdMap& objectIdMap ){
            if ( anyNodes == nullptr )
                return;
            SharedNode node = weakNode.lock();
            if ( !node )
                return;
            gtpo::pb::GTpoNode pbNode;
            serializeGTpoNodeOut( weakNode, pbNode, objectIdMap );
            anyNodes->PackFrom( pbNode );
        } );
    registerNodeInFunctor( [=]( const google::protobuf::Any& anyNode,
                               Graph& graph,
                               IdObjectMap& idObjectMap ) -> WeakNode {
            if ( anyNode.Is< gtpo::pb::GTpoNode >() ) {
                gtpo::pb::GTpoNode pbNode;
                if ( anyNode.UnpackTo( &pbNode ) ) {
                    WeakNode weakNode = graph.createNode( getNodeDefaultName() );
                    SharedNode node = weakNode.lock();
                    if ( node ) {    // Feed the newly created node with PB node data
                        serializeGTpoNodeIn( pbNode, weakNode, idObjectMap );
                        return node;
                    }
                }
            }
            return WeakNode();
        } );

    registerEdgeOutFunctor( getEdgeDefaultName(),
                            []( google::protobuf::Any* anyEdges,
                                const WeakEdge& edge,
                                const ObjectIdMap& objectIdMap ){
            if ( anyEdges == nullptr )
                return false;
            gtpo::pb::GTpoEdge pbEdge;
            int srcNodeId = -1;
            int dstNodeId = -1;
            try {
                SharedEdge sharedEdge = edge.lock();
                if ( !sharedEdge )
                    return false;
                int edgeId = -1;
                try {
                    edgeId = objectIdMap.at( sharedEdge.get() );
                } catch ( std::out_of_range ) { }
                pbEdge.set_edge_id( edgeId );
                SharedNode srcNode = sharedEdge->getSrc().lock();
                SharedNode dstNode = sharedEdge->getDst().lock();
                if ( !srcNode || !dstNode )
                    return false;
                srcNodeId = objectIdMap.at( srcNode.get() );
                dstNodeId = objectIdMap.at( dstNode.get() );
                if ( srcNodeId != -1 && dstNodeId != -1 ) {
                    pbEdge.set_src_node_id(srcNodeId);
                    pbEdge.set_dst_node_id(dstNodeId);
                    pbEdge.set_weight( GraphConfig::getEdgeWeight( sharedEdge ) );
                    anyEdges->PackFrom( pbEdge );
                    return true;
                }
            } catch( std::out_of_range ) { return false; }
              catch( std::bad_weak_ptr ) { return false; }
            return false;
        } );

    registerEdgeInFunctor( [=](  const google::protobuf::Any& anyEdge,
                                Graph& graph,
                                IdObjectMap& idObjectMap ) -> void* {
            if ( anyEdge.Is< gtpo::pb::GTpoEdge >() ) {
                gtpo::pb::GTpoEdge pbEdge;
                if ( anyEdge.UnpackTo( &pbEdge ) ) {
                    try {
                        WeakEdge weakEdge;
                        void* sourceObject = idObjectMap.at( pbEdge.src_node_id() );
                        void* destinationObject = idObjectMap.at( pbEdge.dst_node_id() );
                        if ( sourceObject != nullptr && destinationObject != 0 ) {
                            WeakNode source       = reinterpret_cast< typename GraphConfig::Node* >( sourceObject )->shared_from_this();
                            WeakNode destination  = reinterpret_cast< typename GraphConfig::Node* >( destinationObject )->shared_from_this();
                            weakEdge = graph.createEdge( getEdgeDefaultName(), source, destination );
                            if ( !weakEdge.expired() ) {
                                SharedEdge edge = weakEdge.lock();
                                if ( edge ) {
                                    idObjectMap.insert( std::make_pair( pbEdge.edge_id(), edge.get() ) );
                                    GraphConfig::setEdgeWeight( edge, pbEdge.weight() );
                                }
                                return edge.get();  // FIXME 20160213: return shared edge
                            }
                        }
                    } catch (...) { return nullptr; }
                }
            }
            return nullptr;
        } );
}

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::serializeOut( const Graph& graph,
                                                                std::ostream& os,
                                                                Notifier* progress ) -> void
{
    serializeOut( graph, os, progress, nullptr, nullptr );
}

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::serializeOut( const Graph& graph,
                                                                std::ostream& os,
                                                                Notifier* progress,
                                                                const ::google::protobuf::Message* user1,
                                                                const ::google::protobuf::Message* user2 ) -> void
{
    if ( progress != nullptr )
        progress->beginProgress();

    if ( getObjectIdMap().size() == 0 )
        generateObjectIdMap( graph );
    ObjectIdMap& objectIdMap = getObjectIdMap();

    gtpo::pb::GTpoGraph pbGraph;

    int serializedNodeCout = 0;
    for ( auto& node: graph.getNodes() ) {  // Serialize nodes
        if ( !node->isSerializable() )   // Don't serialize control nodes
            continue;
        auto nodeOutFunctor = _nodeOutFunctors.find( node->getClassName() );
        if ( nodeOutFunctor != _nodeOutFunctors.end() ) {
            ( nodeOutFunctor->second )( pbGraph.add_nodes(), node, objectIdMap );
            ++serializedNodeCout;
        } else
            std::cerr << "gtpo::ProtoSerializer::serializeOut(): no out serialization functor available for node class:" << node->getClassName() << std::endl;
    }

    int serializedEdgeCout = 0;
    for ( auto& edge: graph.getEdges() ) {  // Serialize edges
        auto edgeOutFunctor = _edgeOutFunctors.find( edge->getClassName() );
        if ( edgeOutFunctor != _edgeOutFunctors.end() ) {
            if ( ( edgeOutFunctor->second )( pbGraph.add_edges(), edge, objectIdMap ) )
                ++serializedEdgeCout;
        } else
            std::cerr << "gtpo::ProtoSerializer::serializeOut(): no out serialization functor available for edge class:" << edge->getClassName() << std::endl;
    }

    pbGraph.set_node_count( ( int )graph.getNodeCount() );
    pbGraph.set_edge_count( ( int )graph.getEdges().size() );

    if ( user1 != nullptr )
        pbGraph.mutable_user1()->PackFrom( *user1 );
    if ( user2 != nullptr )
        pbGraph.mutable_user2()->PackFrom( *user2 );

    // Write the new address book back to disk.
    if ( !pbGraph.SerializeToOstream( &os) )
        std::cerr << "gtpo::ProtoSerializer::serializeOut(): Protocol Buffer Error while writing to output stream." << std::endl;

    if ( serializedNodeCout != (int)graph.getNodeCount() ) // Report errors
        std::cerr << "gtpo::ProtoSerializer::serializeOut(): Only " << serializedNodeCout << " nodes serialized while there is " << graph.getNodeCount() << " nodes in graph" << std::endl;
    if ( serializedEdgeCout != (int)graph.getEdges().size() )
        std::cerr << "gtpo::ProtoSerializer::serializeOut(): Only " << serializedEdgeCout << " edges serialized while there is " << graph.getEdges().size() << " edges in graph" << std::endl;

    if ( progress != nullptr )
        progress->endProgress();
}

template < class GraphConfig, class Notifier >
void    ProtoSerializer< GraphConfig, Notifier >::serializeGTpoNodeOut( const WeakNode& weakNode, gtpo::pb::GTpoNode& pbNode, const ObjectIdMap& objectIdMap )
{
    if ( objectIdMap.size() == 0 ) {
        std::cerr << "ProtoSerializer<>::serializeGTpoNodeOut(): Warning: Method called with an empty object ID map." << std::endl;
        return;
    }
    SharedNode node = weakNode.lock();
    if ( !node )
        return;
    pbNode.set_label( GraphConfig::getNodeLabel( node ) );
    pbNode.set_x( GraphConfig::getNodeX( node ) );
    pbNode.set_y( GraphConfig::getNodeY( node ) );
    pbNode.set_width( GraphConfig::getNodeWidth( node ) );
    pbNode.set_height( GraphConfig::getNodeHeight( node ) );
    try {
        pbNode.set_node_id( objectIdMap.at( node.get() ) );
    } catch( ... ) { pbNode.set_node_id( -1 ); }
}

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::registerNodeOutFunctor( std::string nodeClassName,
                                                                NodeOutFunctor nodeOutFunctor ) -> void
{
    _nodeOutFunctors.insert( std::make_pair( nodeClassName, nodeOutFunctor ) );
}

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::registerEdgeOutFunctor( std::string edgeClassName,
                                                                                 EdgeOutFunctor edgeOutFunctor ) -> void
{
    _edgeOutFunctors.insert( std::make_pair( edgeClassName, edgeOutFunctor ) );
}

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::generateObjectIdMap( const Graph& graph ) -> ObjectIdMap&
{
    _objectIdMap.clear();
    int id = 0;
    for ( auto& node: graph.getNodes() )
        _objectIdMap.insert( std::make_pair( node.get(), ++id ) );

    for ( auto& edge: graph.getEdges() )
        _objectIdMap.insert( std::make_pair( edge.get(), ++id ) );

    return _objectIdMap;
}

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::serializeIn( std::istream& is,
                                                               Graph& graph,
                                                               Notifier* progress ) -> void
{
    serializeIn< gtpo::pb::GTpoVoid >( is, graph, progress, nullptr );
}

template < class GraphConfig, class Notifier >
template < class User1 >
auto    ProtoSerializer< GraphConfig, Notifier >::serializeIn( std::istream& is,
                                                               Graph& graph,
                                                               Notifier* progress,
                                                               User1* user1 ) -> void
{
    if ( progress != nullptr )
        progress->beginProgress();

    IdObjectMap& idObjectMap = getIdObjectMap();
    idObjectMap.clear();
    int serializedNodeCout = 0;
    int serializedEdgeCout = 0;
    gtpo::pb::GTpoGraph inGraph;
    if ( inGraph.ParseFromIstream( &is ) ) {
        for ( const google::protobuf::Any& anyNode : inGraph.nodes() ) {    // Serializing nodes in
            WeakNode node;
            for ( auto nodeInFunctor : _nodeInFunctors ) {
                node = nodeInFunctor( anyNode, graph, idObjectMap );
                if ( !node.expired() ) {
                    ++serializedNodeCout;
                    break;
                }
            }
            if ( !node.lock() ) {
                std::cerr << "gtpo::ProtoSerializer::serializeIn(): Warning: A Protocol Buffer node has not been serialized in successfuly." << std::endl;
                std::cerr << "\tProtocol Buffer Error:" << anyNode.type_url() << std::endl;
            }
        }
        if ( idObjectMap.size() > 0 ) {   // No need to start edge serialization if node id map is empty...
            for ( const google::protobuf::Any& anyEdge : inGraph.edges() ) {    // Serializing edges in
                bool edgeSerialized = false;
                for ( auto edgeInFunctor : _edgeInFunctors ) {
                    if ( edgeInFunctor( anyEdge, graph, idObjectMap ) ) {
                        edgeSerialized = true;
                        ++serializedEdgeCout;
                        break;
                    }
                }
                if ( !edgeSerialized ) {
                    std::cerr << "gtpo::ProtoSerializer::serializeIn(): Warning: A Protocol Buffer edge has not been serialized in successfuly." << std::endl;
                    std::cerr << "\tProtocol Buffer Error:" << anyEdge.type_url() << std::endl;
                }
            }
        }

        // Serialize optional user message
        if ( inGraph.mutable_user1() != nullptr && inGraph.user1().Is< User1 >() )
            inGraph.mutable_user1()->UnpackTo( user1 );
    } else
        std::cerr << "gtpo::ProtoSerializer::serializeIn(): Protocol Buffer reports an error while trying to read input stream" << std::endl;

    // Report errors
    if ( serializedNodeCout != (int)inGraph.node_count() )
        std::cerr << "gtpo::ProtoSerializer::serializeIn(): Only " << serializedNodeCout << " nodes serialized while there is " << inGraph.node_count() << " nodes in serialized graph" << std::endl;
    if ( serializedEdgeCout != (int)graph.getEdges().size() )
        std::cerr << "gtpo::ProtoSerializer::serializeOut(): Only " << serializedEdgeCout << " edges serialized while there is " << inGraph.edge_count() << " edges in graph" << std::endl;

    if ( progress != nullptr )
        progress->endProgress();
}

template < class GraphConfig, class Notifier >
void    ProtoSerializer< GraphConfig, Notifier >::serializeGTpoNodeIn( const gtpo::pb::GTpoNode& pbNode,
                                                                       WeakNode& weakNode,
                                                                       IdObjectMap& idObjectMap )
{
    SharedNode node = weakNode.lock();
    if ( node ) {    // Feed the newly created node with PB node data
        idObjectMap.insert( std::make_pair( pbNode.node_id(), node.get() ) );
        GraphConfig::setNodeLabel( node, pbNode.label() );
        GraphConfig::setNodeX( node, pbNode.x() );
        GraphConfig::setNodeY( node, pbNode.y() );
        GraphConfig::setNodeWidth( node, pbNode.width() );
        GraphConfig::setNodeHeight( node, pbNode.height() );
    }
}

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::registerNodeInFunctor( NodeInFunctor nodeInFunctor ) -> void
{
    _nodeInFunctors.push_back( nodeInFunctor );
}

template < class GraphConfig, class Notifier >
auto    ProtoSerializer< GraphConfig, Notifier >::registerEdgeInFunctor( EdgeInFunctor edgeInFunctor ) -> void
{
    _edgeInFunctors.push_back( edgeInFunctor );
}

} // ::gtpo

#endif
