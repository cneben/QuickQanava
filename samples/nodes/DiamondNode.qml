/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library. Copyright 2015 Benoit AUTHEMAN.
//
// \file	DiamondNode.qml
// \author	benoit@destrat.io
// \date	2015 08 25
//-----------------------------------------------------------------------------

import QtQuick              2.8
import QtQuick.Layouts      1.3

import QuickQanava          2.0 as Qan
import "qrc:/QuickQanava"       as Qan

Qan.NodeItem {
    id: diamondNode
    Layout.preferredWidth: 100
    Layout.preferredHeight: 100
    width: Layout.preferredWidth
    height: Layout.preferredHeight

    property var symbolPolygon: new Array(5)
    complexBoundingShape: true                  // SAMPLE: Do not forget to set complexBoundingShape to true for non
    onRequestUpdateBoundingShape: {             // rectangular node otherwise requestUpdateBoundingShape() will never be
        var w = width - 1                       // called (an a rectangular BS will be generated more efficietly from c++)
        var w2 = w / 2
        var w5 = w / 4
        var h = height - 1
        var h2 = h / 2
        symbolPolygon[0] = Qt.point( w2, 1 )
        symbolPolygon[1] = Qt.point( w2 + h2, h2 )
        symbolPolygon[2] = Qt.point( w2, h )
        symbolPolygon[3] = Qt.point( w2 - h2, h2 )
        symbolPolygon[4] = Qt.point( w2, 1 )
        setBoundingShape( symbolPolygon )
    }

    Qan.CanvasNodeTemplate {
        id: template
        anchors.fill: parent
        nodeItem : diamondNode
        symbol: Canvas {
            anchors.fill: parent
            z: 1
            id: nodeSymbol
            onPaint: {
                var ctx = nodeSymbol.getContext( "2d" )
                ctx.clearRect( 0, 0, width, height )
                ctx.lineWidth = diamondNode.borderWidth
                ctx.strokeStyle = diamondNode.borderColor
                ctx.beginPath( )
                var w = width - 1;  var w2 = w / 2
                var h = height - 1; var h2 = h / 2
                ctx.moveTo( w2, 1 )
                ctx.lineTo( w2 + h2, h2 )
                ctx.lineTo( w2, h )
                ctx.lineTo( w2 - h2, h2 )
                ctx.lineTo( w2, 1 )
                ctx.stroke( )
                var gradient = ctx.createLinearGradient(0, 0, width, height);
                gradient.addColorStop(0.1, Qt.lighter( diamondNode.style.backColor, 1.8 )  );
                gradient.addColorStop(0.9, diamondNode.style.backColor );
                ctx.fillStyle = gradient
                ctx.fill( )
            }
        }
    }
}
