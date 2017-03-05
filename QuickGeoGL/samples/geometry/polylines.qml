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
    Frame {
        id: polylineTest1
        anchors.left: parent.left; anchors.leftMargin: 10
        anchors.top: parent.top; anchors.topMargin: 25
        width: 450; height: 220
        Qgl.PolyLine {
            id: polyLine1
            x:15; y:15; width: 150; height: 150
            color: "violet"
            lineWidth: 2
            Component.onCompleted: {
                polyLine1.closed = true
                setPoints( [Qt.point( 0, height ),
                            Qt.point( width / 2., 0 ),
                            Qt.point( width, height )] )
            }
        }
        ColumnLayout {
            anchors.verticalCenter: parent.verticalCenter; anchors.right: parent.right
            Label { text: "Qgl.PolyLine" }
            RowLayout {
                RadioButton { checked: true; text: qsTr("Violet"); onClicked: polyLine1.color="violet" }
                RadioButton { text: qsTr("Green"); onClicked: polyLine1.color="green" }
                RadioButton { text: qsTr("Blue"); onClicked: polyLine1.color="blue" }
            }
            RowLayout {
                SpinBox { from: 0.; to: 15.; value:1; onValueChanged: polyLine1.lineWidth=value }
                CheckBox { text: "Closed"; checked: polyLine1.closed; onClicked: polyLine1.closed = !polyLine1.closed }
            }
        }
    }
}

