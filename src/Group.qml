/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

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
// This file is a part of the Qanava software. Copyright 2015 Benoit AUTHEMAN.
//
// \file	Group.qml
// \author	benoit@qanava.org
// \date	2016 03 22
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QuickQanava 2.0 as Qan
import "." as Qan

Qan.AbstractGroup {
    id: group

    x: 0
    y: 0
    width: content.width + 4
    height: content.height + 4

    property int    nameTextSize: 14
    property bool   nameTextBold: true
    property color  nameTextColor: "black"

    property color  backColor: Qt.rgba( 0.95, 0.95, 0.95, 1.0 )

    default property alias children : content.children

    signal  groupRightClicked( var group, var p )

    container: content   // See qan::Group::container property documentation
    TextInput {
        id: nameText
        x: 0
        y: -contentHeight - 3
        text: group.label
        font.pointSize: group.nameTextSize
        font.bold: group.nameTextBold
        color: group.nameTextColor
        onAccepted: {
            if ( text.length == 0 )
                text = group.label  // Do not allow empty labels
            else group.label = text
            focus = false;  // Release focus once the lable has been edited
        }
    }

    Rectangle {
        id: groupBackground
        anchors.fill: parent
        z: 0
        color: backColor
    }

    MouseArea {
        id: dragArea
        enabled: group.draggable
        anchors.fill: parent
        drag.target: group
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        smooth: true
        onClicked: {
            if ( mouse.button === Qt.RightButton )
                groupRightClicked( group, Qt.point( mouse.x, mouse.y ) )
        }
    }

    Item {
        id: content
        x: 2; y: 2; z: 2

        width: Math.max( groupWidth, minimumWidth )
        height: Math.max( groupHeight, minimumHeight )

        property real   minimumWidth: childrenRect.width + 10
        property real   minimumHeight: childrenRect.height + 10

        property real groupWidth: 150
        property real groupHeight: 80

        BottomRightResizer {
            target: content
            minimumSize: Qt.size( content.minimumWidth, content.minimumHeight )
            targetWidth: 150
            targetHeight: 80
            targetWidthName: "groupWidth"
            targetHeightName: "groupHeight"
        }
    }

    // Emitted by qan::Group when node dragging start
    onNodeDragEnter: {
        groupBackground.color = Qt.darker( group.backColor, 1.05 )
    }
    // Emitted by qan::Group when node dragging ends
    onNodeDragLeave: {
        groupBackground.color = group.backColor
    }
}
