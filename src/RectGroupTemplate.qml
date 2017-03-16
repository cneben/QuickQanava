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
import "qrc:/QuickQanava"   as Qan

Item {
    id: template
    x: 0;   y: 0
    width: Math.max( content.width, 75 )
    height: Math.max( content.height, 40 )

    property color  backColor: Qt.rgba( 0.97, 0.97, 0.97, 1.0 )

    default property alias children : content.children
    property alias  content: content

    property alias  radius: groupBackground.radius
    property alias  border: groupBackground.border

    property var    groupItem: undefined
    Item {
        id: content
        x: 0; y: 0; z: 3
        visible: !groupItem.collapsed
    }
    Rectangle { // 20160328: Do not set as content child to avoid interferring with content.childrenRect
        id: groupBackground
        anchors.fill: content
        visible: !groupItem.collapsed
        z: 1;
        color: backColor
        radius: 6
        border.width: 2; border.color: Qt.darker( backColor, 1.2 )
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
            font.pointSize: 14; font.bold: true
            onClicked: groupItem.collapsed = !groupItem.collapsed
        }
        Item {
            clip: false
            Layout.fillWidth: true; Layout.fillHeight: true
            LabelEditor {
                clip: false
                id: labelEditor
                anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right
                target: groupItem && groupItem.group ? groupItem.group : undefined
                visible: false
            }
            Label {
                id: groupLabel
                anchors.fill: parent
                text: groupItem && groupItem.group ? groupItem.group.label : "              "
                visible: !labelEditor.visible
                verticalAlignment: Text.AlignVCenter
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

    // Emitted by qan::Group when node dragging start
    function onNodeDragEnter() { groupBackground.color = Qt.binding( function() { return Qt.darker( template.backColor, 1.05 ) } ) }
    // Emitted by qan::Group when node dragging ends
    function onNodeDragLeave() { groupBackground.color = Qt.binding( function() { return template.backColor } ) }
}
