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

import QtQuick              2.7
import QtQuick.Layouts      1.3
import QtQuick.Controls     2.0
import QtGraphicalEffects   1.0

import QuickGeoGL           1.0 as Qgl
import "qrc:/QuickGeoGL"    1.0 as Qgl

Flickable {
    contentWidth: parent.width; contentHeight: 1500
    clip: true
    ScrollBar.vertical: ScrollBar { }

    Frame {
        id: polygonTest1
        anchors.top: parent.top; anchors.topMargin: 10
        anchors.left: parent.left; anchors.leftMargin: 10
        width: 100; height: 100
        Qgl.ConvexPolygon {
            anchors.fill: parent
            color: "violet"
            visible: true
            Component.onCompleted: {
                appendPoint( Qt.point( width / 2, 0 ) )
                appendPoint( Qt.point( width, height ) )
                appendPoint( Qt.point( 0, height ) )
            }
        }
        Label { text: "Qgl.Polygon - scale=1.0" }
    }

    Frame {
        id: polygonTest2
        anchors.left: polygonTest1.right; anchors.leftMargin: 75
        anchors.top: polygonTest1.top
        width: 220; height: 220
        Qgl.ConvexPolygon {
            x:50; y:50; width: 100; height: 100
            scale: 2.0
            color: "violet"
            Component.onCompleted: {
                appendPoint( Qt.point( width / 2, 0 ) )
                appendPoint( Qt.point( width, height ) )
                appendPoint( Qt.point( 0, height ) )
            }
        }
        Label { text: "Qgl.Polygon - scale=2.0" }
    }

    Frame {
        id: polygonTest3
        anchors.left: parent.left; anchors.leftMargin: 10
        anchors.top: polygonTest2.bottom; anchors.topMargin: 25
        width: 400; height: 300
        Qgl.ConvexPolygon {
            id: polygon3
            x:100; y:35; width: 200; height: 140
            color: "violet"
            border.width: 1
            border.color: "violet"
            Component.onCompleted: {
                appendPoint( Qt.point( width / 2, 0 ) )
                appendPoint( Qt.point( width, height ) )
                appendPoint( Qt.point( 0, height ) )
            }
        }
        Label { text: "Qgl.Polygon\nborder.width=" + polygon3.border.width }
        ColumnLayout {
            anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter
            RowLayout {
                RadioButton { checked: true; text: qsTr("Violet"); onClicked: polygon3.border.color="violet" }
                RadioButton { text: qsTr("Green"); onClicked: polygon3.border.color="green" }
                RadioButton { text: qsTr("Blue"); onClicked: polygon3.border.color="blue" }
            }
            RowLayout {
                SpinBox { from: 0.; to: 15.; value:1; onValueChanged: polygon3.border.width=value }
                CheckBox { text: "Visible"; checked: polygon3.border.visible; onClicked: polygon3.border.visible = !polygon3.border.visible }
            }
        }
    }

    Frame {
        id: polygonTest4
        anchors.left: parent.left; anchors.leftMargin: 10
        anchors.top: polygonTest3.bottom; anchors.topMargin: 25
        width: 420; height: 200
        Text { text: "Qgl.ConvexPolygon { layer.enabled=" + polygon4.layer.enabled + " layer.effect=DropShadow }" }
        Qgl.ConvexPolygon {
            id: polygon4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom; anchors.bottomMargin: 15
            width: 200; height: 120
            color: "violet"
            border.width: 4
            border.color: "blue"
            Component.onCompleted: {
                appendPoint( Qt.point( width / 2, 2 ) )
                appendPoint( Qt.point( width - 2, height - 2 ) )
                appendPoint( Qt.point( 2, height - 2 ) )
            }
            background.layer.enabled: true
            background.layer.effect: DropShadow{ radius: 15; smooth: true; fast: false; samples: 31 }
        }
    }
    Frame {
        id: polygonTest5
        anchors.left: polygonTest4.right; anchors.leftMargin: 10
        anchors.top: polygonTest4.top
        width: 420; height: 200
        Text { text: "Qgl.ConvexPolygon { layer.enabled=" + polygon4.layer.enabled + " layer.effect=DropShadow }" }
        Qgl.ConvexPolygon {
            id: polygon5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom; anchors.bottomMargin: 15
            width: 200; height: 120
            color: "violet"
            border.width: 4
            border.color: "blue"
            Component.onCompleted: {
                appendPoint( Qt.point( width / 2, 0 ) )
                appendPoint( Qt.point( width, height ) )
                appendPoint( Qt.point( 0, height ) )
            }
            background.layer.enabled: true
            background.layer.effect: LinearGradient {
                start: Qt.point(0.,0.)
                end: Qt.point(polygon5.width, polygon5.height)
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "white" }
                    GradientStop { position: 1.0; color: "lightblue" }
                }
            }
        }
    }
}
