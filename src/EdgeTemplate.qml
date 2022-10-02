/*
 Copyright (c) 2008-2022, Benoit AUTHEMAN All rights reserved.

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
// This file is a part of the QuickQanava software library.
//
// \file	EdgeTemplate.qml
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

    Shape {
        id: edgeShape
        anchors.fill: parent
        visible: edgeItem.visible && !edgeItem.hidden
        antialiasing: true
        smooth: true
        property var curvedLine : undefined
        property var straightLine : undefined
        property var orthoLine : undefined
        property var lineType: edgeTemplate.lineType
        onLineTypeChanged: {
            switch (lineType) {
            case Qan.EdgeStyle.Undefined:   // falltrought
            case Qan.EdgeStyle.Straight:
                if (orthoLine)
                    orthoLine.destroy()
                if (curvedLine)
                    curvedLine.destroy()
                straightLine = qanEdgeStraightPathComponent.createObject(edgeShape, {edgeItem: edgeTemplate.edgeItem, edgeTemplate: edgeTemplate});
                edgeShape.data = straightLine
                break;

            case Qan.EdgeStyle.Ortho:
                if (straightLine)
                    straightLine.destroy()
                if (curvedLine)
                    curvedLine.destroy()
                orthoLine = qanEdgeOrthoPathComponent.createObject(edgeShape, {edgeItem: edgeTemplate.edgeItem, edgeTemplate: edgeTemplate})
                edgeShape.data = orthoLine
                break;

            case Qan.EdgeStyle.Curved:
                if (straightLine)
                    straightLine.destroy()
                if (orthoLine)
                    orthoLine.destroy()
                curvedLine = qanEdgeCurvedPathComponent.createObject(edgeShape, {edgeItem: edgeTemplate.edgeItem, edgeTemplate: edgeTemplate})
                edgeShape.data = curvedLine
                break;
            }
        }
    }  // Shape: edgeShape
}  // Item: edgeTemplate
