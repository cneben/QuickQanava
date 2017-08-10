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


static std::unique_ptr<QQmlComponent>   CustomRectNode_delegate;
static std::unique_ptr<qan::NodeStyle>  CustomRectNode_style;

QQmlComponent*  CustomRectNode::delegate(QObject* caller) noexcept
{
    if ( !CustomRectNode_delegate &&
         caller != nullptr ) {
        const auto engine = qmlEngine(caller);
        if ( engine != nullptr )
            CustomRectNode_delegate = std::make_unique<QQmlComponent>(engine, "qrc:/RectNode.qml");
        else qWarning() << "[static]CustomRectNode::delegate(): Error: QML engine is nullptr.";
    }
    return CustomRectNode_delegate.get();
}

qan::NodeStyle* CustomRectNode::style() noexcept
{
    if ( !CustomRectNode_style ) {
        CustomRectNode_style = std::make_unique<qan::NodeStyle>();
        CustomRectNode_style->setBackColor(QColor("#ff29fc"));
    }
    return CustomRectNode_style.get();
}

static std::unique_ptr<QQmlComponent>   CustomRoundNode_delegate;
static std::unique_ptr<qan::NodeStyle>  CustomRoundNode_style;

QQmlComponent*  CustomRoundNode::delegate(QObject* caller) noexcept
{
    if ( !CustomRoundNode_delegate &&
         caller != nullptr ) {
        const auto engine = qmlEngine(caller);
        if ( engine != nullptr )
            CustomRoundNode_delegate = std::make_unique<QQmlComponent>(engine, "qrc:/RoundNode.qml");
        else qWarning() << "[static]CustomRoundNode::delegate(): Error: QML engine is nullptr.";
    }
    return CustomRoundNode_delegate.get();
}

qan::NodeStyle* CustomRoundNode::style() noexcept
{
    if ( !CustomRoundNode_style ) {
        CustomRoundNode_style = std::make_unique<qan::NodeStyle>();
        CustomRoundNode_style->setBackColor(QColor("#0770ff"));
    }
    return CustomRoundNode_style.get();
}

static std::unique_ptr<QQmlComponent>   CustomEdge_delegate;
static std::unique_ptr<qan::EdgeStyle>  CustomEdge_style;

QQmlComponent*  CustomEdge::delegate(QObject* caller) noexcept
{
    if ( !CustomEdge_delegate &&
         caller != nullptr ) {
        const auto engine = qmlEngine(caller);
        if ( engine != nullptr )
            CustomEdge_delegate = std::make_unique<QQmlComponent>(engine, "qrc:/CustomEdge.qml");
        else qWarning() << "[static]CustomEdge::delegate(): Error: QML engine is nullptr.";
    }
    return CustomEdge_delegate.get();
}

qan::EdgeStyle* CustomEdge::style() noexcept
{
    if ( !CustomEdge_style ) {
        CustomEdge_style = std::make_unique<qan::EdgeStyle>();
    }
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
    if ( source != nullptr && destination != nullptr )
        return qan::Graph::insertEdge<CustomEdge>(*source, destination, nullptr, CustomEdge::delegate(this) );
    return nullptr;
}
