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

    // See qan::Group::container property documentation
    container: content

    Text {
        id: nameText
        x: 0
        y: -contentHeight - 3
        text: group.label
        //font.pointSize: group.nameTextSize
        //font.bold: group.nameTextBold
        //color: group.nameTextColor
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
        x: 2
        y: 2
        z: 2
        width: Math.max( 150, childrenRect.width + 10 )
        height: Math.max( 80, childrenRect.height + 10 )
    }

    // Emitted by qan::Group when node dragging start
    onNodeDragEnter: {
        groupBackground.color = Qt.darker( group.backColor, 1.05 )
    }
    // Emitted by qan::Group when node dragging ends
    onNodeDragLeave: {
        groupBackground.color = group.backColor
    }

    /*Rectangle {
        width: 100
        height: 100
        border.color: "violet"
        border.width: 1
    }*/
}
