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
// \file	qanGraph.cpp
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Std headers
#include <memory>

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
    gtpo::graph< qan::Config >( parent )
{
    setContainerItem(this);
    setAntialiasing(true);
    setSmooth(true);
    // Note: do not accept mouse buttons, mouse events are captured in
    // GraphView containerItem.
}

void    Graph::classBegin()
{
    setPortDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/Port.qml")));
    setHorizontalDockDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/HorizontalDock.qml")));
    setVerticalDockDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/VerticalDock.qml")));
    setGroupDelegate(createComponent(QStringLiteral("qrc:/QuickQanava/Group.qml")));
    // Note: Do not set a default node delegate, otherwise it would be used instead
    //  of qan::Node::delegate(), just let the user specify one.
        //Ie setNodeDelegate(nullpr);
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

void    Graph::clearGraph() noexcept
{
    qan::Graph::clear();
}

void    Graph::clear() noexcept
{
    _selectedNodes.clear();
    gtpo::graph<qan::Config>::clear();
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

qan::Group* Graph::groupAt( const QPointF& p, const QSizeF& s, const QQuickItem* except ) const
{
    // PRECONDITIONS:
        // s must be valid
        // except can be nullptr
    if (!s.isValid())
        return nullptr;

    // Algorithm:
        // 1. Copy internal group list
        // 2. Order the container of group from maximum to minimum z
        // 3. Return the first group containing rect(p,s)

    // 1.
    std::vector<qan::Group*> groups;
    groups.reserve(static_cast<unsigned int>(get_groups().size()));
    for (const auto& group_ptr : qAsConst(get_groups().getContainer())) {
        const auto group = group_ptr.lock();
        if (group)
            groups.push_back(group.get());
    }

    // 2.
    std::sort(groups.begin(), groups.end(), [](const auto g1, const auto g2) -> bool {
        if (g1 == nullptr || g2 == nullptr)
            return false;
        const auto g1Item = g1->getItem();
        const auto g2Item = g2->getItem();
        if (g1Item == nullptr ||
            g2Item == nullptr)
            return false;
        const auto g1GlobalZ = qan::getItemGlobalZ_rec(g1Item);
        const auto g2GlobalZ = qan::getItemGlobalZ_rec(g2Item);
        return g1GlobalZ > g2GlobalZ;
    });

    // 3.
    if (getContainerItem() == nullptr)
            return nullptr;
    for ( const auto group : qAsConst(groups) ) {
        if ( group &&
             group->getItem() != nullptr &&
             group->getItem() != except) {
            const auto groupItem = group->getItem();

            const auto groupRect =  QRectF{ groupItem->mapToItem(getContainerItem(), QPointF{0., 0.}),
                                            QSizeF{ groupItem->width(), groupItem->height() } };
            if ( groupRect.contains( QRectF{ p, s } ) )
                 return group;
        }
    } // for all groups
    return nullptr;
}

void    Graph::setContainerItem(QQuickItem* containerItem)
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
            auto finalPrimitive = qobject_cast<qan::Group*>(primitive.lock().get());
            if ( finalPrimitive != nullptr &&
                 finalPrimitive->getItem() &&
                 finalPrimitive->getItem()->getSelectionItem() != nullptr )   // Replace only existing selection items
                    finalPrimitive->getItem()->setSelectionItem(this->createSelectionItem(finalPrimitive->getItem()));
        };
        std::for_each(get_groups().begin(), get_groups().end(), updateGroupSelectionItem);
        std::for_each(get_nodes().begin(), get_nodes().end(), updateNodeSelectionItem);
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
auto    Graph::insertNonVisualNode( SharedNode node ) noexcept(false) -> WeakNode
{
    auto weakNode = gtpo_graph_t::insert_node(node);
    auto insertedNode = weakNode.lock();
    if (insertedNode) {
        onNodeInserted(*insertedNode);
        emit nodeInserted(insertedNode.get());
    }
    return weakNode;
}

qan::Node*  Graph::insertNode( QQmlComponent* nodeComponent, qan::NodeStyle* nodeStyle )
{
    return insertNode<qan::Node>(nodeComponent, nodeStyle);
}

bool    Graph::insertNode(const SharedNode& node, QQmlComponent* nodeComponent, qan::NodeStyle* nodeStyle)
{
    // PRECONDITIONS:
        // node must be dereferencable
        // nodeComponent and nodeStyle can be nullptr
    if (!node)
        return false;

    if ( nodeComponent == nullptr ) {
        nodeComponent = _nodeDelegate.get(); // If no delegate component is specified, try the default node delegate
    }
    if ( nodeComponent == nullptr ) {               // Otherwise, throw an error, a visual node must have a delegate
        qWarning() << "qan::Graph::insertNode(SharedNode): Can't find a valid node delegate component.";
        return false;
    }
    if ( nodeComponent->isError() ) {
        qWarning() << "qan::Graph::insertNode(SharedNode): Component error: " << nodeComponent->errors();
        return false;
    }
    try {
        QQmlEngine::setObjectOwnership( node.get(), QQmlEngine::CppOwnership );
        qan::NodeItem* nodeItem = nullptr;
        if (nodeStyle != nullptr ) {
            _styleManager.setStyleComponent(nodeStyle, nodeComponent);
            nodeItem = static_cast<qan::NodeItem*>( createFromComponent( nodeComponent,
                                                                         *nodeStyle,
                                                                         node.get() ) );
        }
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
        return false; // node eventually destroyed by shared_ptr
    }
    catch ( const qan::Error& e ) {
        qWarning() << "qan::Graph::insertNode(): Error: " << e.getMsg();
        return false; // node eventually destroyed by shared_ptr
    }
    catch ( ... ) {
        qWarning() << "qan::Graph::insertNode(): Error: Topology error.";
        return false; // node eventually destroyed by shared_ptr
    }
    const auto nodePtr = node.get();
    if (nodePtr != nullptr) {       // Notify user.
        onNodeInserted(*nodePtr);
        emit nodeInserted(nodePtr);
    }
    return node.get();
}

void    Graph::removeNode( qan::Node* node )
{
    // PRECONDITIONS:
        // node can't be nullptr
    if ( node == nullptr )
        return;
    try {
        onNodeRemoved(*node);
        emit nodeRemoved(node);
        if ( _selectedNodes.contains(node) )
            _selectedNodes.removeAll(node);
        gtpo_graph_t::remove_node( std::static_pointer_cast<Config::final_node_t>(node->shared_from_this()) );
    } catch ( std::bad_weak_ptr ) {
        qWarning() << "qan::Graph::removeNode(): Internal error for node " << node;
        return;
    }
}

int     Graph::getNodeCount() const noexcept { return gtpo_graph_t::get_node_count(); }

void    Graph::onNodeInserted(qan::Node& node) { Q_UNUSED(node); /* Nil */ }

void    Graph::onNodeRemoved(qan::Node& node){ Q_UNUSED(node); /* Nil */ }
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
qan::Edge*  Graph::insertEdge( QObject* source, QObject* destination, QQmlComponent* edgeComponent )
{
    auto sourceNode = qobject_cast<qan::Node*>(source);
    qan::Edge* edge = nullptr;
    if ( sourceNode != nullptr ) {
            if ( qobject_cast<qan::Node*>(destination) != nullptr )
                edge = insertEdge( sourceNode, qobject_cast<qan::Node*>( destination ), edgeComponent );
            else if ( qobject_cast<qan::Group*>(destination) != nullptr )
                edge = insertEdge( sourceNode, qobject_cast<qan::Group*>( destination ), edgeComponent );
            else if ( qobject_cast<qan::Edge*>(destination) != nullptr )
                edge = insertEdge( sourceNode, qobject_cast<qan::Edge*>( destination ), edgeComponent );
    }
    if (edge != nullptr)
        emit edgeInserted(edge);
    if (edge == nullptr)
        qWarning() << "qan::Graph::insertEdge(): Error: Unable to find a valid insertEdge() method for arguments " << source << " and " << destination;
    return edge;
}

qan::Edge*  Graph::insertEdge( qan::Node* source, qan::Node* destination, QQmlComponent* edgeComponent )
{
    // PRECONDITION;
        // source and destination can't be nullptr
    if ( source == nullptr ||
         destination == nullptr )
        return nullptr;
    return insertEdge<qan::Edge>(*source, destination, edgeComponent );
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
}

bool    Graph::configureEdge( qan::Edge& edge, QQmlComponent& edgeComponent, qan::EdgeStyle& style,
                              qan::Node& src, qan::Node* dstNode )
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

    edge.set_src( std::static_pointer_cast<Config::final_node_t>(src.shared_from_this()) );
    if ( dstNode != nullptr )
        edge.set_dst( std::static_pointer_cast<Config::final_node_t>(dstNode->shared_from_this()) );

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
        //sharedSource = WeakNode{ source->shared_from_this() };
        //sharedDestination = WeakNode{ destination->shared_from_this() };
        sharedSource = std::static_pointer_cast<Config::final_node_t>( source->shared_from_this() );
        sharedDestination = std::static_pointer_cast<Config::final_node_t>( destination->shared_from_this() );
    } catch ( std::bad_weak_ptr ) { return; }
    return gtpo_graph_t::remove_edge( sharedSource, sharedDestination );
}

void    Graph::removeEdge( qan::Edge* edge )
{
    using WeakEdge = std::weak_ptr<qan::Edge>;
    if ( edge != nullptr )
        gtpo_graph_t::remove_edge( WeakEdge{edge->shared_from_this()} );
}

bool    Graph::hasEdge( qan::Node* source, qan::Node* destination ) const
{
    if ( source == nullptr || destination == nullptr )
        return false;
    WeakNode sharedSource;
    WeakNode sharedDestination;
    try {
        //sharedSource = WeakNode{ source->shared_from_this() };
        //sharedDestination = WeakNode{ destination->shared_from_this() };
        sharedSource = std::static_pointer_cast<Config::final_node_t>( source->shared_from_this() );
        sharedDestination = std::static_pointer_cast<Config::final_node_t>( destination->shared_from_this() );
    } catch ( std::bad_weak_ptr e ) { return false; }
    return gtpo_graph_t::has_edge( sharedSource, sharedDestination );
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
qan::Group* Graph::insertGroup()
{
    return insertGroup<qan::Group>();
}

bool    Graph::insertGroup(const SharedGroup& group, QQmlComponent* groupComponent, qan::NodeStyle* groupStyle)
{
    // PRECONDITIONS:
        // group must be dereferencable
        // groupComponent and groupStyle can be nullptr
    if (!group)
        return false;
    QQmlEngine::setObjectOwnership(group.get(), QQmlEngine::CppOwnership);

    qan::GroupItem* groupItem = nullptr;
    if ( groupComponent == nullptr )
        groupComponent = _groupDelegate.get();

    if (groupStyle == nullptr)
        groupStyle = qobject_cast<qan::NodeStyle*>(qan::Group::style());
    if (groupStyle != nullptr &&
        groupComponent != nullptr) {
        // FIXME: Group styles are still not well supported (20170317)
        //_styleManager.setStyleComponent(style, edgeComponent);
        groupItem = static_cast<qan::GroupItem*>(createFromComponent(groupComponent,
                                                                     *groupStyle,
                                                                     nullptr,
                                                                     nullptr, group.get()));
    }

    // Insertion strategy:
        // If group delegate (groupItem) failed, insert a non visual node.
        // Otherwise, insert a visual item.
    if (groupItem == nullptr) {
        gtpo_graph_t::insert_group( group );
        return true;
    }
    if (groupItem == nullptr) {
        qWarning() << "qan::Graph::insertGroup(): Error: Either group delegate or group style is invalid or nullptr.";
        return false;
    }
    gtpo_graph_t::insert_group( group );
    groupItem->setGroup(group.get());
    groupItem->setGraph(this);
    group->setItem(groupItem);

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

    { // Send group item to front
        _maxZ += 1.0;
        groupItem->setZ(_maxZ);
    }
    if (group) {       // Notify user.
        onNodeInserted(*group);
        emit nodeInserted(group.get());
    }
    return true;
}

void    Graph::removeGroup( qan::Group* group )
{
    if ( group == nullptr )
        return;

    // Reparent all group childrens (ie node) to graph before destructing the group
    // otherwise all child iems get destructed too
    for ( auto& node : group->get_nodes() ) {
        const auto qanNode = qobject_cast<qan::Node*>(node.lock().get());
        if (qanNode != nullptr &&
            qanNode->getItem() != nullptr &&
            group->getGroupItem() != nullptr )
            group->getGroupItem()->ungroupNodeItem(qanNode->getItem());
    }

    onNodeRemoved(*group);      // group are node, notify group
    emit nodeRemoved(group);    // removed as a node

    if ( _selectedNodes.contains(group) )
        _selectedNodes.removeAll(group);

    auto nodeGroupPtr = std::static_pointer_cast<gtpo_graph_t::group_t>(group->shared_from_this());
    gtpo_graph_t::weak_group_t weakNodeGroupPtr = nodeGroupPtr;
    gtpo_graph_t::remove_group(weakNodeGroupPtr);
}

bool    Graph::hasGroup( qan::Group* group ) const
{
    if ( group == nullptr )
        return false;
    return gtpo_graph_t::has_group(gtpo_graph_t::shared_group_t{group});
}

void    qan::Graph::groupNode( qan::Group* group, qan::Node* node, bool transform) noexcept
{
    // PRECONDITIONS:
        // group and node can't be nullptr
    if ( group == nullptr ||
         node == nullptr )
        return;

    try {
        gtpo_graph_t::group_node( std::static_pointer_cast<Config::final_node_t>(node->shared_from_this()),
                                 std::static_pointer_cast<Group>(group->shared_from_this()) );
        if ( node->get_group().lock().get() == group &&  // Check that group insertion succeed
             group->getGroupItem() != nullptr &&
             node->getItem() != nullptr ) {
            group->getGroupItem()->groupNodeItem(node->getItem(), transform);
        }
    } catch ( ... ) { qWarning() << "qan::Graph::groupNode(): Topology error."; }
}

void    qan::Graph::ungroupNode( qan::Node* node, Group* group ) noexcept
{
    // PRECONDITIONS:
        // node can't be nullptr
        // group can be nullptr
        // if group is nullptr node->getGroup() can't be nullptr
        // if group is not nullptr group should not be different from node->getGroup()
    if ( node == nullptr )
        return;
    if ( group == nullptr &&
         !node->get_group().lock() )
        return;
    if ( group != nullptr &&
         group != node->get_group().lock().get() )
        return;
    group = node->get_group().lock().get();
    if ( group != nullptr &&
         node != nullptr ) {
        try {
            if ( group->getGroupItem() )
                 group->getGroupItem()->ungroupNodeItem(node->getItem());
            gtpo_graph_t::ungroup_node( std::static_pointer_cast<Config::final_node_t>(node->shared_from_this()),
                                       std::static_pointer_cast<Group>(group->shared_from_this()) );
            if (node != nullptr &&
                node->getItem() != nullptr) {
                // Update node z to maxZ: otherwise an undroupped node might be behind it's host group.
                _maxZ += 1.0;
                node->getItem()->setZ(_maxZ);
            }
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
            primitive.getItem()->setSelected(false);
            // Note: graph.removeFromSelection() is called from primitive.selected()
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
}

void    Graph::removeFromSelection( qan::Node& node ) { removeFromSelectionImpl<qan::Node>(node, _selectedNodes); }
void    Graph::removeFromSelection( qan::Group& group ) { removeFromSelectionImpl<qan::Group>(group, _selectedGroups); }

// Note: Called from
void    Graph::removeFromSelection( QQuickItem* item ) {
    const auto nodeItem = qobject_cast<qan::NodeItem*>(item);
    if ( nodeItem != nullptr &&
         nodeItem->getNode() != nullptr ) {
        _selectedNodes.removeAll(nodeItem->getNode());
        //nodeItem->setSelected(false);
    } else {
        const auto groupItem = qobject_cast<qan::GroupItem*>(item);
        if ( groupItem != nullptr &&
             groupItem->getGroup() != nullptr ) {
            _selectedGroups.removeAll(groupItem->getGroup());
        }
    }
}

void    Graph::removeSelection()
{
    const auto& selectedNodes = getSelectedNodes();
    for (const auto node: qAsConst(selectedNodes))
        removeNode(node);
    const auto& selectedGroups = getSelectedGroups();
    for (const auto group: qAsConst(selectedGroups))
        removeGroup(group);
    clearSelection();
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
        // Note 20190501: CppOwnership is set in createFromComponen()
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
    std::for_each(node->get_in_edges().begin(), node->get_in_edges().end(), removeConnectEdge);
    std::for_each(node->get_out_edges().begin(), node->get_out_edges().end(), removeConnectEdge);

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

/* Stacking Management *///----------------------------------------------------
void    Graph::sendToFront(QQuickItem* item)
{
    if (item == nullptr)
        return;

    qan::GroupItem* groupItem = qobject_cast<qan::GroupItem*>(item);
    qan::NodeItem* nodeItem = qobject_cast<qan::NodeItem*>(item);
    if (nodeItem == nullptr)
        return;     // item must be a nodeItem or a groupItem

    // Algorithm:
        // 1. If item is an ungrouped node OR a root group: update maxZ and set item.z to maxZ.
        // 2. If item is a group (or is a node inside a group):
            // 2.1 Collect all parents groups.
            // 2.2 For all parents groups: get group parent item childs maximum (local) z, update group z to maximum value + 1.

    // Return a vector groups ordered from the outer group to the root group
    const auto collectGroups_rec = [](qan::GroupItem* groupItem) -> std::vector<qan::GroupItem*> {

        const auto impl = [](std::vector<qan::GroupItem*>& groups,      // Recursive implementation
                             qan::GroupItem* groupItem,
                             const auto& self) -> void {
            if (groupItem == nullptr)
                return;
            groups.push_back(groupItem);
            const auto parentGroup = groupItem->getGroup() != nullptr ? groupItem->getGroup()->getGroup() :
                                                                        nullptr;
            auto parentGroupItem = parentGroup != nullptr ? parentGroup->getGroupItem() : nullptr;
            if (parentGroupItem != nullptr)
                self(groups, parentGroupItem, self);
        };

        std::vector<qan::GroupItem*> groups;

        // PRECONDITIONS:
            // groupItem can't be nullptr
        if (groupItem == nullptr)
            return groups;
        impl(groups, groupItem, impl);
        return groups;
    };

    const auto graphContainerItem = getContainerItem();
    if (graphContainerItem == nullptr) {
        qWarning() << "qan::Graph::sendToFront(): Can't sent an item to front in a graph with no container item.";
        return;
    }

    if (nodeItem != nullptr &&      // 1. If item is an ungrouped node OR a root group: update maxZ and set item.z to maxZ.
        groupItem == nullptr) {
        _maxZ += 1.;
        nodeItem->setZ(_maxZ);
    } else if (groupItem != nullptr &&      // 1.
               groupItem->parentItem() == graphContainerItem ) {
        _maxZ += 1.;
        groupItem->setZ(_maxZ);
    } else if (groupItem != nullptr) {
        // 2. If item is a group (or is a node inside a group)
        const auto groups = collectGroups_rec(groupItem);       // 2.1 Collect all parents groups.
        for ( const auto groupItem : groups ) {
            if (groupItem == nullptr)
                continue;
            const auto groupParentItem = groupItem->parentItem();
            if (groupParentItem == nullptr)
                continue;       // Should not happen, a group necessary have a parent item.
            if (groupParentItem == graphContainerItem) {        // 2.2.1 We have root group, use global graph maxZ property
                _maxZ += 1.;
                groupItem->setZ(_maxZ);
            } else {
                const auto maxZ = maxChildsZ(groupParentItem);  // 2.2.2
                groupItem->setZ(maxZ + 1.);
            }
        } // For all group items
    }
}

void    Graph::updateMaxZ() noexcept
{
    _maxZ = maxChildsZ(getContainerItem());
}

auto    Graph::maxChildsZ(QQuickItem* item) const noexcept -> qreal {
    if (item == nullptr)
        return 0.;
    qreal maxZ = std::numeric_limits<qreal>::min();
    bool hasChild = false;
    const auto childs = item->childItems();
    for (const auto childItem : qAsConst(childs)) {
        if (childItem != nullptr) {
            hasChild = true;
            maxZ = std::max(maxZ, childItem->z());
        }
    }
    return hasChild ? maxZ : 0.;
};
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
