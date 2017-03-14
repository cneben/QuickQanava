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
// \file	qanGroupItem.h
// \author	benoit@destrat.io
// \date	2017 03 02
//-----------------------------------------------------------------------------

#ifndef qanGroupItem_h
#define qanGroupItem_h

// Qt headers
#include <QQuickItem>
#include <QPointF>
#include <QPolygonF>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanGroup.h"

namespace qan { // ::qan

class Graph;
class Group;

/*! \brief Model a graphics group of nodes.
 *
 * \nosubgrouping
 */
class GroupItem : public QQuickItem
{
    /*! \name Group Object Management *///-------------------------------------
    //@{
    Q_OBJECT
public:
    //! Group constructor.
    explicit GroupItem( QQuickItem* parent = nullptr );
    /*! \brief Remove any childs group who have no QQmlEngine::CppOwnership.
     *
     */
    virtual ~GroupItem( );
    GroupItem( const GroupItem& ) = delete;

public:
    Q_PROPERTY( qan::Group* group READ getGroup CONSTANT FINAL )
    auto        getGroup() noexcept -> qan::Group*;
    auto        getGroup() const noexcept -> const qan::Group*;
    inline auto setGroup(qan::Group* group) noexcept { _group = group; }
private:
    QPointer<qan::Group> _group{nullptr};

public:
    Q_PROPERTY( qan::Graph* graph READ getGraph FINAL )
protected:
    //! Secure shortcut to getGroup().getGraph().
    auto    getGraph() const noexcept -> const qan::Graph*;
    //! \copydoc getGraph()
    auto    getGraph() noexcept -> qan::Graph*;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group DnD Management *///----------------------------------------
    //@{
protected slots:
    //! Group is monitored for position change, since group's nodes edges should be updated manually in that case.
    void            groupMoved( );

public:
    /*! \brief Define if the group could actually be dragged by mouse.
     *
     * Set this property to true if you want to allow this group to be moved by mouse (if false, the node position is
     * fixed and should be changed programmatically).
     * Default to true.
     */
    Q_PROPERTY( bool draggable READ getDraggable WRITE setDraggable NOTIFY draggableChanged FINAL )
    void             setDraggable( bool draggable ) { _draggable = draggable; emit draggableChanged( ); }
    bool             getDraggable( ) { return _draggable; }
private:
    bool            _draggable = true;
signals:
    void            draggableChanged( );

public:
    /*! \brief Define if the group actually accept insertion of nodes via drag'n drop (default to true).
     *
     * Default to true.
     *
     * Setting this property to false may lead to a significant performance improvement if DropNode support is not needed.
     */
    Q_PROPERTY( bool acceptDrops READ getAcceptDrops WRITE setAcceptDrops NOTIFY acceptDropsChanged FINAL )
    void             setAcceptDrops( bool acceptDrops ) { _acceptDrops = acceptDrops; setFlag( QQuickItem::ItemAcceptsDrops, acceptDrops ); emit acceptDropsChanged( ); }
    bool             getAcceptDrops( ) { return _acceptDrops; }
private:
    bool            _acceptDrops = true;
signals:
    void            acceptDropsChanged( );

public:
    /*! \brief Define if the group should hilight a node insertion while the user is dragging a node across the group (might be costly).
     *
     *  When sets to true, group will use a shadow node (_shadowDropNode) to hilight the position of a node that is actually dragged over
     *  this group to show its position when dropped and inserted in the group (it is quite costly, and the group layout must include support for
     *  qan::Layout::proposeNodeDrop( ), such as qan::Linear and qan::OrderedTree).
     *
     *  When hilightDrag is set to true, your concrete QML qan::Node should call qan::dropNode() and qan::Node::proposeNodeDrop().
     *  Default to true.
     *
     *  Example of a group with a qan::HierarchyTree layout and group's 'hilightDrag' property sets to true: node "n2" insertion position is hilighted with a shadow node before node is actually dropped into the group:
     *  \image html 20150908_NP_group-hilight-drag.png
     *
     * \sa proposeNodeDrop()
     * \sa qan::Node::dropNode()
     * \sa qan::Node::proposeNodeDrop()
     * \sa qan::Layout::proposeNodeDrop()
     */
    Q_PROPERTY( bool hilightDrag READ getHilightDrag WRITE setHilightDrag NOTIFY hilightDragChanged FINAL )
    void             setHilightDrag( bool hilightDrag ) { _hilightDrag = hilightDrag; emit hilightDragChanged( ); }
    bool             getHilightDrag( ) { return _hilightDrag; }
protected:
    bool            _hilightDrag = true;
signals:
    void            hilightDragChanged( );

public:
    //! Configure \c nodeItem in this group item.
    virtual void    configureNode(qan::NodeItem* nodeItem);

    /*! \brief Called whenever a node is dragged and moved over this group, usually to hilight an insertion point in group.
     */
    virtual void    proposeNodeDrop( qan::Node* node );

    //! Called at the end of a node drag hover.
    virtual void    endProposeNodeDrop();
private:
    //! Shadow node managed internally by group used to hilight node position before a node is dropped in this group, or when a node is moved inside the group.
    qan::Node*      _shadowDropNode = nullptr;

public:
    /*! \brief Should be set from the group concrete QML component to indicate the group content item (otherwise, this will be used).
     *
     * For example, if the actual container for node is a child of the concrete group component (most of the time, an Item or a Rectangle, use the
     * following code to set 'container' property:
     *
     * \code
     * Qan.Group {
     *  id: group
     *  default property alias children : content.children
     *  Item {
     *      id: content
     *      // ...
     *  }
     *  // ...
     *  Component.onCompleted: {
     *      container = content
     *  }
     * }
     * \endcode
     */
    Q_PROPERTY( QQuickItem* container READ getContainer WRITE setContainer NOTIFY containerChanged FINAL )
    void            setContainer( QQuickItem* container ) { _container = container; emit containerChanged( ); }
    QQuickItem*     getContainer( ) { return _container; }
protected:
    QQuickItem*     _container = nullptr;
signals:
    void            containerChanged( );

signals:
    //! Emmited whenever a dragged node enter the group area (could be usefull to hilight it in a qan::Group concrete QML component).
    void            nodeDragEnter( );
    //! Emmited whenever a dragged node leave the group area (could be usefull to hilight it in a qan::Group concrete QML component).
    void            nodeDragLeave( );

signals:
    //! Emmited whenever the group is clicked (even at the start of a dragging operation).
    void    groupClicked( qan::GroupItem* group, QPointF p );
    //! Emmited whenever the group is double clicked.
    void    groupDoubleClicked( qan::GroupItem* group, QPointF p );
    //! Emmited whenever the group is right clicked.
    void    groupRightClicked( qan::GroupItem* group, QPointF p );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::GroupItem )

#endif // qanGroupItem_h
