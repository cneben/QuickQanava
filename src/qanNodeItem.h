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
// \file	qanNodeItem.h
// \author	benoit@destrat.io
// \date	2016 03 04
//-----------------------------------------------------------------------------

#ifndef qanNodeItem_h
#define qanNodeItem_h

// Qt headers
#include <QQuickItem>
#include <QPointF>
#include <QPolygonF>
#include <QDrag>
#include <QPointer>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanStyle.h"
#include "./qanBehaviour.h"
#include "./qanNode.h"
#include "./qanSelectable.h"
#include "./qanDraggable.h"
#include "./qanDraggableCtrl.h"

namespace qan { // ::qan

class Node;
class Graph;

/*! \brief Base class for modelling nodes with attributes and an in/out edges list in a qan::Graph graph.
 *
 * \note If your application does not need drag'n'drop support (ie group insertion via dra'n'drop or VisualConnector are not used nor necessary), consider disabling
 * drag'n'drop support by setting the \c acceptsDrops and \c droppable properties to false, it could improve performances significantly.
 *
 * Bounding shape generation could be done in the following two ways:
 * - Using the default behavior for rectangular node with \c complexBoundingShape set to false (default value), bounding
 * shape is automatically generated on node \c width or \c height change in \c generateDefaultBoundingShape().
 * - Using dedicated code by setting \c complexBoundingShape to true and with a call to \c setBoundingShape() from a custom
 * \c onRequestUpdateBoundingShape() signal handler.
 *
 * Signal \c requestUpdateBoundingShape won't be emitted for non complex bounding shape.
 *
 * Optionally, you could choose to set \c complexBoundingShape to false and override \c generateDefaultBoundingShape() method.
 *
 * \warning NodeItem \c objectName property is set to "qan::NodeItem" and should not be changed in subclasses.
 *
 * \nosubgrouping
 */
class NodeItem : public QQuickItem,
                 public qan::Selectable,
                 public qan::Draggable
{
    /*! \name Node Object Management *///--------------------------------------
    //@{
    Q_OBJECT
    Q_INTERFACES(qan::Selectable)
    Q_INTERFACES(qan::Draggable)
public:
    //! Node constructor.
    explicit NodeItem( QQuickItem* parent = nullptr );
    virtual ~NodeItem();
    NodeItem( const NodeItem& ) = delete;
public:
    qan::DraggableCtrl<qan::Node, qan::NodeItem>& draggableCtrl() { return _draggableCtrl; }
private:
    qan::DraggableCtrl<qan::Node, qan::NodeItem> _draggableCtrl;
public:
    Q_PROPERTY( qan::Node* node READ getNode CONSTANT FINAL )
    auto        getNode() noexcept -> qan::Node*;
    auto        getNode() const noexcept -> const qan::Node*;
    auto        setNode(qan::Node* node) noexcept -> void;
private:
    QPointer<qan::Node> _node{nullptr};

public:
    Q_PROPERTY( qan::Graph* graph READ getGraph )
    auto    setGraph(qan::Graph* graph) noexcept -> void;
protected:
    //! Secure shortcut to getNode().getGraph().
    auto    getGraph() const noexcept -> const qan::Graph*;
    //! \copydoc getGraph()
    auto    getGraph() noexcept -> qan::Graph*;
private:
    QPointer<qan::Graph>    _graph;

public:
    //! Node minimum size (it can't be resized below if resizable is true).
    Q_PROPERTY( QSizeF minimumSize READ getMinimumSize WRITE setMinimumSize NOTIFY minimumSizeChanged FINAL )
    QSizeF          getMinimumSize() const { return _minimumSize; }
    void            setMinimumSize( QSizeF minimumSize ) { _minimumSize = minimumSize; emit minimumSizeChanged( ); }
private:
    QSizeF          _minimumSize = QSizeF{ 100., 45. };
signals:
    void            minimumSizeChanged();
    //@}
    //-------------------------------------------------------------------------

    /*! \name Selection Management *///----------------------------------------
    //@{
public:
    //! Set this property to false to disable node selection (default to true, ie node are selectable by default).
    Q_PROPERTY( bool selectable READ getSelectable WRITE setSelectable NOTIFY selectableChanged FINAL )
    Q_PROPERTY( bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged FINAL )
    //! \brief Item used to hilight selection (usually a Rectangle quick item).
    Q_PROPERTY( QQuickItem* selectionItem READ getSelectionItem WRITE setSelectionItem NOTIFY selectionItemChanged FINAL )
protected:
    virtual void    emitSelectableChanged() { emit selectableChanged(); }
    virtual void    emitSelectedChanged() { emit selectedChanged(); }
    virtual void    emitSelectionItemChanged() { emit selectionItemChanged(); }
signals:
    void            selectableChanged();
    void            selectedChanged();
    void            selectionItemChanged();

protected slots:
    virtual void    onWidthChanged();
    virtual void    onHeightChanged();
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node DnD Management *///-----------------------------------------
    //@{
public:
    // Enable or disable node item resizing (default to true, ie node is resizable).
    Q_PROPERTY( bool resizable READ getResizable WRITE setResizable NOTIFY resizableChanged FINAL )
    inline bool     getResizable() const noexcept { return _resizable; }
    void            setResizable( bool resizable ) noexcept;
protected:
    bool            _resizable{true};
signals:
    void            resizableChanged();
    /*! \name Draggable Management *///----------------------------------------
    //@{
public:
    //! \copydoc qan::Draggable::_draggable
    Q_PROPERTY( bool draggable READ getDraggable WRITE setDraggable NOTIFY draggableChanged FINAL )
    //! \copydoc qan::Draggable::_dragged
    Q_PROPERTY( bool dragged READ getDragged WRITE setDragged NOTIFY draggedChanged FINAL )
    //! \copydoc qan::Draggable::_dropable
    Q_PROPERTY( bool droppable READ getDroppable WRITE setDroppable NOTIFY droppableChanged FINAL )
    //! \copydoc qan::Draggable::_acceptDrops
    Q_PROPERTY( bool acceptDrops READ getAcceptDrops WRITE setAcceptDrops NOTIFY acceptDropsChanged FINAL )
protected:
    virtual void    emitDraggableChanged() override { emit draggableChanged(); }
    virtual void    emitDraggedChanged() override { emit draggedChanged(); }
    virtual void    emitAcceptDropsChanged() override { emit acceptDropsChanged(); }
    virtual void    emitDroppableChanged() override { emit droppableChanged(); }
signals:
    void            draggableChanged();
    void            draggedChanged();
    void            droppableChanged();
    void            acceptDropsChanged();

protected:
    //! Internally used to manage drag and drop over nodes, override with caution, and call base class implementation.
    virtual void    dragEnterEvent( QDragEnterEvent* event ) override;
    //! Internally used to manage drag and drop over nodes, override with caution, and call base class implementation.
    virtual void    dragMoveEvent( QDragMoveEvent* event ) override;
    //! Internally used to manage drag and drop over nodes, override with caution, and call base class implementation.
    virtual void    dragLeaveEvent( QDragLeaveEvent* event ) override;
    //! Internally used to accept style drops.
    virtual void    dropEvent( QDropEvent* event ) override;

    virtual void    mouseDoubleClickEvent(QMouseEvent* event ) override;
    virtual void    mouseMoveEvent(QMouseEvent* event ) override;
    virtual void    mousePressEvent(QMouseEvent* event ) override;
    virtual void    mouseReleaseEvent(QMouseEvent* event ) override;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Style Management *///--------------------------------------------
    //@{
public:
    //! Node current style object (this property is never null, a default style is returned when no style has been manually set).
    Q_PROPERTY( qan::NodeStyle* style READ getStyle WRITE setStyle NOTIFY styleChanged FINAL )
    void                        setStyle( NodeStyle* style ) noexcept;
    inline qan::NodeStyle*      getStyle() const noexcept { return _style.data(); }
private:
    QPointer<qan::NodeStyle>    _style;
signals:
    void                        styleChanged();
private slots:
    //! Called when the style associed to this node is destroyed.
    void                        styleDestroyed( QObject* style );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Intersection Shape Management *///-------------------------------
    //@{
signals:
    //! Emmited whenever the node is clicked (even at the start of a dragging operation).
    void    nodeClicked( qan::NodeItem* node, QPointF p );
    //! Emmited whenever the node is double clicked.
    void    nodeDoubleClicked( qan::NodeItem* node, QPointF p );
    //! Emmited whenever the node is right clicked.
    void    nodeRightClicked( qan::NodeItem* node, QPointF p );

public:
    //! Set to true if the node item has a complex non rounded rectangle bounding shape (and manually install a \c onRequestUpdateBoundingShape() handler in QML delegate).
    Q_PROPERTY( bool complexBoundingShape READ getComplexBoundingShape WRITE setComplexBoundingShape NOTIFY complexBoundingShapeChanged FINAL )
    void            setComplexBoundingShape( bool complexBoundingShape ) noexcept;
    inline bool     getComplexBoundingShape() const noexcept { return _complexBoundingShape; }
private:
    bool            _complexBoundingShape{false};
signals:
    void            complexBoundingShapeChanged( );

public:
    /*! \brief Polygon used for mouse event clipping, and edge arrow clipping.
     *
     * An intersection shape is automatically generated for rectangular nodes, it can be sets by the user
     * manually with setIntersectionShape() or setIntersectionShapeFromQml() if the node graphical representation is
     * not a rectangle.
     * \sa \ref custom
     */
    Q_PROPERTY( QPolygonF boundingShape READ getBoundingShape WRITE setBoundingShape NOTIFY boundingShapeChanged FINAL )
    QPolygonF           getBoundingShape() noexcept;
    void                setBoundingShape( const QPolygonF& boundingShape ) { _boundingShape = boundingShape; emit boundingShapeChanged(); }
signals:
    void                boundingShapeChanged();
    //! signal is emmited when the bounding shape become invalid and should be regenerated from QML.
    void                requestUpdateBoundingShape();
protected:
    QPolygonF           generateDefaultBoundingShape() const;
    //! Generate a default bounding shape (rounded rectangle) and set it as current bounding shape.
    Q_INVOKABLE void    setDefaultBoundingShape();
private:
    QPolygonF           _boundingShape;
protected:
    /*! \brief Invoke this method from a concrete node component in QML for non rectangular nodes.
     * \code
     * // In a component "inheriting" from QanNode:
     *
     * // Define a property in your component
     * property var polygon: new Array( )
     *
     * // In Component.onCompleted():
     * polygon.push( Qt.point( 0, 0 ) )
     * polygon.push( Qt.point( 10, 0 ) )
     * polygon.push( Qt.point( 10, 10 ) )
     * polygon.push( Qt.point( 0, 10 ) )
     * polygon.push( Qt.point( 0, 0 ) )
     * setBoundingShapeFrom( polygon );
     * \endcode
     *
     * \sa isInsideBoundingShape()
     * \ref custom
     */
    Q_INVOKABLE virtual void    setBoundingShape( QVariantList boundingPolygon );

    /*! \brief Test if a point in the node local CCS is inside the current intersection shape.
     *
     * Usefull to accept or reject mouse drag event in QML custom node components.
     * \code
     *  // In the MouseArea component used to drag your node component (with drag.target sets correctly):
     *  onPressed : {
     *   mouse.accepted = ( isInsideBoundingShape( Qt.point( mouse.x, mouse.y ) ) ? true : false )
     *  }
     * \endcode
     *  \ref custom
     *  \sa setBoundShapeFrom()
     */
    Q_INVOKABLE virtual bool    isInsideBoundingShape( QPointF p );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::NodeItem )

#endif // qanNodeItem_h
