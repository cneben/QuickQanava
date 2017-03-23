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
            _connectorItem->setVisible( isVisible() );
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
