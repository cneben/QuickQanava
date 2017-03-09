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

import QtQuick                   2.8
import QtQuick.Controls          2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts           1.3

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "." as Qan

ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720
    title: "Edge/Visual connector sample"
    Pane { anchors.fill: parent }
    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        graph: Qan.Graph {
            enableConnector: true
            id: graph
            Component.onCompleted: {
                var s1 = graph.insertNode()
                s1.label = "S1"; s1.item.x = 15; s1.item.y = 85
                var d1 = graph.insertNode()
                d1.label = "D1"; d1.item.x = 250; d1.item.y = 50
                var d2 = graph.insertNode()
                d2.label = "D2"; d2.item.x = 250; d2.item.y = 150

                graph.insertEdge(s1, d1)
                graph.insertEdge(s1, d2)
            }
        }
    }  // Qan.GraphView
    RowLayout {
        anchors.top: parent.top;    anchors.left: parent.left
        CheckBox {
            text: qsTr("Dark")
            checked: ApplicationWindow.contentItem.Material.theme === Material.Dark
            onClicked: ApplicationWindow.contentItem.Material.theme = checked ? Material.Dark : Material.Light
        }
    }
}

