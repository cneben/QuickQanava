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
// \file	hedges.h
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

#ifndef qanEdge_h
#define qanEdge_h

// Qt headers
#include <QLineF>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanStyle.h"
#include "./qanNode.h"
#include "./qanEdgeItem.h"

namespace qan { // ::qan

class Graph;
class EdgeItem;

//! Weighted directed edge linking two nodes in a graph.
/*!
    \nosubgrouping
 */
class Edge : public gtpo::GenEdge< qan::GraphConfig >
{
    /*! \name Edge Object Management *///--------------------------------------
    //@{
    Q_OBJECT
public:
    //! Edge constructor with source, destination and weight initialization.
    explicit Edge();
    Edge( const Edge& ) = delete;
    virtual ~Edge() { /* Nil */ }

public:
    Q_PROPERTY( qan::Graph* graph READ getGraph CONSTANT FINAL )
    //! Shortcut to gtpo::GenEdge<>::getGraph().
    qan::Graph*         getGraph() noexcept;
    //! \copydoc getGraph()
    const qan::Graph*   getGraph() const noexcept;

public:
    friend class qan::EdgeItem;

    Q_PROPERTY( qan::EdgeItem* item READ getItem CONSTANT )
    inline qan::EdgeItem*   getItem() noexcept { return _item.data(); }
    void                    setItem(qan::EdgeItem* edgeItem) noexcept;
private:
    QPointer<qan::EdgeItem> _item;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Edge Static Factories *///---------------------------------------
    //@{
public:
    /*! \brief Return the default delegate QML component that should be used to generate edge \c item.
     *
     *  \arg caller Use this for \c caller argument, since at component creation a valid QML engine is necessary.
     *  \return Default delegate component or nullptr (when nullptr is returned, QuickQanava default to Qan.Edge component).
     */
    static  QQmlComponent*      delegate(QObject* caller) noexcept;

    /*! \brief Return the default style that should be used with qan::Edge.
     *
     *  \return Default style or nullptr (when nullptr is returned, qan::StyleManager default edge style will be used).
     */
    static  qan::EdgeStyle*     style() noexcept;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Edge Topology Management *///------------------------------------
    //@{
public:
    //! Read-only abstract item model of this edge in hyper nodes.
//    Q_PROPERTY( QAbstractItemModel* inHNodes READ qmlGetInHNodes NOTIFY inHNodesChanged FINAL )
//    QAbstractItemModel* qmlGetInHNodes( ) const { return const_cast<QAbstractItemModel*>( static_cast< const QAbstractItemModel* >( &getInHNodes() ) ); }
//signals:
//    void        inHNodesChanged( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Edge Properties Management *///----------------------------------
    //@{
public:
    Q_PROPERTY( QString label READ getLabel WRITE setLabel NOTIFY labelChanged FINAL )
    //! Set this edge label.
    void            setLabel( const QString& label );
    //! Get this edge label.
    const QString&  getLabel( ) const { return _label; }
protected:
    QString         _label{""};
signals:
    void            labelChanged( );

public:
    Q_PROPERTY( qreal weight READ getWeight WRITE setWeight NOTIFY weightChanged FINAL )
    //! Get edge's weight.
    inline qreal    getWeight( ) const { return _weight; }
    //! Set edge's weight.
    void            setWeight( qreal weight );
protected:
    qreal           _weight{1.0};
signals:
    void            weightChanged( );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Edge )

#endif // qanEdge_h
