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
    title: "Edges sample"

    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        graph: Qan.Graph {
            id: graph
            connectorEnabled: true
            Component.onCompleted: {
                var c = graph.insertNode()
                c.label = "C"; c.item.x = 350; c.item.y = 200


                var tl = graph.insertNode()
                tl.label = "TL"; tl.item.x = 50; tl.item.y = 50
                graph.insertEdge(c, tl);

                var tml = graph.insertNode()
                tml.label = "TML"; tml.item.x = 200; tml.item.y = 50
                graph.insertEdge(c, tml);

                var t = graph.insertNode()
                t.label = "T"; t.item.x = 350; t.item.y = 50
                graph.insertEdge(c, t);

                var tmr = graph.insertNode()
                tmr.label = "TML"; tmr.item.x = 500; tmr.item.y = 50
                graph.insertEdge(c, tmr);

                var tr = graph.insertNode()
                tr.label = "TR"; tr.item.x = 650; tr.item.y = 50
                graph.insertEdge(c, tr);


                var bl = graph.insertNode()
                bl.label = "BL"; bl.item.x = 50; bl.item.y = 350
                graph.insertEdge(c, bl);

                var bml = graph.insertNode()
                bml.label = "BML"; bml.item.x = 200; bml.item.y = 350
                graph.insertEdge(c, bml);

                var b = graph.insertNode()
                b.label = "B"; b.item.x = 350; b.item.y = 350
                graph.insertEdge(c, b);

                var bmr = graph.insertNode()
                bmr.label = "BML"; bmr.item.x = 500; bmr.item.y = 350
                graph.insertEdge(c, bmr);

                var br = graph.insertNode()
                br.label = "BR"; br.item.x = 650; br.item.y = 350
                graph.insertEdge(c, br);


                var l = graph.insertNode()
                l.label = "L"; l.item.x = 50; l.item.y = 200
                graph.insertEdge(c, l);

                var r = graph.insertNode()
                r.label = "R"; r.item.x = 650; r.item.y = 200
                graph.insertEdge(c, r);

                graph.setConnectorSource(c)
            }
        }
    }  // Qan.GraphView
}

