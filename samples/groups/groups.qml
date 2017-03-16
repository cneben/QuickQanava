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
    title: "Custom nodes sample"
    Pane { anchors.fill: parent }
    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        ToolTip { id: toolTip; timeout: 2000 }
        function notifyUser(message) { toolTip.text=message; toolTip.open() }
        graph: Qan.Graph {
            id: topology
            connectorEnabled: true
            objectName: "graph"
            anchors.fill: parent
            Component.onCompleted: {
                var n1 = topology.insertNode()
                n1.label = "N1"
                var n2 = topology.insertNode()
                n2.label = "N2"
                var n3 = topology.insertNode()
                n3.label = "N3"
                //topology.insertEdge( n1, n2 )
                //topology.insertEdge( n2, n3 )

                var g1 = topology.insertGroup()
                g1.label = "GROUP"; g1.item.x = 250; g1.item.y = 45
                //topology.insertEdge( n2, g1 )
            }
            onGroupClicked: {
                graphView.notifyUser( "Group <b>" + group.label + "</b> clicked" )
                groupEditor.group = group
            }
            onGroupDoubleClicked: { graphView.notifyUser( "Group <b>" + group.label + "</b> double clicked" ) }
            onGroupRightClicked: { graphView.notifyUser( "Group <b>" + group.label + "</b> right clicked" ) }
        } // Qan.Graph: graph

        RowLayout {
            anchors.top: parent.top; anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            RoundButton {
                text: "Group"
                onClicked: {
                    var gg = topology.insertGroup()
                    if ( gg )
                        gg.label = "Group"
                }
            }
            RoundButton {
                text: "Node"
                onClicked: {
                    var n = topology.insertNode( )
                    if ( n )
                        n.label = "Node"
                }
            }
        }
        Frame {
            id: groupEditor
            property var group: undefined
            onGroupChanged: groupItem = group ? group.item : undefined
            property var groupItem: undefined
            anchors.bottom: parent.bottom; anchors.bottomMargin: 15
            anchors.right: parent.right; anchors.rightMargin: 15
            ColumnLayout {
                Label {
                    text: groupEditor.group ? "Editing group <b>" + groupEditor.group.label + "</b>": "Select a group..."
                }
                CheckBox {
                    text: "Draggable"
                    enabled: groupEditor.groupItem !== undefined
                    checked: groupEditor.groupItem ? groupEditor.groupItem.draggable : false
                    onClicked: groupEditor.groupItem.draggable = checked
                }
                CheckBox {
                    text: "Selected (read-only)"
                    enabled: false
                    checked: groupEditor.groupItem ? groupEditor.groupItem.selected : false
                }
                CheckBox {
                    text: "Selectable"
                    enabled: groupEditor.groupItem != null
                    checked: groupEditor.groupItem ? groupEditor.groupItem.selectable : false
                    onClicked: groupEditor.groupItem.selectable = checked
                }
            }
        }
    } // Qan.GraphView
}

