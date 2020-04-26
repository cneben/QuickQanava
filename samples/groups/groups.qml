/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
import QtQuick.Shapes            1.0

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "." as Qan

ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720
    title: "Groups sample"
    Pane { anchors.fill: parent }
    ToolTip { x: 0; id: toolTip; timeout: 2000 }
    function notifyUser(message) { toolTip.text=message; toolTip.open() }

    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        function centerItem( item ) {
            if ( !item )
                return
            var windowCenter = Qt.point( ( window.contentItem.width - item.width ) / 2.,
                                        ( window.contentItem.height - item.height ) / 2. )
            var graphNodeCenter = window.contentItem.mapToItem( containerItem, windowCenter.x, windowCenter.y )
            item.x = graphNodeCenter.x
            item.y = graphNodeCenter.y
        }
        graph: Qan.Graph {
            id: topology
            connectorEnabled: true
            objectName: "graph"
            anchors.fill: parent
            Component.onCompleted: {
                var n1 = topology.insertNode()
                n1.label = "N1"
                n1.item.x = 80; n1.item.y = 85
                var n2 = topology.insertNode()
                n2.label = "N2"
                n2.item.x = 80; n2.item.y = 155
                var n3 = topology.insertNode()
                n3.label = "N3"
                n3.item.x = 80; n3.item.y = 225
                //topology.insertEdge( n1, n2 )
                //topology.insertEdge( n2, n3 )

                var g1 = topology.insertGroup()
                g1.label = "GROUP";
                g1.item.x = 300; g1.item.y = 80

                //g1.item.width = 250; g1.item.height = 270
                //topology.insertEdge( n2, g1 )
            }
            onGroupClicked: {
                window.notifyUser( "Group <b>" + group.label + "</b> clicked" )
                groupEditor.group = group
            }
            onGroupDoubleClicked: { window.notifyUser( "Group <b>" + group.label + "</b> double clicked" ) }
            onGroupRightClicked: {
                window.notifyUser( "Group <b>" + group.label + "</b> right clicked" )
                contextMenu.group = group

                if (!window.contentItem ||
                    !group.item)
                    return;
                let globalPos = window.contentItem.mapFromItem(group.item, pos.x, pos.y);
                contextMenu.x = globalPos.x
                contextMenu.y = globalPos.y
                contextMenu.open()
            }
            onNodeClicked: {
                ungroupNodeButton.node = node
                contextMenu.node = node
            }
            onNodeRightClicked: {
                ungroupNodeButton.node = node
                contextMenu.node = node

                if (!window.contentItem ||
                    !node.item)
                    return;
                let globalPos = window.contentItem.mapFromItem(node.item, pos.x, pos.y);
                contextMenu.x = globalPos.x
                contextMenu.y = globalPos.y
                contextMenu.open()
            }
            onNodeMoved: {
                if (node && node.isGroup)
                    window.notifyUser("Group <b>" + node.label + "</b> moved")
            }
        } // Qan.Graph: graph

        onClicked: {
            ungroupNodeButton.node = undefined
            groupEditor.group = undefined
            contextMenu.node = undefined
        }
        onRightClicked: {
            contextMenu.x = pos.x
            contextMenu.y = pos.y
            contextMenu.open()
        }

        Menu {      // Context menu demonstration
            id: contextMenu
            property var node: undefined
            property var group: undefined
            MenuItem {
                text: "Insert Node"
                enabled: contextMenu.node === undefined
                onClicked: {
                    let n = topology.insertNode()
                    n.label = 'New Node'
                    n.item.x = contextMenu.x
                    n.item.y = contextMenu.y
                    if (contextMenu.group)
                        topology.groupNode(contextMenu.group, n)
                }
            }
            MenuItem {
                text: "Remove node"
                enabled: contextMenu.node !== undefined
                onClicked: {
                    topology.removeNode(contextMenu.node)
                    contextMenu.node = undefined
                }
            }
            onClosed: { // Clean internal state when context menu us closed
                contextMenu.node = undefined
                contextMenu.group = undefined
            }
        } // Menu

        RowLayout {
            anchors.top: parent.top; anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            ToolButton {
                text: "Add Group"
                onClicked: {
                    var g = topology.insertGroup()
                    if (g) {
                        g.label = "Group"
                        graphView.centerItem(g.item)
                    }
                }
            }
            ToolButton {
                text: "Add Node"
                onClicked: {
                    var n = topology.insertNode( )
                    if (n) {
                        n.label = "Node"
                        n.x = graphView
                        graphView.centerItem(n.item)
                    }
                }
            }
            ToolButton {
                id: ungroupNodeButton
                text: "Ungroup Node"
                property var node: undefined
                enabled: node !== undefined
                onClicked: {
                    console.info("node.group=" + node.group)
                    if ( node && node.group )
                        topology.ungroupNode(node)
                }
            }
            // Note: QQmlEngine::retranslate() is often use to force applications using QuickQanava to
            // reevaluate all qsTr() bindings: unfortunately all application bindings are actually reevaluated,
            // sometime leading to unexpected behaviours for custom groups.
            ToolButton {
                id: retranslate
                text: "Retranslate"
                onClicked: {
                    ;
                }
            }
        }
        Control {
            id: groupEditor
            property var group: undefined
            onGroupChanged: groupItem = group ? group.item : undefined

            property var groupItem: undefined
            anchors.bottom: parent.bottom; anchors.bottomMargin: 15
            anchors.right: parent.right; anchors.rightMargin: 15

            width: 220; height: 385; padding: 0
            Pane { anchors.fill: parent; opacity: 0.9; padding: 0; Pane { anchors.fill: parent } } // Background
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
                    text: "Resizable"
                    enabled: groupEditor.groupItem != null
                    checked: groupEditor.groupItem ? groupEditor.groupItem.resizable : false
                    onClicked: groupEditor.groupItem.resizable = checked
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
                CheckBox {
                    text: "Label editor"
                    enabled: groupEditor.groupItem !== undefined
                    checked: groupEditor.groupItem ? groupEditor.groupItem.labelEditorVisible : false
                    onClicked: groupEditor.groupItem.labelEditorVisible = checked
                }
                CheckBox {
                    text: "Expand button"
                    enabled: groupEditor.groupItem !== undefined
                    checked: groupEditor.groupItem ? groupEditor.groupItem.expandButtonVisible : false
                    onClicked: groupEditor.groupItem.expandButtonVisible = checked
                }
                ToolButton {
                    text: "Remove group"
                    enabled: groupEditor.groupItem !== undefined
                    onClicked: {
                        if (groupEditor.groupItem !== undefined) {
                            topology.removeGroup(groupEditor.groupItem.group)
                        }
                    }
                }
            } // groupEditor ColumnLayout
        } // Control groupEditor
    } // Qan.GraphView
}

