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
// \file	qanFaceNode.cpp
// \author	benoit@destrat.io
// \date	2017 08 12
//-----------------------------------------------------------------------------

// QuickQanava headers
#include "../../src/QuickQanava.h"
#include "./qanFaceNode.h"

namespace qan { // ::qan

FaceNode::~FaceNode() { /* Nil */ }

static std::unique_ptr<QQmlComponent>   qan_FaceNode_delegate;

QQmlComponent*  FaceNode::delegate(QObject* caller) noexcept
{
    if ( !qan_FaceNode_delegate &&
         caller != nullptr ) {
        const auto engine = qmlEngine(caller);
        if ( engine != nullptr )
            qan_FaceNode_delegate = std::make_unique<QQmlComponent>(engine, "qrc:/FaceNode.qml");
        else qWarning() << "[static]qan::Node::delegate(): Error: QML engine is nullptr.";
    }
    return qan_FaceNode_delegate.get();
}

} // ::qan