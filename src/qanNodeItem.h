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
 * \nosubgrouping
 */
class NodeItem : public QQuickItem
{
    /*! \name Node Object Management *///--------------------------------------
    //@{
    Q_OBJECT
public:
    //! Node constructor.
    explicit NodeItem( QQuickItem* parent = nullptr );
    /*! \brief Remove any childs node who have no QQmlEngine::CppOwnership.
     *
     */
    virtual ~NodeItem();
    NodeItem( const NodeItem& ) = delete;

public:
    Q_PROPERTY( qan::Node* node READ getNode CONSTANT FINAL )
    auto        getNode() noexcept -> qan::Node*;
    auto        getNode() const noexcept -> const qan::Node*;
    inline auto setNode(qan::Node* node) noexcept { _node = node; }
private:
    QPointer<qan::Node> _node{nullptr};

public:
    Q_PROPERTY( qan::Graph* graph READ getGraph )
protected:
    //! Secure shortcut to getNode().getGraph().
    auto    getGraph() const noexcept -> const qan::Graph*;
    //! \copydoc getGraph()
    auto    getGraph() noexcept -> qan::Graph*;

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
protected slots:
    virtual void    onWidthChanged();
    virtual void    onHeightChanged();

public:
    //! Set this property to false to disable node selection (default to true, ie node are selectable by default).
    Q_PROPERTY( bool selectable READ getSelectable WRITE setSelectable NOTIFY selectableChanged FINAL )
    void            setSelectable( bool selectable );
    inline bool     getSelectable() const noexcept { return _selectable; }
    inline bool     isSelectable() const noexcept { return _selectable; }
private:
    bool            _selectable{true};
signals:
    void            selectableChanged();

public:
    Q_PROPERTY( bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged FINAL )
    void            setSelected( bool selected );
    inline bool     getSelected() const noexcept { return _selected; }
private:
    bool            _selected{false};
signals:
    void            selectedChanged();

public:
    /*! \brief Item used to hilight selection (usually a Rectangle quick item).
     *
     */
    Q_PROPERTY( QQuickItem* selectionItem READ getSelectionItem WRITE setSelectionItem NOTIFY selectionItemChanged FINAL )
    inline QQuickItem*  getSelectionItem() { return _selectionItem.get(); }
    void                setSelectionItem( QQuickItem* selectionItem );
private:
    std::unique_ptr< QQuickItem >  _selectionItem{ nullptr };
signals:
    void                selectionItemChanged();

public:
    //! Update selection hilight item with new color, border weight and margin.
    void            configureSelectionItem( QColor selectionColor, qreal selectionWeight, qreal selectionMargin );

    //! Update selection hilight item with new border weight and margin.
    void            configureSelectionItem( qreal selectionWeight, qreal selectionMargin );
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

public:
    /*! \brief Define if the node could actually be dragged by mouse (default to true).
     *
     * Set this property to true if you want to allow this node to be moved by mouse (if false, the node position is
     * fixed and should be changed programmatically).
     *
     * Default to true.
     */
    Q_PROPERTY( bool draggable READ getDraggable WRITE setDraggable NOTIFY draggableChanged FINAL )
    void            setDraggable( bool draggable ) noexcept;
    inline bool     getDraggable( ) const noexcept { return _draggable; }
private:
    bool            _draggable{true};
signals:
    void            draggableChanged();

public:
    /*! \brief Define if the node could actually be dropped in another node or in a node group.
     *
     * Set this property to true if you want to allow this node to be dropped in a qan::Group automatically.
     * Default to true.
     * Setting this property to false may lead to a significant performance improvement if group dropping is not needed.
     */
    Q_PROPERTY( bool dropable READ getDropable WRITE setDropable NOTIFY dropableChanged FINAL )
    void            setDropable( bool dropable ) noexcept;
    inline bool     getDropable() const noexcept { return _dropable; }
private:
    bool            _dropable{true};
signals:
    void            dropableChanged();

public:
    /*! \brief Define if the node actually accept drops from other node (default to true).
     *
     * This property allow use of DropNode component that is a node droppable on another node (that has acceptDrops=true),
     * it it actually used for EdgeCreatorDropNode component to dynamically connect edges.
     *
     * Default to true.
     *
     * Setting this property to false may lead to a significant performance improvement if DropNode support is not needed.
     */
    Q_PROPERTY( bool acceptDrops READ getAcceptDrops WRITE setAcceptDrops NOTIFY acceptDropsChanged FINAL )
    void            setAcceptDrops( bool acceptDrops ) noexcept;
    inline bool     getAcceptDrops() const noexcept { return _acceptDrops; }
private:
    bool            _acceptDrops{true};
signals:
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

public:
    //! \c dragInitialMousePos in window coordinate system.
    inline  auto    beginDragMove( const QPointF& dragInitialMousePos, bool dragSelection = true ) -> void;
    //! \c delta in scene coordinate system.
    inline  auto    dragMove( const QPointF& dragInitialMousePos, const QPointF& delta, bool dragSelection = true ) -> void;
    inline  auto    endDragMove( bool dragSelection = true ) -> void;

public:
    //! Used internally for multiple selection dragging, contain scene position of the node at the beggining of a drag operation.
    auto            getDragInitialPos() const -> const QPointF& { return _dragInitialPos; }

private:
    //! Initial global mouse position at the beginning of a node drag operation.
    QPointF         _dragInitialMousePos{ 0., 0. };
    //! Node position at the beginning of a node drag.
    QPointF         _dragInitialPos{ 0., 0. };
    //! Last group hovered during a node drag (cached to generate a dragLeave signal on qan::Group).
    QPointer< qan::Group >  _lastProposedGroup{ nullptr };

public:
    //! True when the node is currently beeing dragged.
    Q_PROPERTY( bool dragged READ getDragActive WRITE setDragActive NOTIFY dragActiveChanged FINAL )
    void             setDragActive( bool dragActive ) { _dragActive = dragActive; emit dragActiveChanged( ); }
    bool             getDragActive( ) const { return _dragActive; }
private:
    bool            _dragActive = false;
signals:
    void            dragActiveChanged( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Appearance Management *///---------------------------------------
    //@{
private:
    using SharedNodeStyle = QSharedPointer< qan::NodeStyle >;
    SharedNodeStyle _defaultStyle;
public:
    //! Node current style object (this property is never null, a default style is returned when no style has been manually set).
    Q_PROPERTY( qan::NodeStyle* style READ getStyle WRITE setStyle NOTIFY styleChanged FINAL )
    void                    setStyle( NodeStyle* style );
    inline qan::NodeStyle*  getStyle( ) const noexcept { return _style.data(); }
private:
    QPointer<qan::NodeStyle>    _style;
signals:
    void            styleChanged();
private slots:
    //! Called when the style associed to this node is destroyed.
    void            styleDestroyed( QObject* style );
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
