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
// \file	qanSelectable.h
// \author	benoit@destrat.io
// \date	2017 03 15
//-----------------------------------------------------------------------------

#ifndef qanSelectable_h
#define qanSelectable_h

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

/*! \brief Interface for a selectable qan::Node or qan::Group.
 *
 * \nosubgrouping
 */
class Selectable
{
    /*! \name Selectable Object Management *///--------------------------------
    //@{
public:
    explicit Selectable();
    virtual ~Selectable();
    Selectable( const Selectable& ) = delete;

protected:
    //! Configure this \c qan::Selectable interface with a valid graph.
    void    configure(QQuickItem* target, qan::Graph* graph);
private:
    QPointer<QQuickItem>    _target;
    QPointer<qan::Graph>    _graph;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Selection Management *///----------------------------------------
    //@{
public:
    void            setSelectable( bool selectable ) noexcept;
    inline bool     getSelectable() const noexcept { return _selectable; }
    inline bool     isSelectable() const noexcept { return _selectable; }
protected:
    virtual void    emitSelectableChanged() = 0;
private:
    bool            _selectable{true};

public:
    void            setSelected( bool selected ) noexcept;
    inline bool     getSelected() const noexcept { return _selected; }
protected:
    virtual void    emitSelectedChanged() = 0;
private:
    bool            _selected{false};

public:
    /*! \brief Item used to hilight selection (usually a Rectangle quick item). */
    inline QQuickItem*  getSelectionItem() { return _selectionItem.get(); }
    void                setSelectionItem( QQuickItem* selectionItem );
protected:
    virtual void        emitSelectionItemChanged() = 0;
private:
    std::unique_ptr< QQuickItem >  _selectionItem{ nullptr };

public:
    //! Update selection hilight item with new color, border weight and margin.
    void            configureSelectionItem( QColor selectionColor, qreal selectionWeight, qreal selectionMargin );

    //! Update selection hilight item with new border weight and margin.
    void            configureSelectionItem( qreal selectionWeight, qreal selectionMargin );

protected:
    //! Should be called when target item width is modified.
    void            updateSelectionWidth();
    //! Should be called when target item width is modified.
    void            updateSelectionHeight();
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

Q_DECLARE_INTERFACE(
    qan::Selectable,
    "com.destrat.io.QuickQanava.Selectable/3.0"
)

#endif // qanSelectable_h
