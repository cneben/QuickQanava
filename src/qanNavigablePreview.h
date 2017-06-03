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
// \file	qanNavigablePreview.h
// \author	benoit@destrat.io
// \date	2017 06 02
//-----------------------------------------------------------------------------

#ifndef canNavigablePreview_h
#define canNavigablePreview_h

// Qt headers
#include <QQuickItem>

// QuickQanava headers
#include "./qanNavigable.h"

namespace qan { // ::qan

/*! \brief Reduced preview and navigation for qan::Navigable.
 *
 *
 */
class NavigablePreview : public QQuickItem
{
    /*! \name NavigablePreview Object Management *///--------------------------
    //@{
Q_OBJECT
public:
    explicit NavigablePreview( QQuickItem* parent = nullptr );
    virtual ~NavigablePreview() { /* Nil */ }
    NavigablePreview(const NavigablePreview&) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Navigable Management *///----------------------------------------
    //@{
public:
    /*! \brief Source qan::Navigable.
     *
     * \warning Can be nullptr.
     */
    Q_PROPERTY( qan::Navigable* source READ getSource WRITE setSource NOTIFY sourceChanged FINAL )
    //! \sa source
    inline qan::Navigable*      getSource() const noexcept { return _source.data(); }
    //! \sa source
    void                        setSource(qan::Navigable* source) noexcept;
private:
    //! \copydoc source
    QPointer<qan::Navigable>    _source;
signals:
    //! \sa source
    void                        sourceChanged();
};

} // ::qan

QML_DECLARE_TYPE( qan::NavigablePreview )

#endif // qanNavigablePreview_h

