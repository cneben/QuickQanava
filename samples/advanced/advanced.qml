/*
 Copyright (c) 2008-2020, Benoit AUTHEMAN All rights reserved.

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
    title: "Advanced sample"
    Pane { anchors.fill: parent }
    ToolTip { x: 0; id: toolTip; timeout: 2000 }
    function notifyUser(message) { toolTip.text = message; toolTip.open() }

    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable: true
        function centerItem(item) {
            if (!item)
                return
            var windowCenter = Qt.point((window.contentItem.width - item.width) / 2.,
                                        (window.contentItem.height - item.height) / 2.)
            var graphNodeCenter = window.contentItem.mapToItem(containerItem, windowCenter.x, windowCenter.y)
            item.x = graphNodeCenter.x
            item.y = graphNodeCenter.y
        }
        graph: Qan.Graph {
            id: topology
            connectorEnabled: true
            objectName: "graph"
            anchors.fill: parent
            property var advancedNodeDelegate: Qt.createComponent("qrc:/AdvancedNode.qml")
            property var advancedGroupDelegate: Qt.createComponent("qrc:/AdvancedGroup.qml")
            Component.onCompleted: {
                const g1 = topology.insertGroup(advancedGroupDelegate);
                g1.label = "GROUP";
                g1.item.x = 300; g1.item.y = 80;
                g1.item.width = 450; g1.item.height = 220;

                const n1 = topology.insertNode(advancedNodeDelegate);
                topology.groupNode(g1, n1);
                n1.item.x = 10; n1.item.y = 35;

                const n2 = topology.insertNode(advancedNodeDelegate);
                topology.groupNode(g1, n2);
                n2.item.x = 180; n2.item.y = 35;

                topology.insertEdge(n1, n2);

                const n11 = topology.insertNode(advancedNodeDelegate);
                topology.groupNode(g1, n11);
                n11.item.x = 10; n11.item.y = 145;

                const n22 = topology.insertNode(advancedNodeDelegate);
                topology.groupNode(g1, n22);
                n22.item.x = 180; n22.item.y = 145;

                topology.insertEdge(n11, n22);

                const n3 = topology.insertNode(advancedNodeDelegate);
                n3.item.x = 850; n3.item.y = 150;
                topology.insertEdge(n2, n3);
            }
        } // Qan.Graph: graph

        onClicked: {
            ungroupNodeButton.node = undefined
        }

        RowLayout {
            anchors.top: parent.top; anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            width: 550
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
                    var n = topology.insertNode()
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
                    if (node && node.group )
                        topology.ungroupNode(node)
                }
            }

            Switch {
                text: "Snap to Grid"
                checked: topology.snapToGrid
                onClicked: topology.snapToGrid = checked
            }
            Label { text: "Grid size:" }
            SpinBox {
                enabled: topology.snapToGrid
                from: 1
                to: 100
                stepSize: 5
                value: topology.snapToGridSize.width
                onValueModified: { topology.snapToGridSize = Qt.size(value, value) }
            }
        }
    } // Qan.GraphView
}  // ApplicationWindow: window

