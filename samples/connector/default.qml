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
import Qt.labs.platform          1.0    // ColorDialog

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "." as Qan

Qan.GraphView {
    id: graphView
    anchors.fill: parent
    navigable   : true
    graph: Qan.Graph {
        connectorEnabled: true              // SAMPLE: This is where visual connection of node is enabled...
        id: graph
        Component.onCompleted: {
            var d1 = graph.insertNode()
            d1.label = "D1"; d1.item.x = 250; d1.item.y = 50
            var d2 = graph.insertNode()
            d2.label = "D2"; d2.item.x = 250; d2.item.y = 150

            var s1 = graph.insertNode()
            s1.label = "S1"; s1.item.x = 15; s1.item.y = 85

            graph.insertEdge(s1, d1)
            graph.insertEdge(s1, d2)

            var d3 = graph.insertNode()
            d3.label = "D3"; d3.item.x = 250; d3.item.y = 250
            graph.setConnectorSource(s1)    // SAMPLE: ... and eventually configured manually on a specific node until user select another one
        }
        function getEdgeDescription(edge) {
            var edgeSrcDst = "unknown"
            if ( edge && edge.item ) {
                var edgeItem = edge.item
                if ( edgeItem.sourceItem &&
                     edgeItem.sourceItem.node )
                    edgeSrcDst = edgeItem.sourceItem.node.label
                edgeSrcDst += " -> "
                if ( edgeItem.destinationItem &&
                     edgeItem.destinationItem.node )
                    edgeSrcDst += edgeItem.destinationItem.node.label
            }
            return edgeSrcDst
        }

        onConnectorEdgeInserted: { notifyUser("Edge inserted: " + getEdgeDescription(edge)) }
        onEdgeClicked: { notifyUser("Edge " + edge.label + " " + getEdgeDescription(edge) + " clicked") }
        onEdgeDoubleClicked: { notifyUser("Edge " + edge.label + " " + getEdgeDescription(edge) + " double clicked") }
        onEdgeRightClicked: { notifyUser("Edge " + edge.label + " " + getEdgeDescription(edge) + " right clicked") }
    }
    ToolTip { id: toolTip; timeout: 2500 }
    function notifyUser(message) { toolTip.text=message; toolTip.open() }

    ColorDialog {
        id: connectorEdgeColorDialog
        color: graph.connectorEdgeColor
        onAccepted: graph.connectorEdgeColor = color
    }
    ColorDialog {
        id: connectorColorDialog
        color: graph.connectorColor
        onAccepted: graph.connectorColor = color
    }
    Frame {
        anchors.top: parent.top; anchors.right: parent.right; anchors.rightMargin: 10
        ColumnLayout {
            CheckBox {
                text: qsTr("Enabled Visual Connector")
                checked: graph.connectorEnabled
                onClicked: graph.connectorEnabled = checked
            }
            CheckBox {
                text: qsTr("hEdge Enabled")
                checked: graph.connectorHEdgeEnabled
                onClicked: graph.connectorHEdgeEnabled = checked
            }
            RowLayout {
                Rectangle { width:32; height: 32; color: graph.connectorEdgeColor; radius: 5; border.width:1; border.color: Qt.darker(color) }
                Label { text: "Connector Edge Color:" }
                Item { Layout.fillWidth: true }
                ToolButton {
                    text: "..."
                    onClicked: { connectorEdgeColorDialog.open() }
                }
            }
            RowLayout {
                Rectangle { width:32; height: 32; color: graph.connectorColor; radius: 5; border.width:1; border.color: Qt.darker(color) }
                Label { text: "Connector Color:" }
                Item { Layout.fillWidth: true }
                ToolButton {
                    text: "..."
                    onClicked: { connectorColorDialog.open() }
                }
            }
        }
    }
}  // Qan.GraphView

