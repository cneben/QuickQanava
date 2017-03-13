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
// \file	qanConnector.h
// \author	benoit@destrat.io
// \date	2017 03 10
//-----------------------------------------------------------------------------

#ifndef qanConnector_h
#define qanConnector_h

// Qt headers
#include <QQuickItem>

// QuickQanava headers
#include "./qanNodeItem.h"

namespace qan { // ::qan

class Graph;
class Node;

/*! \brief Base class for modelling the connector draggable visual node.
 *
 * \nosubgrouping
 */
class Connector : public qan::NodeItem
{
    /*! \name Connector Object Management *///---------------------------------
    //@{
    Q_OBJECT
public:
    explicit Connector( QQuickItem* parent = nullptr );
    virtual ~Connector();
    Connector(const Connector&) = delete;

public:
    Q_PROPERTY( qan::Graph* graph READ getGraph WRITE setGraph NOTIFY graphChanged FINAL )
    auto    setGraph(qan::Graph* graph) noexcept -> void;
protected:
    auto    getGraph() const noexcept -> qan::Graph*;
    QPointer<qan::Graph>    _graph;
signals:
    void    graphChanged();
    //@}
    //-------------------------------------------------------------------------

    /*! \name Connector Configuration *///-------------------------------------
    //@{
public:
    //! Graphical item used as a draggable destination node selector (initialized and owned from QML).
    Q_PROPERTY( QQuickItem* connectorItem READ getConnectorItem WRITE setConnectorItem NOTIFY connectorItemChanged FINAL )
    auto    getConnectorItem() noexcept -> QQuickItem*;
    auto    setConnectorItem(QQuickItem* connectorItem) noexcept -> void;
signals:
    void    connectorItemChanged();
protected:
    QPointer<QQuickItem>  _connectorItem;

public:
    Q_PROPERTY( QQmlComponent* edgeComponent READ getEdgeComponent WRITE setEdgeComponent NOTIFY edgeComponentChanged FINAL )
    auto    getEdgeComponent() noexcept -> QQmlComponent*;
    auto    setEdgeComponent(QQmlComponent* edgeComponent) noexcept -> void;
signals:
    void    edgeComponentChanged();
protected:
    QPointer<QQmlComponent>  _edgeComponent;

public:
    Q_PROPERTY( qan::EdgeItem* edgeItem READ getEdgeItem NOTIFY edgeItemChanged FINAL )
    auto    getEdgeItem() noexcept -> qan::EdgeItem*;
signals:
    void    edgeItemChanged();
protected:
    QPointer<qan::EdgeItem>  _edgeItem;

public:
    /*! \brief Connector source node (ie host node for the visual draggable connector item).
     *
     * \note Connector item is automatically hidden if \c sourceNode is nullptr or \c sourceNode is
     * destroyed.
     */
    Q_PROPERTY( qan::Node* sourceNode READ getSourceNode WRITE setSourceNode NOTIFY sourceNodeChanged FINAL )
    void                setSourceNode( qan::Node* sourceNode );
    inline qan::Node*   getSourceNode() const noexcept { return _sourceNode.data(); }
private:
    QPointer<qan::Node> _sourceNode;
signals:
    void                sourceNodeChanged();
private slots:
    //! Called when the current source node is destroyed.
    void                sourceNodeDestroyed();
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE(qan::Connector)

#endif // qanConnector_h
