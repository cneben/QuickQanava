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
    if ( !CustomRectNode_style )
        CustomRectNode_style = std::make_unique<qan::NodeStyle>();
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
        qDebug() << "QColor(#00ff00)=" << QColor("#00ff00");
        CustomRoundNode_style->setBackColor(QColor("#00ff00"));
    }
    return CustomRoundNode_style.get();
}


qan::Node*  CustomGraph::insertRectNode()
{
    return qan::Graph::insertNode<CustomRectNode>(nullptr);
}

qan::Node*  CustomGraph::insertRoundNode()
{
    return qan::Graph::insertNode<CustomRoundNode>();
}

