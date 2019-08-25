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
// \file    qanGroupItem.h
// \author  benoit@destrat.io
// \date    2017 03 02
//-----------------------------------------------------------------------------

#pragma once

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
#include "./qanNodeItem.h"
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
class GroupItem : public qan::NodeItem
{
    /*! \name Group Object Management *///-------------------------------------
    //@{
    Q_OBJECT
    //Q_INTERFACES(qan::Selectable)
    //Q_INTERFACES(qan::Draggable)
public:
    //! Group constructor.
    explicit GroupItem( QQuickItem* parent = nullptr );
    virtual ~GroupItem() override = default;
    GroupItem( const GroupItem& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Topology Management *///-----------------------------------------
    //@{
public:
    Q_PROPERTY( qan::Group* group READ getGroup CONSTANT FINAL )
    auto        getGroup() noexcept -> qan::Group*;
    auto        getGroup() const noexcept -> const qan::Group*;
    auto        setGroup(qan::Group* group) noexcept -> void;
private:
    QPointer<qan::Group> _group{nullptr};

public:
    //! Utility function to ease initialization from c++, call setX(), setY(), setWidth() and setHEight() with the content of \c rect bounding rect.
    auto            setRect(const QRectF& r) noexcept -> void;
    //@}
    //-------------------------------------------------------------------------


    /*! \name Selection and Sizing Management *///-----------------------------
    //@{
public:
    //! \brief Group preferred initial size, default to "200 x 150".
    Q_PROPERTY( qreal preferredGroupWidth READ getPreferredGroupWidth WRITE setPreferredGroupWidth NOTIFY preferredGroupWidthChanged FINAL )
    //! \copydoc preferredGroupWidth
    inline qreal    getPreferredGroupWidth() const noexcept { return _preferredGroupWidth; }
    //! \copydoc preferredGroupWidth
    void            setPreferredGroupWidth(qreal preferredGroupWidth) noexcept;
private:
    //! \copydoc preferredGroupWidth
    qreal           _preferredGroupWidth = 250;
signals:
    //! \copydoc preferredGroupWidth
    void            preferredGroupWidthChanged();

public:
    //! \brief Group preferred initial size, default to "200 x 150".
    Q_PROPERTY( qreal preferredGroupHeight READ getPreferredGroupHeight WRITE setPreferredGroupHeight NOTIFY preferredGroupHeightChanged FINAL )
    //! \copydoc preferredGroupHeight
    inline qreal    getPreferredGroupHeight() const noexcept { return _preferredGroupHeight; }
    //! \copydoc preferredGroupHeight
    void            setPreferredGroupHeight(qreal preferredGroupHeight) noexcept;
private:
    //! \copydoc preferredGroupHeight
    qreal           _preferredGroupHeight = 200;
signals:
    //! \copydoc preferredGroupHeight
    void            preferredGroupHeightChanged();

public:
    //! \brief Group minimum size, default to "150 x 100" (group could not be visually resized below this size if \c resizable property is true).
    Q_PROPERTY( qreal minimumGroupWidth READ getMinimumGroupWidth WRITE setMinimumGroupWidth NOTIFY minimumGroupWidthChanged FINAL )
    //! \copydoc minimumGroupWidth
    inline qreal    getMinimumGroupWidth() const noexcept { return _minimumGroupWidth; }
    //! \copydoc minimumGroupWidth
    void            setMinimumGroupWidth(qreal minimumGroupWidth) noexcept;
private:
    //! \copydoc minimumGroupWidth
    qreal           _minimumGroupWidth = 150;
signals:
    //! \copydoc minimumGroupWidth
    void            minimumGroupWidthChanged();

public:
    //! \brief Group minimum size, default to "150 x 100" (group could not be visually resized below this size if \c resizable property is true).
    Q_PROPERTY( qreal minimumGroupHeight READ getMinimumGroupHeight WRITE setMinimumGroupHeight NOTIFY minimumGroupHeightChanged FINAL )
    //! \copydoc minimumGroupHeight
    inline qreal    getMinimumGroupHeight() const noexcept { return _minimumGroupHeight; }
    //! \copydoc minimumGroupHeight
    void            setMinimumGroupHeight(qreal minimumGroupHeight) noexcept;
private:
    //! \copydoc minimumGroupHeight
    qreal           _minimumGroupHeight = 100.;
signals:
    //! \copydoc minimumGroupHeight
    void            minimumGroupHeightChanged();
    //@}
    //-------------------------------------------------------------------------

    /*! \name Collapse Management *///-----------------------------------------
    //@{
public:
    // FIXME document me
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
protected slots:
    //! Group is monitored for position change, since group's nodes edges should be updated manually in that case.
    void            groupMoved();

public:
    /*! \brief Configure \c nodeItem in this group item (modify target item parenthcip, but keep same visual position).
     */
    virtual void    groupNodeItem(qan::NodeItem* nodeItem, bool transform = true);

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
    void                    setContainer(QQuickItem* container) noexcept;
    QQuickItem*             getContainer() noexcept;
    const QQuickItem*       getContainer() const noexcept;
protected:
    QPointer<QQuickItem>    _container = nullptr;
signals:
    void                    containerChanged();

signals:
    //! Emitted whenever a dragged node enter the group area (could be usefull to hilight it in a qan::Group concrete QML component).
    void            nodeDragEnter();
    //! Emitted whenever a dragged node leave the group area (could be usefull to hilight it in a qan::Group concrete QML component).
    void            nodeDragLeave();

protected:
    virtual void    mouseDoubleClickEvent(QMouseEvent* event ) override;
    virtual void    mousePressEvent(QMouseEvent* event ) override;

signals:
    //! Emitted whenever the group is clicked (even at the start of a dragging operation).
    void    groupClicked( qan::GroupItem* group, QPointF p );
    //! Emitted whenever the group is double clicked.
    void    groupDoubleClicked( qan::GroupItem* group, QPointF p );
    //! Emitted whenever the group is right clicked.
    void    groupRightClicked( qan::GroupItem* group, QPointF p );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::GroupItem )
