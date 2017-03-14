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
// \file	qanNodeItem.cpp
// \author	benoit@destrat.io
// \date	2016 03 04
//-----------------------------------------------------------------------------

// Qt headers
#include <QPainter>
#include <QPainterPath>

// QuickQanava headers
#include "./qanNode.h"
#include "./qanNodeItem.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Node Object Management *///-------------------------------------------------
NodeItem::NodeItem(QQuickItem* parent) :
    QQuickItem{parent},
    _defaultStyle{ new qan::NodeStyle{ "", QStringLiteral("qan::Node") } },
    _style{ nullptr }
{
    setAcceptDrops( true );
    setFlag( QQuickItem::ItemAcceptsDrops, true );
    setStyle( _defaultStyle.data() );
    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );
    connect( this, &qan::NodeItem::widthChanged,
             this, &qan::NodeItem::onWidthChanged );
    connect( this, &qan::NodeItem::heightChanged,
             this, &qan::NodeItem::onHeightChanged );
}

NodeItem::~NodeItem() { /* Nil */ }

auto    NodeItem::getNode() noexcept -> qan::Node* { return _node.data(); }
auto    NodeItem::getNode() const noexcept -> const qan::Node* { return _node.data(); }

auto    NodeItem::getGraph() const noexcept -> const qan::Graph* { return _node ? _node->getGraph() : nullptr; }
auto    NodeItem::getGraph() noexcept -> qan::Graph* { return _node? _node->getGraph() : nullptr; }
//-----------------------------------------------------------------------------

/* Selection Management *///---------------------------------------------------
void    NodeItem::onWidthChanged()
{
    if ( _selectionItem != nullptr ) {  // Update selection item
        qreal selectionWeight{ 3. }, selectionMargin{ 3. };
        const auto graph = getGraph();
        if ( graph != nullptr ) {
            selectionWeight = graph->getSelectionWeight();
            selectionMargin = graph->getSelectionMargin();
        }
        _selectionItem->setWidth( width() + selectionWeight + ( selectionMargin * 2 ));
    }
    if ( _complexBoundingShape )            // Invalidate actual bounding shape
        emit requestUpdateBoundingShape();
    else setDefaultBoundingShape();
}

void    NodeItem::onHeightChanged()
{
    if ( _selectionItem != nullptr ) {
        qreal selectionWeight{3.}, selectionMargin{3.};
        const auto graph = getGraph();
        if ( graph != nullptr ) {
            selectionWeight = graph->getSelectionWeight();
            selectionMargin = graph->getSelectionMargin();
        }
        _selectionItem->setHeight( height() + selectionWeight + ( selectionMargin * 2 ));
    }
    if ( _complexBoundingShape )            // Invalidate actual bounding shape
        emit requestUpdateBoundingShape();
    else setDefaultBoundingShape();
}

void    NodeItem::setSelectable( bool selectable )
{
    if ( _selectable == selectable )
        return;
    if ( getSelectionItem() != nullptr &&
         getSelectionItem()->isVisible() )
        getSelectionItem()->setVisible( false );
    _selectable = selectable;
    if ( _selected && !_selectable )
        setSelected(false);
    emit selectableChanged();
}

void    NodeItem::setSelected( bool selected )
{
    if ( getSelectionItem() != nullptr &&
         isSelectable() )
        getSelectionItem()->setVisible( selected );

    if ( selected &&                        // Eventually create selection item
         getSelectionItem() == nullptr &&
         getGraph() != nullptr )
        setSelectionItem(getGraph()->createRectangle( this ) );

    if ( _selected != selected ) { // Binding loop protection
        _selected = selected;
        emit selectedChanged( );
    }
}

void NodeItem::setSelectionItem( QQuickItem* selectionItem )
{
    if ( selectionItem == nullptr ) {
        qWarning() << "qan::NodeItem::setSelectionItem(): Error: Can't set a nullptr selection hilight item.";
        return;
    }
    _selectionItem.reset( selectionItem );
    _selectionItem->setParentItem( this );  // Configure Quick item
    const auto graph = getGraph();
    if ( graph != nullptr )
        configureSelectionItem( graph->getSelectionColor(), graph->getSelectionWeight(), graph->getSelectionMargin() );
    _selectionItem->setVisible( isSelectable() && getSelected() );
    emit selectionItemChanged();
}

void    NodeItem::configureSelectionItem( QColor selectionColor, qreal selectionWeight, qreal selectionMargin )
{
    if ( _selectionItem != nullptr ) {
        _selectionItem->setProperty( "color", QVariant::fromValue( QColor(0,0,0,0) ) );
        QObject* rectangleBorder = _selectionItem->property( "border" ).value<QObject*>();
        if ( rectangleBorder != nullptr )
            rectangleBorder->setProperty( "color", QVariant::fromValue<QColor>(selectionColor) );
    }
    configureSelectionItem( selectionWeight, selectionMargin );
}

void    NodeItem::configureSelectionItem( qreal selectionWeight, qreal selectionMargin )
{
    if ( _selectionItem != nullptr ) {
        _selectionItem->setX( -( selectionWeight / 2. + selectionMargin ) );
        _selectionItem->setY( -( selectionWeight / 2. + selectionMargin ) );
        _selectionItem->setWidth( width() + selectionWeight + ( selectionMargin * 2 ));
        _selectionItem->setHeight( height() + selectionWeight + ( selectionMargin * 2 ));
        _selectionItem->setOpacity( 0.80 );
        _selectionItem->setProperty( "radius", 4. );
        QObject* rectangleBorder = _selectionItem->property( "border" ).value<QObject*>();
        if ( rectangleBorder != nullptr ) {
            rectangleBorder->setProperty( "width", selectionWeight );
        }
    }
}
//-----------------------------------------------------------------------------

/* Drag'nDrop Management *///--------------------------------------------------
void    NodeItem::setResizable( bool resizable ) noexcept
{
    if ( resizable != _resizable ) {
        _resizable = resizable;
        emit resizableChanged();
    }
}

void    NodeItem::setDraggable( bool draggable ) noexcept
{
    if ( draggable != _draggable ) {
        _draggable = draggable;
        if ( !draggable ) {
            setDragActive(false);
            endDragMove();
        }
        emit draggableChanged();
    }
}

void    NodeItem::setDropable( bool dropable ) noexcept
{
    if ( dropable != _dropable ) {
        _dropable = dropable;
        emit dropableChanged();
    }
}

void    NodeItem::setAcceptDrops( bool acceptDrops ) noexcept
{
    if ( acceptDrops != _acceptDrops ) {
        _acceptDrops = acceptDrops;
        if ( acceptDrops &&
             !(flags().testFlag(QQuickItem::ItemAcceptsDrops)) )
            setFlag( QQuickItem::ItemAcceptsDrops, acceptDrops );
        emit acceptDropsChanged();
    }
}

void    NodeItem::dragEnterEvent( QDragEnterEvent* event )
{
    if ( getAcceptDrops() ) {
        if ( event->source() == nullptr ) {
            event->accept(); // This is propably a drag initated with type=Drag.Internal, for exemple a connector drop node, accept by default...
            QQuickItem::dragEnterEvent( event );
            return;
        } else { // Get the source item from the quick drag attached object received
            QQuickItem* sourceItem = qobject_cast<QQuickItem*>(event->source());
            QVariant draggedStyle = sourceItem->property( "draggedNodeStyle" ); // The source item (usually a style node or edge delegate must expose a draggedStyle property.
            if ( draggedStyle.isValid() ) {
                event->accept();
                return;
            }
        }
        event->ignore();
        QQuickItem::dragEnterEvent( event );
    }
}

void	NodeItem::dragMoveEvent( QDragMoveEvent* event )
{
    if ( getAcceptDrops() ) {
        event->acceptProposedAction();
        event->accept();
    }
    QQuickItem::dragMoveEvent( event );
}

void	NodeItem::dragLeaveEvent( QDragLeaveEvent* event )
{
    if ( getAcceptDrops() )
        event->ignore();
    QQuickItem::dragLeaveEvent( event );
}

void    NodeItem::dropEvent( QDropEvent* event )
{
    if ( getAcceptDrops() &&
         event->source() != nullptr ) { // Get the source item from the quick drag attached object received
        QQuickItem* sourceItem = qobject_cast<QQuickItem*>(event->source());
        if ( sourceItem != nullptr ) {
            QVariant draggedStyle = sourceItem->property( "draggedNodeStyle" ); // The source item (usually a style node or edge delegate must expose a draggedStyle property).
            if ( draggedStyle.isValid() ) {
                qan::NodeStyle* draggedNodeStyle = draggedStyle.value< qan::NodeStyle* >( );
                if ( draggedNodeStyle != nullptr )
                    setStyle( draggedNodeStyle );
            }
        }
    }
    QQuickItem::dropEvent( event );
}

void    NodeItem::mouseDoubleClickEvent(QMouseEvent* event )
{
    if ( event->button() == Qt::LeftButton )
        emit nodeDoubleClicked( this, event->localPos() );
}

void    NodeItem::mouseMoveEvent(QMouseEvent* event )
{
    if ( getDraggable() &&      // Dragging management
         event->buttons() | Qt::LeftButton &&
         !getDragActive() )
        beginDragMove( event->windowPos() );

    if ( getDraggable() &&
         event->buttons() | Qt::LeftButton &&
         getDragActive() ) {
        // Inspired by void QQuickMouseArea::mouseMoveEvent(QMouseEvent *event)
        // https://code.woboq.org/qt5/qtdeclarative/src/quick/items/qquickmousearea.cpp.html#47curLocalPos
        // Coordinate mapping in qt quick is even more a nightmare than with graphics view...
        // BTW, this code is probably buggy for deep quick item hierarchy.
        QPointF startLocalPos;
        QPointF curLocalPos;
        if ( parentItem() != nullptr ) {
            startLocalPos = parentItem()->mapFromScene( _dragInitialMousePos );
            curLocalPos = parentItem()->mapFromScene( event->windowPos() );
        } else {
            startLocalPos = _dragInitialMousePos;
            curLocalPos = event->windowPos();
        }
        QPointF delta( curLocalPos - startLocalPos );
        dragMove( event->windowPos(), delta );
    }
    QQuickItem::mouseMoveEvent(event);
}

void    NodeItem::mousePressEvent( QMouseEvent* event )
{
    bool accepted = isInsideBoundingShape( event->localPos() );
    if ( accepted ) {
        forceActiveFocus();

        // Selection management
        if ( event->button() == Qt::LeftButton &&
             getNode() &&
             getNode()->getItem() != nullptr &&
             getNode()->getItem()->isSelectable() ) {
            const auto graph = getGraph();
            if ( graph != nullptr )
                graph->selectNode( *getNode(), event->modifiers() );
        }

        // QML notifications
        if ( event->button() == Qt::LeftButton )
            emit nodeClicked( this, event->localPos() );
        else if ( event->button() == Qt::RightButton )
            emit nodeRightClicked( this, event->localPos() );
        event->accept();
    } else
        event->ignore();
    // Note 20160712: Do not call base QQuickItem implementation.
}

void    NodeItem::mouseReleaseEvent( QMouseEvent* event )
{
    Q_UNUSED( event );
    if ( getDragActive() )
        endDragMove();
}

auto    NodeItem::beginDragMove( const QPointF& dragInitialMousePos, bool dragSelection ) -> void
{
    setDragActive( true );
    _dragInitialMousePos = dragInitialMousePos;
    _dragInitialPos = parentItem() != nullptr ? parentItem()->mapToScene( position() ) : position();

    // If there is a selection, keep start position for all selected nodes.
    if ( dragSelection ) {
        const auto graph = getGraph();
        if ( graph != nullptr &&
             graph->hasMultipleSelection() ) {
            for ( auto& selectedNode : graph->getSelectedNodes() )
                if ( selectedNode != nullptr &&
                     selectedNode->getItem() != nullptr &&
                     selectedNode->getItem() != this )
                    selectedNode->getItem()->beginDragMove( dragInitialMousePos, false );
        }
    }
}

auto    NodeItem::dragMove( const QPointF& dragInitialMousePos, const QPointF& delta, bool dragSelection ) -> void
{
    const auto graph = getGraph();
    if ( _node &&
         graph != nullptr ) {
        if ( _node->getGroup().lock() ) {
            // FIXME QAN3
            graph->ungroupNode(_node->getGroup().lock().get(), _node );
            //_node->getGroup().lock()->removeNode( _node );
            _dragInitialMousePos = dragInitialMousePos;  // Note 20160811: Resetting position cache since the node has changed parent and
            // thus position (same scene pos but different local pos)
            _dragInitialPos = parentItem() != nullptr ? parentItem()->mapToScene( position() ) : position();
        }

        QPointF startPos = parentItem() != nullptr ? parentItem()->mapFromScene( _dragInitialPos ) : _dragInitialPos;
        setX( startPos.x() + delta.x() );
        setY( startPos.y() + delta.y() );

        if ( dragSelection ) {
            for ( auto& node : graph->getSelectedNodes() )
                if ( node != nullptr &&
                     node->getItem() != nullptr &&
                     node->getItem() != this )
                    node->getItem()->dragMove( dragInitialMousePos, delta, false );
        }

        // Eventually, propose a node group drop after move
        if ( getDropable() ) {
            qan::Group* group = graph->groupAt( position(), { width(), height() } );
            qDebug() << "qan::NodeItem::dragMove(): group=" << group;
            if ( _node &&
                 group != nullptr &&
                 group->getItem() != nullptr ) {
                group->getItem()->proposeNodeDrop(_node.data());
                _lastProposedGroup = group;
            } else if ( group == nullptr &&
                        _lastProposedGroup != nullptr &&
                        _lastProposedGroup->getItem() != nullptr ) {
                _lastProposedGroup->getItem()->endProposeNodeDrop();
                _lastProposedGroup = nullptr;
            }
        }
    }
}

auto    NodeItem::endDragMove( bool dragSelection ) -> void
{
    if ( getDropable() ) {
        // FIXME QAN3: map to global graph container
        const auto pos = position(); //parentItem() != nullptr ? parentItem()->mapToScene( position() ) : position();
        qan::Group* group = getGraph()->groupAt( pos, { width(), height() } );
        if ( getGraph() != nullptr &&
             group != nullptr &&
             _node )
            getGraph()->groupNode( group, _node.data() );
    }

    setDragActive(false);
    _dragInitialMousePos = { 0., 0. }; // Invalid all cached coordinates when drag ends
    _dragInitialPos = { 0., 0. };
    _lastProposedGroup = nullptr;

    // If there is a selection, end drag for the whole selection
    if ( dragSelection ) {
        qan::Graph* graph = getGraph();
        if ( graph != nullptr &&
             getGraph()->hasMultipleSelection() ) {
            for ( auto& selectedNode : graph->getSelectedNodes() )
                if ( selectedNode != nullptr &&
                     selectedNode->getItem() != nullptr &&
                     selectedNode->getItem() != this )
                    selectedNode->getItem()->endDragMove( false );
        }
    }
}
//-----------------------------------------------------------------------------

/* Appearance Management *///--------------------------------------------------
void    NodeItem::setStyle( NodeStyle* style )
{
    if ( style == _style )
        return;
    if ( style == nullptr )
        style = _defaultStyle.data();
    if ( _style != nullptr &&
         _style != _defaultStyle.data() )  // Every style that is non default is disconnect from this node
        QObject::disconnect( _style, 0, this, 0 );
    _style = style;
    connect( _style, &QObject::destroyed, this, &NodeItem::styleDestroyed );    // Monitor eventual style destruction
    emit styleChanged( );
}

void    NodeItem::styleDestroyed( QObject* style )
{
    if ( style != nullptr &&
         style != _defaultStyle.data() )
        setStyle( _defaultStyle.data() );   // Set default style when current style is destroyed
}
//-----------------------------------------------------------------------------


/* Intersection Shape Management *///------------------------------------------
void    NodeItem::setComplexBoundingShape( bool complexBoundingShape ) noexcept
{
    if ( complexBoundingShape != _complexBoundingShape ) {
        _complexBoundingShape = complexBoundingShape;
        if ( !complexBoundingShape )
            setDefaultBoundingShape();
        else
            emit requestUpdateBoundingShape();
        emit complexBoundingShapeChanged();
    }
}

QPolygonF   NodeItem::getBoundingShape() noexcept
{
    if ( _boundingShape.isEmpty( ) )
        _boundingShape = generateDefaultBoundingShape( );
    return _boundingShape;
}

QPolygonF    NodeItem::generateDefaultBoundingShape( ) const
{
    // Generate a rounded rectangular intersection shape for this node rect new geometry
    QPainterPath path;
    qreal shapeRadius( 5. );
    path.addRoundedRect( QRectF{ 0., 0., width(), height() }, shapeRadius, shapeRadius );
    return path.toFillPolygon( QTransform{} );
}

void    NodeItem::setDefaultBoundingShape( )
{
    setBoundingShape( generateDefaultBoundingShape() );
}

void    NodeItem::setBoundingShape( QVariantList boundingShape )
{
    QPolygonF shape; shape.resize( boundingShape.size() );
    int p = 0;
    for ( const auto& vp : boundingShape )
        shape[p++] = vp.toPointF( );
    _boundingShape = ( !shape.isEmpty( ) ? shape : generateDefaultBoundingShape() );
    emit boundingShapeChanged();
}

bool    NodeItem::isInsideBoundingShape( QPointF p )
{
    if ( _boundingShape.isEmpty() )
        setBoundingShape( generateDefaultBoundingShape() );
    return _boundingShape.containsPoint( p, Qt::OddEvenFill );
}
//-----------------------------------------------------------------------------

} // ::qan
