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
// \file	qanNode.cpp
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QPainter>
#include <QPainterPath>

// QuickQanava headers
#include "./qanNode.h"
#include "./qanNodeItem.h"
#include "./qanGroup.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Node Object Management *///-------------------------------------------------
Node::Node(QObject* parent) :
    gtpo::GenNode< qan::GraphConfig >{}
{
    Q_UNUSED(parent)
}

Node::~Node()
{
    if ( _item )
        _item->deleteLater();
}

qan::Graph* Node::getGraph() noexcept {
    return qobject_cast< qan::Graph* >( gtpo::GenNode< qan::GraphConfig >::getGraph() );
}

const qan::Graph* Node::getGraph() const noexcept {
    return qobject_cast< const qan::Graph* >( gtpo::GenNode< qan::GraphConfig >::getGraph() );
}

bool    Node::operator==( const qan::Node& right ) const
{
    return getLabel() == right.getLabel();
}

qan::NodeItem*          Node::getItem() noexcept { return _item.data(); }
const qan::NodeItem*    Node::getItem() const noexcept { return _item.data(); }

void    Node::setItem(qan::NodeItem* nodeItem) noexcept
{
    if ( nodeItem != nullptr ) {
        _item = nodeItem;
        if ( nodeItem->getNode() != this )
            nodeItem->setNode(this);
    }
}
//-----------------------------------------------------------------------------

/* Node Static Factories *///--------------------------------------------------
static std::unique_ptr<QQmlComponent>   qan_Node_delegate;
static std::unique_ptr<qan::NodeStyle>  qan_Node_style;

QQmlComponent*  Node::delegate(QObject* caller) noexcept
{
    if ( !qan_Node_delegate &&
         caller != nullptr ) {
        const auto engine = qmlEngine(caller);
        if ( engine != nullptr )
            qan_Node_delegate = std::make_unique<QQmlComponent>(engine, "qrc:/QuickQanava/Node.qml");
        else qWarning() << "[static]qan::Node::delegate(): Error: QML engine is nullptr.";
    }
    return qan_Node_delegate.get();
}

qan::NodeStyle* Node::style() noexcept
{
    if ( !qan_Node_style )
        qan_Node_style = std::make_unique<qan::NodeStyle>();
    return qan_Node_style.get();
}
//-----------------------------------------------------------------------------

/* Behaviours Management *///--------------------------------------------------
void    Node::installBehaviour( std::unique_ptr<qan::NodeBehaviour> behaviour )
{
    addBehaviour( std::move( behaviour ) );
}
//-----------------------------------------------------------------------------

} // ::qan
