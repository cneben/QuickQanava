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

import QtQuick                      2.7
import QtQuick.Controls             2.0
import QtQuick.Controls.Material    2.0
import QtQuick.Layouts              1.3
import QuickProperties 2.0      as Qps
import "qrc:/QuickProperties"   as Qps

Item {
    id: window
    anchors.fill: parent

    Label { id: voidLabel; visible: false }

    Rectangle {
        anchors.fill: parent
        color: Qt.lighter(Material.backgroundColor)
    }
    Switch {
        x: 15; y: 2;
        text: "Material Dark"
        onCheckedChanged: { window.Material.theme = checked ? Material.Dark : Material.Light }
    }

    // Styled Rectangle test --------------------------------------------------
    Qps.Properties {
        id: rectangleStyle

        property string content: "Rectangle content string"
        Qps.StringOptions { target: "content"; maxChars: 40 }

        property real   width: 200
        Qps.RealOptions { target: "width"; min: 50; max: 300; stepSize: 1. }
        property real   height: 150
        Qps.RealOptions { target: "height"; min: 40; max: 250 }

        property font   dummyFont: voidLabel.font
        property bool   isVisible: true

        property real   radius: 5
        Qps.RealOptions { target: "radius"; type: Qps.RealOptions.SpinBox; min: 0; max: 25; stepSize: 0.1 }
        property color  color: "blue"
        property int    borderWidth: 3
        Qps.IntOptions { target: "borderWidth"; type: Qps.IntOptions.SpinBox; min: 1; max: 6 }
        property color  borderColor: "violet"
        property int    dummyInt: 3
        Qps.IntOptions { target: "dummyInt"; min: 1; max: 6 }
    }

    Pane {
        id: styleEditor
        anchors.top: parent.top; anchors.left: parent.left
        anchors.topMargin: 55; anchors.leftMargin: 15
        width: 350; height: 450
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 5
            Label { Layout.fillWidth: true; Layout.fillHeight: false; text: "Rectangle properties"; font.bold: true }
            Frame {
                Layout.fillWidth: true; Layout.fillHeight: true
                Qps.PropertiesEditor {
                    anchors.fill: parent
                    clip: true
                    properties: rectangleStyle
                }
            }
        }
    }

    Rectangle {
        id: rectangle1
        anchors.left: styleEditor.right
        anchors.leftMargin: 25
        visible: rectangleStyle.isVisible
        anchors.top: styleEditor.top
        width: rectangleStyle.width
        height: rectangleStyle.height
        color: rectangleStyle.color
        radius: rectangleStyle.radius
        border.width: rectangleStyle.borderWidth
        border.color: rectangleStyle.borderColor
        Label {
            anchors.fill: parent; anchors.margins: 25
            text: rectangleStyle.content;  color: "white"
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }
    }

    // ComboBox Properties model test -----------------------------------------
    Pane {
        anchors.top: styleEditor.bottom
        anchors.topMargin: 25
        x: 15
        width: 170; height: 150
        //color: "lightblue"; border.width: 2; border.color: "black"
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 5
            Label { Layout.fillWidth: true; Layout.fillHeight: true; text: "QpsProperties model in ComboBox test:"; wrapMode: Text.Wrap}
            ComboBox{
                Layout.fillWidth: true
                model: rectangleStyle
                textRole: "propertyName"
            }
        }
    }

    // C++ Properties model test ----------------------------------------------
    Pane {
        id: cppStyleEditor
        anchors.left: rectangle1.right
        anchors.leftMargin: 40
        anchors.top: styleEditor.top
        width: 350; height: 350
        //color: "lightblue"; border.width: 2; border.color: "black"
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 2
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            Label { text: "Edit C++ properties"; font.bold: true }
            Frame {
                Layout.fillWidth: true; Layout.fillHeight: true
                Qps.PropertiesEditor {
                    anchors.fill: parent
                    properties: cppRectangleStyle   // Defined from rootContext( )->setContextProperty() in qpsMainWindow.cpp
                }
            }
        }
    }

    Rectangle {
        anchors.left: cppStyleEditor.right
        anchors.leftMargin: 25
        anchors.top : cppStyleEditor.top
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

