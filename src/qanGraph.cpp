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
// \file	qanGraph.cpp
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QQmlProperty>
#include <QVariant>
#include <QQmlEngine>
#include <QQmlComponent>

#ifdef USE_GRAPHVIZ
#include <gvc.h>
#endif

// QuickQanava headers
#include "./qanUtils.h"
#include "./qanGraph.h"
#include "./qanNavigable.h"
#include "./qanNodeItem.h"
#include "./qanPortItem.h"
#include "./qanEdgeItem.h"
#include "./qanGroup.h"
#include "./qanGroupItem.h"
#include "./qanConnector.h"

namespace qan { // ::qan

/* Graph Object Management *///------------------------------------------------
Graph::Graph( QQuickItem* parent ) noexcept :
    gtpo::GenGraph< qan::GraphConfig >( parent )
{
    setContainerItem( this );
    setAntialiasing( true );
    setSmooth( true );
}

Graph::~Graph() { /* Nil */ }

void    Graph::classBegin()
{
    setPortDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/Port.qml")));
    setHorizontalDockDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/HorizontalDock.qml")));
    setVerticalDockDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/VerticalDock.qml")));
    setGroupDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/Group.qml")));
    setNodeDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/Node.qml")));
    setEdgeDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/Edge.qml")));
    setSelectionDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/SelectionItem.qml")));

    const auto engine = qmlEngine(this);
    if ( engine != nullptr ) {
        _styleManager.setStyleComponent(qan::Node::style(), qan::Node::delegate(*engine) );
        _styleManager.setStyleComponent(qan::Edge::style(), qan::Edge::delegate(*engine) );
    } else qWarning() << "qan::Graph::classBegin(): Error, no valid QML engine available.";
}

void    Graph::componentComplete()
{
    const auto engine = qmlEngine(this);
    if ( engine ) {
        // Visual connector initialization
        auto connectorComponent = std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/VisualConnector.qml"));
        if ( connectorComponent ) {
            qan::Style* style = qan::Connector::style();
            if ( style != nullptr ) {
                _connector.reset( qobject_cast<qan::Connector*>(createFromComponent(connectorComponent.get(), *style, nullptr)) );
                emit connectorChanged();
                if (_connector) {
                    _connector->setGraph(this);
                    _connector->setEnabled(getConnectorEnabled());
                    _connector->setVisible(false);
                    _connector->setProperty( "edgeColor", getConnectorEdgeColor() );
                    _connector->setProperty( "connectorColor", getConnectorColor() );
                    _connector->setProperty( "hEdgeEnabled", getConnectorHEdgeEnabled() );
                    _connector->setProperty( "createDefaultEdge", getConnectorCreateDefaultEdge() );
                    if ( getConnectorItem() != nullptr )
                        _connector->setConnectorItem( getConnectorItem() );
                    connect( _connector.data(), &qan::Connector::requestEdgeCreation,
                             this,              &qan::Graph::connectorRequestEdgeCreation);
                    connect( _connector.data(), &qan::Connector::edgeInserted,
                             this,              &qan::Graph::connectorEdgeInserted);
                }
            } else qWarning() << "qan::Graph::componentComplete(): Error: No style available for connector creation.";
        }
    } else qWarning() << "qan::Graph::componentComplete(): Error: No QML engine available to register default QML delegates.";
}

void    Graph::qmlClearGraph() noexcept
{
    qan::Graph::clear();
}

void    Graph::clear() noexcept
{
    _selectedNodes.clear();
    gtpo::GenGraph< qan::GraphConfig >::clear();
    _styleManager.clear();
}

QQuickItem* Graph::graphChildAt(qreal x, qreal y) const
{
    if ( getContainerItem() == nullptr )
        return nullptr;
    const QList<QQuickItem*> children = getContainerItem()->childItems();
    for (int i = children.count()-1; i >= 0; --i) {
        QQuickItem *child = children.at(i);
        QPointF point = mapToItem( child, QPointF(x, y) );  // Map coordinates to the child element's coordinate space
        if ( child->isVisible() &&
             child->contains( point ) &&    // Note 20160508: childAt do not call contains()
             point.x() > -0.0001 &&
             child->width() > point.x() &&
             point.y() > -0.0001 &&
             child->height() > point.y() ) {
            if ( child->inherits( "qan::GroupItem" ) ) {  // For group, look in group childs
                const auto groupItem = qobject_cast<qan::GroupItem*>( child );
                if ( groupItem != nullptr &&
                     groupItem->getContainer() != nullptr ) {
                    const QList<QQuickItem *> groupChildren = groupItem->getContainer()->childItems();
                    for (int gc = groupChildren.count()-1; gc >= 0; --gc) {
                        QQuickItem *groupChild = groupChildren.at(gc);
                        point = mapToItem( groupChild, QPointF(x, y) ); // Map coordinates to group child element's coordinate space
                        if ( groupChild->isVisible() &&
                             groupChild->contains( point ) &&
                             point.x() > -0.0001 &&
                             groupChild->width() > point.x() &&
                             point.y() > -0.0001 &&
                             groupChild->height() > point.y() ) {
                            return groupChild;
                        }
                    }
                }
            }
            return child;
        }
    }
    return nullptr;
}

qan::Group* Graph::groupAt( const QPointF& p, const QSizeF& s ) const
{
    for ( const auto& group : getGroups() )
        if ( group &&
             group->getItem() != nullptr ) {
            const auto groupItem = group->getItem();
             if ( QRectF{ groupItem->position(),
                  QSizeF{ groupItem->width(), groupItem->height() } }.contains( QRectF{ p, s } ) )
                 return group.get();
        }
    return nullptr;
}

void    Graph::setContainerItem( QQuickItem* containerItem )
{
    if ( containerItem != nullptr &&
         containerItem != _containerItem.data() ) {
        _containerItem = containerItem;
        emit containerItemChanged();
    }
}
//-----------------------------------------------------------------------------


/* Visual connection Management *///-------------------------------------------
void    Graph::setConnectorSource(qan::Node* sourceNode) noexcept
{
    if ( _connector ) {
        if ( sourceNode != nullptr )
            _connector->setSourceNode(sourceNode);
        _connector->setVisible(getConnectorEnabled());
        _connector->setEnabled(getConnectorEnabled());
    }
}

void    Graph::setConnectorEdgeColor( QColor connectorEdgeColor ) noexcept
{
    if ( connectorEdgeColor != _connectorEdgeColor ) {
        _connectorEdgeColor = connectorEdgeColor;
        if ( _connector )
            _connector->setProperty( "edgeColor", connectorEdgeColor );
        emit connectorEdgeColorChanged();
    }
}

void    Graph::setConnectorColor( QColor connectorColor ) noexcept
{
    if ( connectorColor != _connectorColor ) {
        _connectorColor = connectorColor;
        if ( _connector )
            _connector->setProperty( "connectorColor", connectorColor );
        emit connectorColorChanged();
    }
}

void    Graph::setConnectorHEdgeEnabled( bool connectorHEdgeEnabled ) noexcept
{
    if ( connectorHEdgeEnabled != _connectorHEdgeEnabled ) {
        _connectorHEdgeEnabled = connectorHEdgeEnabled;
        if ( _connector )
            _connector->setProperty( "hEdgeEnabled", connectorHEdgeEnabled );
        emit connectorColorChanged();
    }
}

void    Graph::setConnectorCreateDefaultEdge( bool connectorCreateDefaultEdge ) noexcept
{
    if ( connectorCreateDefaultEdge != _connectorCreateDefaultEdge ) {
        _connectorCreateDefaultEdge = connectorCreateDefaultEdge;
        if ( _connector )
            _connector->setProperty( "createDefaultEdge", connectorCreateDefaultEdge );
        emit connectorCreateDefaultEdgeChanged();
    }
}

void    Graph::setConnectorItem( QQuickItem* connectorItem ) noexcept
{
    if ( connectorItem != _connectorItem ) {
        _connectorItem = connectorItem;
        if ( _connectorItem &&
             _connector )
            _connector->setConnectorItem( _connectorItem.data() );
        emit connectorItemChanged();
    }
}

void    Graph::setConnectorEnabled( bool connectorEnabled ) noexcept
{
    if ( connectorEnabled != _connectorEnabled ) {
        _connectorEnabled = connectorEnabled;
        if ( _connector ) {
            _connector->setVisible(connectorEnabled);
            _connector->setEnabled(connectorEnabled);
        }
        emit connectorEnabledChanged();
    }
}
qan::Connector* Graph::getConnector() noexcept  {  return _connector.data(); }
//-----------------------------------------------------------------------------

/* Delegates Management *///---------------------------------------------------
void    Graph::setNodeDelegate(QQmlComponent* nodeDelegate) noexcept
{
    if ( nodeDelegate != nullptr ) {
        if ( nodeDelegate != _nodeDelegate.get() ) {
            _nodeDelegate.reset(nodeDelegate);
            QQmlEngine::setObjectOwnership( nodeDelegate, QQmlEngine::CppOwnership );
            emit nodeDelegateChanged();
        }
    }
}

void    Graph::setNodeDelegate(std::unique_ptr<QQmlComponent> nodeDelegate) noexcept
{
    setNodeDelegate(nodeDelegate.release());
}

void    Graph::setEdgeDelegate(QQmlComponent* edgeDelegate) noexcept
{
    QQmlEngine::setObjectOwnership( edgeDelegate, QQmlEngine::CppOwnership );
    setEdgeDelegate(std::unique_ptr<QQmlComponent>(edgeDelegate));
}

void    Graph::setEdgeDelegate(std::unique_ptr<QQmlComponent> edgeDelegate) noexcept
{
    if ( edgeDelegate &&
         edgeDelegate != _edgeDelegate ) {
        _edgeDelegate = std::move(edgeDelegate);
        emit edgeDelegateChanged();
    }
}

void    Graph::setGroupDelegate(QQmlComponent* groupDelegate) noexcept
{
    if ( groupDelegate != nullptr ) {
        if ( groupDelegate != _groupDelegate.get() ) {
            _groupDelegate.reset(groupDelegate);
            QQmlEngine::setObjectOwnership( groupDelegate, QQmlEngine::CppOwnership );
            emit groupDelegateChanged();
        }
    }
}

void    Graph::setGroupDelegate(std::unique_ptr<QQmlComponent> groupDelegate) noexcept
{
    setGroupDelegate(groupDelegate.release());
}

QQuickItem* Graph::createFromComponent( QQmlComponent* component,
                                        qan::Style& style,
                                        qan::Node* node,
                                        qan::Edge* edge,
                                        qan::Group* group ) noexcept
{
    if ( component == nullptr ) {
        qWarning() << "qan::Graph::createFromComponent(): Error called with a nullptr delegate component.";
        return nullptr;
    }
    QQuickItem* item = nullptr;
    try {
        if ( !component->isReady() )
            throw qan::Error{ "Error delegate component is not ready." };

        const auto rootContext = qmlContext(this);
        if ( rootContext == nullptr )
            throw qan::Error{ "Error can't access to local QML context." };
        QObject* object = component->beginCreate(rootContext);
        if ( object == nullptr ||
             component->isError() ) {
            if ( object != nullptr )
                object->deleteLater();
            throw qan::Error{ "Failed to create a concrete QQuickItem from QML component:\n\t" +
                              component->errorString() };
        }
        // No error occurs
        if ( node != nullptr ) {
            const auto nodeItem = qobject_cast<qan::NodeItem*>(object);
            if ( nodeItem != nullptr ) {
                node->setItem(nodeItem);
                nodeItem->setNode(node);
                nodeItem->setGraph(this);
                nodeItem->setStyle(qobject_cast<qan::NodeStyle*>(&style));
                _styleManager.setStyleComponent(&style, component );
            }
        } else if ( edge != nullptr ) {
            const auto edgeItem = qobject_cast<qan::EdgeItem*>(object);
            if ( edgeItem != nullptr ) {
                edge->setItem(edgeItem);
                edgeItem->setEdge(edge);
                edgeItem->setGraph(this);
                _styleManager.setStyleComponent(edgeItem->getStyle(), component );
            }
        } else if ( group != nullptr ) {
            const auto groupItem = qobject_cast<qan::GroupItem*>(object);
            if ( groupItem != nullptr ) {
                group->setItem(groupItem);
                groupItem->setGroup(group);
                groupItem->setGraph(this);
                _styleManager.setStyleComponent(groupItem->getStyle(), component );
            }
        } else {
            const auto nodeItem = qobject_cast<qan::NodeItem*>(object); // Note 20170323: Usefull for Qan.StyleListView, where there
            if ( nodeItem != nullptr )                                  // is a preview item, but now actual underlining node.
                nodeItem->setItemStyle(&style);
        }
        component->completeCreate();
        if ( !component->isError() ) {
            QQmlEngine::setObjectOwnership( object, QQmlEngine::CppOwnership );
            item = qobject_cast< QQuickItem* >( object );
            item->setVisible( true );
            item->setParentItem( getContainerItem() );
        } // Note: There is no leak until cpp ownership is set
    } catch ( const qan::Error& e ) {
        Q_UNUSED(e);
        qWarning() << "qan::Graph::createFromComponent(): " << component->errors();
    } catch ( const std::exception& e ) {
        qWarning() << "qan::Graph::createFromComponent(): " << e.what();
    }
    return item;
}

QQuickItem* Graph::createFromComponent( QQmlComponent* component, qan::Style* style )
{
    return ( component != nullptr &&
             style != nullptr ) ? createFromComponent( component, *style, nullptr, nullptr, nullptr ) : nullptr;
}

void Graph::setSelectionDelegate(QQmlComponent* selectionDelegate) noexcept
{
    // Note: Cpp ownership is voluntarily not set to avoid destruction of
    // objects owned from QML
    setSelectionDelegate(std::unique_ptr<QQmlComponent>(selectionDelegate));
}

void Graph::setSelectionDelegate(std::unique_ptr<QQmlComponent> selectionDelegate) noexcept
{
    bool delegateChanged{false};
    if ( selectionDelegate ) {
        if ( selectionDelegate != _selectionDelegate ) {
            _selectionDelegate = std::move(selectionDelegate);
            delegateChanged = true;
        }
    } else {    // Use QuickQanava default selection delegate
        _selectionDelegate = createComponent(QStringLiteral("qrc:/QuickQanava/SelectionItem.qml"));
        delegateChanged = true;
    }
    if ( delegateChanged ) {  // Update all existing delegates...
        // Note: It could be done in a more more 'generic' way!
        auto updateNodeSelectionItem = [this]( auto& primitive ) -> void {
            auto finalPrimitive = qobject_cast<qan::Node*>(primitive.get());
            if ( finalPrimitive != nullptr &&
                 finalPrimitive->getItem() &&
                 finalPrimitive->getItem()->getSelectionItem() != nullptr )   // Replace only existing selection items
                    finalPrimitive->getItem()->setSelectionItem(this->createSelectionItem(finalPrimitive->getItem()));
        };
        auto updateGroupSelectionItem = [this]( auto& primitive ) -> void {
            auto finalPrimitive = qobject_cast<qan::Group*>(primitive.get());
            if ( finalPrimitive != nullptr &&
                 finalPrimitive->getItem() &&
                 finalPrimitive->getItem()->getSelectionItem() != nullptr )   // Replace only existing selection items
                    finalPrimitive->getItem()->setSelectionItem(this->createSelectionItem(finalPrimitive->getItem()));
        };
        std::for_each(getGroups().begin(), getGroups().end(), updateGroupSelectionItem);
        std::for_each(getNodes().begin(), getNodes().end(), updateNodeSelectionItem);
        emit selectionDelegateChanged();
    }
}

QPointer<QQuickItem> Graph::createSelectionItem( QQuickItem* parent ) noexcept
{
    const auto selectionItem = createItemFromComponent(_selectionDelegate.get());
    if ( selectionItem ) {
        selectionItem->setEnabled(false); // Avoid node/edge/group selection problems
        selectionItem->setState("UNSELECTED");
        selectionItem->setVisible(true);
        QQmlEngine::setObjectOwnership( selectionItem, QQmlEngine::CppOwnership );
        if (parent != nullptr ) {
            selectionItem->setParentItem(parent);
            selectionItem->setZ(1.0);
        }
        return selectionItem;
    }
    return QPointer<QQuickItem>{nullptr};
}

std::unique_ptr<QQmlComponent>  Graph::createComponent(const QString& url) noexcept
{
    // PRECONDITIONS
        // url could not be empty
    if ( url.isEmpty() ) {
        qWarning() << "qan::Graph::createComponent(): Error: Empty url.";
        return std::unique_ptr<QQmlComponent>();
    }

    QQmlEngine* engine = qmlEngine( this );
    std::unique_ptr<QQmlComponent> component;
    if ( engine != nullptr ) {
        try {
            component = std::make_unique<QQmlComponent>(engine, url);
            if ( !component->isReady() ||
                 component->isError() ||
                 component->isNull()  ) {
                qWarning() << "qan::Graph::createComponent(): Error while creating component from URL " << url;
                qWarning() << "\tQML Component status=" << component->status();
                qWarning() << "\tQML Component errors=" << component->errors();
            }
            return component;
        } catch (...) { // Might be std::bad_alloc
            qWarning() << "qan::Graph::createComponent(): Error while creating component " << url;
        }
    } else qWarning() << "qan::Graph::createComponent(): No access to QML engine.";

    return component;
}

QPointer<QQuickItem> Graph::createItemFromComponent(QQmlComponent* component) noexcept
{
    // PRECONDITIONS:
        // component should not be nullptr, warning issued
    if ( component == nullptr ) {
        qWarning() << "qan::Graph::createItemFromComponent(): Error called with a nullptr delegate component.";
        return nullptr;
    }
    QQuickItem* item = nullptr;
    try {
        if ( !component->isReady() )
            throw qan::Error{ "Error delegate component is not ready." };

        const auto rootContext = qmlContext(this);
        if ( rootContext == nullptr )
            throw qan::Error{ "Error can't access to local QML context." };
        QObject* object = component->beginCreate(rootContext);
        if ( object == nullptr ||
             component->isError() ) {
            if ( object != nullptr )
                object->deleteLater();
            throw qan::Error{ "Failed to create a concrete QQuickItem from QML component:\n\t" +
                              component->errorString() };
        }
        component->completeCreate();
        if ( !component->isError() ) {
            QQmlEngine::setObjectOwnership( object, QQmlEngine::CppOwnership );
            item = qobject_cast< QQuickItem* >( object );
            item->setVisible( true );
            item->setParentItem( getContainerItem() );
        } // Note QAN3: There is no leak until cpp ownership is set
    } catch ( const qan::Error& e ) {
        qWarning() << "qan::Graph::createItemFromComponent(): " << e.getMsg() << "\n" << component->errors();
    } catch ( const std::exception& e ) {
        qWarning() << "qan::Graph::createItemFromComponent(): " << e.what() << "\n" << component->errors();
    }
    return QPointer<QQuickItem>{item};
}
//-----------------------------------------------------------------------------

/* Graph Factories *///--------------------------------------------------------
qan::Node*  Graph::insertNode( QQmlComponent* nodeComponent )
{
    return insertNode<qan::Node>(nodeComponent);
}

void    Graph::removeNode( qan::Node* node )
{
    if ( node == nullptr )
        return;
    WeakNode weakNode;
    try {
        weakNode = node->shared_from_this();
        //weakNode = WeakNode{ node->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return; }
    if ( _selectedNodes.contains(node) )
        _selectedNodes.removeAll(node);
    GTpoGraph::removeNode( weakNode );
}
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
qan::Edge*  Graph::insertEdge( QObject* source, QObject* destination, QQmlComponent* edgeComponent )
{
    auto sourceNode = qobject_cast<qan::Node*>(source);
    if ( sourceNode != nullptr ) {
            if ( qobject_cast<qan::Node*>(destination) != nullptr )
                return insertEdge( sourceNode, qobject_cast<qan::Node*>( destination ), edgeComponent );
            else if ( qobject_cast<qan::Group*>(destination) != nullptr )
                return insertEdge( sourceNode, qobject_cast<qan::Group*>( destination ), edgeComponent );
            else if ( qobject_cast<qan::Edge*>(destination) != nullptr )
                return insertEdge( sourceNode, qobject_cast<qan::Edge*>( destination ), edgeComponent );
    }
    qWarning() << "qan::Graph::insertEdge(): Error: Unable to find a valid insertEdge() method for arguments " << source << " and " << destination;
    return nullptr;
}

qan::Edge*  Graph::insertEdge( qan::Node* source, qan::Node* destination, QQmlComponent* edgeComponent )
{
    // PRECONDITION;
        // source and destination can't be nullptr
    if ( source == nullptr ||
         destination == nullptr )
        return nullptr;
    return insertEdge<qan::Edge>(*source, destination, nullptr, edgeComponent );
}

qan::Edge*  Graph::insertEdge( qan::Node* source, qan::Edge* destination, QQmlComponent* edgeComponent )
{
    // PRECONDITIONS:
        // source and destination can't be nullptr
    if ( source == nullptr ||
         destination == nullptr )
        return nullptr;
    return insertEdge<qan::Edge>(*source, nullptr, destination, edgeComponent );
}

void    Graph::bindEdgeSource( qan::Edge* edge, qan::PortItem* outPort) noexcept
{
    // PRECONDITIONS:
        // edge and outport must be non nullptr
    if ( edge == nullptr ||
         outPort == nullptr )
        return;
    bindEdgeSource(*edge, *outPort);
}

void    Graph::bindEdgeDestination( qan::Edge* edge, qan::PortItem* inPort ) noexcept
{
    // PRECONDITIONS:
        // edge and outport must be non nullptr
    if ( edge == nullptr ||
         inPort == nullptr )
        return;
    bindEdgeDestination(*edge, *inPort);
}

void    Graph::bindEdge(qan::Edge* edge, qan::PortItem* outPort, qan::PortItem* inPort ) noexcept
{
    bindEdgeDestination(edge, inPort);
    bindEdgeSource(edge, outPort );
}

bool    Graph::isEdgeSourceBindable( const qan::PortItem& outPort) const noexcept
{
    // To allow an edge source to be binded to a port, we must have an outport
    if ( outPort.getType() != qan::PortItem::Type::Out &&
         outPort.getType() != qan::PortItem::Type::InOut )
        return false;

    // Do not connect an edge to a port that has Single multiplicity and
    // already has an in edge
    if ( outPort.getMultiplicity() == qan::PortItem::Multiplicity::Multiple )
        return true;    // Fast exit
    if ( outPort.getMultiplicity() == qan::PortItem::Multiplicity::Single ) {
        const auto outPortInDegree = outPort.getOutEdgeItems().size();
        if ( outPortInDegree == 0 )
            return true;
    }
    return false;
}

bool    Graph::isEdgeDestinationBindable( const qan::PortItem& inPort ) const noexcept
{
    // To allow an edge destination to be binded to a port, we must have an in port
    if ( inPort.getType() != qan::PortItem::Type::In &&
         inPort.getType() != qan::PortItem::Type::InOut )
        return false;

    // Do not connect an edge to a port that has Single multiplicity and
    // already has an in edge
    if ( inPort.getMultiplicity() == qan::PortItem::Multiplicity::Multiple )
        return true;    // Fast exit
    if ( inPort.getMultiplicity() == qan::PortItem::Multiplicity::Single ) {
        const auto inPortInDegree = inPort.getInEdgeItems().size();
        if ( inPortInDegree == 0 )
            return true;
    }
    return false;
}

void    Graph::bindEdgeSource( qan::Edge& edge, qan::PortItem& outPort ) noexcept
{
    // PRECONDITION:
        // edge must have an associed item
    auto edgeItem = edge.getItem();
    if ( edgeItem == nullptr )
        return;

    if ( isEdgeSourceBindable(outPort) ) {
        edgeItem->setSourceItem(&outPort);
        outPort.getOutEdgeItems().append(edgeItem);
    }

    /*
    // PRECONDITION:
        // outPort must be an Out or InOut port
        // edge must have an associed item
    if ( outPort.getType() != qan::PortItem::Type::Out &&
         outPort.getType() != qan::PortItem::Type::InOut )
        return;
    auto edgeItem = edge.getItem();
    if ( edgeItem != nullptr ) {
        // Do not connect an edge to a port that has Single multiplicity and
        // already has an in edge
        const auto outPortInDegree = outPort.getOutEdgeItems().size();
        switch ( outPort.getMultiplicity() ) {
        case qan::PortItem::Multiplicity::Single:
            if ( outPortInDegree == 0 ) {
                edgeItem->setSourceItem(&outPort);
                outPort.getOutEdgeItems().append(edgeItem);
            }
            break;
        case qan::PortItem::Multiplicity::Multiple:
            edgeItem->setSourceItem(&outPort);
            outPort.getOutEdgeItems().append(edgeItem);
            break;
        }
    }
    */
}

void    Graph::bindEdgeDestination( qan::Edge& edge, qan::PortItem& inPort ) noexcept
{
    // PRECONDITION:
        // edge must have an associed item
    auto edgeItem = edge.getItem();
    if ( edgeItem == nullptr )
        return;

    if ( isEdgeDestinationBindable(inPort) ) {
        edgeItem->setDestinationItem(&inPort);
        inPort.getInEdgeItems().append(edgeItem);
    }

    /*
    // PRECONDITION:
        // inPort must be an In or InOut port
        // edge must have an associed item
    if ( inPort.getType() != qan::PortItem::Type::In &&
         inPort.getType() != qan::PortItem::Type::InOut )
        return;
    auto edgeItem = edge.getItem();
    if ( edgeItem != nullptr ) {
        // Do not connect an edge to a port that has Single multiplicity and
        // already has an in edge
        const auto inPortInDegree = inPort.getInEdgeItems().size();
        switch ( inPort.getMultiplicity() ) {
        case qan::PortItem::Multiplicity::Single:
            if ( inPortInDegree == 0 ) {
                edgeItem->setDestinationItem(&inPort);
                inPort.getInEdgeItems().append(edgeItem);
            }
            break;
        case qan::PortItem::Multiplicity::Multiple:
            edgeItem->setDestinationItem(&inPort);
            inPort.getInEdgeItems().append(edgeItem);
            break;
        }
    }*/
}

bool    Graph::configureEdge( qan::Edge& edge, QQmlComponent& edgeComponent, qan::EdgeStyle& style,
                              qan::Node& src, qan::Node* dstNode, qan::Edge* dstEdge )
{
    _styleManager.setStyleComponent(&style, &edgeComponent);
    auto edgeItem = qobject_cast< qan::EdgeItem* >( createFromComponent( &edgeComponent, style, nullptr, &edge ) );
    // FIXME: a leak might occurs if qobject_cast fails, but createFromComponent() return a non nullptr object...
    if ( edgeItem == nullptr ) {
        qWarning() << "qan::Graph::insertEdge(): Warning: Edge creation from QML delegate failed.";
        return false;
    }
    edge.setItem(edgeItem);
    edgeItem->setSourceItem( src.getItem() );
    if ( dstNode != nullptr )
        edgeItem->setDestinationItem( dstNode->getItem() );
    else if ( dstEdge != nullptr )
        edgeItem->setDestinationEdge( dstEdge->getItem() );

    edge.setSrc( src.shared_from_this() );
    if ( dstNode != nullptr )
        edge.setDst( dstNode->shared_from_this() );
    else if ( dstEdge != nullptr)
        edge.setHDst( dstEdge->shared_from_this() );

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
    return true;
}

void    Graph::removeEdge( qan::Node* source, qan::Node* destination )
{
    if ( source == nullptr || destination == nullptr )
        return;
    WeakNode sharedSource;
    WeakNode sharedDestination;
    try {
        sharedSource = WeakNode{ source->shared_from_this() };
        sharedDestination = WeakNode{ destination->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return; }
    return GTpoGraph::removeEdge( sharedSource, sharedDestination );
}

void    Graph::removeEdge( qan::Edge* edge )
{
    using WeakEdge = std::weak_ptr<qan::Edge>;
    if ( edge != nullptr )
        GTpoGraph::removeEdge( WeakEdge{edge->shared_from_this()} );
}

bool    Graph::hasEdge( qan::Node* source, qan::Node* destination ) const
{
    if ( source == nullptr || destination == nullptr )
        return false;
    WeakNode sharedSource;
    WeakNode sharedDestination;
    try {
        sharedSource = WeakNode{ source->shared_from_this() };
        sharedDestination = WeakNode{ destination->shared_from_this() };
    } catch ( std::bad_weak_ptr e ) { return false; }
    return GTpoGraph::hasEdge( sharedSource, sharedDestination );
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
qan::Group* Graph::insertGroup()
{
    return insertGroup<qan::Group>();
}

void    Graph::removeGroup( qan::Group* group )
{
    if ( group == nullptr )
        return;
    // Reparent all group childrens (ie node) to graph before destructing the group
    // otherwise all child iems get destructed too

    // FIXME QAN3       (use std::copy() here...)
    /*auto groupNodes{group->getNodes()};
    for ( auto& node : groupNodes ) {
        auto nodePtr{ node.lock() };
        if ( nodePtr != nullptr ) {
            // FIXME QAN3
            //nodePtr->ungroup();
            //nodePtr->setParentItem( this );
        }
    }
    WeakGroup weakGroup = group->shared_from_this();
    if ( !weakGroup.expired() )
        gtpo::GenGraph< qan::GraphConfig >::removeGroup( weakGroup );
    */
}

bool    Graph::hasGroup( qan::Group* group ) const
{
    if ( group == nullptr )
        return false;
    return GTpoGraph::hasGroup( WeakGroup{std::dynamic_pointer_cast<Group>(group->shared_from_this())} );
}

auto    qan::Graph::groupNode( qan::Group* group, qan::Node* node, bool transformPosition ) noexcept(false) -> void
{
    if ( group != nullptr &&
         node != nullptr ) {
        try {
            GTpoGraph::groupNode( std::dynamic_pointer_cast<Group>(group->shared_from_this()),
                                  node->shared_from_this() );
        } catch ( ... ) { qWarning() << "qan::Graph::groupNode(): Topology error."; }
        if ( node->getGroup().lock().get() == group &&  // Check that group insertion succeed
             group->getItem() != nullptr &&
             node->getItem() != nullptr ) {
            group->getItem()->groupNodeItem(node->getItem(), transformPosition);
        }
    }
}

auto    qan::Graph::groupNode( Group* group, qan::Group* node ) noexcept(false) -> void
{
    if ( group != nullptr &&
         node != nullptr ) {
        qDebug() << "qan::Graph::groupNode(qan::Group*, qan::Group*)";
        try {
            /*if ( group->getItem() )
                group->getItem()->ungroupNodeItem(node->getItem());
            GTpoGraph::ungroupNode( group->shared_from_this(), node->shared_from_this() );*/
        } catch ( ... ) { qWarning() << "qan::Graph::ungroupNode(): Topology error."; }
    }
}

auto    qan::Graph::ungroupNode( Group* group, qan::Node* node ) noexcept(false) -> void
{
    if ( group != nullptr &&
         node != nullptr ) {
        try {
            if ( group->getItem() )
                group->getItem()->ungroupNodeItem(node->getItem());
            GTpoGraph::ungroupNode( std::dynamic_pointer_cast<Group>(group->shared_from_this()),
                                    node->shared_from_this() );
        } catch ( ... ) { qWarning() << "qan::Graph::ungroupNode(): Topology error."; }
    }
}

auto    qan::Graph::ungroupNode( Group* group, qan::Group* node ) noexcept(false) -> void
{
    if ( group != nullptr &&
         node != nullptr ) {
        try {
            /*if ( group->getItem() )
                group->getItem()->ungroupNodeItem(node->getItem());
            GTpoGraph::ungroupNode( group->shared_from_this(), node->shared_from_this() );*/
        } catch ( ... ) { qWarning() << "qan::Graph::ungroupNode(): Topology error."; }
    }
}
//-----------------------------------------------------------------------------


/* Selection Management *///---------------------------------------------------
void    Graph::setSelectionPolicy( SelectionPolicy selectionPolicy ) noexcept
{
    if ( selectionPolicy == _selectionPolicy )  // Binding loop protection
        return;
    _selectionPolicy = selectionPolicy;
    if ( selectionPolicy == SelectionPolicy::NoSelection )
        clearSelection();
    emit selectionPolicyChanged( );
}

void    Graph::setSelectionColor( QColor selectionColor ) noexcept
{
    if ( selectionColor != _selectionColor ) {
        _selectionColor = selectionColor;
        configureSelectionItems();
        emit selectionColorChanged();
    }
}

void    Graph::setSelectionWeight( qreal selectionWeight ) noexcept
{
    if ( !qFuzzyCompare( 1. + selectionWeight, 1. + _selectionWeight ) ) {
        _selectionWeight = selectionWeight;
        configureSelectionItems();
        emit selectionWeightChanged();
    }
}

void    Graph::setSelectionMargin( qreal selectionMargin ) noexcept
{
    if ( !qFuzzyCompare( 1.0 + selectionMargin, 1.0 + _selectionMargin ) ) {
        _selectionMargin = selectionMargin;
        configureSelectionItems();
        emit selectionMarginChanged();
    }
}

void    Graph::configureSelectionItems() noexcept
{
    // PRECONDITIONS: None
    for ( auto node : _selectedNodes )
        if ( node != nullptr &&
             node->getItem() != nullptr )
            node->getItem()->configureSelectionItem();
    for ( auto group : _selectedGroups )
        if ( group != nullptr &&
             group->getItem() != nullptr )
            group->getItem()->configureSelectionItem();
}

template < class Primitive_t >
bool    selectPrimitiveImpl( Primitive_t& primitive,
                         Qt::KeyboardModifiers modifiers,
                         qan::Graph& graph )
{
    if ( graph.getSelectionPolicy() == qan::Graph::SelectionPolicy::NoSelection )
        return false;

    bool selectPrimitive{ false };
    bool ctrlPressed = modifiers & Qt::ControlModifier;
    if ( primitive.getItem() == nullptr )
        return false;

    if ( primitive.getItem()->getSelected() ) {
        if ( ctrlPressed )          // Click on a selected node + CTRL = deselect node
            graph.removeFromSelection( primitive );
    } else {
        switch ( graph.getSelectionPolicy() ) {
        case qan::Graph::SelectionPolicy::SelectOnClick:
            selectPrimitive = true;        // Click on an unselected node with SelectOnClick = select node
            if ( !ctrlPressed )
                graph.clearSelection();
            break;
        case qan::Graph::SelectionPolicy::SelectOnCtrlClick:
            selectPrimitive = ctrlPressed; // Click on an unselected node with CTRL pressed and SelectOnCtrlClick = select node
            break;
        case qan::Graph::SelectionPolicy::NoSelection: break;
        }
    }
    if ( selectPrimitive ) {
        graph.addToSelection( primitive );
        return true;
    }
    return false;
}

bool    Graph::selectNode( qan::Node& node, Qt::KeyboardModifiers modifiers ) { return selectPrimitiveImpl<qan::Node>(node, modifiers, *this); }
bool    Graph::selectGroup( qan::Group& group, Qt::KeyboardModifiers modifiers ) { return selectPrimitiveImpl<qan::Group>(group, modifiers, *this); }

template < class Primitive_t >
void    addToSelectionImpl( Primitive_t& primitive,
                            qcm::Container< QVector, Primitive_t* >& selectedPrimitives,
                            qan::Graph& graph )
{
    if ( !selectedPrimitives.contains( &primitive ) ) {
        selectedPrimitives.append( &primitive );
        if ( primitive.getItem() != nullptr ) {
            // Eventually, create and configure node item selection item
            if ( primitive.getItem()->getSelectionItem() == nullptr )
                primitive.getItem()->setSelectionItem(graph.createSelectionItem(primitive.getItem()).data());   // Safe, any argument might be nullptr
            primitive.getItem()->configureSelectionItem();
            primitive.getItem()->setSelected(true);
        }
    }
}

void    Graph::addToSelection( qan::Node& node ) { addToSelectionImpl<qan::Node>(node, _selectedNodes, *this); }
void    Graph::addToSelection( qan::Group& group ) { addToSelectionImpl<qan::Group>(group, _selectedGroups, *this); }

template < class Primitive_t >
void    removeFromSelectionImpl( Primitive_t& primitive,
                             qcm::Container< QVector, Primitive_t* >& selectedPrimitives )
{
    if ( selectedPrimitives.contains( &primitive ) )
        selectedPrimitives.removeAll( &primitive );
//    if ( primitive.getItem() != nullptr )
//        primitive.getItem()->setSelected(false);
}

void    Graph::removeFromSelection( qan::Node& node ) { removeFromSelectionImpl<qan::Node>(node, _selectedNodes); }
void    Graph::removeFromSelection( qan::Group& group ) { removeFromSelectionImpl<qan::Group>(group, _selectedGroups); }
void    Graph::removeFromSelection( QQuickItem* item ) {
    const auto nodeItem = qobject_cast<qan::NodeItem*>(item);
    if ( nodeItem != nullptr &&
         nodeItem->getNode() != nullptr ) {
        _selectedNodes.removeAll(nodeItem->getNode());
//        nodeItem->setSelected(false);
    } else {
        const auto groupItem = qobject_cast<qan::GroupItem*>(item);
        if ( groupItem != nullptr &&
             groupItem->getGroup() != nullptr ) {
            _selectedGroups.removeAll(groupItem->getGroup());
        }
    }
}

void    Graph::clearSelection()
{
    // Note: getItem()->setSelected() actually _modify_ content
    // of _selectedNodes and _selectedGroups, a deep copy of theses
    // container is necessary to avoid iterating on a vector that
    // has changed while iterator has been modified.
    SelectedNodes selectedNodesCopy;
    std::copy( _selectedNodes.cbegin(),
                _selectedNodes.cend(),
                std::back_inserter(selectedNodesCopy) );
    for ( auto node : selectedNodesCopy )
        if ( node != nullptr &&
             node->getItem() != nullptr )
            node->getItem()->setSelected(false);
    _selectedNodes.clear();

    SelectedGroups selectedGroupsCopy;
    std::copy( _selectedGroups.cbegin(),
                _selectedGroups.cend(),
                std::back_inserter(selectedGroupsCopy) );
    for ( auto group : selectedGroupsCopy )
        if ( group != nullptr &&
             group->getItem() != nullptr )
            group->getItem()->setSelected(false);
    _selectedGroups.clear();
}

void    Graph::mousePressEvent( QMouseEvent* event )
{
    if ( event->button() == Qt::LeftButton ) {
        clearSelection();
        forceActiveFocus();
    } else if ( event->button() == Qt::RightButton ) {
        qDebug() << "qan::Graph::rightClicked()";
        emit rightClicked(event->pos());
    }
    event->ignore();
    qan::GraphConfig::GraphBase::mousePressEvent( event );
}
//-----------------------------------------------------------------------------


/* Port/Dock Management *///---------------------------------------------------
qan::PortItem*  Graph::insertPort(qan::Node* node,
                                  qan::NodeItem::Dock dockType,
                                  qan::PortItem::Type portType,
                                  QString label,
                                  QString id ) noexcept
{
    // PRECONDITIONS:
        // node can't be nullptr
        // node must have an item (to access node style)
        // default _portDelegate must be valid
    if ( node == nullptr ||
         node->getItem() == nullptr )
        return nullptr;
    if ( !_portDelegate ) {
        qWarning() << "qan::Graph::insertPort(): no default port delegate available.";
        return nullptr;
    }

    qan::PortItem* portItem{nullptr};
    const auto nodeStyle = node->getItem()->getStyle();     // Use node style for dock item
    if ( nodeStyle ) {
        portItem = qobject_cast<qan::PortItem*>(createFromComponent(_portDelegate.get(), *nodeStyle ));
        if ( portItem != nullptr ) {
            portItem->setType(portType);
            portItem->setLabel(label);
            portItem->setId(id);
            portItem->setDockType(dockType);

            // Configure port mouse events forwarding to qan::Graph
            const auto notifyPortClicked = [this] (qan::NodeItem* nodeItem, QPointF p) {
                const auto portItem = qobject_cast<qan::PortItem*>(nodeItem);
                if ( portItem != nullptr &&
                     portItem->getNode() != nullptr )
                    emit this->portClicked(portItem, p);
            };
            connect( portItem, &qan::NodeItem::nodeClicked, notifyPortClicked );

            const auto notifyPortRightClicked = [this] (qan::NodeItem* nodeItem, QPointF p) {
                const auto portItem = qobject_cast<qan::PortItem*>(nodeItem);
                if ( portItem != nullptr &&
                     portItem->getNode() != nullptr )
                    emit this->portRightClicked(portItem, p);
            };
            connect( portItem, &qan::NodeItem::nodeRightClicked, notifyPortRightClicked );

            if ( node->getItem() != nullptr ) {
                portItem->setNode(node); // portitem node in fact map to this concrete node.
                node->getItem()->getPorts().append(portItem);
                auto dockItem = node->getItem()->getDock(dockType);
                if ( dockItem == nullptr ) {
                    // Create a dock item from the default dock delegate
                    dockItem = createDockFromDelegate(dockType, *node);
                    if ( dockItem != nullptr )
                        node->getItem()->setDock(dockType, dockItem);
                }
                if ( dockItem != nullptr )
                    portItem->setParentItem(dockItem);
                else {
                    portItem->setParentItem(node->getItem());
                    portItem->setZ(1.5);    // 1.5 because port item should be on top of selection item and under node resizer (selection item z=1.0, resizer z=2.0)
                }
            }
        }
    }
    return portItem;
}

void    Graph::removePort(qan::Node* node, qan::PortItem* port) noexcept
{
    if ( node == nullptr ||             // PRECONDITION: node must have a graphical, non visual nodes can't have ports
         node->getItem() == nullptr )
        return;
    if ( port == nullptr )              // PRECONDITION: port can't be nullptr
        return;

    auto removeConnectEdge = [this, port](auto& edge) {
        auto edgePtr = edge.lock();
        if (edgePtr &&
            edgePtr->getItem() != nullptr &&
            ( edgePtr->getItem()->getSourceItem() == port ||
             edgePtr->getItem()->getDestinationItem() == port ))
            this->removeEdge(edgePtr.get());
    };
    std::for_each(node->getInEdges().begin(), node->getInEdges().end(), removeConnectEdge);
    std::for_each(node->getOutEdges().begin(), node->getOutEdges().end(), removeConnectEdge);

    auto& ports = node->getItem()->getPorts();
    if (ports.contains(port))
        ports.removeAll(port);
    port->deleteLater();        // Note: port is owned by ports qcm::Container
}

void    Graph::qmlSetPortDelegate(QQmlComponent* portDelegate) noexcept
{
    if ( portDelegate != _portDelegate.get() ) {
        if ( portDelegate != nullptr )
            QQmlEngine::setObjectOwnership( portDelegate, QQmlEngine::CppOwnership );
        _portDelegate.reset(portDelegate);
        emit portDelegateChanged();
    }
}
void    Graph::setPortDelegate(std::unique_ptr<QQmlComponent> portDelegate) noexcept { qmlSetPortDelegate(portDelegate.release()); }

void    Graph::setHorizontalDockDelegate(QQmlComponent* horizontalDockDelegate) noexcept
{
    if ( horizontalDockDelegate != nullptr ) {
        if ( horizontalDockDelegate != _horizontalDockDelegate.get() ) {
            _horizontalDockDelegate.reset(horizontalDockDelegate);
            QQmlEngine::setObjectOwnership( horizontalDockDelegate, QQmlEngine::CppOwnership );
            emit horizontalDockDelegateChanged();
        }
    }
}
void    Graph::setHorizontalDockDelegate(std::unique_ptr<QQmlComponent> horizontalDockDelegate) noexcept { setHorizontalDockDelegate(horizontalDockDelegate.release()); }

void    Graph::setVerticalDockDelegate(QQmlComponent* verticalDockDelegate) noexcept
{
    if ( verticalDockDelegate != nullptr ) {
        if ( verticalDockDelegate != _verticalDockDelegate.get() ) {
            _verticalDockDelegate.reset(verticalDockDelegate);
            QQmlEngine::setObjectOwnership( verticalDockDelegate, QQmlEngine::CppOwnership );
            emit verticalDockDelegateChanged();
        }
    }
}
void    Graph::setVerticalDockDelegate(std::unique_ptr<QQmlComponent> verticalDockDelegate) noexcept { setVerticalDockDelegate(verticalDockDelegate.release()); }

QPointer<QQuickItem> Graph::createDockFromDelegate(qan::NodeItem::Dock dock, qan::Node& node) noexcept
{
    using Dock = qan::NodeItem::Dock;
    if ( dock == Dock::Left ||
         dock == Dock::Right ) {
        if ( _verticalDockDelegate ) {
            auto verticalDock = createItemFromComponent(_verticalDockDelegate.get());;
            verticalDock->setParentItem(node.getItem());
            verticalDock->setProperty("hostNodeItem",
                                      QVariant::fromValue(node.getItem()));
            verticalDock->setProperty("dockType",
                                      QVariant::fromValue(dock));
            return verticalDock;
        }
    } else if ( dock == Dock::Top ||
                dock == Dock::Bottom ) {
        if ( _horizontalDockDelegate ) {
            auto horizontalDock = createItemFromComponent(_horizontalDockDelegate.get());
            horizontalDock->setParentItem(node.getItem());
            horizontalDock->setProperty("hostNodeItem",
                                        QVariant::fromValue(node.getItem()));
            horizontalDock->setProperty("dockType",
                                        QVariant::fromValue(dock));
            return horizontalDock;
        }
    }
    return QPointer<QQuickItem>{nullptr};
}
//-----------------------------------------------------------------------------

/* Node auto-positioning *///--------------------------------------------------
void    Graph::autoPositionNodes() noexcept
{
#ifdef USE_GRAPHVIZ
    qDebug() << "autoPositionNodes...";
    GVC_t *gvc = gvContext();
    Agraph_t *g = agopen(const_cast<char*>("g"), Agdirected, nullptr);
    // NOTE: agset() nowhere seems to work, use agxset() evereywhere
    Agsym_t *symOverlap = agattr(g, AGRAPH, const_cast<char*>("overlap"), const_cast<char*>("prism"));
    agxset(g, symOverlap, const_cast<char*>("prism"));

    std::unique_ptr<Agnode_t*[]> nodes = std::make_unique<Agnode_t*[]>(getNodeCount());
    std::map<Node*, int> indexMap;
    for (int i = 0; i < getNodeCount(); ++i) {
        indexMap.insert(std::make_pair(getNodes().at(i).get(), i));

        int left = 0;
        int right = 0;
        int top = 0;
        int bottom = 0;
        for (const auto &p : getNodes().at(i)->getItem()->getPorts()) {
            PortItem *cp = static_cast<PortItem*>(p);
            if (!cp) {
                continue;
            }
            switch (cp->getDockType()) {
            case PortItem::Dock::Left:
                ++left;
                break;
            case PortItem::Dock::Right:
                ++right;
                break;
            case PortItem::Dock::Top:
                ++top;
                break;
            case PortItem::Dock::Bottom:
                ++bottom;
                break;
            }
        }
        QString nodeLabel = "{";
        int j = 0;
        for (; j < left; ++j) {
            if (j == 0) {
                nodeLabel += "<" + QString::number(j) + ">";
            } else {
                nodeLabel += "|<" + QString::number(j) + ">";
            }
        }
        nodeLabel += "} | {";
        int k = j;
        for (; j < top; ++j) {
            if (j == k) {
                nodeLabel += "<" + QString::number(j) + ">";
            } else {
                nodeLabel += "|<" + QString::number(j) + ">";
            }
        }
        nodeLabel += "| |";
        int n = j;
        for (; j < bottom; ++j) {
            if (j == n) {
                nodeLabel += "<" + QString::number(j) + ">";
            } else {
                nodeLabel += "|<" + QString::number(j) + ">";
            }
        }
        nodeLabel += "} | {";
        int m = j;
        for (; j < right; ++j) {
            if (j == m) {
                nodeLabel += "<" + QString::number(j) + ">";
            } else {
                nodeLabel += "|<" + QString::number(j) + ">";
            }
        }
        nodeLabel += "}";
        // 72 points = 1 inch
        double height = getNodes().at(i)->getItem()->property("height").toDouble() / 72;
        double width = getNodes().at(i)->getItem()->property("width").toDouble() / 72;

        Agsym_t *symHeight = agattr(g, AGNODE, const_cast<char*>("height"), const_cast<char*>(qPrintable(QString::number(height))));
        Agsym_t *symWidth = agattr(g, AGNODE, const_cast<char*>("width"), const_cast<char*>(qPrintable(QString::number(width))));
        Agsym_t *symShape = agattr(g, AGNODE, const_cast<char*>("shape"), const_cast<char*>("record"));
        Agsym_t *symFixedsize = agattr(g, AGNODE, const_cast<char*>("fixedsize"), const_cast<char*>("shape"));
        Agsym_t *symLabel = agattr(g, AGNODE, const_cast<char*>("label"), const_cast<char*>(qPrintable(nodeLabel)));

        nodes[i] = agnode(g, nullptr, 1);
        agxset(nodes[i], symShape, const_cast<char*>("record"));
        agxset(nodes[i], symFixedsize, const_cast<char*>("shape"));
        agxset(nodes[i], symHeight, const_cast<char*>(qPrintable(QString::number(height))));
        agxset(nodes[i], symWidth, const_cast<char*>(qPrintable(QString::number(width))));
        agxset(nodes[i], symLabel, const_cast<char*>(qPrintable(nodeLabel)));
    }
    for (const auto &e : getEdges()) {
        Agedge_t *edge = agedge(g, nodes[indexMap[e->getSource()]], nodes[indexMap[e->getDestination()]], nullptr, 1);
        if (!edge) {
            continue;
        }
        // tailport: source of arrow
        // headport: destination part, arrowhead
        QString tp = "e";
        QString hp = "w";
        for (const auto &p : e->getSource()->getItem()->getPorts()) {
            PortItem *cp = static_cast<PortItem*>(p);
            if (!cp) {
                continue;
            }
            for (const auto &ei : cp->getOutEdgeItems()) {
                if (ei == e->getItem()) {
                    switch (cp->getDockType()) {
                    case PortItem::Dock::Left:
                        tp = "w";
                        break;
                    case PortItem::Dock::Right:
                        tp = "e";
                        break;
                    case PortItem::Dock::Top:
                        tp = "n";
                        break;
                    case PortItem::Dock::Bottom:
                        tp = "s";
                        break;
                    }
                }
            }
        }
        for (const auto &p : e->getDestination()->getItem()->getPorts()) {
            PortItem *cp = static_cast<PortItem*>(p);
            if (!cp) {
                continue;
            }
            for (const auto &ei : cp->getInEdgeItems()) {
                if (ei == e->getItem()) {
                    switch (cp->getDockType()) {
                    case PortItem::Dock::Left:
                        hp = "w";
                        break;
                    case PortItem::Dock::Right:
                        hp = "e";
                        break;
                    case PortItem::Dock::Top:
                        hp = "n";
                        break;
                    case PortItem::Dock::Bottom:
                        hp = "s";
                        break;
                    }
                }
            }
        }
        // TODO: use ports
        Agsym_t *symHeadport = agattr(g, AGEDGE, const_cast<char*>("headport"), const_cast<char*>(qPrintable(hp)));
        Agsym_t *symTailport = agattr(g, AGEDGE, const_cast<char*>("tailport"), const_cast<char*>(qPrintable(tp)));
        agxset(edge, symHeadport, const_cast<char*>(qPrintable(hp)));
        agxset(edge, symTailport, const_cast<char*>(qPrintable(tp)));
    }

    gvLayout(gvc, g, "dot"); // apply/compute layout

    // FIXME cneben innefficient iterate over getNodes() directly...
    for (int i = 0; i < getNodeCount(); ++i) {
        getNodes().at(i)->getItem()->setX(ND_coord(nodes[i]).x * 1.2);
        getNodes().at(i)->getItem()->setY(ND_coord(nodes[i]).y * 1.2);
    }

    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
#endif
}
//-----------------------------------------------------------------------------

} // ::qan
