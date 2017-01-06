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

// Qanava headers
#include "./qanConfig.h"
#include "./qanStyle.h"
#include "./qanNode.h"

namespace qan { // ::qan

class Node;

//! Weighted directed edge linking two nodes in a graph.
/*!
    \nosubgrouping
 */
class Edge : public gtpo::GenEdge< qan::Config >
{
    /*! \name Edge Object Management *///--------------------------------------
    //@{
    Q_OBJECT
public:
    //! Edge constructor with source, destination and weight initialization.
    explicit Edge( QQuickItem* parent = nullptr );
    Edge( const Edge& ) = delete;

    //! Return getDynamicClassName() (default to "qan::Edge").
    inline std::string  getClassName() const noexcept { return getDynamicClassName(); }
    virtual std::string getDynamicClassName() const noexcept { return "qan::Edge"; }
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
    Q_PROPERTY( qan::Node* sourceItem READ getSourceItem WRITE setSourceItem NOTIFY sourceItemChanged FINAL )
    qan::Node*  getSourceItem( ) { return static_cast< qan::Node* >( !getSrc().expired() ? getSrc().lock().get() : nullptr ); }
    void        setSourceItem( qan::Node* source );
signals:
    void        sourceItemChanged( );

public:
    Q_PROPERTY( qan::Node* destinationItem READ getDestinationItem() WRITE setDestinationItem NOTIFY destinationItemChanged FINAL )
    qan::Node*  getDestinationItem( ) { return static_cast< qan::Node* >( !getDst().expired() ? getDst().lock().get() : nullptr ); }
    void        setDestinationItem( qan::Node* destination );
signals:
    void        destinationItemChanged( );

public:
    Q_PROPERTY( qan::Edge* destinationEdge READ getDestinationEdge() WRITE setDestinationEdge NOTIFY destinationEdgeChanged FINAL )
    qan::Edge*  getDestinationEdge( ) { return static_cast< qan::Edge* >( !getHDst().expired() ? getHDst().lock().get() : nullptr ); }
    void        setDestinationEdge( qan::Edge* destination );
signals:
    void        destinationEdgeChanged( );

public:
    //! Read-only abstract item model of this edge in hyper nodes.
    Q_PROPERTY( QAbstractItemModel* inHNodes READ qmlGetInHNodes NOTIFY inHNodesChanged FINAL )
    QAbstractItemModel* qmlGetInHNodes( ) const { return const_cast<QAbstractItemModel*>( static_cast< const QAbstractItemModel* >( &getInHNodes() ) ); }
signals:
    void        inHNodesChanged( );

protected:
    //! Configure either a node or an edge (for hyper edges) item.
    void        configureDestinationItem( QQuickItem* item );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Edge Drawing Management *///-------------------------------------
    //@{
public slots:
    //! Call updateItem() (override updateItem() to an empty method for invisible edges).
    virtual void        updateItemSlot( ) { updateItem(); }
public:
    /*! \brief Update edge bounding box according to source and destination item actual position and size.
     *
     * \note When overriding, call base implementation at the beginning of user implementation.
     * \note Override to an empty method with no base class calls for an edge with no graphics content.
     */
    virtual void        updateItem( );
public:
    //! Internally used from QML to set src and dst and display an unitialized edge for previewing edges styles.
    Q_INVOKABLE void    setLine( QPoint src, QPoint dst );
    //! Edge source point in item CS (with accurate source bounding shape intersection).
    Q_PROPERTY( QPointF p1 READ getP1() NOTIFY p1Changed FINAL )
    inline  auto    getP1() const -> const QPointF& { return _p1; }
    //! Edge destination point in item CS (with accurate destination bounding shape intersection).
    Q_PROPERTY( QPointF p2 READ getP2() NOTIFY p2Changed FINAL )
    inline  auto    getP2() const -> const QPointF& { return _p2; }
signals:
    void            p1Changed();
    void            p2Changed();
private:
    QPointF         _p1;
    QPointF         _p2;
protected:
    QPointF         getLineIntersection( const QPointF& p1, const QPointF& p2, const QPolygonF& polygon ) const;
    QLineF          getLineIntersection( const QPointF& p1, const QPointF& p2, const QPolygonF& srcBp, const QPolygonF& dstBp ) const;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Mouse Management *///--------------------------------------------
    //@{
protected:
    virtual void    mouseDoubleClickEvent( QMouseEvent* event ) override;
    virtual void    mousePressEvent( QMouseEvent* event ) override;
signals:
    void            edgeClicked( QVariant edge, QVariant pos );
    void            edgeRightClicked( QVariant edge, QVariant pos );
    void            edgeDoubleClicked( QVariant edge, QVariant pos );
private:
    inline qreal    distanceFromLine( const QPointF& p, const QLineF& line ) const;

public:
    //! Edge label position.
    Q_PROPERTY( QPointF labelPos READ getLabelPos WRITE setLabelPos NOTIFY labelPosChanged FINAL )
    //! Get edge label position.
    QPointF		getLabelPos( ) const { return _labelPos; }
    //! Set edge label position.
    void		setLabelPos( QPointF labelPos ) { _labelPos = labelPos; emit labelPosChanged(); }
protected:
    //! \sa labelPos
    QPointF     _labelPos;
signals:
    //! \sa labelPos
    void        labelPosChanged( );
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

    /*! \name Drag'nDrop Management  *///--------------------------------------
    //@{
public:
    /*! \brief Define if the edge actually accept drops.
     *
     * When set to false, the edge will not be styleable by DnD and hyper edge drop connector
     * won't works.
     *
     * Default to true.
     *
     * Setting this property to false may lead to a significant performance improvement if DropNode support is not needed.
     */
    Q_PROPERTY( bool acceptDrops READ getAcceptDrops WRITE setAcceptDrops NOTIFY acceptDropsChanged FINAL )
    void             setAcceptDrops( bool acceptDrops ) { _acceptDrops = acceptDrops; setFlag( QQuickItem::ItemAcceptsDrops, acceptDrops ); emit acceptDropsChanged( ); }
    bool             getAcceptDrops( ) { return _acceptDrops; }
private:
    bool            _acceptDrops{true};
signals:
    void            acceptDropsChanged( );

protected:
    //! Return true if point is actually on the edge (not only in edge bounding rect).
    virtual bool    contains( const QPointF& point ) const override;

    /*! \brief Internally used to manage drag and drop over nodes, override with caution, and call base class implementation.
     *
     * Drag enter event are not restricted to the edge bounding rect but to the edge line with a distance delta, computing
     * that value is quite coslty, if you don't need to accept drops, use setAcceptDrops( false ).
     */
    virtual void    dragEnterEvent( QDragEnterEvent* event ) override;
    //! Internally used to manage drag and drop over nodes, override with caution, and call base class implementation.
    virtual void    dragMoveEvent( QDragMoveEvent* event ) override;
    //! Internally used to manage drag and drop over nodes, override with caution, and call base class implementation.
    virtual void    dragLeaveEvent( QDragLeaveEvent* event ) override;
    //! Internally used to accept style drops.
    virtual void    dropEvent( QDropEvent* event ) override;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Edge )

#endif // qanEdge_h
