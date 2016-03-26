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
// This file is a part of the Qanava software.
//
// \file	qanNode.h
// \author	benoit@qanava.org
// \date	2004 February 15
//-----------------------------------------------------------------------------

#ifndef qanNode_h
#define qanNode_h

// QT headers
#include <QPointF>
#include <QPolygonF>

// QuickQanava headers
#include "./qanConfig.h"
#include "./qanEdge.h"
#include "./qanStyle.h"

//! Main QuickQanava namespace
namespace qan { // ::qan

class Graph;
class Group;

/*! \brief Base class for modelling nodes with attributes and an in/out edges list in a qan::Graph graph.
 *
 * \note If your application does not need drag'n'drop support (ie group insertion via dra'n'drop or ConnectorDropNode are not used nor necessary), consider disabling
 * drag'n'drop support by setting the \c acceptsDrops and \c droppable properties to false, it could improve performances significantly.
 *
 * \nosubgrouping
*/
class Node : public gtpo::GenNode< qan::Config >
{
    /*! \name Node Object Management *///--------------------------------------
    //@{
    Q_OBJECT
public:
    //! Node constructor.
    Node( QQuickItem* parent = 0 );
    /*! Remove any childs node who have no QQmlEngine::CppOwnership.
     *
     */
    virtual ~Node( );
private:
    Q_DISABLE_COPY( Node )
signals:
    /*! Emitted whenever the node changes (either its position, size, style or internal attribute).
     *
     *  Could be usefull to detect any node change (usually trought the graph observation interface) for
     *  selective serialization, or to allow graph save only once a modification has occurred.
     * \sa qan::Graph::nodeModified()
     */
    void            modified( qan::Node* );
    // ^^^^ FIXME 20160323 remove that, notification is made trough behaviours not direct signals.
public:
    auto            getClassName() -> std::string { return getDynamicClassName(); }
    virtual auto    getDynamicClassName() -> std::string { return "qan::Node"; }
public:
    //! Shortcut to gtpo::GenNode<>::getGraph().
    qan::Graph*     getGraph();

public:
    // Qt property for gtpo::Node serializable standard property.
    Q_PROPERTY( bool serializable READ getSerializable WRITE setSerializableObs NOTIFY serializableChanged )
    void            setSerializableObs( bool serializable ) { setSerializable( serializable ); emit serializableChanged( ); }
signals:
    void            serializableChanged();
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node DnD Management *///-----------------------------------------
    //@{
public:
    /*! Define if the node could actually be dragged by mouse.
     *
     * Set this property to true if you want to allow this node to be moved by mouse (if false, the node position is
     * fixed and should be changed programmatically).
     * Default to true.
     */
    Q_PROPERTY( bool draggable READ getDraggable WRITE setDraggable NOTIFY draggableChanged )
    void             setDraggable( bool draggable ) { _draggable = draggable; emit draggableChanged( ); }
    bool             getDraggable( ) { return _draggable; }
private:
    bool            _draggable = true;
signals:
    void            draggableChanged( );

public:
    /*! Define if the node could actually be dropped in another node or in a node group.
     *
     * Set this property to true if you want to allow this node to be dropped in a qan::Group automatically.
     * Default to true.
     * Setting this property to false may lead to a significant performance improvement if group dropping is not needed.
     */
    Q_PROPERTY( bool dropable READ getDropable WRITE setDropable NOTIFY dropableChanged )
    void             setDropable( bool dropable ) { _dropable = dropable; emit dropableChanged( ); }
    bool             getDropable( ) { return _dropable; }
private:
    bool            _dropable = true;
signals:
    void            dropableChanged( );

public:
    /*! Define if the node actually accept drops from other node (default to true).
     *
     * This property allow use of DropNode component that is a node droppable on another node (that has acceptDrops=true),
     * it it actually used for EdgeCreatorDropNode component to dynamically connect edges.
     *
     * Default to true.
     *
     * Setting this property to false may lead to a significant performance improvement if DropNode support is not needed.
     */
    Q_PROPERTY( bool acceptDrops READ getAcceptDrops WRITE setAcceptDrops NOTIFY acceptDropsChanged )
    void             setAcceptDrops( bool acceptDrops ) { _acceptDrops = acceptDrops; setFlag( QQuickItem::ItemAcceptsDrops, acceptDrops ); emit acceptDropsChanged( ); }
    bool             getAcceptDrops( ) { return _acceptDrops; }
private:
    bool            _acceptDrops = true;
signals:
    void            acceptDropsChanged( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Appearance Management *///---------------------------------------
    //@{
public:
    virtual void    geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry ) override;

private:
    using SharedNodeStyle = QSharedPointer< qan::NodeStyle >;
    SharedNodeStyle _defaultStyle;
public:
    //! Node current style object (this property is never null, a default style is returned when no style has been manually set).
    Q_PROPERTY( qan::NodeStyle* style READ getStyle WRITE setStyle NOTIFY styleChanged )
    void            setStyle( NodeStyle* style );
    qan::NodeStyle* getStyle( ) const { return _style; }
private:
    qan::NodeStyle* _style;
signals:
    void            styleChanged( );
private slots:
    //! Called when the style associed to this node is destroyed.
    void            styleDestroyed( QObject* style );

public:
    Q_PROPERTY( QString label READ getLabel WRITE setLabel NOTIFY labelChanged )
    void            setLabel( const QString& label ) { _label = label; emit labelChanged( ); }
    QString         getLabel( ) const { return _label; }
private:
    QString         _label = QString{ "" };
signals:
    void            labelChanged( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Intersection Shape Management *///-------------------------------
    //@{
signals:
    //! Emmited whenever the node is clicked (even at the start of a dragging operation).
    void    nodeClicked( qan::Node* node, QPointF p );
    //! Emmited whenever the node is double clicked.
    void    nodeDoubleClicked( qan::Node* node, QPointF p );
    //! Emmited whenever the node is right clicked.
    void    nodeRightClicked( qan::Node* node, QPointF p );

public:
    /*! Polygon used for mouse event clipping, and edge arrow clipping.
     *
     * An intersection shape is automatically generated for rectangular nodes, it can be sets by the user
     * manually with setIntersectionShape() or setIntersectionShapeFromQml() if the node graphical representation is
     * not a rectangle.
     * \sa \ref custom
     */
    Q_PROPERTY( QPolygonF boundingShape READ getBoundingShape WRITE setBoundingShape NOTIFY boundingShapeChanged )
    QPolygonF           getBoundingShape( );
    const QPolygonF     getBoundingShape( ) const;
    void                setBoundingShape( const QPolygonF& boundingShape ) { _boundingShape = boundingShape; emit boundingShapeChanged(); }
signals:
    void                boundingShapeChanged( );
protected:
    QPolygonF           generateDefaultBoundingShape( ) const;
private:
    QPolygonF           _boundingShape;
protected:
    /*! Invoke this method from a concrete node component in QML for non rectangular nodes.
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

    /*! Test if a point in the node local CCS is inside the current intersection shape.
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

    /*! \name Node Group Management *///---------------------------------------
    //@{
public:
    /*! Must be called by the QML component modelling concrete node when a node is dropped on another quick item.
     *
     * For example, on a concrete node item configured that way:
     * \code
     * QanNode { //...
     *     id: rectNode
     *     Drag.active: nodeDragArea.drag.active
     *     Drag.dragType: Drag.Internal
     *     // ...
     * \endcode
     * The corresponding MouseArea that actually implement dragging must call dropNode with the following code:
     * \code
     * MouseArea { //...
     * id: nodeDragArea
     * drag.target: rectNode
     * onReleased: { rectNode.dropNode( rectNode.Drag.target ); }
     * }
     * //...
     * }
     * \endcode
     *
     *  \param target item on which this node has been dropped on (could be nullptr...)
     */
    Q_INVOKABLE virtual void    dropNode( QQuickItem* target );

    /*! Called from a concrete node QML component to inform that a node drop over a target currently occurs (similar to a dropMoveEvent()).
     *
     * \sa qan::Layout::proposeNodeDrop() and qan::Group::proposeNodeDrop() for a more detailled description.
     */
    Q_INVOKABLE virtual void    proposeNodeDrop( QQuickItem* target );

    /*! Ungroup this node from its current group.
     *
     * Method can be called even if the node is not actually part of a group.
     * \sa qan::Group::ungroup()
     * \sa qan::Graph::insertNode()
     */
    Q_INVOKABLE virtual void    ungroup( );

    //! Shortcut to gtpo::GenNode<>::getGroup().
    qan::Group*                 getGroup( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Drag'nDrop Management  *///--------------------------------------
    //@{
protected:
    //! Internally used to manage drag and drop over nodes, override with caution, and call base class implementation.
    virtual void    dragEnterEvent( QDragEnterEvent* event ) override;
    //! Internally used to manage drag and drop over nodes, override with caution, and call base class implementation.
    virtual void    dragMoveEvent( QDragMoveEvent* event ) override;
    //! Internally used to manage drag and drop over nodes, override with caution, and call base class implementation.
    virtual void    dragLeaveEvent( QDragLeaveEvent* event ) override;
    //! Internally used to accept style drops.
    virtual void    dropEvent( QDropEvent* event ) override;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Node )
Q_DECLARE_METATYPE( std::shared_ptr<qan::Node> )
Q_DECLARE_METATYPE( std::weak_ptr<qan::Node> )

#endif // qanNode_h
