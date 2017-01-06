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

import QtQuick              2.7
import QtQuick.Controls     2.0
import QtQuick.Layouts      1.3
import QtQuick.Dialogs      1.2

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "qrc:/QuickContainers" as Qps

Item {
    id: window
    anchors.fill: parent
    FileDialog {
        id: openImageDialog
        property var targetGraph: undefined
        title: "Select an image file"
        selectMultiple: false; selectExisting: true
        onAccepted: {
            if ( targetGraph !== undefined ) {
                var imgNode = topology.insertNode( "qan::ImgNode" )
                if ( imgNode ) {
                    imgNode.loadImageFromFile( fileUrls );
                    centerItem( imgNode );
                }
            }
        }
    }
    FileDialog {
        id: openGraphDialog
        title: "Load a graph from a QuickQanava GTpo file"
        selectMultiple: false; selectExisting: true
        onAccepted: { qanSerializer.loadGraphFrom( openGraphDialog.fileUrls, graph, progressNotifier ) }
    }
    FileDialog {
        id: saveGraphAsDialog
        title: "Save graph to a QuickQanava GTpo file"
        selectMultiple: false; selectExisting: false
        onAccepted: { qanSerializer.saveGraphTo( graph, saveGraphAsDialog.fileUrls, progressNotifier ) }
    }

    Menu {
        id: menu
        title: "Main Menu"
        property var targetNode: undefined
        property var targetEdge: undefined
        MenuItem { text: "Clear Graph"; onTriggered: topology.clear() }
        MenuItem { text: "Load"; onTriggered: openGraphDialog.open() }
        MenuItem { text: "Save As"; onTriggered: saveGraphAsDialog.open() }
        MenuItem {
            text: "Insert Node"
            onTriggered: {
                var n = topology.insertNode()
                centerItem( n );
                n.label = "Node #" + topology.getNodeCount()
            }
        }
        MenuItem {
            text: "Insert Image Node"
            onTriggered: { openImageDialog.targetGraph = topology; openImageDialog.open() }
        }
        MenuItem {
            text: "Remove node"
            enabled: menu.targetNode !== undefined
            onTriggered: {
                if ( menu.targetNode !== undefined )
                    topology.removeNode( menu.targetNode )
                menu.targetNode = undefined
            }
        }
        MenuItem {
            text: "Remove edge"
            enabled: menu.targetEdge !== undefined
            onTriggered: {
                if ( menu.targetEdge !== undefined )
                    topology.removeEdge( menu.targetEdge )
                menu.targetEdge = undefined;
            }
        }
        MenuItem {
            text: "Insert Group"
            onTriggered: {
                var n = topology.insertGroup()
                centerItem( n );
                n.label = "Group #" + topology.getGroupCount()
            }
        }
    }

    Label {
        text: "Right click for main menu:
               \t-Add content with Add Node or Add Image Node entries.
               \t-Use the DnD connector to add edges between nodes.
               \t-Save current topology to Protocol Buffer v3 with Save As command.
               \t-Use Clear Graph command before openning an existing topology file.
               \t-Load existing binary content with Load."
    }

    //! Move a generic item in view current center.
    function centerItem( item ) {
        if ( item ) {
            var windowCenter = Qt.point( ( window.width - item.width ) / 2.,
                                        ( window.height - item.height ) / 2. )
            var containerNodeCenter = window.mapToItem( topology.containerItem, windowCenter.x, windowCenter.y )
            item.x = containerNodeCenter.x
            item.y = containerNodeCenter.y
        }
    }
    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        graph       : topology
        navigable   : true
        Qan.Graph {
            id: topology
            objectName: "graph"
            anchors.fill: parent
            clip: true
            enableConnectorDropNode: true

            property Component imgNodeComponent: Qt.createComponent( "qrc:/ImgNode.qml" )
            Component.onCompleted: {
                topology.registerNodeDelegate( "qan::ImgNode", topology.imgNodeComponent )
            }
            onNodeRightClicked: {
                var globalPos = node.mapToItem( topology, pos.x, pos.y )
                menu.x = globalPos.x
                menu.y = globalPos.y
                menu.targetNode = node;
                menu.open()
            }
            onEdgeRightClicked: {
                var globalPos = edge.mapToItem( topology, pos.x, pos.y )
                menu.x = globalPos.x
                menu.y = globalPos.y
                menu.targetEdge = edge; menu.open()
            }
        } // Qan.Graph: graph
        onRightClicked: {
            var globalPos = graphView.mapToItem( topology, pos.x, pos.y )
            menu.x = globalPos.x
            menu.y = globalPos.y
            menu.targetNode = undefined; menu.targetEdge = undefined; menu.open()
        }
    }

    Item {
        anchors.bottom: parent.bottom; anchors.right: parent.right; anchors.left: parent.left
        height: 130
        Frame { anchors.fill: parent; opacity: 0.8; padding: 0; Pane { anchors.fill: parent } } // Background
        RowLayout {
            anchors.fill: parent; anchors.margins: 10
            spacing: 25
            ColumnLayout {
                RowLayout {
                    Label { text: "Node cout:" }
                    SpinBox { id: nodeCount; from: 1; to: 250; value: 25 }
                }
                RowLayout {
                    Label { text: "OutNodes range:" }
                    SpinBox { id: minOutNodes; from: 1; to: 4; value: 1 }
                    SpinBox { id: maxOutNodes; from: minOutNodes.value + 1; to: minOutNodes.value + 5; value: 2 }
                }
            }
            ColumnLayout {
                RowLayout {
                    Label { text: "Width range:" }
                    SpinBox { id: minWidth; from: 50; to: 150; value: 75 }
                    SpinBox { id: maxWidth; from: minWidth.value + 50; to: minWidth.value + 150; value: 100 }
                }
                RowLayout {
                    Label { text: "Height range:" }
                    SpinBox { id: minHeight; from: 20; to: 750; value: 50 }
                    SpinBox { id: maxHeight; from: minHeight.value + 25; to: minHeight.value + 50; value: 60 }
                }
            }
            Button {
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                text: "Generate"
                onClicked: {
                    var rect = Qt.rect( 0, 0, window.width, window.height )
                    topology.initializeRandom( nodeCount.value,
                                              minOutNodes.value, maxOutNodes.value,
                                              minWidth.value, maxWidth.value,
                                              minHeight.value, maxHeight.value,
                                              rect )
                }
            }
        } // RowLayout: random generator options
    } // Item: graph generator

    Item {
        id: edgeList
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: parent.right; anchors.rightMargin: 15
        width: 200; height: 300
        Frame { anchors.fill: parent; opacity: 0.8; padding: 0; Pane { anchors.fill: parent } } // Background
        ColumnLayout {
            anchors.fill: parent; anchors.margins: 10
            Label {
                Layout.margins: 3; text: "Edges:"
                font.bold: true; horizontalAlignment: Text.AlignLeft
            }
            ListView {
                id: edgesList
                Layout.fillWidth: true; Layout.fillHeight: true
                clip: true
                model: topology.edges
                spacing: 4; focus: true; flickableDirection : Flickable.VerticalFlick
                highlight: Rectangle {
                    x: 0; y: ( edgesList.currentItem != null ? edgesList.currentItem.y : 0 )
                    width: edgesList.width; height: ( edgesList.currentItem != null ? edgesList.currentItem.height : 100 )
                    color: "lightsteelblue"; opacity: 0.7; radius: 5
                    Behavior on y { SpringAnimation { duration: 200; spring: 2; damping: 0.1 } }
                }
                delegate: Item {
                    id: edgeDelegate
                    width: ListView.view.width; height: edgeLayout.implicitHeight
                    ColumnLayout {
                        anchors.fill: parent
                        id: edgeLayout
                        Label { text: "Label: " + itemData.label }
                        Label { text: "  Src: " + itemData.sourceItem.label }
                        Label { text: "  Dst: " + itemData.destinationItem.label }
                    }
                    MouseArea { anchors.fill: parent; onClicked: { edgeDelegate.ListView.view.currentIndex = index } }
                }
            }
            ComboBox {
                Layout.fillWidth: true; Layout.fillHeight: false
                model: topology.edges
                textRole: "itemLabel"
            }
        }
    }

    Item {
        id: nodeList
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: edgeList.left; anchors.rightMargin: 15
        width: 200; height: 300
        Frame { anchors.fill: parent; opacity: 0.8; padding: 0; Pane { anchors.fill: parent } } // Background
        ColumnLayout {
            anchors.fill: parent; anchors.margins: 10
            Label {
                Layout.margins: 3; text: "Nodes:"
                font.bold: true; horizontalAlignment: Text.AlignLeft
            }
            ListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                clip: true
                model: topology.nodes
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
                    Label { id: nodeLabel; text: "Label: " + itemData.label }
                    MouseArea {
                        anchors.fill: nodeDelegate
                        onClicked: { nodeDelegate.ListView.view.currentIndex = index }
                    }
                }
            }
            ComboBox {
                Layout.fillWidth: true; Layout.fillHeight: false
                model: topology.nodes
                textRole: "itemLabel"
            }
        }
    }

    ColorDialog {
        id: selectionColorDialog
        title: "Selection hilight color"
        onAccepted: { topology.selectionColor = color; }
    }
    Item {
        id: selectionView
        anchors.top: nodeList.bottom;   anchors.topMargin: 15
        anchors.right: parent.right;    anchors.rightMargin: 15
        width: 250; height: 280
        Frame { anchors.fill: parent; opacity: 0.8; padding: 0; Pane { anchors.fill: parent } } // Background
        ColumnLayout {
            anchors.fill: parent; anchors.margins: 10
            Label {
                Layout.margins: 3; text: "Selection:"
                font.bold: true; horizontalAlignment: Text.AlignLeft
            }
            ListView {
                id: selectionListView
                Layout.fillWidth: true; Layout.fillHeight: true
                clip: true
                model: topology.selectedNodes
                spacing: 4; focus: true; flickableDirection : Flickable.VerticalFlick
                highlightFollowsCurrentItem: false
                highlight: Rectangle {
                    x: 0; y: ( selectionListView.currentItem !== null ? selectionListView.currentItem.y : 0 );
                    width: selectionListView.width; height: selectionListView.currentItem.height
                    color: "lightsteelblue"; opacity: 0.7; radius: 5
                    Behavior on y { SpringAnimation { duration: 200; spring: 2; damping: 0.1 } }
                }
                delegate: Item {
                    id: selectedNodeDelegate
                    width: ListView.view.width; height: 30;
                    Label { text: "Label: " + itemData.label }
                    MouseArea {
                        anchors.fill: selectedNodeDelegate
                        onClicked: { selectedNodeDelegate.ListView.view.currentIndex = index }
                    }
                }
            }
            RowLayout {
                Layout.margins: 2
                Label { text:"Policy:" }
                Item { Layout.fillWidth: true }
                ColumnLayout {
                    CheckBox {
                        Layout.preferredHeight: 25
                        height: 15
                        autoExclusive: true
                        text: "NoSelection"
                        checked: topology.selectionPolicy === Qan.AbstractGraph.NoSelection
                        onCheckedChanged: {
                            if ( checked )
                                topology.selectionPolicy = Qan.AbstractGraph.NoSelection;
                        }
                    }
                    CheckBox {
                        Layout.preferredHeight: 25
                        height: 15
                        autoExclusive: true
                        text: "SelectOnClick"
                        checked: topology.selectionPolicy === Qan.AbstractGraph.SelectOnClick
                        onCheckedChanged: {
                            if ( checked )
                                topology.selectionPolicy = Qan.AbstractGraph.SelectOnClick;
                        }
                    }
                    CheckBox {
                        Layout.preferredHeight: 25
                        height: 15
                        autoExclusive: true
                        text: "SelectOnCtrlClick"
                        checked: topology.selectionPolicy === Qan.AbstractGraph.SelectOnCtrlClick
                        onCheckedChanged: {
                            if ( checked )
                                topology.selectionPolicy = Qan.AbstractGraph.SelectOnCtrlClick;
                        }
                    }
                }
            }
            RowLayout {
                Layout.margins: 2
                Label { text:"Color:" }
                Item { Layout.fillWidth: true }
                Rectangle { Layout.preferredWidth: 25; Layout.preferredHeight: 25; color: topology.selectionColor; radius: 3; border.width: 1; border.color: Qt.lighter(topology.selectionColor) }
                Button {
                    Layout.preferredHeight: 30; Layout.preferredWidth: 30
                    text: "..."
                    onClicked: {
                        selectionColorDialog.color = topology.selectionColor
                        selectionColorDialog.open();
                    }
                }
            }
            RowLayout {
                Layout.margins: 2
                Label { text:"Weight:" }
                Slider {
                    Layout.preferredHeight: 20
                    Layout.fillWidth: true
                    from: 1.0
                    to: 15.
                    stepSize: 0.1
                    value: topology.selectionWeight
                    onValueChanged: { topology.selectionWeight = value  }
                }
            }
            RowLayout {
                Layout.margins: 2
                Label { text:"Margin:" }
                Slider {
                    Layout.preferredHeight: 20
                    Layout.fillWidth: true
                    from: 1.0
                    to: 15.
                    stepSize: 0.1
                    value: topology.selectionMargin
                    onValueChanged: { topology.selectionMargin = value  }
                }
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
        Frame { anchors.fill: parent; opacity: 0.8; padding: 0; Pane { anchors.fill: parent } } // Background
        ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter
            width: 220
            ProgressBar {
                Layout.fillWidth: true; Layout.fillHeight: false
                value: progressNotifier.progress
                Label {
                    anchors.centerIn: parent
                    text: Math.round( progressNotifier.progress * 100. ) + " %"
                }
            }
            ProgressBar {
                Layout.fillWidth: true; Layout.fillHeight: false
                value: progressNotifier.phaseProgress
                onValueChanged: { indeterminate = value < 0. }
                Label {
                    anchors.centerIn: parent
                    text: Math.round( progressNotifier.phaseProgress * 100. ) + " %"
                }
            }
            Label {
                Layout.alignment: Layout.Center
                text: progressNotifier.phaseLabel
            }
        }
    }
}
