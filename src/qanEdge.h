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

public:
    Q_PROPERTY( qan::EdgeItem* item READ getItem FINAL )
    inline qan::EdgeItem*   getItem() noexcept { return _item.data(); }
    void                    setItem(qan::EdgeItem* edgeItem) noexcept;
private:
    QPointer<qan::EdgeItem> _item;

public:
    // Qt property for gtpo::Edge serializable standard property.
    Q_PROPERTY( bool serializable READ getSerializable WRITE setSerializableObs NOTIFY serializableChanged FINAL )
    void            setSerializableObs( bool serializable ) { setSerializable( serializable ); emit serializableChanged( ); }
signals:
    void            serializableChanged();
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

    /*! \name Style and Properties Management *///-----------------------------
    //@{
private:
    using SharedEdgeStyle = QSharedPointer< qan::EdgeStyle >;
    SharedEdgeStyle _defaultStyle;
public:
    //! Edge current style object (this property is never null, a default style is returned when no style has been manually set).
    Q_PROPERTY( qan::EdgeStyle* style READ getStyle WRITE setStyle NOTIFY styleChanged FINAL )
    void            setStyle( EdgeStyle* style );
    qan::EdgeStyle* getStyle( ) const { return _style; }
private:
    qan::EdgeStyle* _style{nullptr};
signals:
    void            styleChanged( );
private slots:
    //! Called when the style associed to this edge is destroyed.
    void            styleDestroyed( QObject* style );

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
