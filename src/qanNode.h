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
// \file	qanNode.h
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

#ifndef qanNode_h
#define qanNode_h

// Qt headers
#include <QQuickItem>
#include <QPointF>
#include <QPolygonF>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanEdge.h"
#include "./qanStyle.h"
#include "./qanBehaviour.h"
//#include "./qanNodeItem.h"

namespace qan { // ::qan

class NodeBehaviour;
class Graph;
class Group;
class NodeItem;

/*! \brief Base class for modelling nodes with attributes and an in/out edges list in a qan::Graph graph.
 *
 * \note If your application does not need drag'n'drop support (ie group insertion via dra'n'drop or VisualConnector are not used nor necessary), consider disabling
 * drag'n'drop support by setting the \c acceptsDrops and \c droppable properties to false, it could improve performances significantly.
 *
 * \nosubgrouping
*/
class Node : public gtpo::GenNode< qan::GraphConfig >
{
    /*! \name Node Object Management *///--------------------------------------
    //@{
    Q_OBJECT
public:
    //! Node constructor.
    explicit Node(QObject* parent=nullptr);
    virtual ~Node();
    Node( const Node& ) = delete;

public:
    Q_PROPERTY( qan::Graph* graph READ getGraph FINAL )
    //! Shortcut to gtpo::GenNode<>::getGraph().
    qan::Graph*         getGraph() noexcept;
    //! \copydoc getGraph()
    const qan::Graph*   getGraph() const noexcept;

public:
    /*!
     * \note only label is taken into account for equality comparison.
     */
    bool    operator==( const qan::Node& right ) const;

public:
    Q_PROPERTY( qan::NodeItem* item READ getItem CONSTANT )
    qan::NodeItem*          getItem() noexcept;
    const qan::NodeItem*    getItem() const noexcept;
    void                    setItem(qan::NodeItem* nodeItem) noexcept;
protected:
    QPointer<qan::NodeItem> _item;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node Static Factories *///---------------------------------------
    //@{
public:
    /*! \brief Return the default delegate QML component that should be used to generate node \c item.
     *
     *  \arg caller Use this for \c caller argument, since at component creation a valid QML engine is necessary.
     *  \return Default delegate component or nullptr (when nullptr is returned, QuickQanava default to Qan.Node component).
     */
    static  QQmlComponent*      delegate(QObject* caller) noexcept;

    /*! \brief Return the default style that should be used with qan::Node.
     *
     *  \return Default style or nullptr (when nullptr is returned, qan::StyleManager default node style will be used).
     */
    static  qan::NodeStyle*     style() noexcept;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Topology Interface *///------------------------------------------
    //@{
public:
    //! Read-only abstract item model of this node in nodes.
    Q_PROPERTY( QAbstractItemModel* inNodes READ qmlGetInNodes CONSTANT FINAL )
    QAbstractItemModel* qmlGetInNodes( ) const { return const_cast<QAbstractItemModel*>( static_cast< const QAbstractItemModel* >( &getInNodes() ) ); }

public:
    //! Read-only abstract item model of this node out nodes.
    Q_PROPERTY( QAbstractItemModel* outNodes READ qmlGetOutNodes CONSTANT FINAL )
    QAbstractItemModel* qmlGetOutNodes() const { return const_cast< QAbstractItemModel* >( qobject_cast< const QAbstractItemModel* >( &getOutNodes() ) ); }

public:
    //! Read-only abstract item model of this node out nodes.
    Q_PROPERTY( QAbstractItemModel* outEdges READ qmlGetOutEdges CONSTANT FINAL )
    QAbstractItemModel* qmlGetOutEdges() const { return const_cast< QAbstractItemModel* >( qobject_cast< const QAbstractItemModel* >( &gtpo::GenNode< qan::GraphConfig >::getOutEdges() ) ); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Behaviours Management *///---------------------------------------
    //@{
public:
    virtual void    installBehaviour( std::unique_ptr<qan::NodeBehaviour> behaviour );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Appearance Management *///---------------------------------------
    //@{
public:
    Q_PROPERTY( QString label READ getLabel WRITE setLabel NOTIFY labelChanged FINAL )
    void            setLabel( const QString& label ) { _label = label; emit labelChanged( ); }
    QString         getLabel( ) const { return _label; }
private:
    QString         _label = QString{ "" };
signals:
    void            labelChanged( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node Group Management *///---------------------------------------
    //@{
public:
    Q_PROPERTY( qan::Group* group READ qmlGetGroup FINAL )
protected:
    inline qan::Group*  qmlGetGroup() noexcept { return getGroup().lock().get(); }
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Node )

#endif // qanNode_h
