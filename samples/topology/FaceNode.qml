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
// This file is a part of the QuickQanava software library. Copyright 2015 Benoit AUTHEMAN.
//
// \file	ImgNode.qml
// \author	benoit@destrat.io
// \date	2016 02 11
//-----------------------------------------------------------------------------

import QtQuick              2.7
import QtQuick.Controls     2.0
import QtQuick.Layouts      1.3
import QtGraphicalEffects   1.0

import QuickQanava          2.0 as Qan
import "qrc:/QuickQanava"   as Qan

Qan.NodeItem {
    id: faceNodeItem
    Layout.preferredWidth: 100
    Layout.preferredHeight: 125
    width: Layout.preferredWidth
    height: Layout.preferredHeight

    DropShadow {
        id: backgroundShadow
        anchors.fill: parent
        source: image
        horizontalOffset: faceNodeItem.style.shadowRadius
        verticalOffset: faceNodeItem.style.shadowRadius
        radius: 4; samples: 8
        color: faceNodeItem.style.shadowColor
        visible: faceNodeItem.style.hasShadow
        transparentBorder: true
    }
    ColumnLayout {
        z: 2
        Label { text: "w=" + image.sourceSize.width }
        Label { text: "h=" + image.sourceSize.height }
    }
    Image {
        id: image
        z: 1
        anchors.fill: parent
        smooth: true
        source: faceNodeItem.node.image
    }
}
