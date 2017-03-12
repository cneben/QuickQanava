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
// \file	qanGraph.cpp
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QQmlProperty>
#include <QVariant>
#include <QQmlEngine>
#include <QQmlComponent>

// GTpo headers
#include "gtpoRandomGraph.h"

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanNavigable.h"
#include "./qanNodeItem.h"
#include "./qanEdgeItem.h"
#include "./qanGroupItem.h"
#include "./qanConnector.h"

namespace qan { // ::qan

/* Graph Object Management *///------------------------------------------------
Graph::Graph( QQuickItem* parent ) noexcept :
    gtpo::GenGraph< qan::GraphConfig >( parent ),
    _styleManager{ SharedStyleManager{ new qan::StyleManager( this ) } }
{
    setContainerItem( this );
    setAntialiasing( true );
    setSmooth( true );
}

Graph::~Graph() { /* Nil */ }

void    Graph::componentComplete()
{
    const auto engine = qmlEngine(this);
    if ( engine ) {
        setNodeDelegate(std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/Node.qml")));
        setEdgeDelegate(std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/Edge.qml")));
        setGroupDelegate(std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/Group.qml")));

        // Visual connector initialization
        auto connectorComponent = std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/VisualConnector.qml"));
        if ( connectorComponent ) {
            _connector.reset( qobject_cast<qan::Connector*>(createFromComponent(connectorComponent.get(), nullptr)) );
            if (_connector)
                _connector->setGraph(this);
            _connector->setEnabled(getConnectorEnabled());
            _connector->setVisible(getConnectorEnabled());
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
    _styleManager->clear();
}

QQuickItem* Graph::graphChildAt(qreal x, qreal y) const
{
    if ( getContainerItem() == nullptr )
        return nullptr;

    const QList<QQuickItem *> children = getContainerItem()->childItems();
    //qDebug() << "graphChildAt(): x=" << x << " y=" << y;
    //qDebug() << " \tchildren=" << children;
    for (int i = children.count()-1; i >= 0; --i) {
        QQuickItem *child = children.at(i);
        QPointF point = mapToItem( child, QPointF(x, y) );  // Map coordinates to the child element's coordinate space
        if ( child->isVisible() &&
             child->contains( point ) &&    // Note 20160508: childAt do not call contains()
             point.x() >= 0 &&
             child->width() > point.x() &&
             point.y() >= 0 &&
             child->height() > point.y() ) {
            if ( child->inherits( "qan::Group" ) ) {  // For group, look in group childs
                qan::Group* group = qobject_cast<qan::Group*>( child );
                // FIXME QAN3
                /*
                if ( group != nullptr &&
                     group->getContainer() != nullptr ) {
                    const QList<QQuickItem *> groupChildren = group->getContainer()->childItems();
                    for (int gc = groupChildren.count()-1; gc >= 0; --gc) {
                        QQuickItem *groupChild = groupChildren.at(gc);
                        point = mapToItem( groupChild, QPointF(x, y) ); // Map coordinates to group child element's coordinate space
                        if ( groupChild->isVisible() &&
                             groupChild->contains( point ) &&
                             point.x() >= 0 &&
                             groupChild->width() > point.x() &&
                             point.y() >= 0 &&
                             groupChild->height() > point.y() ) {
                            return groupChild;
                        }
                    }
                }*/
            }
            return child;
        }
    }
    return nullptr;
}

qan::Group* Graph::groupAt( const QPointF& p, const QSizeF& s ) const
{
    for ( const auto& g : getGroups() ) {
        if ( g == nullptr )
            continue;
        // FIXME QAN3
        //if ( QRectF{ g->position(), QSizeF{ g->width(), g->height() } }.contains( QRectF{ p, s } ) )
        //    return g.get();
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
    if ( edgeDelegate != nullptr ) {
        if ( edgeDelegate != _edgeDelegate.get() ) {
            _edgeDelegate.reset(edgeDelegate);
            QQmlEngine::setObjectOwnership( edgeDelegate, QQmlEngine::CppOwnership );
            emit edgeDelegateChanged();
        }
    }
}

void    Graph::setEdgeDelegate(std::unique_ptr<QQmlComponent> edgeDelegate) noexcept
{
    setEdgeDelegate(edgeDelegate.release());
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
                                        qan::Node* node,
                                        qan::Edge* edge,
                                        qan::Group* group )
{
    if ( component == nullptr ) {
        qWarning() << "qan::Graph::createFromComponent(): Error called with a nullptr delegate component.";
        return nullptr;
    }
    QQuickItem* item = nullptr;
    if ( component->isReady() ) {
        const auto rootContext = qmlContext(this);
        if ( rootContext == nullptr ) {
            qWarning() << "qan::Graph::createFromComponent(): Error can't access to local QML context.";
            return nullptr;
        }
        QObject* object = component->beginCreate(rootContext);
        if ( object != nullptr &&
             !component->isError() ) {
            if ( node != nullptr ) {
                const auto nodeItem = qobject_cast<qan::NodeItem*>(object);
                if ( nodeItem != nullptr )
                    nodeItem->setNode(node);
            } else if ( edge != nullptr ) {
                const auto edgeItem = qobject_cast<qan::EdgeItem*>(object);
                if ( edgeItem != nullptr ) {
                    edgeItem->setEdge(edge);
                    edgeItem->setGraph(this);
                }
            } else if ( group != nullptr ) {
                const auto groupItem = qobject_cast<qan::GroupItem*>(object);
                if ( groupItem != nullptr )
                    groupItem->setGroup(group);
            }
            component->completeCreate();
            if ( !component->isError() ) {
                QQmlEngine::setObjectOwnership( object, QQmlEngine::CppOwnership );
                item = qobject_cast< QQuickItem* >( object );
                item->setVisible( true );
                item->setParentItem( getContainerItem() );
            } // Note QAN3: There is no leak until cpp ownership is set
        } else {
            qWarning() << "qan::Graph::createFromComponent(): Failed to create a concrete QQuickItem from QML component:";
            qWarning() << "\t" << component->errorString();
        }
    } else {
        qWarning() << "qan::Graph::createFromComponent(): QML component is not ready:";
        qWarning() << "\t" << component->errorString();
    }

    return item;
}

void Graph::setCppOwnership( QQuickItem* item )
{
    if ( item == nullptr )
        return;
    QQmlEngine::setObjectOwnership( item, QQmlEngine::CppOwnership );
    for ( auto childItem: item->childItems() )
        qan::Graph::setCppOwnership( childItem );
}

QQuickItem* Graph::createRectangle( QQuickItem* parent )
{
    // Initialize rectangle component for the first factory call.
    if ( _rectangleComponent == nullptr ) {
        QQmlEngine* engine = qmlEngine( this );
        if ( engine != nullptr ) {
            _rectangleComponent = std::make_unique<QQmlComponent>(engine);
            QString componentQml = "import QtQuick 2.7\n  Rectangle{ }";
            _rectangleComponent->setData( componentQml.toUtf8(), QUrl() );
            if ( !_rectangleComponent->isReady() ) {
                qWarning() << "qan::Graph::createRectangle(): Error: Can't create at Qt Quick Rectangle object:";
                qWarning() << "\t" << componentQml;
                qWarning() << "\tQML Component status=" << _rectangleComponent->status();
                qWarning() << "\tQML Component errors=" << _rectangleComponent->errors();
            }
        }
    }

    // Create a Qt Quick Rectangle object
    if ( _rectangleComponent != nullptr &&
         _rectangleComponent->isReady() ) {
        QObject* object = _rectangleComponent->create();
        QQuickItem* rectangle = qobject_cast< QQuickItem* >( object );
        if ( rectangle != nullptr ) {
            rectangle->setVisible( false );
            if ( parent != nullptr )
                rectangle->setParentItem( parent );
            QQmlEngine::setObjectOwnership( rectangle, QQmlEngine::CppOwnership );
        }
        if ( rectangle == nullptr && object != nullptr ) {
            qWarning() << "qan::Graph::createRectangle(): Error: A Qt Quick Object has been created, but it is not a Quick Canvas.";
            delete object;      // Somtehing has been created, but it is not a rectangle !
            return nullptr;
        }
        return rectangle;
    } else {
        qWarning() << "qan::Graph::createRectangle(): Error: Can't create a Qt Quick Rectangle Object.";
    }
    return nullptr;
}
//-----------------------------------------------------------------------------

/* Graph Factories *///--------------------------------------------------------
qan::Node*  Graph::insertNode( QQmlComponent* nodeComponent )
{
    if ( nodeComponent == nullptr )
        nodeComponent = _nodeDelegate.get();
    if ( nodeComponent == nullptr ) {
        qWarning() << "qan::Graph::insertNode(): Error: Can't find a valid node delegate component.";
        return nullptr;
    }
    auto node = std::make_shared<qan::Node>();
    if ( node ) {
        QQmlEngine::setObjectOwnership( node.get(), QQmlEngine::CppOwnership );
        qan::NodeItem* nodeItem = static_cast<qan::NodeItem*>( createFromComponent( nodeComponent, node.get() ) );
        if ( nodeItem != nullptr ) {
            nodeItem->setNode(node.get());
            auto notifyNodeClicked = [this,&node] (qan::NodeItem* nodeItem, QPointF p) {
                if ( _itemNodeMap.contains(nodeItem) )
                    emit this->nodeClicked(node.get(), p);
            };
            connect( nodeItem, &qan::NodeItem::nodeClicked, notifyNodeClicked );

            auto notifyNodeRightClicked = [this, &node] (qan::NodeItem* nodeItem, QPointF p) {
                if ( _itemNodeMap.contains(nodeItem) )
                    emit this->nodeRightClicked(node.get(), p);
            };
            connect( nodeItem, &qan::NodeItem::nodeRightClicked, notifyNodeRightClicked );

            auto notifyNodeDoubleClicked = [this, &node] (qan::NodeItem* nodeItem, QPointF p) {
                if ( _itemNodeMap.contains(nodeItem) )
                    emit this->nodeDoubleClicked(node.get(), p);
            };
            connect( nodeItem, &qan::NodeItem::nodeDoubleClicked, notifyNodeDoubleClicked );
            node->setItem(nodeItem);

            // FIXME QAN3 à faire uniqument lorsque l'item est selectionné bordel
            //node->setSelectionItem( createRectangle( node ) );
        }
        GTpoGraph::insertNode( node );
    } else
        qWarning() << "qan::Graph::insertNode(): Warning: Node creation failed with the corresponding delegate";
    return node.get();
}

template < class Node_t >
qan::Node*  Graph::insertNode()
{
    const auto node = std::make_shared<Node_t>();
    setCppOwnership(node.get());
    const qan::NodeItem* nodeItem = qobject_cast<qan::NodeItem*>(Node_t::createDelegate());
    if ( node != nullptr &&
         nodeItem  != nullptr ) {
        nodeItem->setNode(node);
        auto notifyNodeClicked = [this,&node] (qan::NodeItem* nodeItem, QPointF p) {
            if ( _itemNodeMap.contains(nodeItem) )
                emit this->nodeClicked(node.get(), p);
        };
        connect( nodeItem, &qan::NodeItem::nodeClicked, notifyNodeClicked );

        auto notifyNodeRightClicked = [this, &node] (qan::NodeItem* nodeItem, QPointF p) {
            if ( _itemNodeMap.contains(nodeItem) )
                emit this->nodeRightClicked(node.get(), p);
        };
        connect( nodeItem, &qan::NodeItem::nodeRightClicked, notifyNodeRightClicked );

        auto notifyNodeDoubleClicked = [this, &node] (qan::NodeItem* nodeItem, QPointF p) {
            if ( _itemNodeMap.contains(nodeItem) )
                emit this->nodeDoubleClicked(node.get(), p);
        };
        connect( nodeItem, &qan::NodeItem::nodeDoubleClicked, notifyNodeDoubleClicked );
        node->setItem(nodeItem);

        // FIXME QAN3 à faire uniqument lorsque l'item est selectionné bordel
        //node->setSelectionItem( createRectangle( node ) );
        GTpoGraph::insertNode( node );
    } else {
        if ( node != nullptr )
            delete node;
        if ( nodeItem != nullptr )
            delete nodeItem;
        qWarning() << "qan::Graph::insertNode(): Warning: Node creation failed with the corresponding delegate";
    }
    return node;
}

void    Graph::removeNode( qan::Node* node )
{
    if ( node == nullptr )
        return;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return; }
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
            else if ( qobject_cast<qan::Edge*>(destination) != nullptr )
                return insertEdge( sourceNode, qobject_cast<qan::Edge*>( destination ), edgeComponent );
    }
    qWarning() << "qan::Graph::insertEdge(): Error: Unable to find a valid insertEdge() method for arguments " << source << " and " << destination;
    return nullptr;
}

qan::Edge*  Graph::insertEdge( qan::Node* source, qan::Node* destination, QQmlComponent* edgeComponent )
{
    if ( source == nullptr ||
         destination == nullptr )
        return nullptr;
    if ( edgeComponent == nullptr )
        edgeComponent = _edgeDelegate.get();
    if ( edgeComponent == nullptr )
        return nullptr;
    auto edge = std::make_shared<qan::Edge>();
    try {
        QQmlEngine::setObjectOwnership( edge.get(), QQmlEngine::CppOwnership );
        auto edgeItem = static_cast< qan::EdgeItem* >( createFromComponent( edgeComponent, nullptr, edge.get() ) );
        if ( edgeItem != nullptr ) {
            edge->setItem(edgeItem);
            edgeItem->setSourceItem( source->getItem() );
            edgeItem->setDestinationItem( destination->getItem() );

            WeakNode sharedSource{ source->shared_from_this() };
            WeakNode sharedDestination{ destination->shared_from_this() };
            edge->setSrc( sharedSource );
            edge->setDst( sharedDestination );
            GTpoGraph::insertEdge( SharedEdge{edge} );  // Note: Do not use shared_from_this()

            qan::EdgeStyle* defaultStyle = qobject_cast< qan::EdgeStyle* >( getStyleManager()->getDefaultEdgeStyle( "qan::Edge" ) );
            if ( defaultStyle != nullptr )
                edge->setStyle( defaultStyle );
            // FIXME QAN3
            /*
            edge->setVisible( true );
            edge->updateItem();

            // FIXME QAN3: Remove SIGNAL()/SLOT() SYNTAX
            connect( edge, SIGNAL( edgeClicked( QVariant, QVariant ) ),
                     this, SIGNAL( edgeClicked( QVariant, QVariant ) ) );
            connect( edge, SIGNAL( edgeRightClicked( QVariant, QVariant ) ),
                     this, SIGNAL( edgeRightClicked( QVariant, QVariant ) ) );
            connect( edge, SIGNAL( edgeDoubleClicked( QVariant, QVariant ) ),
                     this, SIGNAL( edgeDoubleClicked( QVariant, QVariant ) ) );
            */
        }
    } catch ( gtpo::bad_topology_error e ) {
        qWarning() << "qan::Graph::insertEdge(): Error: Topology error:" << e.what();
    }
    catch ( ... ) {
        qWarning() << "qan::Graph::insertEdge(): Error: Topology error.";
    }
    return edge.get();
}

qan::Edge*  Graph::insertEdge( qan::Node* source, qan::Edge* destination, QQmlComponent* edgeComponent )
{
    if ( source == nullptr ||
         destination == nullptr )
        return nullptr;
    if ( edgeComponent == nullptr )
        return nullptr;
    qan::Edge* edge = nullptr;
    try {
        edge = new qan::Edge{};
        if ( edge != nullptr ) {
            // FIXME QAN3
            //edge = static_cast< qan::Edge* >( createFromComponent( edgeComponent ) );
            //edge->setSourceItem( source );
            //edge->setDestinationEdge( destination );

            WeakNode sourcePtr{ source->shared_from_this() };
            WeakEdge destinationPtr{ destination->shared_from_this() };
            edge->setSrc( sourcePtr );
            edge->setHDst( destinationPtr );
            GTpoGraph::insertEdge( SharedEdge{edge} );  // Note: Do not use shared_from_this()

            qan::EdgeStyle* defaultStyle = qobject_cast< qan::EdgeStyle* >( getStyleManager()->getDefaultEdgeStyle( "qan::Edge" ) );
            if ( defaultStyle != nullptr )
                edge->setStyle( defaultStyle );

            // FIXME QAN3
            /*edge->setVisible( true );
            edge->updateItem();
            connect( edge, SIGNAL( edgeClicked( QVariant, QVariant ) ),
                     this, SIGNAL( edgeClicked( QVariant, QVariant ) ) );
            connect( edge, SIGNAL( edgeRightClicked( QVariant, QVariant ) ),
                     this, SIGNAL( edgeRightClicked( QVariant, QVariant ) ) );
            connect( edge, SIGNAL( edgeDoubleClicked( QVariant, QVariant ) ),
                     this, SIGNAL( edgeDoubleClicked( QVariant, QVariant ) ) );
                     */
        }
    } catch ( gtpo::bad_topology_error e ) {
        qWarning() << "qan::Graph::insertEdge(): Error: Topology error:" << e.what();
    }
    catch ( ... ) {
        qWarning() << "qan::Graph::insertEdge(): Error: Topology error.";
    }
    return edge;
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
    if ( edge != nullptr )
        GTpoGraph::removeEdge( edge->shared_from_this() );
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
/*Graph::WeakGroup   Graph::createGroup( const std::string& className )
{
    if ( className.size() == 0 )
        return WeakGroup{};
    qan::Group* group = insertGroup( QString::fromStdString( className ) );
    if ( group != nullptr ) {
        // FIXME QAN3
        /*
        connect( group, &qan::Group::groupClicked,
                 this,  &qan::Graph::groupClicked );
        connect( group, &qan::Group::groupRightClicked,
                 this,  &qan::Graph::groupRightClicked );
        connect( group, &qan::Group::groupDoubleClicked,
                 this,  &qan::Graph::groupDoubleClicked );
        */
//    }
//    return ( group != nullptr ? group->shared_from_this() : WeakGroup{} );
//}

qan::Group* Graph::insertGroup()
{
    QQmlComponent* groupComponent{ _groupDelegate.get() };
    if ( groupComponent == nullptr ) {
        qWarning() << "qan::Graph::insertGroup(): Error: Can't find a valid group delegate component.";
        return nullptr;
    }
    //auto group = std::make_shared<qan::Group>();
    auto group = new qan::Group{};
    if ( group != nullptr ) {
        QQmlEngine::setObjectOwnership( group, QQmlEngine::CppOwnership );
        qan::GroupItem* groupItem = static_cast<qan::GroupItem*>( createFromComponent( groupComponent, nullptr, nullptr, group ) );
        if ( groupItem != nullptr ) {
            groupItem->setGroup(group);

            group->setItem(groupItem);

        // FIXME QAN3
        //auto group =  static_cast< qan::Group* >( createFromComponent( groupComponent ) );
        //GTpoGraph::insertGroup( group );
        //QQmlEngine::setObjectOwnership( group, QQmlEngine::CppOwnership );

        // FIXME QAN3
        /*
        connect( group.get(), &qan::Group::groupClicked,
                 this, &qan::Graph::groupClicked );
        connect( group.get(), &qan::Group::groupRightClicked,
                 this, &qan::Graph::groupRightClicked );
        connect( group.get(), &qan::Group::groupDoubleClicked,
                 this, &qan::Graph::groupDoubleClicked );
        */
        }
    } else
        qWarning() << "qan::Graph::insertGroup(): Warning: Group creation failed.";
    return group;
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
    // FIXME QAN3
    /*WeakGroup weakGroup;
    try {
        weakGroup = WeakGroup{ group->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return false; }*/
    Q_ASSERT(false);
    //return gtpo::GenGraph< qan::GraphConfig >::hasGroup( weakGroup );
    return false;
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
    _selectionColor = selectionColor;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item

        // FIXME QAN3
        /*if ( node != nullptr &&
             node->getSelectionItem() != nullptr ) {
            QQuickItem* selectionItem = node->getSelectionItem();
            selectionItem->setProperty( "color", QVariant::fromValue( QColor(0,0,0,0) ) );
            QObject* rectangleBorder = node->getSelectionItem()->property( "border" ).value<QObject*>();
            if ( rectangleBorder != nullptr )
                rectangleBorder->setProperty( "color", selectionColor );
        }*/
    }
    emit selectionColorChanged();
}

void    Graph::setSelectionWeight( qreal selectionWeight ) noexcept
{
    if ( qFuzzyCompare( selectionWeight, _selectionWeight ) )   // Never 0
        return;
    _selectionWeight = selectionWeight;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        // FIXME QAN3
        /*if ( node != nullptr )
            node->configureSelectionItem( selectionWeight, getSelectionMargin() );
            */
    }
    emit selectionWeightChanged();
}

void    Graph::setSelectionMargin( qreal selectionMargin ) noexcept
{
    if ( qFuzzyCompare( selectionMargin, _selectionMargin ) )   // Never 0
        return;
    _selectionMargin = selectionMargin;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        // FIXME QAN3
        /*
        if ( node != nullptr )
            node->configureSelectionItem( getSelectionWeight(), selectionMargin );
        */
    }
    emit selectionMarginChanged();
}

bool    Graph::selectNode( qan::Node& node, Qt::KeyboardModifiers modifiers )
{
    if ( getSelectionPolicy() == SelectionPolicy::NoSelection )
        return false;

    bool selectNode{ false };
    bool ctrlPressed = modifiers & Qt::ControlModifier;

    if ( node.getSelected() ) {
        if ( ctrlPressed )          // Click on a selected node + CTRL = deselect node
            removeFromSelection( node );
    } else {
        switch ( getSelectionPolicy() ) {
        case SelectionPolicy::SelectOnClick:
            selectNode = true;        // Click on an unselected node with SelectOnClick = select node
            if ( !ctrlPressed )
                clearSelection();
            break;
        case SelectionPolicy::SelectOnCtrlClick:
            selectNode = ctrlPressed; // Click on an unselected node with CTRL pressed and SelectOnCtrlClick = select node
            break;
        case SelectionPolicy::NoSelection:
        default: break;
        }
    }
    if ( selectNode ) {
        addToSelection( node );
        return true;
    }
    return false;
}

void    Graph::addToSelection( qan::Node& node )
{
    if ( !_selectedNodes.contains( &node ) ) {
        _selectedNodes.append( &node );

        // FIXME QAN3
        //node.configureSelectionItem( getSelectionColor(), getSelectionWeight(), getSelectionMargin() );
        node.setSelected( true );
    }
}

void    Graph::removeFromSelection( qan::Node& node )
{
    if ( _selectedNodes.contains( &node ) )
        _selectedNodes.remove( &node );
    node.setSelected( false );
}

void    Graph::clearSelection()
{
    for ( auto& node : _selectedNodes )
        if ( node != nullptr )
            node->setSelected( false );
    _selectedNodes.clear();
}

void    Graph::mousePressEvent( QMouseEvent* event )
{
    if ( event->button() == Qt::LeftButton ) {
        clearSelection();
        forceActiveFocus();
    }
    event->ignore();
    qan::GraphConfig::GraphBase::mousePressEvent( event );
}
//-----------------------------------------------------------------------------


/* Graph Initialization Management *///----------------------------------------
void    Graph::initializeRandom( int nodeCount,
                                 int   minOutNodes, int maxOutNodes,
                                 qreal minWidth, qreal maxWidth,
                                 qreal minHeight, qreal maxHeight,
                                 QRectF br )
{
    if ( !br.isValid() || br.isEmpty() )
        return;
    RandomGraph::RandomConfig rc;
    rc.nodeClassName = "qan::Node";
    rc.edgeClassName = "qan::Edge";
    rc.nodeCount = nodeCount;
    rc.outNodeRng = std::make_pair( minOutNodes, maxOutNodes );
    rc.widthRng = std::make_pair( minWidth, maxWidth );
    rc.heightRng = std::make_pair( minHeight, maxHeight );
    rc.xRng = std::make_pair( 0, br.width() - maxWidth );
    rc.yRng = std::make_pair( 0, br.height() - maxHeight );
    RandomGraph::generate< qan::Graph >( *this, rc );
    qreal nz{0.};
    for ( auto& node : getNodes() ) {
        // FIXME QAN3
        //if ( node->item() )
        //    node->item()->setZ(nz);   // Force valid z values
        nz += 1.0;
    }
}
//-----------------------------------------------------------------------------

} // ::qan

