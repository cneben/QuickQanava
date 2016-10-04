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
        id: arrowTest1
        anchors.left: parent.left;  anchors.leftMargin: 10
        anchors.top: parent.top;    anchors.topMargin: 10
        width: 700; height: arrowControlsLayout.implicitHeight + arrowTest1.padding * 2
        RowLayout {
            anchors.fill: parent; anchors.margins: 0
            Layout.margins: 0
            Qgl.Arrow {
                id: arrow
                Layout.fillWidth: true; Layout.fillHeight: true; Layout.margins: 0
                p1: Qt.point(15, height-15)
                p2: Qt.point(width-15, 15)
                smooth: smoothArrow.checked
                antialiasing: antialiasArrow.checked
                lineWidth: arrowLineWidth.value
                p1CapSize: arrowSrcCapSize.value
                p2CapSize: arrowDstCapSize.value
                Rectangle {
                    x: 0; y: 0; width: parent.width; height: parent.height
                    color: Qt.rgba(0,0,0,0); border.width:1; border.color: "violet"
                    visible: showArrowBr.checked
                }
            }
            ColumnLayout {
                id: arrowControlsLayout
                Layout.preferredWidth: 230; Layout.fillHeight: true
                Layout.margins: 0
                Label { text: "Qgl.Arrow - scale=1.0:" }
                CheckBox { id: showArrowBr; text: "Bounding Rect" }
                CheckBox { id: smoothArrow; checked: true; text: "Smooth" }
                CheckBox { id: antialiasArrow; checked: true; text: "Antialias" }
                RowLayout { SpinBox { id: arrowSrcCapSize; from: 0; to: 25; value: 8.0 } Label { text: "Source Cap size" } }
                RowLayout { SpinBox { id: arrowDstCapSize; from: 0; to: 25; value: 8.0 } Label { text: "Dest. Cap size" } }
                RowLayout{ SpinBox { id: arrowLineWidth; from: 0; to: 25; value: 4.0 } Label{ text: "Line width" } }
                ComboBox{
                    Layout.preferredWidth: 190
                    model: [ "Source NoCap", "Source ArrowCap", "Source CircleCap" ]
                    currentIndex: arrow.p1CapStyle === Qgl.Arrow.NoCap ? 0 :
                                                                             arrow.p1CapStyle === Qgl.Arrow.ArrowCap ? 1 :
                                                                                                                           arrow.p1CapStyle === Qgl.Arrow.CircleCap ? 2 : -1
                    onActivated: {
                        arrow.p1CapStyle = index === 0 ? Qgl.Arrow.NoCap :
                                                             ( index === 1 ? Qgl.Arrow.ArrowCap :
                                                                             ( index === 2 ? Qgl.Arrow.CircleCap : Qgl.Arrow.NoCap ) )
                    }
                }
                ComboBox{
                    Layout.preferredWidth: 190
                    model: [ "Destination NoCap", "Destination ArrowCap", "Destination CircleCap" ]
                    currentIndex: arrow.p2CapStyle === Qgl.Arrow.NoCap ? 0 :
                                                                                  arrow.p2CapStyle === Qgl.Arrow.ArrowCap ? 1 :
                                                                                                                                     arrow.p2CapStyle === Qgl.Arrow.CircleCap ? 2 : -1
                    onActivated: {
                        arrow.p2CapStyle = index === 0 ? Qgl.Arrow.NoCap :
                                                                  ( index === 1 ? Qgl.Arrow.ArrowCap :
                                                                                  ( index === 2 ? Qgl.Arrow.CircleCap : Qgl.Arrow.NoCap ) )
                    }
                }
            }
        }
    }
}

