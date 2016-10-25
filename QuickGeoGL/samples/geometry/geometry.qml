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

ColumnLayout {
    id: window
    TabBar {
        id: tabBar
        Layout.fillWidth: true; Layout.fillHeight: false
        TabButton { text: qsTr("Crash") }
        TabButton { text: qsTr("Polygons") }
        TabButton { text: qsTr("Arrows") }
        TabButton { text: qsTr("Lines") }
        TabButton { text: qsTr("Polylines") }
        TabButton { text: qsTr("Points & Circles") }
    }
    StackLayout {
        Layout.fillWidth: true; Layout.fillHeight: true
        currentIndex: tabBar.currentIndex
        Item {
            id: crash
            Loader { anchors.fill: parent; source: "qrc:/crashtest.qml"}
        }
        Item {
            id: polygons
            Loader { anchors.fill: parent; source: "qrc:/polygons.qml"}
        }
        Item {
            id: arrows
            anchors.fill: parent
            Loader { anchors.fill: parent; source: "qrc:/arrows.qml"}
        }
        Item {
            id: lines
            anchors.fill: parent
            Loader { anchors.fill: parent; source: "qrc:/lines.qml"}
        }
        Item {
            id: polylines
            anchors.fill: parent
            Loader { anchors.fill: parent; source: "qrc:/polylines.qml"}
        }
        Item {
            id: points
        }
    }
}

