/*
 Copyright (c) 2008-2024, Benoit AUTHEMAN All rights reserved.
 Copyright (c) 2025, Siemens Energy Global GmbH & Co. KG

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
// \file    EdgeOrthoDoublePath.qml
// \author	morgan.bengtsson.ext@siemens-energy.com
// \date    2025 07 21
//-----------------------------------------------------------------------------

import QtQuick
import QtQuick.Shapes

ShapePath {
    id: edgeShapePath

    // Set in EdgeTemplate.qml createObject() from global qanEdgeOrthoPathComponent
    property var edgeTemplate: undefined
    property var edgeItem: edgeTemplate.edgeItem

    startX: edgeItem.p1.x
    startY: edgeItem.p1.y
    capStyle: ShapePath.FlatCap
    strokeWidth: edgeItem?.style?.lineWidth ?? 2
    strokeColor: edgeTemplate.color
    strokeStyle: edgeTemplate.dashed
    dashPattern: edgeItem?.style?.dashPattern ?? [4, 2]
    fillColor: Qt.rgba(0,0,0,0)

    property real edgeLengthY: Math.abs(edgeItem.p2.y - edgeItem.p1.y)
    property real edgeLengthX: Math.abs(edgeItem.p2.x - edgeItem.p1.x)
    property int horizontalDir: Math.sign(edgeItem.p1.x - edgeItem.p2.x);
    property int verticalDir: Math.sign(edgeItem.p1.y - edgeItem.p2.y);
    property bool isPoint: edgeItem.c1.x === edgeItem.c2.x && edgeItem.c1.y === edgeItem.c2.y
    property bool horizontalEdge: (edgeItem.c1.x === edgeItem.c2.x) && !isPoint
    property bool verticalEdge: (edgeItem.c1.y === edgeItem.c2.y) && !isPoint
    property real radius: Math.min(edgeItem?.style?.orthoRadius ?? 10, (horizontalEdge ? edgeLengthY : edgeLengthX) / 2)

    PathLine {
        x: edgeShapePath.edgeItem.c1.x + edgeShapePath.horizontalEdge * edgeShapePath.horizontalDir * edgeShapePath.radius
        y: edgeShapePath.edgeItem.c1.y + edgeShapePath.verticalEdge * edgeShapePath.verticalDir * edgeShapePath.radius
    }
    PathArc {
        x: edgeShapePath.edgeItem.c1.x - edgeShapePath.verticalEdge * edgeShapePath.horizontalDir * edgeShapePath.radius
        y: edgeShapePath.edgeItem.c1.y - edgeShapePath.horizontalEdge * edgeShapePath.verticalDir * edgeShapePath.radius
        radiusX: edgeShapePath.radius
        radiusY: edgeShapePath.radius
        useLargeArc: false
        direction: (edgeShapePath.verticalEdge
                    ? edgeShapePath.verticalDir !== edgeShapePath.horizontalDir
                    : edgeShapePath.verticalDir === edgeShapePath.horizontalDir)
                    ? PathArc.Clockwise
                    : PathArc.Counterclockwise;
    }
    PathLine {
        x: edgeShapePath.edgeItem.c2.x + edgeShapePath.verticalEdge * edgeShapePath.horizontalDir * edgeShapePath.radius
        y: edgeShapePath.edgeItem.c2.y + edgeShapePath.horizontalEdge * edgeShapePath.verticalDir * edgeShapePath.radius
    }
    PathArc {
        x: edgeShapePath.edgeItem.c2.x - edgeShapePath.horizontalEdge * edgeShapePath.horizontalDir * edgeShapePath.radius
        y: edgeShapePath.edgeItem.c2.y - edgeShapePath.verticalEdge * edgeShapePath.verticalDir * edgeShapePath.radius
        radiusX: edgeShapePath.radius
        radiusY: edgeShapePath.radius
        useLargeArc: false
        direction: (edgeShapePath.horizontalEdge
                    ? edgeShapePath.verticalDir !== edgeShapePath.horizontalDir
                    : edgeShapePath.verticalDir === edgeShapePath.horizontalDir)
                    ? PathArc.Clockwise
                    : PathArc.Counterclockwise;
    }
    PathLine {
        x: edgeShapePath.edgeItem.p2.x
        y: edgeShapePath.edgeItem.p2.y
    }
}
