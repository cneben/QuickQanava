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
// \file	qanFlowNode.h
// \author	benoit@destrat.io
// \date	2016 12 12
//-----------------------------------------------------------------------------

#ifndef qanFlowNode_h
#define qanFlowNode_h

// QuickQanava headers
#include <QuickQanava>

// Qt headers
#include <QQuickPaintedItem>

namespace qan { // ::qan

class FlowNodeBehaviour : public qan::NodeBehaviour
{
    Q_OBJECT
public:
    explicit FlowNodeBehaviour(QObject* parent = nullptr) : qan::NodeBehaviour{ "FlowNodeBehaviour", parent } { /* Nil */ }
    virtual ~FlowNodeBehaviour( ) { /* Nil*/ }
    FlowNodeBehaviour(FlowNodeBehaviour&) = delete;
protected:
    virtual void    inNodeInserted( qan::Node& inNode, qan::Edge& edge ) noexcept override;
    virtual void    inNodeRemoved( qan::Node& inNode, qan::Edge& edge ) noexcept override;
};

class FlowNode : public qan::Node
{
    Q_OBJECT
public:
    explicit FlowNode( QQuickItem* parent = nullptr ) : qan::Node{parent} { /* Nil */ }
    virtual ~FlowNode() { /* Nil */ }

    FlowNode(const FlowNode&) = delete;
    FlowNode& operator=(const FlowNode&) = delete;
    FlowNode(FlowNode&&) = delete;
    FlowNode& operator=(FlowNode&&) = delete;

public:
    enum class Type : unsigned int {
        INPUT,
        NUM_OP,
        IMAGE,
        TINT_OP
    };
    Q_ENUM(Type)

    Q_PROPERTY(Type type READ getType CONSTANT FINAL)
    inline  Type    getType() const noexcept { return _type; }
protected:
    Type        _type{Type::INPUT};

public slots:
    void            inNodeOutputChanged();

public:
    Q_PROPERTY(QVariant output READ getOutput WRITE setOutput NOTIFY outputChanged)
    inline QVariant getOutput() const noexcept { return _output; }
    void            setOutput(QVariant output) noexcept;
protected:
    QVariant        _output;
signals:
    void            outputChanged();

    /*! \name Node Static Factories *///---------------------------------------
    //@{
public:
    static  QQmlComponent*      delegate(QObject* caller) noexcept;
    //@}
    //-------------------------------------------------------------------------
};

class FlowGraph : public qan::Graph
{
    Q_OBJECT
public:
    explicit FlowGraph( QQuickItem* parent = nullptr ) noexcept : qan::Graph(parent) { }
public:
    Q_INVOKABLE qan::Node* insertFlowNode();
};

} // ::qan

QML_DECLARE_TYPE( qan::FlowNode )
QML_DECLARE_TYPE( qan::FlowGraph )

#endif // qanFlowNode_h

