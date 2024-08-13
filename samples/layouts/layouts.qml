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
        graph: Qan.Graph {
            parent: graphView
            id: graph
            Component.onCompleted: {
                let n1 = graph.insertNode()
                n1.label = "n1"; n1.item.x=15; n1.item.y= 25
                n1.item.ratio = 0.4

                let n11 = graph.insertNode()
                n11.label = "n11"; n11.item.x=15; n11.item.y= 125
                let n12 = graph.insertNode()
                n12.label = "n12"; n12.item.x=125; n12.item.y= 125


                let n121 = graph.insertNode()
                n121.label = "n121"; n121.item.x=125; n121.item.y= 225

                graph.insertEdge(n1, n12);
                graph.insertEdge(n1, n11);
                graph.insertEdge(n12, n121);

                orgTreeLayout.layout(n1);
            }
            Qan.OrgTreeLayout {
                id: orgTreeLayout
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
    }  // Qan.GraphView
}

