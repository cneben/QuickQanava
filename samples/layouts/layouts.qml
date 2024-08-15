/*
 Copyright (c) 2008-2024, Benoit AUTHEMAN All rights reserved.

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

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan

ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720
    title: "Layouts Sample"

    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        resizeHandlerColor: "#03a9f4"
        gridThickColor: Material.theme === Material.Dark ? "#4e4e4e" : "#c1c1c1"
        property var treeRoot: undefined
        graph: Qan.Graph {
            parent: graphView
            id: graph
            Component.onCompleted: {
                let n1 = graph.insertNode()
                id: graphView
                graphView.treeRoot = n1
                n1.label = "n1"; n1.item.x = 15; n1.item.y = 25
                n1.item.ratio = 0.4

                let n11 = graph.insertNode()
                n11.label = "n11"; n11.item.x = 115; n11.item.y = 100
                let n111 = graph.insertNode()
                n111.label = "n111"; n111.item.x = 215; n111.item.y = 170
                let n1111 = graph.insertNode()
                n1111.label = "n1111"; n1111.item.x = 315; n1111.item.y = 240

                let n12 = graph.insertNode()
                n12.label = "n12"; n12.item.x = 115; n12.item.y= 310
                let n121 = graph.insertNode()
                n121.label = "n121"; n121.item.x = 215; n121.item.y = 380
                let n122 = graph.insertNode()
                n122.label = "n122"; n122.item.x = 215; n122.item.y = 450

                let n13 = graph.insertNode()
                n13.label = "n13"; n13.item.x = 115; n13.item.y = 520
                let n131 = graph.insertNode()
                n131.label = "n131"; n131.item.x = 225; n131.item.y = 590

                graph.insertEdge(n1, n11)
                graph.insertEdge(n1, n12)
                graph.insertEdge(n1, n13)

                graph.insertEdge(n11, n111)
                graph.insertEdge(n111, n1111)

                graph.insertEdge(n12, n121)
                graph.insertEdge(n12, n122)

                graph.insertEdge(n13, n131)
            }
            Qan.OrgTreeLayout {
                id: orgTreeLayout
            }
            Qan.RandomLayout {
                id: randomLayout
                layoutRect: Qt.rect(100, 100, 1000, 1000)
            }
        } // Qan.Graph
        Menu {      // Context menu demonstration
            id: contextMenu
            MenuItem {
                text: "Insert Node"
                onClicked: {
                    let n = graph.insertNode()
                    n.label = 'New Node'
                    n.item.x = contextMenu.x
                    n.item.y = contextMenu.y
                }
            }
        } // Menu
        onRightClicked: function(pos) {
            contextMenu.x = pos.x
            contextMenu.y = pos.y
            contextMenu.open()
        }
        Pane {
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            width: 470
            height: 50
            padding: 2
            RowLayout {
                anchors.fill: parent
                Label {
                    text: "Apply OrgTree:"
                }
                Button {
                    text: 'Random'
                    Material.roundedScale: Material.SmallScale
                    onClicked: randomLayout.layout(graphView.treeRoot)
                }
                Button {
                    text: 'Mixed'
                    Material.roundedScale: Material.SmallScale
                    onClicked: {
                        orgTreeLayout.layoutOrientation = Qan.OrgTreeLayout.Mixed
                        orgTreeLayout.layout(graphView.treeRoot);
                    }
                }
                Button {
                    text: 'Vertical'
                    Material.roundedScale: Material.SmallScale
                    onClicked: {
                        orgTreeLayout.layoutOrientation = Qan.OrgTreeLayout.Vertical
                        orgTreeLayout.layout(graphView.treeRoot);
                    }
                }
                Button {
                    text: 'Horizontal'
                    Material.roundedScale: Material.SmallScale
                    onClicked: {
                        orgTreeLayout.layoutOrientation = Qan.OrgTreeLayout.Horizontal
                        orgTreeLayout.layout(graphView.treeRoot);
                    }
                }
            }
        }
    }  // Qan.GraphView
}

