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
// \file	custom.h
// \author	benoit@destrat.io
// \date	2017 03 19
//-----------------------------------------------------------------------------

#ifndef custom_h
#define custom_h

// QuickQanava headers
#include "QuickQanava"

class CustomRectNode : public qan::Node
{
    Q_OBJECT
public:
    explicit CustomRectNode(QObject* parent=nullptr)  : qan::Node{parent} { /* Nil */ }
    virtual ~CustomRectNode() { /* Nil */ }
    CustomRectNode( const CustomRectNode& ) = delete;

public:
    static  QQmlComponent*      delegate(QObject* caller) noexcept;
    static  qan::NodeStyle*     style() noexcept;
};

class CustomRoundNode : public qan::Node
{
    Q_OBJECT
public:
    explicit CustomRoundNode(QObject* parent=nullptr) : qan::Node{parent} { }
    virtual ~CustomRoundNode() { /* Nil */ }
    CustomRoundNode( const CustomRoundNode& ) = delete;

public:
    static  QQmlComponent*      delegate(QObject* caller) noexcept;
    static  qan::NodeStyle*     style() noexcept;
};

QML_DECLARE_TYPE( CustomRectNode )
QML_DECLARE_TYPE( CustomRoundNode )

class CustomEdge : public qan::Edge
{
    Q_OBJECT
public:
    explicit CustomEdge() : qan::Edge{} { }
    virtual ~CustomEdge() { /* Nil */ }
    CustomEdge( const CustomEdge& ) = delete;

public:
    static  QQmlComponent*      delegate(QObject* caller) noexcept;
    static  qan::EdgeStyle*     style() noexcept;
};

QML_DECLARE_TYPE( CustomEdge )

class CustomGraph : public qan::Graph
{
    Q_OBJECT
public:
    explicit CustomGraph( QQuickItem* parent = nullptr ) : qan::Graph(parent) { /* Nil */ }
    virtual ~CustomGraph() { /* Nil */ }
    CustomGraph( const CustomGraph& ) = delete;

public:
    Q_INVOKABLE qan::Node*  insertRectNode();
    Q_INVOKABLE qan::Node*  insertRoundNode();
    Q_INVOKABLE qan::Edge*  insertCustomEdge(qan::Node* source, qan::Node* destination);
};

QML_DECLARE_TYPE( CustomGraph )

#endif // custom_h

