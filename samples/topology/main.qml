/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

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

ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720    // MPEG - 2 HD 720p - 1280 x 720 16:9
    title: "Topology test"
    Menu {
        id: menu
        title: "Main Menu"
        property var targetNode: undefined
        property var targetEdge: undefined
        MenuItem { text: "Clear Graph"; onTriggered: topology.clear() }
        MenuItem {
            text: "Insert Node"
            onTriggered: {
                var n = topology.insertNode()
                centerItem( n.item );
                n.label = "Node #" + topology.getNodeCount()
            }
        }
        MenuItem {
            text: "Insert Face Node"
            // FIXME
            //onTriggered: { openImageDialog.targetGraph = topology; openImageDialog.open() }
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
                centerItem( n.item );
                n.label = "Group #" + topology.getGroupCount()
            }
        }
    }

    Label {
        text: "Right click for main menu:
               \t-Add content with Add Node or Add Face Node entries.
               \t-Use the DnD connector to add edges between nodes."
    }

    function centerItem( item ) {
        if ( !item ||
             !window.contentItem )
            return
        var windowCenter = Qt.point( ( window.contentItem.width - item.width ) / 2.,
                                    ( window.contentItem.height - item.height ) / 2. )
        var graphNodeCenter = window.contentItem.mapToItem( graphView.containerItem,
                                                            windowCenter.x, windowCenter.y )
        item.x = graphNodeCenter.x
        item.y = graphNodeCenter.y
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
            connectorEnabled: true

            property Component faceNodeComponent: Qt.createComponent( "qrc:/FaceNode.qml" )
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
                        property string srcLabel: ""
                        property string dstLabel: ""
                        property var edgeItemData: itemData
                        onEdgeItemDataChanged: {
                            if ( itemData && itemData.item ) {
                                if ( itemData.item.sourceItem &&
                                     itemData.item.sourceItem.node )
                                    srcLabel = itemData.item.sourceItem.node.label
                                if ( itemData.item.destinationItem &&
                                     itemData.item.destinationItem.node )
                                    dstLabel = itemData.item.destinationItem.node.label
                                else if ( itemData.item.destinationItem &&
                                         itemData.item.destinationItem.node )
                                    dstLabel = itemData.item.destinationEdge.edge.label
                            } else {
                                srcLabel = "";
                                dstLabel = "";
                            }
                        }
                        Label { text: "Label: " + itemData.label }
                        Label { text: "  Src: " + parent.srcLabel }
                        Label { text: "  Dst: " + parent.dstLabel }
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
                id: nodesListView
                Layout.fillWidth: true; Layout.fillHeight: true
                clip: true
                model: topology.nodes
                spacing: 4; focus: true; flickableDirection : Flickable.VerticalFlick
                highlightFollowsCurrentItem: false
                highlight: Rectangle {
                    x: 0; y: nodesListView.currentItem.y;
                    width: nodesListView.width; height: nodesListView.currentItem.height
                    color: "lightsteelblue"; opacity: 0.7; radius: 5
                    Behavior on y { SpringAnimation { duration: 200; spring: 2; damping: 0.1 } }
                }
                delegate: Item {
                    id: nodeDelegate
                    width: ListView.view.width; height: 30;
                    Label { id: nodeLabel; text: "Label: " + itemData.label }
                    MouseArea {
                        anchors.fill: nodeDelegate
                        onClicked: { nodesListView.currentIndex = index }
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
                        checked: topology.selectionPolicy === Qan.Graph.NoSelection
                        onCheckedChanged: {
                            if ( checked )
                                topology.selectionPolicy = Qan.Graph.NoSelection;
                        }
                    }
                    CheckBox {
                        Layout.preferredHeight: 25
                        height: 15
                        autoExclusive: true
                        text: "SelectOnClick"
                        checked: topology.selectionPolicy === Qan.Graph.SelectOnClick
                        onCheckedChanged: {
                            if ( checked )
                                topology.selectionPolicy = Qan.Graph.SelectOnClick;
                        }
                    }
                    CheckBox {
                        Layout.preferredHeight: 25
                        height: 15
                        autoExclusive: true
                        text: "SelectOnCtrlClick"
                        checked: topology.selectionPolicy === Qan.Graph.SelectOnCtrlClick
                        onCheckedChanged: {
                            if ( checked )
                                topology.selectionPolicy = Qan.Graph.SelectOnCtrlClick;
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
}
