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
// \file	CustomNode.qml
// \author	benoit@destrat.io
// \date	2015 08 01
//-----------------------------------------------------------------------------

import QtQuick              2.8
import QtQuick.Controls     2.1
import QtQuick.Layouts      1.3
import QtGraphicalEffects   1.0

import QuickQanava          2.0 as Qan

Qan.NodeItem {
    id: customNode
    width: 110; height: 60
    x: 15;      y: 15
    Item {
        anchors.fill: parent
        Rectangle {
            id: background
            anchors.fill: parent
            radius: 10; color: light
            border.color: "violet"; border.width: 2
            readonly property color dark: "blue"
            readonly property color light: "lightblue"
            Label {
                anchors.fill: parent
                anchors.margins: 5
                text: "Custom Node"
            }
            CheckBox {
                anchors.bottom: parent.bottom; anchors.left: parent.left
                text: "Light"
                checked: background.color === background.light
                onClicked: background.color = ( background.color === background.light ? background.dark : background.light )
            }
        }
    }
}
