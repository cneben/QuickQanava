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
#include "./qanGroupItem.h"

namespace qan { // ::qan

class Graph;
class GroupItem;

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
    Q_PROPERTY( qan::Graph* graph READ getGraph FINAL )
    //! Shortcut to gtpo::GenGroup<>::getGraph().
    qan::Graph*         getGraph() noexcept;
    //! \copydoc getGraph()
    const qan::Graph*   getGraph() const noexcept;

public:
    friend class qan::GroupItem;

    Q_PROPERTY( qan::GroupItem* item READ getItem FINAL )
    inline qan::GroupItem*  getItem() noexcept { return _item.data(); }
    void                    setItem(qan::GroupItem* item) noexcept;
private:
    QPointer<qan::GroupItem> _item;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Static Factories *///---------------------------------------
    //@{
public:
    /*! \brief Return the default delegate QML component that should be used to generate group \c item.
     *
     *  \arg caller Use this for \c caller argument, since at component creation a valid QML engine is necessary.
     *  \return Default delegate component or nullptr (when nullptr is returned, QuickQanava default to Qan.Group component).
     */
    static  QQmlComponent*      delegate(QObject* caller) noexcept;

    /*! \brief Return the default style that should be used with qan::Group.
     *
     *  \return Default style or nullptr (when nullptr is returned, qan::StyleManager default group style will be used).
     */
    static  qan::Style*         style() noexcept;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Nodes Management *///--------------------------------------
    //@{
public:
    //! Return true if node \c node is registered in this group, shortcut to gtpo::GenGroup<qan::GraphConfig>::hasNode().
    Q_INVOKABLE bool    hasNode( qan::Node* node ) const;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Appearance Management *///---------------------------------------
    //@{
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
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Group )

#endif // qanGroup_h
