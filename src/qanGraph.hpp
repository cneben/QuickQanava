/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

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
// \file	qanGraph.hpp
// \author	benoit@destrat.io
// \date	2017 03 19
//-----------------------------------------------------------------------------


namespace qan { // ::qan

/* Graph Factories *///--------------------------------------------------------

template < class Node_t >
qan::Node*  Graph::insertNode(QQmlComponent* nodeComponent)
{
    if ( nodeComponent == nullptr ) {
        nodeComponent = Node_t::delegate(this);     // If no delegate component is specified, try the node type delegate() factory
        if ( nodeComponent == nullptr )
            nodeComponent = _nodeDelegate.get();    // Otherwise, use default node delegate component
    }
    if ( nodeComponent == nullptr ) {               // Otherwise, throw an error, a visual node must have a delegate
        qWarning() << "qan::Graph::insertNode<>(): Error: Can't find a valid node delegate component.";
        return nullptr;
    }
    const auto node = std::make_shared<Node_t>();
    try {
        if ( node ) {
            QQmlEngine::setObjectOwnership( node.get(), QQmlEngine::CppOwnership );
            qan::NodeStyle* nodeStyle = Node_t::style();
            if ( nodeStyle != nullptr ) {
                _styleManager.setStyleComponent(nodeStyle, nodeComponent);
                qan::NodeItem* nodeItem = static_cast<qan::NodeItem*>( createFromComponent( nodeComponent,
                                                                                            *nodeStyle,
                                                                                            node.get() ) );
                if ( nodeItem  != nullptr ) {
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
                    GTpoGraph::insertNode( node );
                } else
                    qWarning() << "qan::Graph::insertNode(): Warning: Node creation failed with the corresponding delegate";
            } else qWarning() << "qan::Graph::insertNode(): Error: style() factory has returned a nullptr style.";
        }
    } catch ( gtpo::bad_topology_error e ) {
        qWarning() << "qan::Graph::insertNode(): Error: Topology error:" << e.what();
    }
    catch ( ... ) {
        qWarning() << "qan::Graph::insertNode(): Error: Topology error.";
    }
    return node.get();
}
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
template < class Edge_t >
qan::Edge*  Graph::insertEdge( qan::Node& src, qan::Node* dstNode, qan::Edge* dstEdge, QQmlComponent* edgeComponent )
{
    if ( dstNode == nullptr &&
         dstEdge == nullptr )
        return nullptr;
    if ( edgeComponent == nullptr ) {
        edgeComponent = Edge_t::delegate(this);     // If no delegate component is specified, try the edge type delegate() factory
        if ( edgeComponent == nullptr )
            edgeComponent = _edgeDelegate.get();    // Otherwise, use default edge delegate component
    }
    if ( edgeComponent == nullptr ) {               // Otherwise, throw an error, a visual edge must have a delegate
        qWarning() << "qan::Graph::insertEdge<>(): Error: Can't find a valid edge delegate component.";
        return nullptr;
    }
    auto edge = std::make_shared<Edge_t>();
    try {
        QQmlEngine::setObjectOwnership( edge.get(), QQmlEngine::CppOwnership );
        qan::Style* style = Edge_t::style();
        if ( style != nullptr ) {
            _styleManager.setStyleComponent(style, edgeComponent);
            auto edgeItem = static_cast< qan::EdgeItem* >( createFromComponent( edgeComponent, *style, nullptr, edge.get() ) );
            if ( edgeItem != nullptr ) {
                edge->setItem(edgeItem);
                edgeItem->setSourceItem( src.getItem() );
                if ( dstNode != nullptr )
                    edgeItem->setDestinationItem( dstNode->getItem() );
                else if ( dstEdge != nullptr )
                    edgeItem->setDestinationEdge( dstEdge->getItem() );

                edge->setSrc( src.shared_from_this() );
                if ( dstNode != nullptr )
                    edge->setDst( dstNode->shared_from_this() );
                else if ( dstEdge != nullptr)
                    edge->setHDst( dstEdge->shared_from_this() );

                GTpoGraph::insertEdge( edge );

                auto notifyEdgeClicked = [this] (qan::EdgeItem* edgeItem, QPointF p) {
                    if ( edgeItem != nullptr && edgeItem->getEdge() != nullptr )
                        emit this->edgeClicked(edgeItem->getEdge(), p);
                };
                connect( edgeItem, &qan::EdgeItem::edgeClicked, notifyEdgeClicked );

                auto notifyEdgeRightClicked = [this] (qan::EdgeItem* edgeItem, QPointF p) {
                    if ( edgeItem != nullptr && edgeItem->getEdge() != nullptr )
                        emit this->edgeRightClicked(edgeItem->getEdge(), p);
                };
                connect( edgeItem, &qan::EdgeItem::edgeRightClicked, notifyEdgeRightClicked );

                auto notifyEdgeDoubleClicked = [this] (qan::EdgeItem* edgeItem, QPointF p) {
                    if ( edgeItem != nullptr && edgeItem->getEdge() != nullptr )
                        emit this->edgeDoubleClicked(edgeItem->getEdge(), p);
                };
                connect( edgeItem, &qan::EdgeItem::edgeDoubleClicked, notifyEdgeDoubleClicked );
            } else
                qWarning() << "qan::Graph::insertEdge(): Warning: Edge creation from QML delegate failed.";
        } else qWarning() << "qan::Graph::insertEdge(): Error: style() factory has returned a nullptr style.";
    } catch ( gtpo::bad_topology_error e ) {
        qWarning() << "qan::Graph::insertEdge<>(): Error: Topology error:" << e.what();
    }
    catch ( ... ) {
        qWarning() << "qan::Graph::insertEdge<>(): Error: Topology error.";
    }
    return edge.get();
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
//-----------------------------------------------------------------------------

} // ::qan

