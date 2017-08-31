/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

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
// \file	qanConnector.cpp
// \author	benoit@destrat.io
// \date	2017 03 10
//-----------------------------------------------------------------------------

// Qt headers
#include <QQuickItem>

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanNode.h"
#include "./qanEdgeItem.h"
#include "./qanPortItem.h"
#include "./qanConnector.h"

namespace qan { // ::qan

/* Connector Object Management *///--------------------------------------------
Connector::Connector(QQuickItem* parent) :
    qan::NodeItem( parent )
{
    setAcceptDrops(false);
    setVisible(false);
}

Connector::~Connector() { /* Nil */ }

auto    Connector::setGraph(qan::Graph* graph) noexcept -> void
{
    if ( graph != _graph.data() ) {
        _graph = graph;
        if ( _edgeItem ) {
            _edgeItem->setParentItem(graph->getContainerItem());
            _edgeItem->setGraph(graph);
            _edgeItem->setVisible(false);
        }
        if ( _graph == nullptr )
            setVisible( false );
        emit graphChanged();
    }
}

auto    Connector::getGraph() const noexcept -> qan::Graph* { return _graph.data(); }
//-----------------------------------------------------------------------------

/* Node Static Factories *///--------------------------------------------------
static std::unique_ptr<QQmlComponent>   qan_Connector_delegate;
static std::unique_ptr<qan::NodeStyle>  qan_Connector_style;

QQmlComponent*  Connector::delegate(QObject* caller) noexcept
{
    if ( !qan_Connector_delegate &&
         caller != nullptr ) {
        const auto engine = qmlEngine(caller);
        if ( engine != nullptr )
            qan_Connector_delegate = std::make_unique<QQmlComponent>(engine, "qrc:/QuickQanava/VisualConnector.qml");
        else qWarning() << "[static]qan::Connector::delegate(): Error: QML engine is nullptr.";
    }
    return qan_Connector_delegate.get();
}

qan::NodeStyle* Connector::style() noexcept
{
    if ( !qan_Connector_style )
        qan_Connector_style = std::make_unique<qan::NodeStyle>();
    return qan_Connector_style.get();
}
//-----------------------------------------------------------------------------

/* Connector Configuration *///------------------------------------------------
void    Connector::connectorReleased(QQuickItem* target) noexcept
{
    // Restore original position
    if ( _connectorItem )
        _connectorItem->setState("NORMAL");

    if ( _edgeItem )    // Hide connector "transcient" edge item
        _edgeItem->setVisible(false);
    if ( !_graph )
        return;

    const auto dstNodeItem = qobject_cast<qan::NodeItem*>(target);
    const auto dstPortItem = qobject_cast<qan::PortItem*>(target);
    const auto dstEdgeItem = qobject_cast<qan::EdgeItem*>(target);

    const auto srcPortItem = _sourcePort;
    const auto srcNode = _sourceNode ? _sourceNode :
                                       _sourcePort ? _sourcePort->getNode() : nullptr;
    const auto dstNode = dstNodeItem ? dstNodeItem->getNode() :
                                       dstPortItem ? dstPortItem->getNode() : nullptr;
    const auto dstEdge = dstEdgeItem ? dstEdgeItem->getEdge() : nullptr;

    qan::Edge* createdEdge = nullptr;   // Result created edge
    if ( srcNode != nullptr &&          //// Regular edge node to node connection //////////
         dstNode != nullptr ) {
        if ( getCreateDefaultEdge() ) {
            createdEdge = _graph->insertEdge( srcNode, dstNode );
            if ( createdEdge != nullptr ) {     // Special handling for src or dst port item
                if ( dstPortItem != nullptr )
                    _graph->bindEdgeDestination(*createdEdge, *dstPortItem );   // Bind created edge to a destination port
                if ( srcPortItem != nullptr )
                    _graph->bindEdgeSource(*createdEdge, *srcPortItem);
            }
        } else
            emit requestEdgeCreation(srcNode, dstNode);
    } else if ( srcNode != nullptr &&   //// Hyper edge node to edge connection ///////////
                dstEdge != nullptr &&
                getHEdgeEnabled() ) {
        if ( !dstEdgeItem->isHyperEdge() ) {            // Do not create an hyper edge on an hyper edge
            if ( getCreateDefaultEdge() )
                createdEdge = _graph->insertEdge( srcNode, dstEdge );
            else emit requestEdgeCreation(srcNode, dstEdge);
        }
    }

    // FIXME: it sucks
    //if ( createdEdge ) // Notify user of the edge creation
    //    createdEdge.color = Qt.binding( function() { return visualConnector.edgeColor; } );
   if ( createdEdge ) // Notify user of the edge creation
        emit edgeInserted( createdEdge );
}

void    Connector::connectorPressed() noexcept
{
    if ( _graph != nullptr &&
         _edgeItem != nullptr ) {
        _edgeItem->setGraph(_graph);    // Eventually, configure edge item
        const auto srcItem = _sourcePort ? _sourcePort :
                                           _sourceNode ? _sourceNode->getItem() : nullptr;
        _edgeItem->setSourceItem(srcItem);
        _edgeItem->setDestinationItem(this);
        _edgeItem->setVisible(true);
    }
}

auto    Connector::getCreateDefaultEdge() const noexcept -> bool { return _createDefaultEdge; }
auto    Connector::setCreateDefaultEdge(bool createDefaultEdge) noexcept -> void
{
    if ( createDefaultEdge != _createDefaultEdge ) {
        _createDefaultEdge = createDefaultEdge;
        emit createDefaultEdgeChanged();
    }
}

auto    Connector::getHEdgeEnabled() const noexcept -> bool { return _hEdgeEnabled; }
auto    Connector::setHEdgeEnabled(bool hEdgeEnabled) noexcept -> void
{
    if ( hEdgeEnabled != _hEdgeEnabled ) {
        _hEdgeEnabled = hEdgeEnabled;
        emit hEdgeEnabledChanged();
    }
}

auto    Connector::getConnectorItem() noexcept -> QQuickItem* { return _connectorItem.data(); }
auto    Connector::setConnectorItem(QQuickItem* connectorItem) noexcept -> void
{
    if ( _connectorItem != connectorItem ) {
        if ( _connectorItem )
            _connectorItem->deleteLater();
        _connectorItem = connectorItem;
        if ( _connectorItem ) {
            _connectorItem->setParentItem(this);
            _connectorItem->setVisible( isVisible() &&
                                        _sourceNode != nullptr );
        }
        emit connectorItemChanged();
    }
}

auto    Connector::getEdgeComponent() noexcept -> QQmlComponent* { return _edgeComponent.data(); }

auto    Connector::setEdgeComponent(QQmlComponent* edgeComponent) noexcept -> void
{
    if ( edgeComponent != _edgeComponent ) {
        _edgeComponent = edgeComponent;
        if ( _edgeComponent &&
             _edgeComponent->isReady() ) {
            const auto context = qmlContext(this);
            if ( context != nullptr ) {
                const auto edgeObject = _edgeComponent->create(context);    // Create a new edge
                _edgeItem = qobject_cast<qan::EdgeItem*>(edgeObject);
                if ( _edgeItem &&
                     !_edgeComponent->isError() ) {
                    QQmlEngine::setObjectOwnership( _edgeItem.data(), QQmlEngine::CppOwnership );
                    _edgeItem->setVisible( false );
                    _edgeItem->setAcceptDrops(false);
                    if ( getGraph() != nullptr ) {
                        _edgeItem->setGraph( getGraph() );
                        _edgeItem->setParentItem( getGraph()->getContainerItem() );
                    }
                    if ( _sourceNode &&
                         _sourceNode->getItem() ) {
                        _edgeItem->setSourceItem(_sourceNode->getItem());
                        _edgeItem->setDestinationItem(this);
                    }
                    emit edgeItemChanged();
                } else {
                    qWarning() << "qan::Connector::setEdgeComponent(): Error while creating edge:";
                    qWarning() << "\t" << _edgeComponent->errors();
                }
            }
        }
        emit edgeComponentChanged();
    }
}

auto    Connector::getEdgeItem() noexcept -> qan::EdgeItem*
{
    return _edgeItem.data();
}

void    Connector::setSourcePort( qan::PortItem* sourcePort ) noexcept
{
    if ( sourcePort != _sourcePort.data() ) {
        if ( _sourcePort )  // Disconnect destroyed() signal connection with old  source node
            _sourcePort->disconnect(this);
        _sourcePort = sourcePort;

        if ( sourcePort != nullptr ) {      //// Connector configuration with port host /////
            if ( _sourceNode ) {    // Erase source node, we can't have both a source port and node
                _sourceNode->disconnect(this);
                _sourceNode = nullptr;
            }

            connect( sourcePort, &QObject::destroyed,
                     this,       &Connector::sourcePortDestroyed );
            setVisible(true);
            if ( sourcePort->getNode() != nullptr ) {
                setParentItem(sourcePort);
                // Configure connector position according to port item dock type
                if ( _edgeItem )
                    _edgeItem->setSourceItem(sourcePort);
                if ( _connectorItem ) {
                    _connectorItem->setParentItem(this);
                    _connectorItem->setState("NORMAL");
                    _connectorItem->setVisible(true);
                }
                setVisible(true);
            } else {    // Error: hide everything
                setVisible(false);
                if( _edgeItem )
                    _edgeItem->setVisible(false);
                if( _connectorItem )
                    _connectorItem->setVisible(false);
            }
        }

        if ( sourcePort == nullptr &&
             _sourceNode == nullptr )
            setVisible(false);

        emit sourcePortChanged();
    }
}

void    Connector::sourcePortDestroyed()
{
    if ( sender() == _sourcePort.data() ) {
        if ( _sourcePort != nullptr &&
             _sourcePort->getNode() != nullptr )       // Fall back on port node when port is destroyed
            setSourceNode(_sourcePort->getNode());
        setSourcePort(nullptr);
    }
}

void    Connector::setSourceNode( qan::Node* sourceNode ) noexcept
{
    if ( sourceNode != _sourceNode.data() ) {
        if ( _sourceNode )  // Disconnect destroyed() signal connection with old  source node
            _sourceNode->disconnect(this);
        _sourceNode = sourceNode;

        if ( _sourceNode ) {    //// Connector configuration with port host /////
            if ( _sourcePort ) { // Erase source port, we can't have both a source port and node
                _sourcePort->disconnect(this);
                _sourcePort = nullptr;
            }
            if ( _sourceNode->getItem() != nullptr ) {
                setParentItem(_sourceNode->getItem());
                if ( _edgeItem )
                    _edgeItem->setSourceItem(_sourceNode->getItem());
                if ( _connectorItem ) {
                    _connectorItem->setParentItem(this);
                    _connectorItem->setState("NORMAL");
                    _connectorItem->setVisible(true);
                }
                setVisible(true);
            } else {    // Error: hide everything
                setVisible(false);
                if ( _edgeItem )
                    _edgeItem->setVisible(false);
                if( _connectorItem )
                    _connectorItem->setVisible(false);
            }
        }

        if ( sourceNode == nullptr &&
             _sourcePort == nullptr )
            setVisible(false);
        else {
            connect( sourceNode, &QObject::destroyed,
                     this,       &Connector::sourceNodeDestroyed );
            setVisible(true);
        }
        emit sourceNodeChanged();
    }
}

void    Connector::sourceNodeDestroyed()
{
    if ( sender() == _sourceNode.data() )
        setSourceNode(nullptr);
}
//-----------------------------------------------------------------------------


} // ::qan
