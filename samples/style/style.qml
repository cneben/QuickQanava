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

        graph       : Qan.CustomGraph {
            id: graph
            selectionColor: Material.accent
            connectorEnabled: true
            connectorColor: Material.accent
            connectorEdgeColor: Material.accent
            Component.onCompleted: {
                var n1 = graph.insertRoundNode()
                n1.label = "n1"; n1.item.x = 10; n1.item.y = 95
                connector.setSource(n1)
                var n2 = graph.insertRoundNode()
                n2.label = "n2"; n2.item.x = 10; n2.item.y = 300


                var n11 = graph.insertRoundNode()
                n11.label = "n11"; n11.item.x = 150; n11.item.y = 45
                graph.insertCustomEdge(n1, n11)
                var n12 = graph.insertRoundNode()
                n12.label = "n12"; n12.item.x = 150; n12.item.y = 145
                graph.insertCustomEdge(n1, n12)

                var n111 = graph.insertRectNode()
                n111.label = "n111"; n111.item.x = 300; n111.item.y = 45
                graph.insertCustomEdge(n11, n111)
                var n121 = graph.insertRectNode()
                n121.label = "n121"; n121.item.x = 300; n121.item.y = 145
                graph.insertCustomEdge(n12, n121)

                var n21 = graph.insertRoundNode()
                n21.label = "n21"; n21.item.x = 150; n21.item.y = 250
                graph.insertCustomEdge(n2, n21)
                var n22 = graph.insertRoundNode()
                n22.label = "n22"; n22.item.x = 150; n22.item.y = 350
                graph.insertCustomEdge(n2, n22)

                var n211 = graph.insertRectNode()
                n211.label = "n211"; n211.item.x = 300; n211.item.y = 250
                graph.insertCustomEdge(n21, n211)
            }
            onNodeRightClicked: { }
        } // Qan.Graph: graph
        onRightClicked: { }
    }
    Frame {
        id: styleBrowser
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: parent.right; anchors.rightMargin: 15
        width: 200; height: 350
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
