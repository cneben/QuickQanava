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

import QuickQanava          2.0 as Qan
import "qrc:/QuickQanava"   as Qan

import "qrc:/./"              as Sample

ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720
    title: "Style sample"
    Pane { anchors.fill: parent }
    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        ToolTip { id: toolTip; timeout: 2500 }
        function notifyUser(message) { toolTip.text=message; toolTip.open() }

        property var rectNode: Component{ Sample.RectNode{} }
        property var roundNode: Component{ Sample.RoundNode{} }

        resizeHandlerColor: Material.accent
        connectorColor: Material.accent

        graph       : Qan.CustomGraph {
            id: graph
            connectorEnabled: true
            Component.onCompleted: {
                console.debug("graph.insertNode()")
                var n1 = graph.insertNode(graphView.rectNode)
                console.debug("n1=" + n1)
                n1.label = "N1"
                var n2 = graph.insertNode()
                n2.label = "N2"
                var n3 = graph.insertNode()
                n3.label = "N3"
                graph.insertEdge( n1, n2 )
                graph.insertEdge( n2, n3 )

                var n4 = graph.insertRectNode()
                var n5 = graph.insertRoundNode()
            }
            onNodeRightClicked: { }
        } // Qan.Graph: graph
        onRightClicked: { }
    }
    Frame {
        id: styleBrowser
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: parent.right; anchors.rightMargin: 15
        width: 280; height: 350
        Qan.StyleListView {
            anchors.fill: parent
            anchors.margins: 4
            styleManager: graph.styleManager
            graph: graph
            onStyleClicked: { graphView.notifyUser("Style clicked") }
            onStyleDoubleClicked: {
                graphView.notifyUser("Style double clicked")
                /*styleEditor.hilightStyle( style )*/
            }
        }
    } // Frame: styleBrowser
    CheckBox {
        anchors.bottom: parent.bottom;    anchors.right: parent.right
        text: qsTr("Dark")
        checked: ApplicationWindow.contentItem.Material.theme === Material.Dark
        onClicked: ApplicationWindow.contentItem.Material.theme = checked ? Material.Dark : Material.Light
    }
}
