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
// \file	CustomGroup.qml
// \author	benoit@destrat.io
//-----------------------------------------------------------------------------

import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Controls.Material

import QuickQanava 2.0 as Qan

Qan.GroupItem {
    id: customGroup
    width: 400
    height: 200

    // Note: This a completely custom group sample where you could customize group geometry,
    // appearance. If you just need a rectangular group, use RectGroupTemplate and default
    // QuickQanava styling options.

    // Customize your group appearance here
    // <---------------------  Begin Customization
    readonly property color groupColor: "blue"
    readonly property color backColor: "violet"
    Rectangle {
        id: background
        z: 0
        anchors.fill: parent
        radius: 2; color: "yellow"
        border.color: Material.accent; border.width: 4
        clip: true
        gradient: Gradient {
            GradientStop { position: 0.0; color: customGroup.groupColor }
            GradientStop {
                position: 1.0;
                color: Qt.tint(customGroup.groupColor, customGroup.backColor)
            }
        }
    }
    MultiEffect {
        source: background
        anchors.centerIn: parent
        z: -1
        width: background.width + (6 * 2)       // glowRadius = 6
        height: background.height + (6 * 2)
        blurEnabled: true
        blurMax: 30
        blur: 1.
        colorization: 1.0
        colorizationColor: Qt.rgba(0.8, 0.8, 0.8, 0.8)
    }
    Rectangle {
        id: content
        anchors.fill: parent; anchors.margins: background.border.width / 2.
        color: Qt.rgba(0,0,0,0) // === "transparent"
        z: 3
    } // Rectangle
    Label {
        z: 4
        text: group ? group.label : ""
        anchors.centerIn: parent
    }
    Label {
        z: 4
        text: "Custom group"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom; anchors.bottomMargin: 15
    }
    Label {
        id: dndNotifier
        z: 4
        visible: false
        text: "Drop node"
        color: "red"
        font.bold: true
        anchors.centerIn: parent
    }
    // <--------------------- End Customization

    // QuickQanava interface (enable node DnD and selection), necessary for custom groups
    // not relying on Qan.RectGroupTemplate

    // Parent Qan.GroupItem (and qan::GroupItem) must be aware of the concrete group content item to
    // enable DnD for grouping/ungrouping nodes, see qan::GroupItem::container property documentation.
    // MANDATORY
    container: content   // NOTE: content.z _must_ be > to background.z, otherwise nodes drag and dropped into
                         // the group would be behing our background linear gradient effect (backgroundEffet)

    // Enable custom visual notifications when a node is dragged inside a groupe.
    // OPTIONAL

    // Emitted by qan::GroupItem when node dragging start
    onNodeDragEnter: { dndNotifier.visible = true }
    // Emitted by qan::GroupItem when node dragging ends
    onNodeDragLeave: { dndNotifier.visible = false}
}
