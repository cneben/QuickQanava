/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#include "./qanGroupItem.h"
#include "./qanNodeItem.h"

namespace qan { // ::qan

class Graph;
class Node;

/*! \brief Base class for modelling the connector draggable visual node.
 *
 * \note While qan::Connector is not a qan::Node, it also have a default factory interface component() and style().
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

    /*! \name Connector Static Factories *///----------------------------------
    //@{
public:
    static  QQmlComponent*      delegate(QObject* caller) noexcept;
    static  qan::NodeStyle*     style() noexcept;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Connector Configuration *///-------------------------------------
    //@{
signals:
    //! Emitted when \c createDefaultEdge is set to false to request creation of an edge after the visual connector has been dragged on a destination node or edge.
    void    requestEdgeCreation(qan::Node* src, QObject* dst);
    //! Emmited after an edge has been created to allow user configuration (not emmited when \c createDefaultEdge is set to false).
    void    edgeInserted(qan::Edge* edge);

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
