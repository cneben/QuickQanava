/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

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
// \file	qanNode.cpp
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QPainter>
#include <QPainterPath>

// Qanava headers
#include "./qanNode.h"
#include "./qanGroup.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Node Object Management *///-------------------------------------------------
Node::Node( QQuickItem* parent ) :
    gtpo::GenNode< qan::Config >( parent ),
    _defaultStyle{ new qan::NodeStyle{ "", QStringLiteral("qan::Node") } },
    _style{ nullptr }
{
    setAcceptDrops( true );
    setStyle( _defaultStyle.data() );
    setResizable( true );

    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );

    connect( this, &qan::Node::widthChanged, this, &qan::Node::onWidthChanged );
    connect( this, &qan::Node::heightChanged, this, &qan::Node::onHeightChanged );
}

Node::~Node( ) { }

qan::Graph* Node::getGraph() noexcept
{
    return qobject_cast< qan::Graph* >( gtpo::GenNode< qan::Config >::getGraph() );
}

bool    Node::operator==( const qan::Node& right ) const
{
    return getLabel() == right.getLabel();
}
//-----------------------------------------------------------------------------

/* Selection Management *///---------------------------------------------------
void    Node::onWidthChanged()
{
    qreal selectionWeight{ 3. };
    qreal selectionMargin{ 3. };
    qan::Graph* graph = getGraph();
    if ( graph != nullptr ) {
        selectionWeight = graph->getSelectionWeight();
        selectionMargin = graph->getSelectionMargin();
    }
    if ( _selectionItem != nullptr )
        _selectionItem->setWidth( width() + selectionWeight + ( selectionMargin * 2 ));
}

void    Node::onHeightChanged()
{
    qreal selectionWeight{ 3. };
    qreal selectionMargin{ 3. };
    qan::Graph* graph = getGraph();
    if ( graph != nullptr ) {
        selectionWeight = graph->getSelectionWeight();
        selectionMargin = graph->getSelectionMargin();
    }
    if ( _selectionItem != nullptr )
        _selectionItem->setHeight( height() + selectionWeight + ( selectionMargin * 2 ));
}

void    Node::setSelectable( bool selectable )
{
    if ( _selectable == selectable )
        return;
    if ( getSelectionItem() != nullptr &&
         getSelectionItem()->isVisible() )
        getSelectionItem()->setVisible( false );
    _selectable = selectable;
    emit selectableChanged();
}

void    Node::setSelected( bool selected )
{
    if ( getSelectionItem() != nullptr &&
         isSelectable() )
        getSelectionItem()->setVisible( selected );
    if ( _selected == selected ) // Binding loop protection, and avoid unnecessary redraws
        return;
    _selected = selected;
    emit selectedChanged( );
}

void Node::setSelectionItem( QQuickItem* selectionItem )
{
    if ( selectionItem == nullptr ) {
        qDebug() << "qan::Node::setSelectionItem(): Error: Can't set a nullptr selection hilight item.";
        return;
    }
    _selectionItem.reset( selectionItem );
    _selectionItem->setParentItem( this );  // Configure Quick item
    qan::Graph* graph = getGraph();
    if ( graph != nullptr )
        configureSelectionItem( graph->getSelectionColor(), graph->getSelectionWeight(), graph->getSelectionMargin() );
    _selectionItem->setVisible( isSelectable() && getSelected() );
    emit selectionItemChanged();
}

void    Node::configureSelectionItem( QColor selectionColor, qreal selectionWeight, qreal selectionMargin )
{
    if ( _selectionItem != nullptr ) {
        _selectionItem->setProperty( "color", QVariant::fromValue( QColor(0,0,0,0) ) );
        QObject* rectangleBorder = _selectionItem->property( "border" ).value<QObject*>();
        if ( rectangleBorder != nullptr )
            rectangleBorder->setProperty( "color", QVariant::fromValue<QColor>(selectionColor) );
    }
    configureSelectionItem( selectionWeight, selectionMargin );
}

void    Node::configureSelectionItem( qreal selectionWeight, qreal selectionMargin )
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

/* Behaviours Management *///--------------------------------------------------
void    Node::installBehaviour( std::unique_ptr<qan::NodeBehaviour> behaviour )
{
    addBehaviour( std::move( behaviour ) );
}
//-----------------------------------------------------------------------------

/* Drag'nDrop Management *///--------------------------------------------------
void    Node::dragEnterEvent( QDragEnterEvent* event )
{
    if ( getAcceptDrops() ) {
        if ( event->source() == nullptr ) {
            event->accept(); // This is propably a drag initated with type=Drag.Internal, for exemple a connector drop node, accept by default...
            QQuickItem::dragEnterEvent( event );
            return;
        }

        if ( event->source() != nullptr ) { // Get the source item from the quick drag attached object received
            QVariant source = event->source()->property( "source" );
            if ( source.isValid() ) {
                QQuickItem* sourceItem = source.value< QQuickItem* >( );
                QVariant draggedStyle = sourceItem->property( "draggedNodeStyle" ); // The source item (usually a style node or edge delegate must expose a draggedStyle property.
                if ( draggedStyle.isValid() ) {
                    event->accept();
                    return;
                }
            }
        }
        event->ignore();
        QQuickItem::dragEnterEvent( event );
    }
}

void	Node::dragMoveEvent( QDragMoveEvent* event )
{
    if ( getAcceptDrops() ) {
        event->acceptProposedAction();
        event->accept();
    }
    QQuickItem::dragMoveEvent( event );
}

void	Node::dragLeaveEvent( QDragLeaveEvent* event )
{
    if ( getAcceptDrops() )
        event->ignore();
    QQuickItem::dragLeaveEvent( event );
}

void    Node::dropEvent( QDropEvent* event )
{
    if ( getAcceptDrops() && event->source() != nullptr ) { // Get the source item from the quick drag attached object received
        QVariant source = event->source()->property( "source" );
        if ( source.isValid() ) {
            QQuickItem* sourceItem = source.value< QQuickItem* >( );
            QVariant draggedStyle = sourceItem->property( "draggedNodeStyle" ); // The source item (usually a style node or edge delegate must expose a draggedStyle property.
            if ( draggedStyle.isValid() ) {
                qan::NodeStyle* draggedNodeStyle = draggedStyle.value< qan::NodeStyle* >( );
                if ( draggedNodeStyle != nullptr )
                    setStyle( draggedNodeStyle );
            }
        }
    }
    QQuickItem::dropEvent( event );
}

void    Node::mouseDoubleClickEvent(QMouseEvent* event )
{
    if ( event->button() == Qt::LeftButton )
        emit nodeDoubleClicked( this, event->localPos() );
}

void    Node::mouseMoveEvent(QMouseEvent* event )
{
    if ( getDraggable() &&
         event->buttons() |  Qt::LeftButton &&
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
}

void    Node::mousePressEvent( QMouseEvent* event )
{
    bool accepted = isInsideBoundingShape( event->localPos() );
    if ( accepted ) {
        forceActiveFocus();

        // Selection management
        if ( isSelectable() &&
             event->button() == Qt::LeftButton &&
             getGraph() != nullptr ) {
            qan::Graph* graph = getGraph();
            graph->selectNode( *this, event->modifiers() );
        }

        // QML notifications
        if ( event->button() == Qt::LeftButton )
            emit nodeClicked( this, event->localPos() );
        else if ( event->button() == Qt::RightButton )
            emit nodeRightClicked( this, event->localPos() );

        // Dragging management
        if ( getDraggable() &&
             event->button() == Qt::LeftButton ) {
            beginDragMove( event->windowPos() );
        }
        event->accept();
    } else
        event->ignore();
    // Note 20160712: Do not call base QQuickItem implementation.
}

void    Node::mouseReleaseEvent( QMouseEvent* event )
{
    Q_UNUSED( event );
    if ( getDragActive() )
        endDragMove();
}

auto    Node::beginDragMove( const QPointF& dragInitialMousePos, bool dragSelection ) -> void
{
    setDragActive( true );
    _dragInitialMousePos = dragInitialMousePos;
    _dragInitialPos = parentItem() != nullptr ? parentItem()->mapToScene( position() ) : position();

    // If there is a selection, keep start position for all selected nodes.
    if ( dragSelection ) {
        qan::Graph* graph = getGraph();
        if ( graph != nullptr && getGraph()->hasMultipleSelection() ) {
            for ( auto& selectedNode : graph->getSelectedNodes() )
                if ( selectedNode != nullptr && selectedNode != this )
                    selectedNode->beginDragMove( dragInitialMousePos, false );
        }
    }
}

auto    Node::dragMove( const QPointF& dragInitialMousePos, const QPointF& delta, bool dragSelection ) -> void
{
    if ( getGraph()->getControlNodes().contains( this->shared_from_this() ) )   // Do not try to drag a control node in a group...
         return;

    if ( getQanGroup() != nullptr ) {
        getQanGroup()->removeNode( this );
        _dragInitialMousePos = dragInitialMousePos;  // Note 20160811: Resetting position cache since the node has changed parent and
                                                    // thus position (same scene pos but different local pos)
        _dragInitialPos = parentItem() != nullptr ? parentItem()->mapToScene( position() ) : position();
    }

    QPointF startPos = parentItem() != nullptr ? parentItem()->mapFromScene( _dragInitialPos ) : _dragInitialPos;
    setX( startPos.x() + delta.x() );
    setY( startPos.y() + delta.y() );

    if ( dragSelection ) {
        if ( getGraph() != nullptr ) {
            for ( auto& node : getGraph()->getSelectedNodes() )
                if ( node != nullptr && node != this ) {
                    node->dragMove( dragInitialMousePos, delta, false );
                    /*QPointF startPos = node->getDragInitialPos();
                    node->setX( startPos.x() + delta.x() );
                    node->setY( startPos.y() + delta.y() );*/
                }
        }
    }

    // Eventually, propose a node group drop after move
    qan::Group* group = getGraph()->groupAt( position(), { width(), height() } );
    if ( group != nullptr &&
         getDropable() ) {
            group->proposeNodeDrop( group->getContainer(), this );
            _lastProposedGroup = group;
    }
    if ( group == nullptr && _lastProposedGroup != nullptr ) {
        _lastProposedGroup->endProposeNodeDrop();
        _lastProposedGroup = nullptr;
    }
}

auto    Node::endDragMove( bool dragSelection ) -> void
{
    if ( getDropable() ) {
        qan::Group* group = getGraph()->groupAt( position(), { width(), height() } );
        if ( group != nullptr )
            group->insertNode( this );
    }

    setDragActive( false );
    _dragInitialMousePos = { 0., 0. }; // Invalid all cached coordinates when drag ends
    _dragInitialPos = { 0., 0. };
    _lastProposedGroup = nullptr;

    // If there is a selection, end drag for the whole selection
    if ( dragSelection ) {
        qan::Graph* graph = getGraph();
        if ( graph != nullptr && getGraph()->hasMultipleSelection() ) {
            for ( auto& selectedNode : graph->getSelectedNodes() )
                if ( selectedNode != nullptr && selectedNode != this )
                    selectedNode->endDragMove( false );
        }
    }
}
//-----------------------------------------------------------------------------

/* Appearance Management *///--------------------------------------------------
void    Node::setStyle( NodeStyle* style )
{
    if ( style == _style )
        return;
    if ( style == nullptr )
        style = _defaultStyle.data();
    if ( _style != nullptr &&
         _style != _defaultStyle.data() )  // Every style that is non default is disconnect from this node
        QObject::disconnect( _style, 0, this, 0 );
    _style = style;
    connect( _style, &QObject::destroyed, this, &Node::styleDestroyed );    // Monitor eventual style destruction
    emit styleChanged( );
}

void    Node::styleDestroyed( QObject* style )
{
    if ( style != nullptr &&
         style != _defaultStyle.data() )
        setStyle( _defaultStyle.data() );   // Set default style when current style is destroyed
}
//-----------------------------------------------------------------------------


/* Intersection Shape Management *///------------------------------------------
void    Node::geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry )
{
    QQuickItem::geometryChanged( newGeometry, oldGeometry );
    emit updateBoundingShape(); // Invalidate actual bounding shape
}

QPolygonF   Node::getBoundingShape( )
{
    if ( _boundingShape.isEmpty( ) )
        _boundingShape = generateDefaultBoundingShape( );
    return _boundingShape;
}

QPolygonF    Node::generateDefaultBoundingShape( ) const
{
    // Generate a rounded rectangular intersection shape for this node rect new geometry
    QPainterPath path;
    qreal shapeRadius( 5. );
    path.addRoundedRect( QRectF{ 0., 0., width( ), height( ) }, shapeRadius, shapeRadius );
    return path.toFillPolygon( QTransform{} );
}

void    Node::setDefaultBoundingShape( )
{
    setBoundingShape( generateDefaultBoundingShape() );
}

void    Node::setBoundingShape( QVariantList boundingShape )
{
    QPolygonF shape; shape.resize( boundingShape.size() );
    int p = 0;
    for ( const auto& vp : boundingShape )
        shape[p++] = vp.toPointF( );
    _boundingShape = ( !shape.isEmpty( ) ? shape : generateDefaultBoundingShape() );
    emit boundingShapeChanged();
}

bool    Node::isInsideBoundingShape( QPointF p )
{
    if ( _boundingShape.isEmpty() )
        setBoundingShape( generateDefaultBoundingShape() );
    return _boundingShape.containsPoint( p, Qt::OddEvenFill );
}
//-----------------------------------------------------------------------------

/* Node Group Management *///--------------------------------------------------
void    Node::ungroup( )
{
    if ( getQanGroup() != nullptr &&
         getGraph()->hasGroup( getQanGroup() ) )
        getQanGroup()->removeNode( this );
}

qan::Group* Node::getQanGroup( )
{
    WeakGroup weakGroup = gtpo::GenNode<qan::Config>::getGroup();
    if ( weakGroup.expired() )
        return nullptr;
    return weakGroup.lock().get();
}
//-----------------------------------------------------------------------------

} // ::qan
