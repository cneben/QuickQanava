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
    ToolTip { id: toolTip; timeout: 2500 }
    function notifyUser(message) { toolTip.text=message; toolTip.open() }

    graph: Qan.Graph {
        connectorEnabled: true
        id: graph
        Qan.VisualConnector {
            id: customConnector
            graph: graphView.graph                    // SAMPLE: When configuring  a custom visual connector, VisualConnector.graph must be set manually
            topMargin: graph.connector.height + 15
            connectorItem: Control {
                parent: customConnector
                anchors.fill: parent
                hoverEnabled: true
                ToolTip.visible: hovered &&
                                 ( !customConnector.connectorDragged || state === "HILIGHT" )
                onStateChanged: {
                    ToolTip.text = ( state === "HILIGHT" ? "Drop to connect" : "Drag on a target node" )
                }
                states: [
                    State { name: "NORMAL"; PropertyChanges { target: customConnectorItem; scale: 1.0 } },
                    State { name: "HILIGHT"; PropertyChanges { target: customConnectorItem; scale: 1.7 } }
                ]
                transitions: [
                    Transition { from: "NORMAL"; to: "HILIGHT"; PropertyAnimation { target: customConnectorItem; properties: "borderWidth, scale"; duration: 100 } },
                    Transition { from: "HILIGHT"; to: "NORMAL"; PropertyAnimation { target: customConnectorItem; properties: "borderWidth, scale"; duration: 150 } }
                ]
                Image {
                    anchors.fill: parent
                    id: customConnectorItem
                    source: "qrc:/fa_link.png"
                    state: "NORMAL"; smooth: true;   antialiasing: true
                }
            }
            createDefaultEdge: false    // SAMPLE: When createDefaultEdge is set to false, VisualConnector does not use Qan.Graph.insertEdge()
                                        // to create edge, but instead emit requestEdgeCreation (see below) to allow user to create custom
                                        // edge (either specifying a custom edge component, or calling a user defined method on graph).
            onRequestEdgeCreation: {
                notifyUser("Edge creation requested between " + src.label + " and " + dst.label )
            }
        }
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
            graph.setConnectorSource(s1)
            customConnector.sourceNode = s1
        }
        onNodeClicked: {
            if ( node && node.item ) {
                customConnector.sourceNode = node
            } else
                customConnector.visible = false
        }
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
        }
    }
}  // Qan.GraphView

