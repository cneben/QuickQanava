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
// \file	qanGraphView.h
// \author	benoit@destrat.io
// \date	2016 08 15
//-----------------------------------------------------------------------------

#ifndef qanGraphView_h
#define qanGraphView_h

// GTpo headers
#include <GTpo>

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanGroup.h"
#include "./qanNavigable.h"

// Qt headers
#include <QQuickItem>
#include <QPointer>

namespace qan { // ::qan

/*! \brief Abstract view component for the qan::Graph class.
 *
 * \nosubgrouping
*/
class GraphView : public qan::Navigable
{
    Q_OBJECT
    /*! \name GraphView Object Management *///---------------------------------
    //@{
public:
    //! GraphView default constructor.
    explicit GraphView( QQuickItem* parent = nullptr );
    virtual ~GraphView( ) { }
    GraphView( const GraphView& ) = delete;

public:
    //! Graph that should be displayed in this graph view.
    Q_PROPERTY( qan::Graph* graph READ getGraph WRITE setGraph NOTIFY graphChanged FINAL )
    void                    setGraph( qan::Graph* graph );
    inline qan::Graph*      getGraph( ) const noexcept { return _graph.data(); }
private:
    QPointer<qan::Graph>    _graph{ nullptr };
signals:
    void                    graphChanged( );

protected:
    //! Called when the mouse is clicked in the container (base implementation empty).
    virtual void    navigableClicked(QPointF pos) override;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::GraphView )

#endif // qanGraphView_h

