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

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library. Copyright 2014 Benoit AUTHEMAN.
//
// \file	CurvedEdge.qml
// \author	benoit@destrat.io
// \date	2017 11 17
//-----------------------------------------------------------------------------

import QtQuick          2.7
import QtQuick.Layouts  1.3
import QtQuick.Shapes   1.0

import QuickQanava      2.0 as Qan

Item {
    id: edgeTemplate
    antialiasing: true
    property var edgeItem: undefined

    property color color: edgeItem &&
                          edgeItem.style ? edgeItem.style.lineColor : Qt.rgba(0.,0.,0.,1.)
    // Allow direct bypass of lstyle
    property var    lineType: edgeItem.style ? edgeItem.style.lineType : Qan.EdgeStyle.Straight
    property var    dashed  : edgeItem.style && style.dashed ? ShapePath.DashLine : ShapePath.SolidLine

    Shape {
        transformOrigin: Item.TopLeft
        rotation: edgeItem.dstAngle
        x: edgeItem.p2.x
        y: edgeItem.p2.y
        visible: ((edgeItem.dstShape === Qan.EdgeStyle.Arrow) || (edgeItem.dstShape === Qan.EdgeStyle.ArrowOpen))
                 && edgeItem.visible && !edgeItem.hidden
        ShapePath {
            strokeColor: edgeTemplate.color
            fillColor: edgeItem.dstShape === Qan.EdgeStyle.ArrowOpen ? Qt.rgba(0.,0.,0.,0.) : edgeTemplate.color
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            startX: edgeItem.dstA1.x;   startY: edgeItem.dstA1.y
            PathLine { x: edgeItem.dstA3.x; y: edgeItem.dstA3.y }
            PathLine { x: edgeItem.dstA2.x; y: edgeItem.dstA2.y }
            PathLine { x: edgeItem.dstA1.x; y: edgeItem.dstA1.y }
        }
    }
    Shape {
        transformOrigin: Item.TopLeft
        rotation: edgeItem.dstAngle
        x: edgeItem.p2.x
        y: edgeItem.p2.y
        visible: ((edgeItem.dstShape === Qan.EdgeStyle.Circle) || (edgeItem.dstShape === Qan.EdgeStyle.CircleOpen))
                 && edgeItem.visible && !edgeItem.hidden
        ShapePath {
            strokeColor: edgeTemplate.color
            fillColor: edgeItem.dstShape === Qan.EdgeStyle.CircleOpen ? Qt.rgba(0.,0.,0.,0.) : edgeTemplate.color
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            startX: 0;   startY: 0
            PathArc {
                relativeX: edgeItem.dstA2.x; relativeY: edgeItem.dstA2.y
                radiusX: edgeItem.dstA1.x; radiusY: edgeItem.dstA1.y;
            }
            PathArc {
                relativeX: -edgeItem.dstA2.x; relativeY: edgeItem.dstA2.y
                radiusX: edgeItem.dstA1.x; radiusY: edgeItem.dstA1.y;
            }
        }
    }
    Shape {
        transformOrigin: Item.TopLeft
        rotation: edgeItem.dstAngle
        x: edgeItem.p2.x
        y: edgeItem.p2.y
        visible: ((edgeItem.dstShape === Qan.EdgeStyle.Rect) || (edgeItem.dstShape === Qan.EdgeStyle.RectOpen))
                 && edgeItem.visible && !edgeItem.hidden
        ShapePath {
            strokeColor: edgeTemplate.color
            fillColor: edgeItem.dstShape === Qan.EdgeStyle.RectOpen ? Qt.rgba(0.,0.,0.,0.) : edgeTemplate.color
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            startX: edgeItem.dstA1.x;   startY: edgeItem.dstA1.y
            PathLine { x: 0.;               y: 0.               }
            PathLine { x: edgeItem.dstA3.x; y: edgeItem.dstA3.y }
            PathLine { x: edgeItem.dstA2.x; y: edgeItem.dstA2.y }
            PathLine { x: edgeItem.dstA1.x; y: edgeItem.dstA1.y }
        }
    }
    Shape {
        transformOrigin: Item.TopLeft
        rotation: edgeItem.srcAngle
        x: edgeItem.p1.x
        y: edgeItem.p1.y
        visible: ((edgeItem.srcShape === Qan.EdgeStyle.Arrow) || (edgeItem.srcShape === Qan.EdgeStyle.ArrowOpen))
                 && edgeItem.visible && !edgeItem.hidden
        ShapePath {
            strokeColor: edgeTemplate.color
            fillColor: edgeItem.srcShape === Qan.EdgeStyle.ArrowOpen ? Qt.rgba(0.,0.,0.,0.) : edgeTemplate.color
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            startX: edgeItem.srcA1.x;   startY: edgeItem.srcA1.y
            PathLine { x: edgeItem.srcA3.x; y: edgeItem.srcA3.y }
            PathLine { x: edgeItem.srcA2.x; y: edgeItem.srcA2.y }
            PathLine { x: edgeItem.srcA1.x; y: edgeItem.srcA1.y }
        }
    }
    Shape {
        transformOrigin: Item.TopLeft
        rotation: edgeItem.srcAngle
        x: edgeItem.p1.x
        y: edgeItem.p1.y
        visible: ((edgeItem.srcShape === Qan.EdgeStyle.Circle) || (edgeItem.srcShape === Qan.EdgeStyle.CircleOpen))
                 && edgeItem.visible && !edgeItem.hidden
        ShapePath {
            strokeColor: edgeTemplate.color
            fillColor: edgeItem.srcShape === Qan.EdgeStyle.CircleOpen ? Qt.rgba(0.,0.,0.,0.) : edgeTemplate.color
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            startX: 0;   startY: 0
            PathArc {
                relativeX: edgeItem.srcA2.x; relativeY: edgeItem.srcA2.y
                radiusX: edgeItem.srcA1.x; radiusY: edgeItem.srcA1.y;
            }
            PathArc {
                relativeX: -edgeItem.srcA2.x; relativeY: edgeItem.srcA2.y
                radiusX: edgeItem.srcA1.x; radiusY: edgeItem.srcA1.y;
            }
        }
    }
    Shape {
        transformOrigin: Item.TopLeft
        rotation: edgeItem.srcAngle
        x: edgeItem.p1.x
        y: edgeItem.p1.y
        visible: ((edgeItem.srcShape === Qan.EdgeStyle.Rect) || (edgeItem.srcShape === Qan.EdgeStyle.RectOpen))
                 && edgeItem.visible && !edgeItem.hidden
        ShapePath {
            strokeColor: edgeTemplate.color
            fillColor: edgeItem.srcShape === Qan.EdgeStyle.RectOpen ? Qt.rgba(0.,0.,0.,0.) : edgeTemplate.color
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            startX: edgeItem.srcA1.x;   startY: edgeItem.srcA1.y
            PathLine { x: 0.;               y: 0.               }
            PathLine { x: edgeItem.srcA3.x; y: edgeItem.srcA3.y }
            PathLine { x: edgeItem.srcA2.x; y: edgeItem.srcA2.y }
            PathLine { x: edgeItem.srcA1.x; y: edgeItem.srcA1.y }
        }
    }
    Component {
        id: straightShapePath
        ShapePath {
            id: edgeShapePath
            startX: edgeItem.p1.x
            startY: edgeItem.p1.y
            capStyle: ShapePath.FlatCap
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            strokeColor: edgeTemplate.color
            strokeStyle: edgeTemplate.dashed
            dashPattern: style ? style.dashPattern : [4, 2]
            fillColor: Qt.rgba(0,0,0,0)
            PathLine {
                x: edgeItem.p2.x
                y: edgeItem.p2.y
            }
        }
    }
    Component {
        id: orthoShapePath
        ShapePath {
            id: edgeShapePath
            startX: edgeItem.p1.x
            startY: edgeItem.p1.y
            capStyle: ShapePath.FlatCap
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            strokeColor: edgeTemplate.color
            strokeStyle: edgeTemplate.dashed
            dashPattern: style ? style.dashPattern : [4, 2]
            fillColor: Qt.rgba(0,0,0,0)
            PathLine {
                x: edgeItem.c1.x
                y: edgeItem.c1.y
            }
            PathLine {
                x: edgeItem.p2.x
                y: edgeItem.p2.y
            }
        }
    }
    Component {
        id: curvedShapePath
        ShapePath {
            id: edgeShapePath
            startX: edgeItem.p1.x
            startY: edgeItem.p1.y
            capStyle: ShapePath.FlatCap
            strokeWidth: edgeItem.style ? edgeItem.style.lineWidth : 2
            strokeColor: edgeTemplate.color
            strokeStyle: edgeTemplate.dashed
            dashPattern: edgeItem.style ? style.dashPattern : [4, 2]
            fillColor: Qt.rgba(0,0,0,0)
            PathCubic {
                x: edgeItem.p2.x
                y: edgeItem.p2.y
                control1X: edgeItem.c1.x
                control1Y: edgeItem.c1.y
                control2X: edgeItem.c2.x
                control2Y: edgeItem.c2.y
            }
        }
    }

    Shape {
        id: edgeShape
        anchors.fill: parent
        visible: edgeItem.visible && !edgeItem.hidden
        //asynchronous: true    // FIXME: Benchmark that
        antialiasing: true
        smooth: true
        property var curvedLine : undefined
        property var straightLine : undefined
        property var orthoLine : undefined
        property var lineType: edgeTemplate.lineType
        onLineTypeChanged: {
            switch (lineType) {
            case Qan.EdgeStyle.Straight:
                if ( orthoLine )
                    orthoLine.destroy()
                if ( curvedLine )
                    curvedLine.destroy()
                straightLine = straightShapePath.createObject(edgeShape)
                edgeShape.data = straightLine
                break;

            case Qan.EdgeStyle.Ortho:
                if ( straightLine )
                    straightLine.destroy()
                if ( curvedLine )
                    curvedLine.destroy()
                orthoLine = orthoShapePath.createObject(edgeShape)
                edgeShape.data = orthoLine
                break;

            case Qan.EdgeStyle.Curved:
                if ( straightLine )
                    straightLine.destroy()
                if ( orthoLine )
                    orthoLine.destroy()
                curvedLine = curvedShapePath.createObject(edgeShape)
                edgeShape.data = curvedLine
                break;
            }
        }
    }
    // Debug control points display code.
    /*
    Rectangle {
        width: 8; height: 8
        x: edgeItem.c1.x - 4
        y: edgeItem.c1.y - 4
        radius: 4
        color: "red"
    }
    Rectangle {
        width: 8; height: 8
        x: edgeItem.c2.x - 4
        y: edgeItem.c2.y - 4
        radius: 4
        color: "green"
    }
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.width: 1; border.color: "violet"
    }
    Rectangle {
        x: p1.x - 2; y: p1.y - 2
        width: 4; height: 4; radius: 4; color: "red"
    }
    Rectangle {
        x: edgeItem.p2.x - 2; y: edgeItem.p2.y - 2
        width: 4; height: 4; radius: 2; color: "blue"
    }
    */
}
