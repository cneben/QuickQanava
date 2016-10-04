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

Flickable {
    contentWidth: parent.width; contentHeight: 1500
    clip: true
    ScrollBar.vertical: ScrollBar { }

    Component {
        id: lines
        Item {
            property real   inclination: 0.5
            property real   spacing: 30.
            property color  lineColor: "violet"
            Qgl.Line {
                id: l0; lineWidth: 0.5; color: lineColor
                p1: Qt.point( 0, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
            }
            Qgl.Line {
                id: l1; lineWidth: 1.0; color: lineColor
                p1: Qt.point( l0.p1.x + spacing, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
            }
            Qgl.Line {
                id: l2; lineWidth: 2.0; color: lineColor
                p1: Qt.point( l1.p1.x + spacing, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
            }
            Qgl.Line {
                id: l3
                p1: Qt.point( l2.p1.x + spacing, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
                lineWidth: 3.0; color: lineColor
            }
            Qgl.Line {
                id: l4; lineWidth: 4.0; color: lineColor
                p1: Qt.point( l3.p1.x + spacing, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
            }
            Qgl.Line {
                id: l5; lineWidth: 5.0; color: lineColor
                p1: Qt.point( l4.p1.x + spacing, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
            }
            Qgl.Line {
                id: l6; lineWidth: 6.0; color: lineColor
                p1: Qt.point( l5.p1.x + spacing, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
            }
            Qgl.Line {
                id: l7; lineWidth: 7.0; color: lineColor
                p1: Qt.point( l6.p1.x + spacing, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
            }
            Qgl.Line {
                id: l8; lineWidth: 8.0; color: lineColor
                p1: Qt.point( l7.p1.x + spacing, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
            }
            Qgl.Line {
                id: l9; lineWidth: 9.0; color: lineColor
                p1: Qt.point( l8.p1.x + spacing, parent.height );
                p2: Qt.point( p1.x + inclination * parent.height, 0.0 )
            }
        }
    }

    Frame {
        id: linesBasic1
        anchors.left: parent.left;  anchors.leftMargin: 10
        anchors.top: parent.top;    anchors.topMargin: 10
        width: 400; height: 250
        Text { text: "Horizontal Qgl.Line" }
        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 30
            Item {
                Layout.fillWidth: true; Layout.preferredHeight: 25
                Qgl.Line { lineWidth: 0.1; p1: Qt.point( 0, 0 ); p2: Qt.point( parent.width, 0 ); color: "blue" }
                Text { text: "lineWidth=0.1"; anchors.verticalCenter: parent.verticalCenter; anchors.right: parent.right }
            }
            Item {
                Layout.fillWidth: true; Layout.preferredHeight: 25
                Qgl.Line { lineWidth: 0.2; p1: Qt.point( 0, 0 ); p2: Qt.point( parent.width, 0 ); color: "blue" }
                Text { text: "lineWidth=0.2"; anchors.verticalCenter: parent.verticalCenter; anchors.right: parent.right }
            }
            Item {
                Layout.fillWidth: true; Layout.preferredHeight: 25
                Qgl.Line { lineWidth: 0.5; p1: Qt.point( 0, 0 ); p2: Qt.point( parent.width, 0 ); color: "blue" }
                Text { text: "lineWidth=0.5"; anchors.verticalCenter: parent.verticalCenter; anchors.right: parent.right }
            }
            Item {
                Layout.fillWidth: true; Layout.preferredHeight: 25
                Qgl.Line { lineWidth: 1.0; p1: Qt.point( 0, 0 ); p2: Qt.point( parent.width, 0 ); color: "blue" }
                Text { text: "lineWidth=1.0"; anchors.verticalCenter: parent.verticalCenter; anchors.right: parent.right }
            }
            Item {
                Layout.fillWidth: true; Layout.preferredHeight: 25
                Qgl.Line { lineWidth: 1.5; p1: Qt.point( 0, 0 ); p2: Qt.point( parent.width, 0 ); color: "blue" }
                Text { text: "lineWidth=1.5"; anchors.verticalCenter: parent.verticalCenter; anchors.right: parent.right }
            }
            Item {
                Layout.fillWidth: true; Layout.preferredHeight: 25
                Qgl.Line { lineWidth: 2.0; p1: Qt.point( 0, 0 ); p2: Qt.point( parent.width, 0 ); color: "blue" }
                Text { text: "lineWidth=2.0"; anchors.verticalCenter: parent.verticalCenter; anchors.right: parent.right }
            }
        }
    }

    Frame {
        id: linesBasic2
        anchors.left: linesBasic1.right;    anchors.leftMargin: 10
        anchors.top: parent.top;            anchors.topMargin: 10
        width: 400; height: 200
        Text { text: "Vertical Qgl.Line" }
        RowLayout {
            anchors.fill: parent
            anchors.topMargin: 30
            Item {
                Layout.fillHeight: true; Layout.preferredWidth: 45
                Qgl.Line { lineWidth: 0.1; p1: Qt.point( parent.width/2, 0 ); p2: Qt.point( parent.width/2, parent.height ); color: "blue" }
                Text { text: "0.1"; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top  }
            }
            Item {
                Layout.fillHeight: true; Layout.preferredWidth: 45
                Qgl.Line { lineWidth: 0.2; p1: Qt.point( parent.width/2, 0 ); p2: Qt.point( parent.width/2, parent.height ); color: "blue" }
                Text { text: "0.2"; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top  }
            }
            Item {
                Layout.fillHeight: true; Layout.preferredWidth: 45
                Qgl.Line { lineWidth: 0.5; p1: Qt.point( parent.width/2, 0 ); p2: Qt.point( parent.width/2, parent.height ); color: "blue" }
                Text { text: "0.5"; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top  }
            }
            Item {
                Layout.fillHeight: true; Layout.preferredWidth: 45
                Qgl.Line { lineWidth: 1.0; p1: Qt.point( parent.width/2, 0 ); p2: Qt.point( parent.width/2, parent.height ); color: "blue" }
                Text { text: "1.0"; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top }
            }
            Item {
                Layout.fillHeight: true; Layout.preferredWidth: 45
                Qgl.Line { lineWidth: 1.5; p1: Qt.point( parent.width/2, 0 ); p2: Qt.point( parent.width/2, parent.height ); color: "blue" }
                Text { text: "1.5"; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top  }
            }
            Item {
                Layout.fillHeight: true; Layout.preferredWidth: 45
                Qgl.Line { lineWidth: 2.0; p1: Qt.point( parent.width/2, 0 ); p2: Qt.point( parent.width/2, parent.height ); color: "blue" }
                Text { text: "2.0"; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top  }
            }
        }
    }

    Frame {
        id: linesScale1
        anchors.left: parent.left;          anchors.leftMargin: 10
        anchors.top: linesBasic1.bottom;    anchors.topMargin: 10
        width: 500; height: 200
        ColumnLayout {
            anchors.fill: parent; anchors.margins: 0
            Label { Layout.fillHeight: false; text: "Qgl.Line - scale=1.0:" }
            Loader {
                Layout.fillWidth: true; Layout.fillHeight: true
                sourceComponent: lines
            }
        }
    }
    Frame {
        id: linesScale2
        anchors.left: parent.left;          anchors.leftMargin: 10
        anchors.top: linesScale1.bottom;    anchors.topMargin: 10
        width: 800; height: 400
        ColumnLayout {
            anchors.fill: parent; anchors.margins: 0
            Label { Layout.fillHeight: false; text: "Qgl.Line - scale=2.0:" }
            Item {
                anchors.centerIn: parent
                width: 500; height: 200
                transformOrigin: Item.Center
                scale: 2.
                Loader {
                    anchors.fill: parent
                    sourceComponent: lines
                }
            }
        }
    }
}

