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
// \file	qanNode.h
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

#ifndef qanNode_h
#define qanNode_h

// Qt headers
#include <QQuickItem>
#include <QPointF>
#include <QPolygonF>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanEdge.h"
#include "./qanStyle.h"
#include "./qanBehaviour.h"
#include "./qanNodeItem.h"

//! Main QuickQanava namespace
namespace qan { // ::qan

class Graph;
class Group;
class NodeItem;

/*! \brief Base class for modelling nodes with attributes and an in/out edges list in a qan::Graph graph.
 *
 * \note If your application does not need drag'n'drop support (ie group insertion via dra'n'drop or ConnectorDropNode are not used nor necessary), consider disabling
 * drag'n'drop support by setting the \c acceptsDrops and \c droppable properties to false, it could improve performances significantly.
 *
 * \nosubgrouping
*/
class Node : public gtpo::GenNode< qan::GraphConfig >
{
    /*! \name Node Object Management *///--------------------------------------
    //@{
    Q_OBJECT
public:
    //! Node constructor.
    explicit Node(QObject* parent=nullptr);
    /*! \brief Remove any childs node who have no QQmlEngine::CppOwnership.
     *
     */
    virtual ~Node();
    Node( const Node& ) = delete;
public:
    //! Shortcut to gtpo::GenNode<>::getGraph().
    qan::Graph*         getGraph() noexcept;
    //! \copydoc getGraph()
    const qan::Graph*   getGraph() const noexcept;
public:
    /*!
     * \note only label is taken into account for equality comparison.
     */
    bool    operator==( const qan::Node& right ) const;

    QPointer<qan::Node> _selfff;
public:
    Q_PROPERTY( qan::NodeItem* item READ getItem FINAL )
    inline qan::NodeItem*   getItem() noexcept { return _item.data(); }
    void                    setItem(qan::NodeItem* nodeItem) noexcept;
private:
    QPointer<qan::NodeItem> _item;

public:
    // Qt property for gtpo::Node serializable standard property.
    // FIXME QAN3
/*    Q_PROPERTY( bool serializable READ getSerializable WRITE setSerializableObs NOTIFY serializableChanged FINAL )
    void            setSerializableObs( bool serializable ) { setSerializable( serializable ); emit serializableChanged( ); }
signals:
    void            serializableChanged();
    */
public:
    // Qt property for gtpo::Node resizable standard property.
    // FIXME QAN3
    Q_PROPERTY( bool resizable READ getResizable WRITE setResizable NOTIFY resizableChanged FINAL )
    inline bool     getResizable() const noexcept { return _resizable; }
    void            setResizable( bool resizable );
private:
    bool            _resizable{true};
signals:
    void            resizableChanged();

public:
    //! Node minimum size (it can't be resized below if resizable is true).
    Q_PROPERTY( QSizeF minimumSize READ getMinimumSize WRITE setMinimumSize NOTIFY minimumSizeChanged FINAL )
    QSizeF          getMinimumSize() const { return _minimumSize; }
    void            setMinimumSize( QSizeF minimumSize ) { _minimumSize = minimumSize; emit minimumSizeChanged( ); }
private:
    QSizeF          _minimumSize = QSizeF{ 100., 45. };
signals:
    void            minimumSizeChanged();

public:
    //! Read-only abstract item model of this node in nodes.
    Q_PROPERTY( QAbstractItemModel* inNodes READ qmlGetInNodes CONSTANT FINAL )
    QAbstractItemModel* qmlGetInNodes( ) const { return const_cast<QAbstractItemModel*>( static_cast< const QAbstractItemModel* >( &getInNodes() ) ); }

public:
    //! Read-only abstract item model of this node out nodes.
    Q_PROPERTY( QAbstractItemModel* outNodes READ qmlGetOutNodes CONSTANT FINAL )
    QAbstractItemModel* qmlGetOutNodes() const { return const_cast< QAbstractItemModel* >( qobject_cast< const QAbstractItemModel* >( &getOutNodes() ) ); }

public:
    //! Read-only abstract item model of this node out nodes.
    Q_PROPERTY( QAbstractItemModel* outEdges READ qmlGetOutEdges CONSTANT FINAL )
    QAbstractItemModel* qmlGetOutEdges() const { return const_cast< QAbstractItemModel* >( qobject_cast< const QAbstractItemModel* >( &getOutEdges() ) ); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Selection Management *///----------------------------------------
    //@{
public:
    //! Set this property to false to disable node selection (default to true, ie node are selectable by default).
    Q_PROPERTY( bool selectable READ getSelectable WRITE setSelectable NOTIFY selectableChanged FINAL )
    void            setSelectable( bool selectable );
    inline bool     getSelectable( ) const { return _selectable; }
    inline bool     isSelectable( ) const { return _selectable; }
private:
    bool            _selectable = true;
signals:
    void            selectableChanged( );

public:
    Q_PROPERTY( bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged FINAL )
    //! FIXME: Actually, selected state cannot be set programmatically.
    void            setSelected( bool selected );
    inline bool     getSelected( ) const { return _selected; }
private:
    bool            _selected = false;
signals:
    void            selectedChanged( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Behaviours Management *///---------------------------------------
    //@{
public:
    virtual void    installBehaviour( std::unique_ptr<qan::NodeBehaviour> behaviour );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node DnD Management *///-----------------------------------------
    //@{
public:
    /*! \brief Define if the node could actually be dragged by mouse.
     *
     * Set this property to true if you want to allow this node to be moved by mouse (if false, the node position is
     * fixed and should be changed programmatically).
     * Default to true.
     */
    Q_PROPERTY( bool draggable READ getDraggable WRITE setDraggable NOTIFY draggableChanged FINAL )
    void            setDraggable( bool draggable ) noexcept { _draggable = draggable; emit draggableChanged( ); }
    inline bool     getDraggable( ) const noexcept { return _draggable; }
private:
    bool            _draggable = true;
signals:
    void            draggableChanged( );

public:
    /*! \brief Define if the node could actually be dropped in another node or in a node group.
     *
     * Set this property to true if you want to allow this node to be dropped in a qan::Group automatically.
     * Default to true.
     * Setting this property to false may lead to a significant performance improvement if group dropping is not needed.
     */
    Q_PROPERTY( bool dropable READ getDropable WRITE setDropable NOTIFY dropableChanged FINAL )
    void            setDropable( bool dropable ) noexcept { _dropable = dropable; emit dropableChanged( ); }
    inline bool     getDropable( ) const noexcept { return _dropable; }
private:
    bool            _dropable = true;
signals:
    void            dropableChanged( );
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
    void            setStyle( NodeStyle* style );
    qan::NodeStyle* getStyle( ) const { return _style.data(); }
private:
    QPointer<qan::NodeStyle>    _style;
signals:
    void            styleChanged( );
private slots:
    //! Called when the style associed to this node is destroyed.
    void            styleDestroyed( QObject* style );

public:
    Q_PROPERTY( QString label READ getLabel WRITE setLabel NOTIFY labelChanged FINAL )
    void            setLabel( const QString& label ) { _label = label; emit labelChanged( ); }
    QString         getLabel( ) const { return _label; }
private:
    QString         _label = QString{ "" };
signals:
    void            labelChanged( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node Group Management *///---------------------------------------
    //@{
public:
    /*! \brief Ungroup this node from its current group.
     *
     * Method can be called even if the node is not actually part of a group.
     * \sa qan::Group::ungroup()
     * \sa qan::Graph::insertNode()
     */
    // FIXME QAN3
    /*Q_INVOKABLE virtual void    ungroup( );

    Q_INVOKABLE qan::Group*     qmlGetGroup( ) { return getQanGroup(); }

    //! Shortcut to gtpo::GenNode<>::getGroup().
    qan::Group*                 getQanGroup( );*/
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Node )

#endif // qanNode_h
