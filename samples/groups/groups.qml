/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    ToolTip { id: toolTip; timeout: 2000 }
    function notifyUser(message) { toolTip.text=message; toolTip.open() }
    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
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
                window.notifyUser( "Group <b>" + group.label + "</b> clicked" )
                groupEditor.group = group
            }
            onGroupDoubleClicked: { window.notifyUser( "Group <b>" + group.label + "</b> double clicked" ) }
            onGroupRightClicked: { window.notifyUser( "Group <b>" + group.label + "</b> right clicked" ) }
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

