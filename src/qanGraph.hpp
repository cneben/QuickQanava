/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library.
//
// \file	qanGraph.hpp
// \author	benoit@destrat.io
// \date	2017 03 19
//-----------------------------------------------------------------------------


namespace qan { // ::qan

/* Graph Factories *///--------------------------------------------------------
template < class Node_t >
qan::Node*  Graph::insertNode(QQmlComponent* nodeComponent, qan::NodeStyle* nodeStyle)
{
    if ( nodeComponent == nullptr ) {
        const auto engine = qmlEngine(this);
        nodeComponent = _nodeDelegate.get(); // If no delegate component is specified, try the node type delegate() factory
        if ( nodeComponent == nullptr && engine != nullptr ) // Otherwise, use default node delegate component
                nodeComponent = Node_t::delegate(*engine);
    }
    if ( nodeComponent == nullptr ) {               // Otherwise, throw an error, a visual node must have a delegate
        qWarning() << "Can't find a valid node delegate component.";
        return nullptr;
    }
    if ( nodeComponent->isError() ) {
        qWarning() << "Component error: " << nodeComponent->errors();
        return nullptr;
    }
    const auto node = std::make_shared<Node_t>();
    try {
        QQmlEngine::setObjectOwnership( node.get(), QQmlEngine::CppOwnership );
        if (nodeStyle == nullptr )
            nodeStyle = Node_t::style();
        if ( nodeStyle == nullptr )
            throw qan::Error{"style() factory has returned a nullptr style."};
        _styleManager.setStyleComponent(nodeStyle, nodeComponent);
        qan::NodeItem* nodeItem = static_cast<qan::NodeItem*>( createFromComponent( nodeComponent,
                                                                                    *nodeStyle,
                                                                                    node.get() ) );
        if ( nodeItem  == nullptr )
            throw qan::Error{"Node item creation failed."};
        nodeItem->setNode(node.get());
        nodeItem->setGraph(this);
        node->setItem(nodeItem);
        auto notifyNodeClicked = [this] (qan::NodeItem* nodeItem, QPointF p) {
            if ( nodeItem != nullptr && nodeItem->getNode() != nullptr )
                emit this->nodeClicked(nodeItem->getNode(), p);
        };
        connect( nodeItem, &qan::NodeItem::nodeClicked, notifyNodeClicked );

        auto notifyNodeRightClicked = [this] (qan::NodeItem* nodeItem, QPointF p) {
            if ( nodeItem != nullptr && nodeItem->getNode() != nullptr )
                emit this->nodeRightClicked(nodeItem->getNode(), p);
        };
        connect( nodeItem, &qan::NodeItem::nodeRightClicked, notifyNodeRightClicked );

        auto notifyNodeDoubleClicked = [this] (qan::NodeItem* nodeItem, QPointF p) {
            if ( nodeItem != nullptr && nodeItem->getNode() != nullptr )
                emit this->nodeDoubleClicked(nodeItem->getNode(), p);
        };
        connect( nodeItem, &qan::NodeItem::nodeDoubleClicked, notifyNodeDoubleClicked );
        node->setItem(nodeItem);
        {   // Send item to front
            _maxZ += 1;
            nodeItem->setZ(_maxZ);
        }
        gtpo_graph_t::insert_node( node );
    } catch ( const gtpo::bad_topology_error& e ) {
        qWarning() << "qan::Graph::insertNode(): Error: Topology error: " << e.what();
        return nullptr; // node eventually destroyed by shared_ptr
    }
    catch ( const qan::Error& e ) {
        qWarning() << "qan::Graph::insertNode(): Error: " << e.getMsg();
        return nullptr; // node eventually destroyed by shared_ptr
    }
    catch ( ... ) {
        qWarning() << "qan::Graph::insertNode(): Error: Topology error.";
        return nullptr; // node eventually destroyed by shared_ptr
    }
    const auto nodePtr = node.get();
    if (nodePtr != nullptr) {       // Notify user.
        onNodeInserted(*nodePtr);
        emit nodeInserted(nodePtr);
    }
    return node.get();
}

template < class Node_t >
qan::Node*  Graph::insertNonVisualNode()
{
    const auto node = std::make_shared<Node_t>();
    try {
        QQmlEngine::setObjectOwnership( node.get(), QQmlEngine::CppOwnership );
        gtpo_graph_t::insert_node( node );
    } catch ( const gtpo::bad_topology_error& e ) {
        qWarning() << "qan::Graph::insertNonVisualNode(): Error: Topology error:" << e.what();
        return nullptr; // node eventually destroyed by shared_ptr
    }
    catch ( ... ) {
        qWarning() << "qan::Graph::insertNonVisualNode(): Error: Topology error.";
        return nullptr; // node eventually destroyed by share_ptr
    }
    const auto nodePtr = node.get();
    if (nodePtr != nullptr) {       // Notify user.
        onNodeInserted(*nodePtr);
        emit nodeInserted(nodePtr);
    }
    return node.get();
}
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
template < class Edge_t >
qan::Edge*  Graph::insertEdge( qan::Node& src, qan::Node* dstNode, QQmlComponent* edgeComponent )
{
    if ( dstNode == nullptr )
        return nullptr;
    if ( edgeComponent == nullptr ) {
        const auto engine = qmlEngine(this);
        if ( engine != nullptr )
            edgeComponent = Edge_t::delegate(*engine);     // If no delegate component is specified, try the edge type delegate() factory
        if ( edgeComponent == nullptr )
            edgeComponent = _edgeDelegate.get();    // Otherwise, use default edge delegate component
    }
    if ( edgeComponent == nullptr ) {               // Otherwise, throw an error, a visual edge must have a delegate
        qWarning() << "qan::Graph::insertEdge<>(): Error: Can't find a valid edge delegate component.";
        return nullptr;
    }
    const auto style = qobject_cast<qan::EdgeStyle*>(Edge_t::style());
    if ( style == nullptr ) {
        qWarning() << "qan::Graph::insertEdge(): Error: style() factory has returned a nullptr style.";
        return nullptr;
    }
    qan::Edge* configuredEdge = nullptr;
    try {
        auto edge = std::make_shared<Edge_t>();
        QQmlEngine::setObjectOwnership( edge.get(), QQmlEngine::CppOwnership );
        if ( configureEdge( *edge,  *edgeComponent, *style,
                            src,    dstNode ) ) {
            gtpo_graph_t::insert_edge( edge );
            configuredEdge = edge.get();
        }
    } catch ( gtpo::bad_topology_error e ) {
        qWarning() << "qan::Graph::insertEdge<>(): Error: Topology error:" << e.what();
        // Note: edge is cleaned automatically if it has still not been inserted to graph
    } catch ( ... ) {
        qWarning() << "qan::Graph::insertEdge<>(): Error: Topology error.";
        // Note: edge is cleaned automatically if it has still not been inserted to graph
    }
    if (configuredEdge != nullptr)
        emit edgeInserted(configuredEdge);
    return configuredEdge;
}

template < class Edge_t >
qan::Edge*  Graph::insertNonVisualEdge( qan::Node& src, qan::Node* dstNode )
{
    if ( dstNode == nullptr )
        return nullptr;
    auto edge = std::make_shared<Edge_t>();
    try {
        QQmlEngine::setObjectOwnership( edge.get(), QQmlEngine::CppOwnership );
        edge->set_src( std::static_pointer_cast<Config::final_node_t>(src.shared_from_this()) );
        if ( dstNode != nullptr )
            edge->set_dst( std::static_pointer_cast<Config::final_node_t>(dstNode->shared_from_this()) );
        gtpo_graph_t::insert_edge( edge );
    } catch ( gtpo::bad_topology_error e ) {
        qWarning() << "qan::Graph::insertNonVisualEdge<>(): Error: Topology error:" << e.what();
    }
    catch ( ... ) {
        qWarning() << "qan::Graph::insertNonVisualEdge<>(): Error: Topology error.";
    }
    return edge.get();
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
template <class Group_t>
qan::Group* Graph::insertGroup()
{
    const auto engine = qmlEngine(this);
    QQmlComponent* groupComponent = nullptr;
    if ( engine != nullptr )
        groupComponent = Group_t::delegate(*engine);
    if ( groupComponent == nullptr )
        groupComponent = _groupDelegate.get();
    auto group = std::make_shared<Group_t>();
    if (!insertGroup(group, groupComponent, nullptr))
        qWarning() << "qan::Graph::insertGroup<>(): Warning: Error at group insertion.";
    return group.get();
}
//-----------------------------------------------------------------------------

} // ::qan

