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
#include "./qanNodeItem.h"
#include "./qanSelectable.h"
#include "./qanDraggable.h"
#include "./qanDraggableCtrl.h"

namespace qan { // ::qan

class Graph;
class Group;

/*! \brief Model a graphics group of nodes.
 *
 * \warning \c objectName property is set to "qan::GroupItem" and should not be changed in subclasses.
 * \nosubgrouping
 */
class GroupItem : public QQuickItem,
                  public qan::Selectable,
                  public qan::Draggable
{
    /*! \name Group Object Management *///-------------------------------------
    //@{
    Q_OBJECT
    Q_INTERFACES(qan::Selectable)
    Q_INTERFACES(qan::Draggable)
public:
    //! Group constructor.
    explicit GroupItem( QQuickItem* parent = nullptr );
    virtual ~GroupItem();
    GroupItem( const GroupItem& ) = delete;
public:
    qan::DraggableCtrl<qan::Group, qan::GroupItem>& draggableCtrl() { return _draggableCtrl; }
private:
    qan::DraggableCtrl<qan::Group, qan::GroupItem> _draggableCtrl;
public:
    Q_PROPERTY( qan::Group* group READ getGroup CONSTANT FINAL )
    auto        getGroup() noexcept -> qan::Group*;
    auto        getGroup() const noexcept -> const qan::Group*;
    auto        setGroup(qan::Group* group) noexcept -> void;
private:
    QPointer<qan::Group> _group{nullptr};

public:
    Q_PROPERTY( qan::Graph* graph READ getGraph FINAL )
    auto    setGraph(qan::Graph* graph) noexcept -> void;
protected:
    //! Secure shortcut to getGroup().getGraph().
    auto    getGraph() const noexcept -> const qan::Graph*;
    //! \copydoc getGraph()
    auto    getGraph() noexcept -> qan::Graph*;
private:
    QPointer<qan::Graph>    _graph;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Style Management *///--------------------------------------------
    //@{
public:
    //! Group style object (this property is never null, a default style is returned when no style has been manually set).
    Q_PROPERTY( qan::Style* style READ getStyle WRITE setStyle NOTIFY styleChanged FINAL )
    void                    setStyle( qan::Style* style ) noexcept;
    inline qan::Style*      getStyle() const noexcept { return _style.data(); }
private:
    QPointer<qan::Style>    _style;
signals:
    void                    styleChanged();
private slots:
    //! Called when the style associed to this group is destroyed.
    void                    styleDestroyed( QObject* style );
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

    /*! \name Collapse Management *///-----------------------------------------
    //@{
public:
    Q_PROPERTY( bool collapsed READ getCollapsed WRITE setCollapsed NOTIFY collapsedChanged FINAL )
    inline bool getCollapsed() const noexcept { return _collapsed; }
    void        setCollapsed( bool collapsed ) noexcept;
private:
    bool        _collapsed{false};
signals:
    void        collapsedChanged();
    //@}
    //-------------------------------------------------------------------------

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

protected slots:
    //! Group is monitored for position change, since group's nodes edges should be updated manually in that case.
    void            groupMoved();

public:
    //! Configure \c nodeItem in this group item (modify target item parenthcip, but keep same visual position).
    virtual void    groupNodeItem(qan::NodeItem* nodeItem);

    //! Configure \c nodeItem outside this group item (modify parentship, keep same visual position).
    virtual void    ungroupNodeItem(qan::NodeItem* nodeItem);

    //! Call at the beginning of another group or node hover operation on this group (usually trigger a visual change to notify user that insertion is possible trought DND).
    inline void     proposeNodeDrop() noexcept { emit nodeDragEnter( ); }

    //! End an operation started with proposeNodeDrop().
    inline void     endProposeNodeDrop() noexcept { emit nodeDragLeave( ); }

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
