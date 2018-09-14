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
#include "./qanSelectable.h"
#include "./qanDraggable.h"
#include "./qanAbstractDraggableCtrl.h"
#include "./qanNode.h"
#include "./qanGroup.h"

namespace qan { // ::qan

class Graph;

/*! \brief Model a visual group of nodes.
 *
 * \note Groups are styled with qan::NodeStyle.
 * \warning \c objectName property is set to "qan::GroupItem" and should not be changed in subclasses.
 *
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
    qan::AbstractDraggableCtrl&                 draggableCtrl();
private:
    std::unique_ptr<qan::AbstractDraggableCtrl> _draggableCtrl;

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

public:
    //! Utility function to ease initialization from c++, call setX(), setY(), setWidth() and setHEight() with the content of \c rect bounding rect.
    auto            setRect(const QRectF& r) noexcept -> void;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Style Management *///--------------------------------------------
    //@{
public:
    //! Group style object (this property is never null, a default style is returned when no style has been manually set).
    Q_PROPERTY( qan::Style* style READ getStyle WRITE setStyle NOTIFY styleChanged FINAL )
    void                    setStyle( qan::Style* style ) noexcept;
    //! Generic interface for qan::DraggableCtrl<>::handleDropEvent().
    void                    setItemStyle( qan::Style* style ) noexcept;
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

    /*! \name Selection and Sizing Management *///-----------------------------
    //@{
public:
    //! Group minimum size, default to "150 x 100" (group could not be visually resized below this size if \c resizable property is true).
    Q_PROPERTY( QSizeF minimumSize READ getMinimumSize WRITE setMinimumSize NOTIFY minimumSizeChanged FINAL )
    //! \copydoc minimumSize
    inline const QSizeF&  getMinimumSize() const noexcept { return _minimumSize; }
    //! \copydoc minimumSize
    void            setMinimumSize(QSizeF minimumSize) noexcept;
private:
    QSizeF          _minimumSize{150., 100};
signals:
    //! \internal
    void            minimumSizeChanged();

public:
    //! Enable or disable group resizing (default to true, ie group is resizable).
    Q_PROPERTY( bool resizable READ getResizable WRITE setResizable NOTIFY resizableChanged FINAL )
    //! \copydoc resizable
    inline bool     getResizable() const noexcept { return _resizable; }
    //! \copydoc resizable
    void            setResizable( bool resizable ) noexcept;
protected:
    //! \copydoc resizable
    bool            _resizable{true};
signals:
    //! \copydoc resizable
    void            resizableChanged();

public:
    //! Set this property to false to disable node selection (default to true, ie node are selectable by default).
    Q_PROPERTY( bool selectable READ getSelectable WRITE setSelectable NOTIFY selectableChanged FINAL )
    Q_PROPERTY( bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged FINAL )
    //! \brief Item used to hilight selection (usually a Rectangle quick item).
    Q_PROPERTY( QQuickItem* selectionItem READ getSelectionItem WRITE setSelectionItem NOTIFY selectionItemChanged FINAL )
protected:
    virtual void    emitSelectableChanged() override { emit selectableChanged(); }
    virtual void    emitSelectedChanged() override { emit selectedChanged(); }
    virtual void    emitSelectionItemChanged() override { emit selectionItemChanged(); }
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

    /*! \name Dragging Support Management *///---------------------------------
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
    /*! Configure \c nodeItem in this group item (modify target item parenthcip, but keep same visual position).
     * \param transformPosition set to false to avoid mapping of item position to group coordinate system (usefull for serialization).
     */
    virtual void    groupNodeItem(qan::NodeItem* nodeItem, bool transformPosition = true);

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
     *  container = content
     * }
     * \endcode
     */
    Q_PROPERTY(QQuickItem* container READ getContainer WRITE setContainer NOTIFY containerChanged FINAL)
    void                    setContainer(QQuickItem* container) noexcept { _container = container; emit containerChanged( ); }
    inline QQuickItem*      getContainer() noexcept { return _container; }
protected:
    QPointer<QQuickItem>    _container = nullptr;
signals:
    void                    containerChanged();

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
