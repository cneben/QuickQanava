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
// \file	PointGrid.qml
// \author	benoit@destrat.io
// \date	2017 11 16
//-----------------------------------------------------------------------------

import QtQuick          2.7
import QtQuick.Shapes   1.0

import QuickQanava      2.0 as Qan

Qan.AbstractLineGrid {
    opacity: 0.9
    gridShape : lineGridShape
    Shape {
        id: lineGridShape
        anchors.fill: parent
        smooth: false
    }
    gridScale: 25
    geometryComponent: Component {
        ShapePath {
            property alias endX: line.x
            property alias endY: line.y
            property bool visible: false
            startX: 0; startY: 0
            capStyle: ShapePath.FlatCap
            strokeWidth: 1
            strokeColor: visible ? lineGrid.thickColor : Qt.rgba(0,0,0,0)
            strokeStyle: ShapePath.SolidLine
            fillColor: Qt.rgba(0,0,0,0)
            PathLine { id: line; x: 0; y: 0 }
        }
    }
    onAddLine: lineGridShape.data.push(line);
}
