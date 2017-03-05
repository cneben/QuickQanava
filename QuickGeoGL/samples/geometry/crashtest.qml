/*
    This file is part of QuickGeoGL library.

    Copyright (C) 2016 Benoit AUTHEMAN

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

import QtQuick          2.7
import QtQuick.Layouts  1.3
import QtQuick.Controls 2.0

import QuickGeoGL           1.0 as Qgl
import "qrc:/QuickGeoGL"    1.0 as Qgl

Item {
    Component{ id: polyLineComponent; Qgl.PolyLine{} }
    Component{ id: lineComponent; Qgl.Line{} }
    Component{ id: arrowComponent; Qgl.Arrow{} }
    Component{ id: polygonComponent; Qgl.ConvexPolygon{} }

    property var colors: [ "black", "red", "blue", "green", "lightblue", "violet" ]
    function randomColor() {
        return colors[Math.floor(Math.random()*6)]
    }

    Frame {
        id: crashBox
        anchors.fill: parent
    }

    property var polyLines: new Array()
    property var arrows: new Array()
    property var polygons: new Array()
    RowLayout {
        z: 10
        Button {
            text: "Random polylines"
            onClicked: {
                var lastP = polyLines.length
                for ( var pl = lastP; pl < lastP + 1000; pl++ ) {
                    var polyLine = polyLineComponent.createObject(crashBox, {"lineWidth":1.0} )
                    var pointsCount = Math.floor( Math.random() * 2 + 2 )   // Add a random number of points between 2 and 4
                    var points = new Array()
                    for ( var p = 0; p < pointsCount; p++ )
                        points[p] = Qt.point( Math.random() * crashBox.width, Math.random() * crashBox.height )
                    polyLines[pl] = polyLine
                    polyLine.setPoints( points )
                    polyLine.lineWidth = Math.floor( Math.random() * 3.0 + 1.0 )
                    polyLine.color = randomColor()
                }
            }
        }
        Button {
            text: "Modify polylines"
            onClicked: {
                for ( var pl = 0; pl < polyLines.length; pl++ ) {
                    var polyLine = polyLines[pl]
                    if ( polyLine ) {
                        var pointsCount = Math.floor( Math.random() * 3 + 2 )   // Add a random number of points between 2 and 4
                        var points = new Array()
                        for ( var p = 0; p < pointsCount; p++ )
                            points[p] = Qt.point( Math.random() * crashBox.width, Math.random() * crashBox.height )
                        polyLine.setPoints( points )
                        polyLine.lineWidth = Math.floor( Math.random() * 3.0 + 1.0 )
                        polyLine.color = randomColor()
                    }
                }
            }
        }
        Button {
            text: "Delete Polylines"
            onClicked: {
                for ( var a = 0; a < polyLines.length; a++ ) {
                    if ( polyLines[a] )
                        polyLines[a].destroy()
                    polyLines[a] = undefined
                }
                polyLines = new Array()
            }
        }
        Button {
            text: "Random lines"
            onClicked: {
                for ( var l = 0; l < 1000; l++ ) {
                    var line = lineComponent.createObject(crashBox)
                    line.p1 = Qt.point( Math.random() * crashBox.width, Math.random() * crashBox.height )
                    line.p2 = Qt.point( Math.random() * crashBox.width, Math.random() * crashBox.height )
                    line.lineWidth = Math.floor( Math.random() * 2.0 + 1.0 )
                    line.color = randomColor()
                }
            }
        }
        Button {
            text: "Random Arrows"
            onClicked: {
                var lastA = arrows.length
                for ( var a = lastA; a < lastA + 1000; a++ ) {
                    var arrow = arrowComponent.createObject(crashBox)
                    arrow.p1 = Qt.point( Math.random() * crashBox.width, Math.random() * crashBox.height )
                    arrow.p2 = Qt.point(arrow.p1.x + 16., arrow.p1.y + 16.)
                    arrow.lineWidth = Math.floor( Math.random() * 2.0 + 1.0 )
                    arrow.p2CapSize = Math.floor( Math.random() * 8.0 + 8.0 )
                    arrow.color = randomColor()
                    arrows[a] = arrow
                }
            }
        }
        Button {
            text: "Modify Arrows"
            onClicked: {
                for ( var a = 0; a < arrows.length; a++ ) {
                    var arrow = arrows[a]
                    if ( arrow ) {
                        arrow.p1 = Qt.point( Math.random() * crashBox.width, Math.random() * crashBox.height )
                        arrow.p2 = Qt.point( Math.random() * crashBox.width, Math.random() * crashBox.height )
                        arrow.lineWidth = Math.floor( Math.random() * 2.0 + 1.0 )
                        arrow.p2CapSize = Math.floor( Math.random() * 8.0 + 8.0 )
                        arrow.color = randomColor()
                    }
                }
            }
        }
        Button {
            text: "Delete Arrows"
            onClicked: {
                for ( var a = 0; a < arrows.length; a++ ) {
                    if ( arrows[a] )
                        arrows[a].destroy()
                    arrows[a] = undefined
                }
                arrows = new Array()
            }
        }

        Button {
            text: "Random Polygons"
            onClicked: {
                var lastP = polygons.length
                for ( var pl = lastP; pl < lastP + 1000; pl++ ) {
                    var polygon= polygonComponent.createObject(crashBox)
                    var pointsCount = 3
                    var points = new Array()
                    for ( var p = 0; p < pointsCount; p++ )
                        points[p] = Qt.point( Math.random() * crashBox.width, Math.random() * crashBox.height )
                    polygons[pl] = polygon
                    polygon.setPolygon( points )
                    polygon.color = randomColor()
                    polygon.border.width = Math.floor( Math.random() * 10.0 + 1.0 )
                    polygon.border.color = randomColor()
                }
            }
        }
        Button {
            text: "Modify Polygon"
            onClicked: {
                for ( var pl = 0; pl < polygons.length; pl++ ) {
                    var polygon = polygons[pl]
                    if ( polygon ) {
                        var pointsCount = 3
                        var points = new Array()
                        for ( var p = 0; p < pointsCount; p++ )
                            points[p] = Qt.point( Math.random() * crashBox.width, Math.random() * crashBox.height )
                        polygon.setPolygon( points )
                        polygon.color = randomColor()
                        polygon.border.width = Math.floor( Math.random() * 10.0 + 1.0 )
                        polygon.border.color = randomColor()
                    }
                }
            }
        }
        Button {
            text: "Delete Polygons"
            onClicked: {
                for ( var a = 0; a < polygons.length; a++ ) {
                    if ( polygons[a] )
                        polygons[a].destroy()
                    polygons[a] = undefined
                }
                polygons = new Array()
            }
        }
    }
}

