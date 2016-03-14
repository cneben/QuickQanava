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

    // Define a default empty properties
    Qps.Properties {
        id: empty

        property string label: "default empty content string"
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

    // Current Rectangle Properties List Visualisation ------------------------
    Rectangle {
        id: propertiesListViewDialog
        x: 25;      y: 15
        width: 250; height: 300
        color: "lightblue"; border.width: 2; border.color: "black"
        /* title: "Rectangle properties list" */

        property alias current : propertiesListView.current
        Qps.PropertiesListView {
            id: propertiesListView
            anchors.fill: parent; anchors.margins: 4
            model: rectangleStyles
            onPropertiesClicked: { console.debug( "onPropertiesClicked():" + properties.label ) }
            onPropertiesDoubleClicked: { console.debug( "onPropertiesDoubleClicked():" + properties.label ) }
        }
    }

    // Current Rectangle Properties List Visualisation ------------------------
    Rectangle {
        id: customListViewDialog
        y: 15
        anchors.left: propertiesListViewDialog.right; anchors.leftMargin: 30
        width: 250; height: 300
        color: "lightblue"; border.width: 2; border.color: "black"
        /* title: "Custom properties list" */

        Qps.PropertiesListView {
            id: customListView
            anchors.fill: parent; anchors.margins: 4
            model: rectangleStyles
            onPropertiesClicked: {
                console.debug( "Custom onPropertiesClicked():" + properties.label )
            }
            onPropertiesDoubleClicked: {
                console.debug( "Custom onPropertiesDoubleClicked():" + properties.label )
                propertiesEditorDialog.properties = properties
            }
            delegate: Component {
                Item {
                    id: customDelegate
                    width: customDelegate.ListView.view.width - 2
                    height: 65
                    Column {
                        anchors.fill: parent
                        Text {
                            text: "label=" + item.label
                        }
                        Text {
                            text: "radius=" + item.radius + "  borderWidth=" + item.borderWidth
                        }
                        Rectangle {
                            width: textColor.implicitWidth
                            height: textColor.implicitHeight
                            color: item.color
                            Text {
                                id: textColor
                                text: "color=" + item.color
                                color: item.borderColor
                            }
                        }
                        Rectangle {
                            width: borderColor.implicitWidth
                            height: borderColor.implicitHeight
                            color: item.borderColor
                            Text {
                                id: borderColor
                                text: "borderColor=" + item.borderColor
                                color: item.color
                            }
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            customDelegate.ListView.view.currentIndex = index
                            customListView.propertiesClicked( item )
                        }
                        onDoubleClicked: {
                            customDelegate.ListView.view.currentIndex = index
                            customListView.propertiesDoubleClicked( item )
                        }
                    }
                }
            }
        }
    }

    // Current RectangleProperty Visualisation --------------------------------
    Rectangle {
        id: propertiesEditorDialog
        y: 15
        anchors.left: customListViewDialog.right;   anchors.leftMargin: 30
        width: 350; height: 300
        color: "lightblue"; border.width: 2; border.color: "black"
        /* title: "Edit current Rectangle properties" */

        property alias properties : propertiesEditor.properties
        Qps.PropertiesEditor {
            id: propertiesEditor
            anchors.fill: parent
            anchors.margins: 4
            properties: ( propertiesListViewDialog.current != undefined ? propertiesListViewDialog.current : empty )
        }
        //QpsHideablePanel {
        //    title: "Create property"
        //    Layout.fillWidth: true
            /*Qps.PropertyCreator {
                properties: propertiesEditor.properties
                onPropertyCreated: { console.debug( "Property added..." ) }
            }*/
        //}
    }

    Rectangle {
        anchors.left: propertiesEditorDialog.right
        anchors.leftMargin: 30
        y: 50
        width: 200
        height: 150
        color: propertiesEditorDialog.properties.color
        radius: propertiesEditorDialog.properties.radius
        border.width: propertiesEditorDialog.properties.borderWidth
        border.color: propertiesEditorDialog.properties.borderColor
        Text {
            anchors.fill: parent; anchors.margins: 25
            text: propertiesEditorDialog.properties.label;  color: "white"; font.bold: true; font.pointSize: 12
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }
    }
}

