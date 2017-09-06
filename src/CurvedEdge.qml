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

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library. Copyright 2014 Benoit AUTHEMAN.
//
// \file	CurvedEdge.qml
// \author	benoit@destrat.io
// \date	2018 09 04
//-----------------------------------------------------------------------------

import QtQuick          2.7
import QtQuick.Layouts  1.3
import QtQuick.Shapes   1.0

import QuickQanava      2.0 as Qan

Qan.CurveEdgeItem {
    id: edgeItem

    property color color: style ? style.lineColor : Qt.black

    Shape {
        id: edgeCap
        transformOrigin: Item.TopLeft
        rotation: edgeItem.angle
        x: edgeItem.p2.x
        y: edgeItem.p2.y
        ShapePath {
            id: cap
            strokeColor: "black"
            fillColor: edgeItem.color
            strokeWidth: 2

            // FIXME: move everything to qan::CurveEdgeItem
            property real   arrowLength: 9
            property real   capSize: 7
            property point  base : Qt.point( -arrowLength, 0. )

            property point  a1: Qt.point(base.x, base.y - capSize)
            property point  a2: Qt.point(base.x + arrowLength, base.y)
            property point  a3: Qt.point(base.x, base.y + capSize)

            startX: a1.x;   startY: a1.y
            PathLine { x: cap.a3.x; y: cap.a3.y }
            PathLine { x: cap.a2.x; y: cap.a2.y }
            PathLine { x: cap.a1.x; y: cap.a1.y }
        }
    }

    Shape {
        id: edgeShape
        anchors.fill: parent
        visible: edgeItem.visible && !edgeItem.hidden
        //asynchronous: true

        ShapePath {
            id: arrow
            startX: edgeItem.p1.x
            startY: edgeItem.p1.y
            capStyle: ShapePath.FlatCap //edgeItem.style ? edgeItem.style.arrowSize : 4
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            strokeColor: edgeItem.color
            strokeStyle: ShapePath.SolidLine
            fillColor: "transparent"

            property real defaultOffset: 200
            property real minimum: Math.min(defaultOffset, Math.abs(xDistance))
            property real verticalOffset: xDistance <= 0 ? -minimum : 0

            /*property real xDistance: edgeItem.p2.x - (edgeItem.p1.x + 20)
            property real ratio: xDistance <= 0 ? 1 : 0.5
            property real cp1_x: edgeItem.p1.x + minimum * ratio
            property real cp1_y: edgeItem.p1.y + verticalOffset
            property real cp2_x: edgeItem.p2.x - minimum * ratio
            property real cp2_y: edgeItem.p2.y + verticalOffset
            */

            property real xDistance: edgeItem.p2.x - edgeItem.p1.x
            property real ratio: Math.abs(xDistance) <= 0 ? 1 : 0.5
            property real cp1_x: edgeItem.p1.x + xDistance * ratio
            property real cp1_y: edgeItem.p1.y
            property real cp2_x: edgeItem.p2.x - xDistance * ratio
            property real cp2_y: edgeItem.p2.y

            PathCubic {
                x: edgeItem.p2.x
                y: edgeItem.p2.y
                control1X: arrow.cp1_x
                control1Y: arrow.cp1_y
                control2X: arrow.cp2_x
                control2Y: arrow.cp2_y
            }
        }
    }
}
