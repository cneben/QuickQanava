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
// \file	qanGroup.cpp
// \author	benoit@destrat.io
// \date	2016 03 22
//-----------------------------------------------------------------------------

// Qt headers
#include <QBrush>
#include <QPainter>
#include <QPainterPath>

// QuickQanava headers
#include "./qanNode.h"
#include "./qanGroup.h"
#include "./qanGroupItem.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Group Object Management *///------------------------------------------------
Group::Group( QObject* parent ) :
    gtpo::GenGroup< qan::GraphConfig >{}
{
    Q_UNUSED(parent);
}

Group::~Group() {
    if ( _item )
        _item->deleteLater();
}

qan::Graph*         Group::getGraph() noexcept {
    return qobject_cast< qan::Graph* >( gtpo::GenGroup< qan::GraphConfig >::getGraph() );
}

const qan::Graph*   Group::getGraph() const noexcept {
    return qobject_cast< const qan::Graph* >( gtpo::GenGroup< qan::GraphConfig >::getGraph() );
}

qan::GroupItem*  Group::getItem() noexcept { return _item.data(); }
const qan::GroupItem*   Group::getItem() const noexcept { return _item.data(); }

void    Group::setItem(qan::GroupItem* item) noexcept
{
    if ( item != nullptr ) {
        _item = item;
        if ( item->getGroup() != this )
            item->setGroup(this);
    }
}

void    Group::itemProposeNodeDrop()
{
    if ( _item )
        _item->proposeNodeDrop();
}

void    Group::itemEndProposeNodeDrop()
{
    if ( _item )
        _item->endProposeNodeDrop();
}
//-----------------------------------------------------------------------------

/* Group Static Factories *///-------------------------------------------------
static std::unique_ptr<QQmlComponent>   qan_Group_delegate;
static std::unique_ptr<qan::NodeStyle>  qan_Group_style;

QQmlComponent*  Group::delegate(QObject* caller) noexcept
{
    if ( !qan_Group_delegate &&
         caller != nullptr ) {
        const auto engine = qmlEngine(caller);
        if ( engine != nullptr )
            qan_Group_delegate = std::make_unique<QQmlComponent>(engine, "qrc:/QuickQanava/Group.qml");
    }
    return qan_Group_delegate.get();
}

qan::Style*     Group::style() noexcept
{
    if ( !qan_Group_style )
        qan_Group_style = std::make_unique<qan::NodeStyle>();
    return qan_Group_style.get();
}
//-----------------------------------------------------------------------------

/* Group Nodes Management *///-------------------------------------------------
bool    Group::hasNode( qan::Node* node ) const
{
    if ( node == nullptr )
        return false;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return false; }
    return gtpo::GenGroup< qan::GraphConfig >::hasNode( weakNode );
}
//-----------------------------------------------------------------------------

} // ::qan
