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
// \file	CustomNode.qml
// \author	benoit@destrat.io
// \date	2015 08 01
//-----------------------------------------------------------------------------

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects

import QuickQanava          2.0 as Qan
import "qrc:/QuickQanava" as Qan

Qan.NodeItem {
    id: roundNode
    width: 60; height: 60
    minimumSize: Qt.size(60,60)
    x: 15;      y: 15
    Rectangle {
        id: background
        z: 1
        anchors.fill: parent
        radius: width / 2;
        border.color: Material.accent; border.width: 2
        color: roundNode.style.backColor
        gradient: Gradient {
            GradientStop { position: 0.0; color: roundNode.nodeColor }
            GradientStop {
                position: 1.0;
                color: Qt.tint(roundNode.nodeColor, roundNode.backColor)
            }
        }
    }
    property color styleBackColor: style.backColor
    onStyleBackColorChanged: nodeColor = Qt.rgba( style.backColor.r, style.backColor.g, style.backColor.b, 0.2 )
    property color nodeColor
    property color backColor: Material.background

    Rectangle {
        id: border
        z: 3
        anchors.fill: parent
        radius: width / 2;
        border.color: Material.accent; border.width: 2
        color: Qt.rgba(0, 0, 0, 0)
    }
    Label {
        text: roundNode.node ? roundNode.node.label : ""
        z: 3
        anchors.centerIn: parent
    }
    MultiEffect {
        source: background
        anchors.centerIn: parent
        width: background.width + (6 * 2)       // glowRadius = 6
        height: background.height + (6 * 2)
        blurEnabled: true
        blurMax: 30
        blur: 1.
        colorization: 1.0
        colorizationColor: Qt.rgba(1, 1, 1, 0.3)
    }
}  // Qan.NodeItem: roundNode
