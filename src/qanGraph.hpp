/*
 Copyright (c) 2008-2022, Benoit AUTHEMAN All rights reserved.

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
template <class Node_t>
qan::Node*  Graph::insertNode(QQmlComponent* nodeComponent, qan::NodeStyle* nodeStyle)
{
    if (nodeComponent == nullptr) {
        const auto engine = qmlEngine(this);
        nodeComponent = _nodeDelegate.get(); // If no delegate component is specified, try the node type delegate() factory
        if (nodeComponent == nullptr &&
            engine != nullptr) // Otherwise, use default node delegate component
            nodeComponent = Node_t::delegate(*engine);
    }
    if (nodeComponent == nullptr) {          // Otherwise, generate a warning and create a "non visual node"
        qWarning() << "qan::Graph::insertNode(): Can't find a valid node delegate component.";
        return nullptr;
    }
    if (nodeComponent->isError()) { // If component exists, it should be instanciable
        qWarning() << "Component error: " << nodeComponent->errors();
        return nullptr;
    }
    const auto node = new Node_t{};   // Might leak, but using unique_ptr is unsafe given current mxed bool/except error handling
    try {
        QQmlEngine::setObjectOwnership(node, QQmlEngine::CppOwnership);
        if (nodeStyle == nullptr)
            nodeStyle = Node_t::style(nullptr);
        if (nodeStyle == nullptr)
            throw qan::Error{"style() factory has returned a nullptr style."};
        _styleManager.setStyleComponent(nodeStyle, nodeComponent);      // nullptr nodeComponent is ok
        qan::NodeItem* nodeItem = static_cast<qan::NodeItem*>(createFromComponent(nodeComponent,
                                                                                  *nodeStyle,
                                                                                  node));
        if (nodeItem == nullptr)
            throw qan::Error{"Node item creation failed."};
        nodeItem->setNode(node);
        nodeItem->setGraph(this);
        node->setItem(nodeItem);
        auto notifyNodeClicked = [this] (qan::NodeItem* nodeItem, QPointF p) {
            if (nodeItem != nullptr && nodeItem->getNode() != nullptr)
                Q_EMIT this->nodeClicked(nodeItem->getNode(), p);
        };
        connect(nodeItem, &qan::NodeItem::nodeClicked,
                this,     notifyNodeClicked);

        auto notifyNodeRightClicked = [this] (qan::NodeItem* nodeItem, QPointF p) {
            if (nodeItem != nullptr && nodeItem->getNode() != nullptr)
                Q_EMIT this->nodeRightClicked(nodeItem->getNode(), p);
        };
        connect(nodeItem, &qan::NodeItem::nodeRightClicked,
                this,     notifyNodeRightClicked);

        auto notifyNodeDoubleClicked = [this] (qan::NodeItem* nodeItem, QPointF p) {
            if (nodeItem != nullptr && nodeItem->getNode() != nullptr)
                Q_EMIT this->nodeDoubleClicked(nodeItem->getNode(), p);
        };
        connect(nodeItem, &qan::NodeItem::nodeDoubleClicked,
                this,     notifyNodeDoubleClicked);
        {   // Send item to front
            _maxZ += 1;
            nodeItem->setZ(_maxZ);
        }
        insert_node(node);        // Insert visual or non visual node
    } catch (const qan::Error& e) {
        qWarning() << "qan::Graph::insertNode(): Error: " << e.getMsg();
        return nullptr; // node eventually destroyed by shared_ptr
    }
    catch (...) {
        qWarning() << "qan::Graph::insertNode(): Error: Topology error.";
        return nullptr; // node eventually destroyed by shared_ptr
    }
    if (node != nullptr) {       // Notify user.
        onNodeInserted(*node);
        Q_EMIT nodeInserted(node);
    }
    return node;
}

template <class Node_t>
qan::Node*  Graph::insertNonVisualNode()
{
    const auto node = new Node_t();
    try {
        QQmlEngine::setObjectOwnership(node, QQmlEngine::CppOwnership);
        if (!insert_node(node))
            qWarning() << "qan::Graph::insertNonVisualNode(): Error: Insertion error.";
    } catch ( ... ) {
        qWarning() << "qan::Graph::insertNonVisualNode(): Error: Topology error.";
        return nullptr;
    }
    if (node != nullptr) {       // Notify user.
        onNodeInserted(*node);
        Q_EMIT nodeInserted(node);
    }
    return node;
}
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
template < class Edge_t >
qan::Edge*  Graph::insertEdge(qan::Node& src, qan::Node* dstNode, QQmlComponent* edgeComponent)
{
    if (dstNode == nullptr)
        return nullptr;
    if (edgeComponent == nullptr) {
        const auto engine = qmlEngine(this);
        if (engine != nullptr)
            edgeComponent = Edge_t::delegate(*engine, nullptr);     // If no delegate component is specified, try the edge type delegate() factory
        if (edgeComponent == nullptr)
            edgeComponent = _edgeDelegate.get();    // Otherwise, use default edge delegate component
    }
    if (edgeComponent == nullptr) {               // Otherwise, throw an error, a visual edge must have a delegate
        qWarning() << "qan::Graph::insertEdge<>(): Error: Can't find a valid edge delegate component.";
        return nullptr;
    }
    const auto style = qobject_cast<qan::EdgeStyle*>(Edge_t::style(nullptr));
    if (style == nullptr) {
        qWarning() << "qan::Graph::insertEdge(): Error: style() factory has returned a nullptr style.";
        return nullptr;
    }
    qan::Edge* configuredEdge = nullptr;
    try {
        auto edge = new Edge_t{nullptr};
        QQmlEngine::setObjectOwnership(edge, QQmlEngine::CppOwnership);
        if (configureEdge(*edge,  *edgeComponent, *style,
                           src,    dstNode)) {
            insert_edge(edge);
            configuredEdge = edge;
        } else {
            qWarning() << "qan::Graph::insertEdge<>(): Error: Internal error during edge configuration.";
            // Note: edge is deleted since it is unreferenced...
        }
    } catch (...) {
        qWarning() << "qan::Graph::insertEdge<>(): Error: Topology error.";
        // Note: edge is cleaned automatically if it has still not been inserted to graph
    }
    if (configuredEdge != nullptr)
        Q_EMIT edgeInserted(configuredEdge);
    return configuredEdge;
}

template < class Edge_t >
qan::Edge*  Graph::insertNonVisualEdge(qan::Node& src, qan::Node* dstNode)
{
    if (dstNode == nullptr)
        return nullptr;
    auto edge = new Edge_t();
    try {
        QQmlEngine::setObjectOwnership(edge, QQmlEngine::CppOwnership);
        edge->set_src(&src);
        if (dstNode != nullptr)
            edge->set_dst(dstNode);
        insert_edge(edge);
    } catch (...) {
        qWarning() << "qan::Graph::insertNonVisualEdge<>(): Error: Topology error.";
    }
    return edge;
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
template <class Group_t>
qan::Group* Graph::insertGroup()
{
    const auto engine = qmlEngine(this);
    QQmlComponent* groupComponent = nullptr;
    if (engine != nullptr)
        groupComponent = Group_t::delegate(*engine, nullptr);
    if (groupComponent == nullptr)
        groupComponent = _groupDelegate.get();
    auto group = new Group_t();
    if (!insertGroup(group, groupComponent, nullptr))
        qWarning() << "qan::Graph::insertGroup<>(): Warning: Error at group insertion.";
    return group;
}
//-----------------------------------------------------------------------------

} // ::qan

