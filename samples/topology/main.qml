/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

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

import QtQuick 2.5
import QtQuick.Extras 1.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "qrc:/QuickProperties2" as Qps

Item {
    id: window
    anchors.fill: parent
    FileDialog {
        id: openImageDialog
        property var targetGraph: undefined
        title: "Select an image file"
        selectMultiple: false
        selectExisting: true
        onAccepted: {
            console.debug( "Inserting an image node..." )
                if ( targetGraph != undefined ) {
                var imgNode = graph.insertNode( "qan::ImgNode" )
                if ( imgNode != null ) {
                    imgNode.loadImageFromFile( fileUrls );
                    centerItem( imgNode );
                }
            }
        }
    }
    FileDialog {
        id: openGraphDialog
        title: "Load a graph from a QuickQanava GTpo file"
        selectMultiple: false
        selectExisting: true
        onAccepted: {
            qanSerializer.loadGraphFrom( openGraphDialog.fileUrls, graph, progressNotifier )
        }
    }
    FileDialog {
        id: saveGraphAsDialog
        title: "Save graph to a QuickQanava GTpo file"
        selectMultiple: false
        selectExisting: false
        onAccepted: {
            qanSerializer.saveGraphTo( graph, saveGraphAsDialog.fileUrls, progressNotifier )
        }
    }

    Menu {
        id: menu
        title: "Main Menu"
        property var targetNode: undefined
        property var targetEdge: undefined
        MenuItem {
            text: "Clear Graph"
            onTriggered: { graph.clear() }
        }
        MenuItem {
            text: "Load"
            onTriggered: { openGraphDialog.open() }
        }
        MenuItem {
            text: "Save As"
            onTriggered: { saveGraphAsDialog.open() }
        }
        MenuItem {
            text: "Insert Node"
            onTriggered: {
                var n = graph.insertNode()
                centerItem( n );
                n.label = "Node #" + graph.getNodeCount()
            }
        }
        MenuItem {
            text: "Insert Image Node"
            onTriggered: { openImageDialog.targetGraph = graph; openImageDialog.open() }
        }
        MenuItem {
            text: "Remove node"
            enabled: menu.targetNode != undefined
            onTriggered: {
                if ( menu.targetNode != undefined )
                    graph.removeNode( menu.targetNode )
                menu.targetNode = undefined
            }
        }
        MenuItem {
            text: "Remove edge"
            enabled: menu.targetEdge != undefined
            onTriggered: {
                if ( menu.targetEdge != undefined )
                    graph.removeEdge( menu.targetEdge )
                menu.targetEdge = undefined;
            }
        }
        MenuItem {
            text: "Insert Group"
            onTriggered: {
                var n = graph.insertGroup()
                centerItem( n );
                n.label = "Group #" + graph.getGroupCount()
            }
        }
    }

    Text {
        text: "Right click for main menu:
               \t-Add content with Add Node or Add Image Node entries.
               \t-Use the DnD connector to add edges between nodes.
               \t-Save current topology to Protocol Buffer v3 with Save As command.
               \t-Use Clear Graph command before openning an existing topology file.
               \t-Load existing binary content with Load."
    }

    //! Move a generic item in view current center.
    function centerItem( item ) {
        if ( item == undefined || item == null )
            return
        var windowCenter = Qt.point( ( window.width - item.width ) / 2.,
                                     ( window.height - item.height ) / 2. )
        var containerNodeCenter = window.mapToItem( navigable.containerItem, windowCenter.x, windowCenter.y )
        item.x = containerNodeCenter.x
        item.y = containerNodeCenter.y
    }

    Qan.Navigable {
        id: navigable
        anchors.fill: parent
        Qan.Graph {
            parent: navigable.containerItem
            id: graph
            objectName: "graph"
            anchors.fill: parent
            enableConnectorDropNode: true

            property Component imgNodeComponent: Qt.createComponent( "qrc:/ImgNode.qml" )

            Component.onCompleted: {
                graph.registerNodeDelegate( "qan::ImgNode", graph.imgNodeComponent )
            }
            onNodeRightClicked: { menu.targetNode = node; menu.popup() }
            onEdgeRightClicked: { menu.targetEdge = edge; menu.popup() }
        } // Qan.Graph: graph

        onRightClicked: { menu.targetNode = undefined; menu.targetEdge = undefined; menu.popup() }
    } // Qan.Navigable: navigable

    Item {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        height: 120
        Rectangle { // Background
            anchors.fill: parent
            color: "lightblue"; opacity: 0.6; border.width: 1; border.color: "black"
        }
        RowLayout {
            anchors.fill: parent; anchors.margins: 4
            ColumnLayout {
                Layout.preferredWidth: 250; Layout.fillHeight: true
                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ColumnLayout {
                        RowLayout {
                            Text { text: "Node cout:" }
                            SpinBox { id: nodeCount; minimumValue: 1; maximumValue: 250; value: 25 }
                        }
                        RowLayout {
                            Text { text: "OutNodes range:" }
                            SpinBox { id: minOutNodes; minimumValue: 1; maximumValue: 4; value: 1 }
                            SpinBox { id: maxOutNodes; minimumValue: minOutNodes.value + 1; maximumValue: minOutNodes.value + 5; value: 2 }
                        }
                    }
                    ColumnLayout {
                        RowLayout {
                            Text { text: "Width range:" }
                            SpinBox { id: minWidth; minimumValue: 50; maximumValue: 150; value: 75 }
                            SpinBox { id: maxWidth; minimumValue: minWidth.value + 50; maximumValue: minWidth.value + 150; value: 100 }
                        }
                        RowLayout {
                            Text { text: "Height range:" }
                            SpinBox { id: minHeight; minimumValue: 20; maximumValue: 750; value: 50 }
                            SpinBox { id: maxHeight; minimumValue: minHeight.value + 25; maximumValue: minHeight.value + 50; value: 60 }
                        }
                    }
                } // RowLayout: random generator options
                Button {
                    Layout.alignment: Qt.AlignRight
                    text: "Generate"
                    onClicked: {
                        var rect = Qt.rect( 0, 0, window.width, window.height )
                        graph.initializeRandom( nodeCount.value,
                                               minOutNodes.value, maxOutNodes.value,
                                               minWidth.value, maxWidth.value,
                                               minHeight.value, maxHeight.value,
                                               rect )
                    }
                }
            } // ColumnLayout: random generator
        } // RowLayout: graph generator main layout
    } // Item: graph generator

    Item {
        id: edgeList
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: parent.right; anchors.rightMargin: 15
        width: 200; height: 300
        Rectangle { // Background
            anchors.fill: parent
            color: "lightblue"; opacity: 0.6; border.width: 1; border.color: "black"
        }
        ColumnLayout {
            anchors.fill: parent
            ListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                clip: true
                model: graph.edges
                spacing: 4; focus: true; flickableDirection : Flickable.VerticalFlick
                highlight: Rectangle {
                    x: 0; y: ( ListView.view.currentItem != null ? ListView.view.currentItem.y : 0 )
                    width: ListView.view.width; height: ( ListView.view.currentItem != null ? ListView.view.currentItem.height : 100 )
                    color: "lightsteelblue"; opacity: 0.7; radius: 5
                    Behavior on y { SpringAnimation { duration: 200; spring: 2; damping: 0.1 } }
                }
                delegate: Item {
                    id: edgeDelegate
                    width: ListView.view.width; height: edgeLayout.implicitHeight
                    ColumnLayout {
                        anchors.fill: parent
                        id: edgeLayout
                        Text { text: "Label: " + itemData.label }
                        Text { text: "  Src: " + itemData.sourceItem.label }
                        Text { text: "  Dst: " + itemData.destinationItem.label }
                    }
                    MouseArea { anchors.fill: parent; onClicked: { edgeDelegate.ListView.view.currentIndex = index } }
                }
            }
            ComboBox {
                Layout.fillWidth: true; Layout.fillHeight: false
                model: graph.edges
                textRole: "label"
            }
        }
    }

    Item {
        id: nodeList
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: edgeList.left; anchors.rightMargin: 15
        width: 200; height: 300
        Rectangle { // Background
            anchors.fill: parent
            color: "lightblue"; opacity: 0.6; border.width: 1; border.color: "black"
        }
        ColumnLayout {
            anchors.fill: parent
            ListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                clip: true
                model: graph.nodes
                spacing: 4; focus: true; flickableDirection : Flickable.VerticalFlick
                highlightFollowsCurrentItem: false
                highlight: Rectangle {
                    x: 0; y: ListView.view.currentItem.y;
                    width: ListView.view.width; height: ListView.view.currentItem.height
                    color: "lightsteelblue"; opacity: 0.7; radius: 5
                    Behavior on y { SpringAnimation { duration: 200; spring: 2; damping: 0.1 } }
                }
                delegate: Item {
                    id: nodeDelegate
                    width: ListView.view.width; height: 30;
                    Text { id: nodeLabel; text: "Label: " + itemData.label }
                    MouseArea {
                        anchors.fill: nodeDelegate
                        onClicked: { nodeDelegate.ListView.view.currentIndex = index }
                    }
                }
            }
            ComboBox {
                Layout.fillWidth: true; Layout.fillHeight: false
                model: graph.nodes
                textRole: "itemLabel"
            }
        }
    }

    Qan.ProgressNotifier {
        id: progressNotifier
        onShowProgress: { progressDialog.visible = true; }
        onHideProgress: { progressDialog.visible = false; }
    }
    Item {
        id: progressDialog
        anchors.fill: parent
        visible: false
        onVisibleChanged: console.debug( "progressDialog.visible=" + visible )
        Rectangle {     // Background
            anchors.fill: parent
            color: "black"
            opacity: 0.3
        }
        ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 220
            ProgressBar {
                Layout.fillWidth: true; Layout.fillHeight: false
                value: progressNotifier.progress
                Text {
                    anchors.centerIn: parent
                    text: Math.round( progressNotifier.progress * 100. ) + " %"
                }
            }
            ProgressBar {
                Layout.fillWidth: true; Layout.fillHeight: false
                value: progressNotifier.phaseProgress
                onValueChanged: { indeterminate = value < 0. }
                Text {
                    anchors.centerIn: parent
                    text: Math.round( progressNotifier.phaseProgress * 100. ) + " %"
                }
            }
            Text {
                Layout.alignment: Layout.Center
                text: progressNotifier.phaseLabel
            }
        }
    }
}
