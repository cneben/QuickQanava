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

import QuickQanava          2.0 as Qan
import QuickQanava.Samples  1.0
import "qrc:/QuickQanava"   as Qan

ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720
    title: "Dataflow sample"
    Pane { anchors.fill: parent }

    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        graph: FlowGraph {
            id: graph
            connectorEnabled: true
            Component.onCompleted: {
                var i1 = graph.insertFlowNode(FlowNode.Type.Percentage)
                i1.label = "Input"; i1.item.x = 50; i1.item.y = 50
                var i2 = graph.insertFlowNode(FlowNode.Type.Percentage)
                i2.label = "Input"; i2.item.x = 50; i2.item.y = 150
            }
        }
    }  // Qan.GraphView

    Pane { x: menu.x; y: menu.y; width: menu.width; height: menu.height; opacity: 0.8 } // Pane: menu transparent background
    RowLayout  {
        id: menu
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top; anchors.topMargin: 4
        ToolButton {
            text: "%"
            onClicked: {
                var r = graph.insertFlowNode(FlowNode.Type.Percentage)
                r.label = "Percentage"
            }
        }
        ToolButton {
            text: "+/*"
            onClicked: {
                var r = graph.insertFlowNode(FlowNode.Type.Operation)
                r.label = "Operation"
            }
        }
        ToolButton {
            text: "Img"
            onClicked: graph.insertFlowNode(ImageNode.Image)
        }
        ToolButton {
            text: "Tint"
            onClicked: graph.insertFlowNode(TintNode.Tint)
        }
    }
}

