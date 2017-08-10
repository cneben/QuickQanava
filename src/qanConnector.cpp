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

void    Connector::setSourceNode( qan::Node* sourceNode )
{
    if ( sourceNode != _sourceNode.data() ) {
        if ( _sourceNode )  // Disconnect destroyed() signal connection with old  source node
            _sourceNode->disconnect(this);
        _sourceNode = sourceNode;
        if ( sourceNode == nullptr )
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
