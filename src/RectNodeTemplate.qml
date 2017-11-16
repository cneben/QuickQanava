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
// This file is a part of the QuickQanava software. Copyright 2015 Benoit AUTHEMAN.
//
// \file	NodeRectTemplate.qml
// \author	benoit@destrat.io
// \date	2015 11 30
//-----------------------------------------------------------------------------

import QtQuick              2.7
import QtQuick.Layouts      1.3
import QtQuick.Controls     2.0
import QtGraphicalEffects   1.0

import QuickQanava          2.0 as Qan

/*! \brief Default template component for building a custom rectangular qan::Node item.
 *
 * Node with custom content definition using "templates" is described in \ref qanavacustom
 */
Item {
    id: template
    property var            nodeItem: undefined
    default property alias  children : contentLayout.children

    property real shadowMargin: 15
    property real rectRadius: 4
    Item {
        id: fakeBackground
        anchors.centerIn: parent
        layer.enabled: true
        width: template.width + shadowMargin; height: template.height + shadowMargin
        visible: false
        Rectangle {
            anchors.centerIn: parent
            width: template.width - 1;  height: template.height - 1
            radius: rectRadius
            color: Qt.rgba(0,0,0,1)
            antialiasing: true
            visible: false
            layer.enabled: true
            layer.effect: DropShadow {
                horizontalOffset: nodeItem.style.shadowRadius
                verticalOffset: nodeItem.style.shadowRadius
                radius: 4; samples: 8
                color: nodeItem.style.shadowColor
                visible: nodeItem.style.hasShadow
                transparentBorder: true
                cached: false
            }
        }
    }
    Item {
        id: backgroundMask
        anchors.centerIn: parent
        width: parent.width + shadowMargin; height: parent.height + shadowMargin
        visible: false
        Rectangle {
            anchors.centerIn: parent
            width: template.width + 1;  height: template.height + 1
            radius: rectRadius
            color: Qt.rgba(0,0,0,1)
            antialiasing: true
        }
    }
    OpacityMask {
        anchors.centerIn: parent
        width: parent.width + shadowMargin; height: parent.height + shadowMargin
        source: ShaderEffectSource { sourceItem: fakeBackground; hideSource: false }
        maskSource: ShaderEffectSource { format: ShaderEffectSource.Alpha; sourceItem: backgroundMask; hideSource: false }
        invert: true
    }

    Rectangle {
        id: background
        anchors.fill: parent    // Background follow the content layout implicit size
        radius: rectRadius
        color: nodeItem.style.backColor
        border.color: nodeItem.style.borderColor
        border.width: nodeItem.style.borderWidth
        antialiasing: true
        opacity: nodeItem.style.backOpacity
        // Note: Do not enable layer to avoid aliasing at high scale
    }
    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: background.radius / 2; spacing: 0
        visible: !labelEditor.visible
        Label {
            id: nodeLabel
            Layout.fillWidth: true
            Layout.fillHeight: contentLayout.children.length === 0
            Layout.preferredHeight: contentHeight
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            textFormat: Text.PlainText
            text: nodeItem && nodeItem.node ? nodeItem.node.label : ""
            horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
            maximumLineCount: 3 // Must be set, otherwise elide don't work and we end up with single line text
            elide: Text.ElideRight; wrapMode: Text.Wrap
        }
        Item {
            id: contentLayout
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true;
            Layout.fillHeight: true
            visible: contentLayout.children.length > 0  // Hide if the user has not added any content
        }
    }
    Connections {
        target: nodeItem
        onNodeDoubleClicked: labelEditor.visible = true
    }
    LabelEditor {
        id: labelEditor
        anchors.fill: parent
        anchors.margins: background.radius / 2
        target: parent.nodeItem.node
        visible: false
    }
}
