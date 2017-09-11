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
import "qrc:/QuickQanava"   as Qan

import "qrc:/./"              as Sample

ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720
    title: "Style sample"
    Pane { anchors.fill: parent }
    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        ToolTip { id: toolTip; timeout: 2500 }
        function notifyUser(message) { toolTip.text=message; toolTip.open() }

        property var rectNode: Component{ Sample.RectNode{} }
        property var roundNode: Component{ Sample.RoundNode{} }

        resizeHandlerColor: Material.accent

        graph       : Qan.CustomGraph {
            id: graph
            selectionColor: Material.accent
            connectorEnabled: true
            connectorColor: Material.accent
            connectorEdgeColor: Material.accent
            Component.onCompleted: {
                var n1 = graph.insertRoundNode()
                n1.label = "n1"; n1.item.x = 10; n1.item.y = 95
                //connector.setSource(n1)
                var n2 = graph.insertRoundNode()
                n2.label = "n2"; n2.item.x = 10; n2.item.y = 300


                var n11 = graph.insertRoundNode()
                n11.label = "n11"; n11.item.x = 150; n11.item.y = 45
                graph.insertCustomEdge(n1, n11)
                var n12 = graph.insertRoundNode()
                n12.label = "n12"; n12.item.x = 150; n12.item.y = 145
                graph.insertCustomEdge(n1, n12)

                var n111 = graph.insertRectNode()
                n111.label = "n111"; n111.item.x = 300; n111.item.y = 45
                graph.insertCustomEdge(n11, n111)
                var n121 = graph.insertRectNode()
                n121.label = "n121"; n121.item.x = 300; n121.item.y = 145
                graph.insertCustomEdge(n12, n121)

                var n21 = graph.insertRoundNode()
                n21.label = "n21"; n21.item.x = 150; n21.item.y = 250
                graph.insertCustomEdge(n2, n21)
                var n22 = graph.insertRoundNode()
                n22.label = "n22"; n22.item.x = 150; n22.item.y = 350
                graph.insertCustomEdge(n2, n22)

                var n211 = graph.insertRectNode()
                n211.label = "n211"; n211.item.x = 300; n211.item.y = 250
                graph.insertCustomEdge(n21, n211)
            }
            onNodeRightClicked: { }
        } // Qan.Graph: graph
        onRightClicked: { }
    }
    Frame {
        id: styleBrowser
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: parent.right; anchors.rightMargin: 15
        width: 200; height: 350
        Qan.StyleListView {
            anchors.fill: parent
            anchors.margins: 4
            styleManager: graph.styleManager
            graph: graph
            onStyleClicked: { graphView.notifyUser("Style clicked") }
            onStyleDoubleClicked: {
                graphView.notifyUser("Style double clicked")
                /*styleEditor.hilightStyle( style )*/
            }
        }
    } // Frame: styleBrowser
    CheckBox {
        anchors.bottom: parent.bottom;    anchors.right: parent.right
        text: qsTr("Dark")
        checked: ApplicationWindow.contentItem.Material.theme === Material.Dark
        onClicked: ApplicationWindow.contentItem.Material.theme = checked ? Material.Dark : Material.Light
    }
}
