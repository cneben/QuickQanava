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
// \file	qanGroup.h
// \author	benoit@destrat.io
// \date	2016 03 22
//-----------------------------------------------------------------------------

#ifndef qanGroup_h
#define qanGroup_h

// Qt headers
#include <QQuickItem>
#include <QPointF>
#include <QPolygonF>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanNode.h"

namespace qan { // ::qan

/*! \brief Model a graphics group of nodes.
 *
 * \nosubgrouping
 */
class Group : public gtpo::GenGroup< qan::GraphConfig >
{
    /*! \name Group Object Management *///-------------------------------------
    //@{
    Q_OBJECT
public:
    //! Group constructor.
    explicit Group( QObject* parent = nullptr );
    /*! \brief Remove any childs group who have no QQmlEngine::CppOwnership.
     *
     */
    virtual ~Group();
    Group( const Group& ) = delete;
public:
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Nodes Management *///--------------------------------------
    //@{
public:
    //! Shortcut to gtpo::GenGroup<qan::Config>::insertNode().
    //auto                insertNode( WeakNode weakNode ) -> void;

    /*! \brief Insert an existing QuickQanava node \c node into gthis group.
     *
     * \param drop specify if the node insertion is the result of a drag and drop (default), in that case,
     * the node position will be mapped in group coordinate system. If false (for serialization or in case
     * of a "prograObjectIdMapmmatic" insertion, node keep its current position and size.
     *
     * \note Call gtpo::GenGroup<qan::Config>::insertNode() during insertion.
     */
    //Q_INVOKABLE auto    insertNode( qan::Node* node, bool drop = true ) -> void;

    //! Shortcut to gtpo::GenGroup<qan::Config>::removeNode().
    //auto                removeNode( const qan::Node* node ) -> void;

    //! Return true if node \c node is registered in this group, shortcut to gtpo::GenGroup<qan::Config>::hasNode().
    Q_INVOKABLE bool    hasNode( qan::Node* node ) const;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Appearance Management *///---------------------------------------
    //@{
public:
    Q_PROPERTY( bool collapsed READ getCollapsed WRITE setCollapsed NOTIFY collapsedChanged FINAL )
    void        setCollapsed( bool collapsed );
    bool        getCollapsed( ) const { return _collapsed; }
private:
    bool        _collapsed = false;
signals:
    void        collapsedChanged( );

public:
    Q_PROPERTY( QString label READ getLabel WRITE setLabel NOTIFY labelChanged FINAL )
    void        setLabel( const QString& label ) { _label = label; emit labelChanged( ); }
    QString     getLabel( ) const { return _label; }
private:
    QString     _label = QString{ "" };
signals:
    void        labelChanged( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group DnD Management *///----------------------------------------
    //@{
public:
    /*! \brief Define if the group could actually be dragged by mouse.
     *
     * Set this property to true if you want to allow this group to be moved by mouse (if false, the node position is
     * fixed and should be changed programmatically).
     * Default to true.
     */
    Q_PROPERTY( bool draggable READ getDraggable WRITE setDraggable NOTIFY draggableChanged FINAL )
    void            setDraggable( bool draggable ) { _draggable = draggable; emit draggableChanged( ); }
    bool            getDraggable( ) { return _draggable; }
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
    // FIXME QAN3
/*    Q_PROPERTY( bool acceptDrops READ getAcceptDrops WRITE setAcceptDrops NOTIFY acceptDropsChanged FINAL )
    void             setAcceptDrops( bool acceptDrops ) { _acceptDrops = acceptDrops; setFlag( QQuickItem::ItemAcceptsDrops, acceptDrops ); emit acceptDropsChanged( ); }
    bool             getAcceptDrops( ) { return _acceptDrops; }
private:
    bool            _acceptDrops = true;
signals:
    void            acceptDropsChanged( );*/

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
    /*! \brief Called whenever a node is dragged and moved over this group, usually to hilight an insertion point in group.
     *
     * \sa qan::Layout::proposeNodeDrop( ) for a detailled explanation.
     */
    virtual void    proposeNodeDrop( QQuickItem* container, qan::Node* node );

    //! Called at the end of a node drag hover.
    virtual void    endProposeNodeDrop();
private:
    //! Shadow node managed internally by group used to hilight node position before a node is dropped in this group, or when a node is moved inside the group.
    qan::Node*      _shadowDropNode = nullptr;

signals:
    //! Emmited whenever a dragged node enter the group area (could be usefull to hilight it in a qan::Group concrete QML component).
    void            nodeDragEnter( );
    //! Emmited whenever a dragged node leave the group area (could be usefull to hilight it in a qan::Group concrete QML component).
    void            nodeDragLeave( );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

//QML_DECLARE_TYPE( qan::Group )
//Q_DECLARE_METATYPE( std::shared_ptr<qan::Group> )
//Q_DECLARE_METATYPE( std::weak_ptr<qan::Group> )

#endif // qanGroup_h
