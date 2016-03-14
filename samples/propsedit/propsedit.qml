/*
    This file is part of QuickProperties2 library.

    Copyright (C) 2016  Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QuickProperties 2.0 as Qps
import "qrc:/QuickProperties2" as Qps

Item {
    id: mainVindow
    anchors.fill: parent

    // Styled Rectangle test --------------------------------------------------
    Qps.Properties {
        id: rectangleStyle

        property string content: "Rectangle content string"
        Qps.StringLimit { target: "content"; maxChars: 40 }

        property real   width: 200
        Qps.RealLimit { target: "width"; min: 50; max: 300; stepSize: 1. }
        property real   height: 150
        Qps.RealLimit { target: "height"; min: 40; max: 250 }

        property real   radius: 5
        property color  color: "blue"
        property int    borderWidth: 3
        Qps.IntLimit { target: "borderWidth"; min: 1; max: 6 }
        property color  borderColor: "violet"
    }

    Rectangle {
        x: 15; y: 15
        width: 250; height: 300
        color: "lightblue"; border.width: 2; border.color: "black"
        Qps.PropertiesEditor {
            id: styleEditor
            anchors.fill: parent; anchors.margins: 5
            properties: rectangleStyle
        }
    }

    Rectangle {
        x: 300; y : 50
        width: rectangleStyle.width
        height: rectangleStyle.height
        color: rectangleStyle.color
        radius: rectangleStyle.radius
        border.width: rectangleStyle.borderWidth
        border.color: rectangleStyle.borderColor
        Text {
            anchors.fill: parent; anchors.margins: 25
            text: rectangleStyle.content;  color: "white"
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }
    }

    // ComboBox Properties model test -----------------------------------------
    Rectangle {
        x: 15; y: 350
        width: 150; height: 80
        color: "lightblue"; border.width: 2; border.color: "black"
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 5
            Text { Layout.fillWidth: true; Layout.fillHeight: true; text: "QpsProperties model in ComboBox test:"; wrapMode: Text.Wrap}
            ComboBox{
                Layout.fillWidth: true
                model: rectangleStyle
                textRole: "propertyName"
            }
        }
    }

    // C++ Properties model test ----------------------------------------------
    Rectangle {
        x: 560;     y: 15
        width: 250; height: 260
        color: "lightblue"; border.width: 2; border.color: "black"
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 2
            id: layout
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            Text { text: "Edit C++ properties" }
            Qps.PropertiesEditor {
                Layout.fillWidth: true; Layout.fillHeight: true
                id: cppStyleEditor
                properties: cppRectangleStyle   // Defined from rootContext( )->setContextProperty() in qpsMainWindow.cpp
            }
        }
    }

    Rectangle {
        x: 850; y : 50
        width: 200
        height: 150
        color: "lightblue"
        radius: cppRectangleStyle.radius
        border.width: cppRectangleStyle.borderWidth
        border.color: "red"
        Text {
            anchors.fill: parent; anchors.margins: 25
            text: cppRectangleStyle.content;  color: "white"; font.bold: true; font.pointSize: 12
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }
    }
}

