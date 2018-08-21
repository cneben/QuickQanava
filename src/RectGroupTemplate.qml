/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
// \file	RectGroupTemplate.qml
// \author	benoit@destrat.io
// \date	2016 06 21
//-----------------------------------------------------------------------------

import QtQuick              2.7
import QtQuick.Controls     2.1
import QtQuick.Layouts      1.3
import QtGraphicalEffects   1.0

import QuickQanava 2.0      as Qan

Item {
    id: template

    default property alias children : content.children
    property alias  content: content

    property var    groupItem: undefined

    //! Show or hide group top left label editor (default to visible).
    property alias labelEditorVisible : labelEditorControl.visible

    //! Show or hide group top left expand button (default to visible).
    property alias expandButtonVisible : collapser.visible

    Item {
        id: content
        anchors.fill: parent
        x: 0; y: 0; z: 3
        visible: !groupItem.collapsed
        enabled: !groupItem.collapsed
    }
    RectSolidBackground {        // Node background and shadow with backOpacity and backRadius support
        id: groupBackground
        anchors.fill: content   // Note 20160328: Do not set as content child to avoid interferring with content.childrenRect
        nodeItem: template.groupItem
        visible: !groupItem.collapsed
    }
    RowLayout {
        id: headerLayout
        x: 0;   y: -collapser.height; z:2
        width: content.width; height: collapser.height
        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
        spacing: 0
        ToolButton {
            id: collapser
            padding: 0
            Layout.preferredWidth: 32; Layout.preferredHeight: 32
            text: groupItem ? ( groupItem.collapsed ? "+" : "-" ) : "-"
            font.pointSize: 13; font.bold: true
            onClicked: groupItem.collapsed = !groupItem.collapsed
        }
        Item {
            id: labelEditorControl
            clip: false
            Layout.fillWidth: true; Layout.fillHeight: true
            property int fontPointSize : groupItem.style.fontPointSize  // Do not set pointSize for -1 value
            onFontPointSizeChanged: {
                if ( fontPointSize != -1 )
                    labelEditor.pointSize = fontPointSize
                groupLabel.font.pointSize = fontPointSize
            }
            LabelEditor {
                clip: false
                id: labelEditor
                anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right
                target: groupItem && groupItem.group ? groupItem.group : undefined
                visible: false
                //pointSize: groupItem.style.fontPointSize != -1
                bold: groupItem.style.fontBold
            }
            Label {
                id: groupLabel
                anchors.fill: parent
                text: groupItem && groupItem.group ? groupItem.group.label : "              "
                visible: !labelEditor.visible
                verticalAlignment: Text.AlignVCenter
                font.bold: groupItem.style.fontBold
                elide:  Text.ElideRight
                MouseArea {
                    anchors.fill: parent
                    preventStealing: true; propagateComposedEvents: true // Ensure event are forwarded to collapserArea
                    drag.target: groupItem.draggable ? groupItem : null
                    onDoubleClicked: labelEditor.visible = true
                }
            }
        }
    } // RowLayout: collapser + label

    // Emitted by qan::GroupItem when node dragging start
    function onNodeDragEnter() { groupBackground.backColor = Qt.binding( function() { return Qt.darker( template.groupItem.style.backColor, 1.05 ) } ) }
    // Emitted by qan::GroupItem when node dragging ends
    function onNodeDragLeave() { groupBackground.backColor = Qt.binding( function() { return template.groupItem.style.backColor } ) }
}
