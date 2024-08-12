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
// This file is a part of the QuickQanava software library.
//
// \file	RectGroupTemplate.qml
// \author	benoit@destrat.io
// \date	2016 06 21
//-----------------------------------------------------------------------------

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickQanava 2.0 as Qan

Item {
    id: template

    default property alias children : content.children

    // Binded to Tpp.Group.container in Tpp.Group.
    property alias  container: content

    property var    groupItem: undefined

    //! Show or hide group top left label editor (default to visible).
    readonly property alias labelEditorVisible: labelEditorControl.visible

    property alias  header: headerLayout

    property alias  backRadius: groupBackground.backRadius

    enabled: {
        if (groupItem &&
            groupItem.group) {
            if (groupItem.group.isTable())   // If group is a table, let hlg::TableGroup manage locking.
                return true
            return !groupItem.group.locked   // Otherwise, disable the delegate for locked group
        }
        return true;
    }
    RectGradientBackground {    // Apply node background styling
        id: groupBackground
        anchors.fill: parent
        style: template.groupItem?.style
        visible: !groupItem.collapsed
    }
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        RowLayout {
            id: headerLayout
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            z: 2
            spacing: 0
            ToolButton {
                id: collapser
                padding: 0
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                text: groupItem ? (groupItem.collapsed ? "+" : "-") : "-"
                font.pixelSize: 13
                font.bold: true
                onClicked: groupItem.collapsed = !groupItem.collapsed
                visible: groupItem?.expandButtonVisible
            }
            Item {
                id: labelEditorControl
                clip: false
                Layout.fillWidth: true
                Layout.fillHeight: true
                property int labelPointSize : groupItem?.style?.fontPointSize || Material.fontSize
                LabelEditor {
                    id: labelEditor
                    clip: false
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    target: groupItem?.group
                    visible: false
                    fontPixelSize: labelEditorControl.labelPointSize
                }
                Label {
                    id: groupLabel
                    anchors.fill: parent
                    anchors.leftMargin: collapser.visible ? 0 : 10
                    text: groupItem &&
                          groupItem.group ? groupItem.group.label :
                                            "              "
                    visible: !labelEditor.visible
                    verticalAlignment: Text.AlignVCenter
                    font.bold: groupItem.style.fontBold
                    font.pointSize: labelEditorControl.labelPointSize
                    color: groupItem?.style?.labelColor || "black"
                    elide:  Text.ElideRight
                    MouseArea {
                        anchors.fill: parent
                        enabled: !groupItem.group.isProtected &&
                                 !groupItem.group.locked    // Do not allow dragging of locked groups
                        preventStealing: true
                        propagateComposedEvents: true // Ensure event are forwarded to collapserArea
                        onDoubleClicked: labelEditor.visible = true
                    }
                }
            } // labelEditor Item
        } // RowLayout: collapser + label
        Rectangle {
            // FIXME #238 do not show for tables...
            Layout.fillWidth: true
            Layout.margins: 0
            height: 1
            border.width: 0
            color: groupItem?.style?.borderColor || Qt.rgba(1., 1., 1., 0.)
            opacity: 0.8
            visible: !groupItem.collapsed
        }
        Item {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true
            z: 3
            visible: !groupItem.collapsed
            enabled: !groupItem.collapsed
        }
    }


    // Emitted by qan::GroupItem when node dragging start
    function onNodeDragEnter() { /*groupBackground.backColor = Qt.binding( function() { return Qt.darker( template.groupItem.style.backColor, 1.05 ) } ) */}
    // Emitted by qan::GroupItem when node dragging ends
    function onNodeDragLeave() { /*groupBackground.backColor = Qt.binding( function() { return template.groupItem.style.backColor } ) */}
}
