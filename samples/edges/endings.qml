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
import QtQuick.Dialogs           1.2
import QtQuick.Shapes            1.0

import QuickQanava          2.0 as Qan
import "qrc:/QuickQanava"   as Qan

Qan.GraphView {
    id: graphView
    anchors.fill: parent
    navigable   : true
    property var edgeItems: []
    graph: Qan.Graph {
        id: graph
        connectorEnabled: true
        Component.onCompleted: {
            edgeItems.push(generateGeom(20, 50, Qan.EdgeStyle.Arrow))
            edgeItems.push(generateGeom(20, 150, Qan.EdgeStyle.ArrowOpen))
            edgeItems.push(generateGeom(20, 250, Qan.EdgeStyle.Circle))
            edgeItems.push(generateGeom(20, 350, Qan.EdgeStyle.CircleOpen))
            edgeItems.push(generateGeom(20, 450, Qan.EdgeStyle.Rect))
            edgeItems.push(generateGeom(20, 550, Qan.EdgeStyle.RectOpen))
        } // Qan.Graph.Component.onCompleted()

        function generateGeom(x, y, endingType) {
            var items = []
            var s1 = graph.insertNode()
            s1.label = "SRC"; s1.item.x = x; s1.item.y = y
            var d1 = graph.insertNode()
            d1.label = "DST"; d1.item.x = x + 200; d1.item.y = y + 50
            var e = graph.insertEdge(s1, d1);
            e.item.style = null                     // NOTE: Remove binding with default_edge_style
            e.item.srcShape = Qan.EdgeStyle.None
            e.item.dstShape = endingType
            items.push(e.item)

            var s2 = graph.insertNode()
            s2.label = "SRC"; s2.item.x = x + 400; s2.item.y = y
            var d2 = graph.insertNode()
            d2.label = "DST"; d2.item.x = x + 600; d2.item.y = y + 50
            e = graph.insertEdge(s2, d2);
            e.item.style = null
            e.item.srcShape = endingType
            e.item.dstShape = Qan.EdgeStyle.None
            items.push(e.item)

            var s3 = graph.insertNode()
            s3.label = "SRC"; s3.item.x = x + 800; s3.item.y = y
            var d3 = graph.insertNode()
            d3.label = "DST"; d3.item.x = x + 1000; d3.item.y = y + 50
            e = graph.insertEdge(s3, d3);
            e.item.style = null
            e.item.srcShape = endingType
            e.item.dstShape = endingType
            items.push(e.item)
            return items
        }
    } // Qan.Graph

    SpinBox {
        anchors.top: parent.top
        anchors.right: parent.right
        value: 4
        onValueModified: {
            for (var e=0; e < edgeItems.length; e++)
                edgeItems[e].arrowSize = value
        }
    }
}  // Qan.GraphView
