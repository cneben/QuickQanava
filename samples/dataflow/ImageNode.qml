/*
 Copyright (c) 2008-2024, Benoit AUTHEMAN All rights reserved.

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
// This file is a part of the QuickQanava software library. Copyright 2024 Benoit AUTHEMAN.
//
// \file	ImageNode.qml
// \author	benoit@destrat.io
// \date	2017 12 12
//-----------------------------------------------------------------------------

import QtQuick
import QtQuick.Effects
import QtQuick.Controls
import QtQuick.Layouts

import QuickQanava as Qan

Qan.NodeItem {
    id: imageNodeItem
    Layout.preferredWidth: 125
    Layout.preferredHeight: 125
    width: Layout.preferredWidth
    height: Layout.preferredHeight
    connectable: Qan.NodeItem.UnConnectable     // Do not show visual edge connector, use out port instead

    MultiEffect {
        source: image
        anchors.fill: parent
        anchors.margins: 1
        autoPaddingEnabled: false
        readonly property real shadowOffset: imageNodeItem.style.effectRadius
        paddingRect: Qt.rect(shadowOffset,  shadowOffset,
                             parent.width + shadowOffset,
                             parent.height + shadowOffset)
        shadowEnabled: imageNodeItem.style?.effectEnabled || false
        shadowColor: imageNodeItem.style.effectColor
        blurMax: imageNodeItem.style.effectRadius
        shadowHorizontalOffset:  imageNodeItem.style.effectRadius
        shadowVerticalOffset:  imageNodeItem.style.effectRadius
    }
    ComboBox {
        z: 2
        anchors.left: parent.left; anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 4
        opacity: 0.85
        model: [ "Lenna", "Jason"]
        onCurrentIndexChanged: {
            node.output = currentIndex === 0 ? "qrc:/qt/qml/SampleDataFlow/Lenna.jpeg" : "qrc:/qt/qml/SampleDataFlow/JS.jpeg"
        }
    }
    Pane {
        z: 2
        padding: 1
        anchors.left: parent.left; anchors.bottom: parent.bottom;
        opacity: 0.9
        Label { text: image.sourceSize.width + "x" + image.sourceSize.height + "px" }
    }
    Image {
        id: image
        z: 1
        anchors.fill: parent
        smooth: true
        source: imageNodeItem.node.output
    }
}
