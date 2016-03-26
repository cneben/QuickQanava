/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

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
// This file is a part of the Qanava software.
//
// \file	qanLinear.h
// \author	benoit@qanava.org
// \date	2016 03 23
//-----------------------------------------------------------------------------

#ifndef qanLinear_h
#define qanLinear_h

// Qt headers
#include <Qt>
#include <QSizeF>
#include <QtQml>

// Qanava headers
#include "./qanLayout.h"

namespace qan { // ::qan

/*! Layout nodes on a line, either a row or a column.
 *
 * \nosubgrouping
 */
class Linear : public qan::Layout
{
    /*! \name Linear Object Management *///------------------------------------
    //@{
    Q_OBJECT
public:
    //! Linear constructor.
    Linear( QObject* parent = 0 );
private:
    Q_DISABLE_COPY( Linear )
    //@}
    //-------------------------------------------------------------------------

    /*! \name Linear Layout Management *///------------------------------------
    //@{
public:
    /*! Spacing used between laidout nodes.
     *
     * Default to 15.
     */
    Q_PROPERTY( qreal spacing READ getSpacing WRITE setSpacing NOTIFY spacingChanged )
    void            setSpacing( qreal spacing ) { _spacing = spacing; emit spacingChanged( ); }
    qreal           getSpacing( ) { return _spacing; }
protected:
    qreal           _spacing;
signals:
    void            spacingChanged( );

public slots:
    //! Layout nodes on a line.
    virtual void    layout( ) override;

public:
    //! \copydoc qan::Layout::proposeNodeDrop()
    virtual void    proposeNodeDrop( QQuickItem* layoutItem, qan::Node* node, qan::Node* shadowNode );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Linear* )

#endif // qanLinear_h
