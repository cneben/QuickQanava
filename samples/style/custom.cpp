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
// \file	custom.cpp
// \author	benoit@destrat.io
// \date	2017 03 19
//-----------------------------------------------------------------------------

// Qt headers
#include <QQmlEngine>
#include <QQmlComponent>

// QuickQanava headers
#include "../../src/qanGraph.h"
#include "./custom.h"

QQmlComponent*  CustomRectNode::delegate(QQmlEngine& engine) noexcept
{
    static std::unique_ptr<QQmlComponent>   CustomRectNode_delegate;
    if ( !CustomRectNode_delegate )
        CustomRectNode_delegate = std::make_unique<QQmlComponent>(&engine, "qrc:/RectNode.qml");
    return CustomRectNode_delegate.get();
}

qan::NodeStyle* CustomRectNode::style() noexcept
{
    static std::unique_ptr<qan::NodeStyle>  CustomRectNode_style;
    if ( !CustomRectNode_style ) {
        CustomRectNode_style = std::make_unique<qan::NodeStyle>();
        CustomRectNode_style->setBackColor(QColor("#ff29fc"));
    }
    return CustomRectNode_style.get();
}


QQmlComponent*  CustomRoundNode::delegate(QQmlEngine& engine) noexcept
{
    static std::unique_ptr<QQmlComponent>   CustomRoundNode_delegate;
    if ( !CustomRoundNode_delegate )
        CustomRoundNode_delegate = std::make_unique<QQmlComponent>(&engine, "qrc:/RoundNode.qml");
    return CustomRoundNode_delegate.get();
}

qan::NodeStyle* CustomRoundNode::style() noexcept
{
    static std::unique_ptr<qan::NodeStyle>  CustomRoundNode_style;
    if ( !CustomRoundNode_style ) {
        CustomRoundNode_style = std::make_unique<qan::NodeStyle>();
        CustomRoundNode_style->setBackColor(QColor("#0770ff"));
    }
    return CustomRoundNode_style.get();
}


QQmlComponent*  CustomEdge::delegate(QQmlEngine& engine) noexcept
{
    static std::unique_ptr<QQmlComponent>   CustomEdge_delegate;
    if ( !CustomEdge_delegate )
        CustomEdge_delegate = std::make_unique<QQmlComponent>(&engine, "qrc:/CustomEdge.qml");
    return CustomEdge_delegate.get();
}

qan::EdgeStyle* CustomEdge::style() noexcept
{
    static std::unique_ptr<qan::EdgeStyle>  CustomEdge_style;
    if ( !CustomEdge_style )
        CustomEdge_style = std::make_unique<qan::EdgeStyle>();
    return CustomEdge_style.get();
}

qan::Node*  CustomGraph::insertRectNode()
{
    return qan::Graph::insertNode<CustomRectNode>(nullptr);
}

qan::Node*  CustomGraph::insertRoundNode()
{
    return qan::Graph::insertNode<CustomRoundNode>();
}

qan::Edge*  CustomGraph::insertCustomEdge(qan::Node* source, qan::Node* destination)
{
    const auto engine = qmlEngine(this);
    if ( source != nullptr && destination != nullptr && engine != nullptr )
        return qan::Graph::insertEdge<CustomEdge>(*source, destination, nullptr, CustomEdge::delegate(*engine) );
    return nullptr;
}
