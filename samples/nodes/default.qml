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

Qan.GraphView {
    id: graphView
    anchors.fill: parent
    navigable   : true
    resizeHandlerColor: "#03a9f4"       // SAMPLE: Set resize handler color to blue for 'resizable' nodes
    graph: Qan.Graph {
        id: graph
        Component.onCompleted: {
            var n1 = graph.insertNode()
            n1.label = "Node 1"; n1.item.x=15; n1.item.y= 25
            var n2 = graph.insertNode()
            n2.label = "Node 2"; n2.item.x=15; n2.item.y= 125
        }
        onNodeClicked: {
            notifyUser( "Node <b>" + node.label + "</b> clicked" )
            nodeEditor.node = node
        }
        onNodeRightClicked: { notifyUser( "Node <b>" + node.label + "</b> right clicked" ) }
        onNodeDoubleClicked: { notifyUser( "Node <b>" + node.label + "</b> double clicked" ) }
    }
    ToolTip { id: toolTip; timeout: 2500 }
    function notifyUser(message) { toolTip.text=message; toolTip.open() }
    Label {
        anchors.left: parent.left; anchors.leftMargin: 15
        anchors.bottom: parent.bottom; anchors.bottomMargin: 15
        text: "Use CTRL+Click to select multiples nodes"
    }
    Frame {
        id: nodeEditor
        property var node: undefined
        onNodeChanged: nodeItem = node ? node.item : undefined
        property var nodeItem: undefined
        anchors.bottom: parent.bottom; anchors.bottomMargin: 15
        anchors.right: parent.right; anchors.rightMargin: 15
        ColumnLayout {
            Label {
                text: nodeEditor.node ? "Editing node <b>" + nodeEditor.node.label + "</b>": "Select a node..."
            }
            CheckBox {
                text: "Draggable"
                enabled: nodeEditor.nodeItem !== undefined
                checked: nodeEditor.nodeItem ? nodeEditor.nodeItem.draggable : false
                onClicked: nodeEditor.nodeItem.draggable = checked
            }
            CheckBox {
                text: "Resizable"
                enabled: nodeEditor.nodeItem !== undefined
                checked: nodeEditor.nodeItem ? nodeEditor.nodeItem.resizable : false
                onClicked: nodeEditor.nodeItem.resizable = checked
            }
            CheckBox {
                text: "Selected (read-only)"
                enabled: false
                checked: nodeEditor.nodeItem ? nodeEditor.nodeItem.selected : false
            }
            CheckBox {
                text: "Selectable"
                enabled: nodeEditor.nodeItem != null
                checked: nodeEditor.nodeItem ? nodeEditor.nodeItem.selectable : false
                onClicked: nodeEditor.nodeItem.selectable = checked
            }
        }
    }
}  // Qan.GraphView

