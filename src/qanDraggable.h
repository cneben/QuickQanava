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
// \file	qanDraggable.h
// \author	benoit@destrat.io
// \date	2017 03 15
//-----------------------------------------------------------------------------

#ifndef qanDraggable_h
#define qanDraggable_h

// Std headers
#include <memory>

// Qt headers
#include <QObject>      // Q_DECLARE_INTERFACE
#include <QPointer>
#include <QQuickItem>

// QuickQanava headers
/* Nil */

namespace qan { // ::qan

class Graph;

/*! \brief Interface for a draggable and droppable qan::Node or qan::Group.
 *
 * \nosubgrouping
 */
class Draggable
{
    /*! \name Draggable Object Management *///---------------------------------
    //@{
public:
    explicit Draggable();
    virtual ~Draggable();
    Draggable( const Draggable& ) = delete;

protected:
    //! Configure this \c qan::Draggable interface with a valid \c target (usually a qan::Node or qan::Group).
    void    configure(QQuickItem* target);
private:
    QPointer<QQuickItem>    _target;
    QPointer<qan::Graph>    _graph;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Draggable Management *///----------------------------------------
    //@{
public:
    void            setDraggable( bool draggable ) noexcept;
    inline bool     getDraggable() const noexcept { return _draggable; }
protected:
    virtual void    emitDraggableChanged() = 0;
private:
    /*! \brief Define if the node could actually be dragged by mouse (default to true).
     *
     * Set this property to true if you want to allow this node to be moved by mouse (if false, the node position is
     * fixed and should be changed programmatically).
     *
     * Default to true.
     */
    bool            _draggable{true};

public:
    void            setDragged( bool dragged ) noexcept;
    inline bool     getDragged() const noexcept { return _dragged; }
protected:
    virtual void    emitDraggedChanged() = 0;
private:
    //! True when the node is currently beeing dragged.
    bool            _dragged{false};

public:
    void            setDroppable( bool droppable ) noexcept;
    inline bool     getDroppable() const noexcept { return _droppable; }
protected:
    virtual void    emitDroppableChanged() = 0;
private:
    /*! \brief Define if the target node or group could be dropped in another node or in a node group.
     *
     * Set this property to true if you want to allow this node to be dropped in a qan::Group automatically.
     * Default to true.
     * Setting this property to false may lead to a significant performance improvement if group dropping is not needed.
     */
    bool            _droppable{true};

public:
    void            setAcceptDrops( bool acceptDrops ) noexcept;
    inline bool     getAcceptDrops() const noexcept { return _acceptDrops; }
protected:
    virtual void    emitAcceptDropsChanged() = 0;
private:
    /*! \brief Define if the group actually accept insertion of nodes via drag'n drop (default to true).
     *
     * Default to true.
     *
     * Setting this property to false may lead to a significant performance improvement if DropNode support is not needed.
     */
    bool            _acceptDrops{true};
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

Q_DECLARE_INTERFACE(
    qan::Draggable,
    "com.destrat.io.QuickQanava.Draggable/3.0"
)

#endif // qanDraggable_h
