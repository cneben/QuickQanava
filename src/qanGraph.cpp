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

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanNavigable.h"
#include "./qanNodeItem.h"
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

void    Graph::componentComplete()
{
    const auto engine = qmlEngine(this);
    if ( engine ) {
        setNodeDelegate(std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/Node.qml")));
        setEdgeDelegate(std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/Edge.qml")));
        setGroupDelegate(std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/Group.qml")));

        _styleManager.setStyleComponent(qan::Node::style(), qan::Node::delegate(this) );
        _styleManager.setStyleComponent(qan::Edge::style(), qan::Edge::delegate(this) );

        // Visual connector initialization
        auto connectorComponent = std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/VisualConnector.qml"));
        if ( connectorComponent ) {
            qan::Style* style = qan::Connector::style();
            if ( style != nullptr ) {
                _connector.reset( qobject_cast<qan::Connector*>(createFromComponent(connectorComponent.get(), *style, nullptr)) );
                emit connectorChanged();
                if (_connector)
                    _connector->setGraph(this);
                _connector->setEnabled(getConnectorEnabled());
                _connector->setVisible(getConnectorEnabled());
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
                                        qan::Style& style,
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
                if ( nodeItem != nullptr ) {
                    nodeItem->setNode(node);
                    nodeItem->setGraph(this);
                    _styleManager.setStyleComponent(&style, component );
                    nodeItem->setStyle(qobject_cast<qan::NodeStyle*>(&style));
                }
            } else if ( edge != nullptr ) {
                const auto edgeItem = qobject_cast<qan::EdgeItem*>(object);
                if ( edgeItem != nullptr ) {
                    edgeItem->setEdge(edge);
                    edgeItem->setGraph(this);
                    _styleManager.setStyleComponent(edgeItem->getStyle(), component );
                }
            } else if ( group != nullptr ) {
                const auto groupItem = qobject_cast<qan::GroupItem*>(object);
                if ( groupItem != nullptr ) {
                    groupItem->setGroup(group);
                    groupItem->setGraph(this);
                    _styleManager.setStyleComponent(groupItem->getStyle(), component );
                }
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

QQuickItem* Graph::createFromComponent( QQmlComponent* component, qan::Style* style )
{
    return ( component != nullptr &&
             style != nullptr ) ? createFromComponent( component, *style, nullptr, nullptr, nullptr ) : nullptr;
}


void    Graph::setCppOwnership( QQuickItem* item )
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
            rectangle->setEnabled( false ); // Avoid node/edge/group selection problems
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
    return insertNode<qan::Node>(nodeComponent);
}

void    Graph::removeNode( qan::Node* node )
{
    if ( node == nullptr )
        return;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return; }
    if ( _selectedNodes.contains(node) )
        _selectedNodes.remove(node);
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
    if ( source == nullptr ||
         destination == nullptr )
        return nullptr;
    return insertEdge<qan::Edge>(*source, destination, nullptr, edgeComponent );
}

qan::Edge*  Graph::insertEdge( qan::Node* source, qan::Edge* destination, QQmlComponent* edgeComponent )
{
    if ( source == nullptr ||
         destination == nullptr )
        return nullptr;
    return insertEdge<qan::Edge>(*source, nullptr, destination, edgeComponent );
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
    QQmlComponent* groupComponent{ _groupDelegate.get() };
    if ( groupComponent == nullptr ) {
        qWarning() << "qan::Graph::insertGroup(): Error: Can't find a valid group delegate component.";
        return nullptr;
    }
    auto group = std::make_shared<qan::Group>();
    if ( group ) {
        QQmlEngine::setObjectOwnership( group.get(), QQmlEngine::CppOwnership );
        qan::Style* style = qan::Group::style();
        if ( style != nullptr ) {
            // Group styles are not well supported (for the moment 20170317)
            //_styleManager.setStyleComponent(style, edgeComponent);
            qan::GroupItem* groupItem = static_cast<qan::GroupItem*>( createFromComponent( groupComponent,
                                                                                           *style,
                                                                                           nullptr,
                                                                                           nullptr,                                                                                           group.get() ) );
            if ( groupItem != nullptr ) {
                groupItem->setGroup(group.get());
                groupItem->setGraph(this);
                group->setItem(groupItem);

                GTpoGraph::insertGroup( group );

                auto notifyGroupClicked = [this] (qan::GroupItem* groupItem, QPointF p) {
                    if ( groupItem != nullptr && groupItem->getGroup() != nullptr )
                        emit this->groupClicked(groupItem->getGroup(), p);
                };
                connect( groupItem, &qan::GroupItem::groupClicked, notifyGroupClicked );

                auto notifyGroupRightClicked = [this] (qan::GroupItem* groupItem, QPointF p) {
                    if ( groupItem != nullptr && groupItem->getGroup() != nullptr )
                        emit this->groupRightClicked(groupItem->getGroup(), p);
                };
                connect( groupItem, &qan::GroupItem::groupRightClicked, notifyGroupRightClicked );

                auto notifyGroupDoubleClicked = [this] (qan::GroupItem* groupItem, QPointF p) {
                    if ( groupItem != nullptr && groupItem->getGroup() != nullptr )
                        emit this->groupDoubleClicked(groupItem->getGroup(), p);
                };
                connect( groupItem, &qan::GroupItem::groupDoubleClicked, notifyGroupDoubleClicked );
            } else
                qWarning() << "qan::Graph::insertGroup(): Warning: Group delegate from QML component creation failed.";
        } else qWarning() << "qan::Graph::insertGroup(): Error: style() factory has returned a nullptr style.";
    }
    return group.get();
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
    return GTpoGraph::hasGroup( WeakGroup{group->shared_from_this()} );
}

auto    qan::Graph::groupNode( qan::Group* group, qan::Node* node ) noexcept(false) -> void
{
    if ( group != nullptr &&
         node != nullptr ) {
        try {
            GTpoGraph::groupNode( WeakGroup{group->shared_from_this()}, node->shared_from_this() );
        } catch ( ... ) { qWarning() << "qan::Graph::groupNode(): Topology error."; }
        if ( node->getGroup().lock().get() == group &&  // Check that group insertion succeed
             group->getItem() != nullptr &&
             node->getItem() != nullptr ) {
            group->getItem()->groupNodeItem(node->getItem());
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
        qDebug() << "qan::Graph::ungroupNode()";
        try {
            if ( group->getItem() )
                group->getItem()->ungroupNodeItem(node->getItem());
            GTpoGraph::ungroupNode( group->shared_from_this(), node->shared_from_this() );
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
    _selectionColor = selectionColor;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        if ( node != nullptr &&
             node->getItem() != nullptr &&
             node->getItem()->getSelectionItem() != nullptr ) {
            QQuickItem* selectionItem = node->getItem()->getSelectionItem();
            selectionItem->setProperty( "color", QVariant::fromValue( QColor(0,0,0,0) ) );
            QObject* rectangleBorder = selectionItem->property( "border" ).value<QObject*>();
            if ( rectangleBorder != nullptr )
                rectangleBorder->setProperty( "color", selectionColor );
        }
    }
    emit selectionColorChanged();
}

void    Graph::setSelectionWeight( qreal selectionWeight ) noexcept
{
    if ( qFuzzyCompare( selectionWeight, _selectionWeight ) )   // Never 0
        return;
    _selectionWeight = selectionWeight;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        if ( node != nullptr &&
             node->getItem() != nullptr )
            node->getItem()->configureSelectionItem( selectionWeight, getSelectionMargin() );
    }
    emit selectionWeightChanged();
}

void    Graph::setSelectionMargin( qreal selectionMargin ) noexcept
{
    if ( qFuzzyCompare( selectionMargin, _selectionMargin ) )   // Never 0
        return;
    _selectionMargin = selectionMargin;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        if ( node != nullptr &&
             node->getItem() != nullptr )
            node->getItem()->configureSelectionItem( getSelectionWeight(), selectionMargin );
    }
    emit selectionMarginChanged();
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
                            qcm::ContainerModel< QVector, Primitive_t* >& selectedPrimitives,
                            qan::Graph& graph )
{
    if ( !selectedPrimitives.contains( &primitive ) ) {
        selectedPrimitives.append( &primitive );
        if ( primitive.getItem() != nullptr ) {
            // Eventually, create and configure node item selection item
            if ( primitive.getItem()->getSelectionItem() == nullptr )
                primitive.getItem()->setSelectionItem(graph.createRectangle(primitive.getItem()));
            primitive.getItem()->configureSelectionItem( graph.getSelectionColor(),
                                                         graph.getSelectionWeight(),
                                                         graph.getSelectionMargin() );
            primitive.getItem()->setSelected( true );
        }
    }
}

void    Graph::addToSelection( qan::Node& node ) { addToSelectionImpl<qan::Node>(node, _selectedNodes, *this); }
void    Graph::addToSelection( qan::Group& group ) { addToSelectionImpl<qan::Group>(group, _selectedGroups, *this); }

template < class Primitive_t >
void    removeFromSelectionImpl( Primitive_t& primitive,
                             qcm::ContainerModel< QVector, Primitive_t* >& selectedPrimitives )
{
    if ( selectedPrimitives.contains( &primitive ) )
        selectedPrimitives.remove( &primitive );
    if ( primitive.getItem() != nullptr )
        primitive.getItem()->setSelected( false );
}

void    Graph::removeFromSelection( qan::Node& node ) { removeFromSelectionImpl<qan::Node>(node, _selectedNodes); }
void    Graph::removeFromSelection( qan::Group& group ) { removeFromSelectionImpl<qan::Group>(group, _selectedGroups); }
void    Graph::removeFromSelection( QQuickItem* item ) {
    const auto nodeItem = qobject_cast<qan::NodeItem*>(item);
    if ( nodeItem != nullptr &&
         nodeItem->getNode() != nullptr ) {
        _selectedNodes.remove(nodeItem->getNode());
    } else {
        const auto groupItem = qobject_cast<qan::GroupItem*>(item);
        if ( groupItem != nullptr &&
             groupItem->getGroup() != nullptr )
            _selectedGroups.remove(groupItem->getGroup());
    }
}

void    Graph::clearSelection()
{
    for ( auto node : _selectedNodes )
        if ( node != nullptr &&
             node->getItem() != nullptr )
            node->getItem()->setSelected( false );
    _selectedNodes.clear();
    for ( auto group : _selectedGroups )
        if ( group != nullptr &&
             group->getItem() != nullptr )
            group->getItem()->setSelected( false );
    _selectedGroups.clear();
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

} // ::qan

