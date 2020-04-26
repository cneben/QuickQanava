/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
// \file	qanGraphView.cpp
// \author	benoit@destrat.io
// \date	2016 08 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QQuickItem>

// QuickQanava headers
#include "./qanNavigable.h"
#include "./qanGraphView.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* GraphView Object Management *///--------------------------------------------
GraphView::GraphView(QQuickItem* parent) :
    qan::Navigable{parent}
{
    setAntialiasing(true);
    setSmooth(true);
}

void    GraphView::setGraph(qan::Graph* graph)
{
    if ( graph == nullptr ) {
        qWarning() << "qan::GraphView::setGraph(): Error: Setting a nullptr graph in Qan.GraphView is not supported.";
        return;
    }
    if ( graph != _graph ) {
        if ( _graph != nullptr )
            disconnect(_graph, 0, this, 0 );
        _graph = graph;
        _graph->setContainerItem( getContainerItem() );
        connect( _graph, &qan::Graph::nodeClicked,
                 this,   &qan::GraphView::nodeClicked );

        connect( _graph, &qan::Graph::connectorChanged,
                 this,   &qan::GraphView::connectorChanged );

        connect( _graph, &qan::Graph::nodeRightClicked,
                 this,   &qan::GraphView::nodeRightClicked );
        connect( _graph, &qan::Graph::nodeDoubleClicked,
                 this,   &qan::GraphView::nodeDoubleClicked );

        connect( _graph, &qan::Graph::portClicked,
                 this,   &qan::GraphView::portClicked );
        connect( _graph, &qan::Graph::portRightClicked,
                 this,   &qan::GraphView::portRightClicked );

        connect( _graph, &qan::Graph::edgeClicked,
                 this,   &qan::GraphView::edgeClicked );
        connect( _graph, &qan::Graph::edgeRightClicked,
                 this,   &qan::GraphView::edgeRightClicked );
        connect( _graph, &qan::Graph::edgeDoubleClicked,
                 this,   &qan::GraphView::edgeDoubleClicked );

        connect( _graph, &qan::Graph::groupClicked,
                 this,   &qan::GraphView::groupClicked );
        connect( _graph, &qan::Graph::groupRightClicked,
                 this,   &qan::GraphView::groupRightClicked );
        connect( _graph, &qan::Graph::groupDoubleClicked,
                 this,   &qan::GraphView::groupDoubleClicked );
        emit graphChanged();
    }
}

void    GraphView::navigableClicked(QPointF pos)
{
    Q_UNUSED(pos)
    if ( _graph )
        _graph->clearSelection();
}

void    GraphView::navigableRightClicked(QPointF pos) {
    emit    rightClicked(pos);
}
//-----------------------------------------------------------------------------

} // ::qan

