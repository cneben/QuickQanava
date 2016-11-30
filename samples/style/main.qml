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

import QtQuick          2.7
import QtQuick.Extras   1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts  1.3
import QtQuick.Dialogs  1.2

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "qrc:/QuickProperties" as Qps

Item {
    anchors.fill: parent

    /*Qan.Edge {
        id: edge
        anchors.centerIn: parent; width: 150; height: 55
        label: "Qan.Edge"
        onWidthChanged: {
            edge.p1 = Qt.point( 0, 0 )
            edge.p2 = Qt.point( 35, 10 )
            //edge.p1 = Qt.point( 10, previewHeight / 2 );
            //edge.p2 = Qt.point( width - 10, previewHeight / 2 );
        }
        //style: styleProperties
        //Drag.active: mouseArea.drag.active
        //Drag.dragType: Drag.Automatic; Drag.hotSpot.x: 10; Drag.hotSpot.y: 10
        acceptDrops: false // Don't allow style DnD inside style browser
    }*/

    Popup {
        id: styleSelectionDialog
        x: ( parent.width - width ) / 2.
        y: ( parent.height - height) / 2.
        width: 350; height: 300
        visible: false
        modal: true
        property var targetNode: undefined
        property var styleToApply: undefined
        ColumnLayout {
            anchors.fill: parent; anchors.margins: 4
            Label {
                text: "Select Node Style:"
                font.bold: true
            }
            Qan.StyleListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                id: nodeStyleList
                graph: graph
                model: graph.styleManager.nodeStylesModel
                onStyleDoubleClicked: { styleSelectionDialog.styleToApply = style }
                onStyleClicked: { styleSelectionDialog.styleToApply = style }
            }
            Button {
                Layout.alignment: Qt.AlignRight
                text: "Apply"
                enabled: styleSelectionDialog.styleToApply !== undefined
                onClicked: {
                    if ( styleSelectionDialog.styleToApply &&
                            styleSelectionDialog.targetNode )
                        styleSelectionDialog.targetNode.style = styleSelectionDialog.styleToApply
                    styleSelectionDialog.targetNode = undefined
                    styleSelectionDialog.styleToApply = undefined
                    styleSelectionDialog.close()
                }
            }
        }
    } // Popup: styleSelectionDialog
    Qan.ProgressNotifier { // Empty progress notifier
        id: progressNotifier
        onShowProgress: { }
        onHideProgress: { }
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
        closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
        property var targetNode: undefined
        property var targetEdge: undefined
        onClosed: { targetNode = undefined; targetEdge = undefined }
        MenuItem {
            text: "Load"
            onTriggered: { openGraphDialog.open() }
        }
        MenuItem {
            text: "Save As"
            onTriggered: { saveGraphAsDialog.open() }
        }
        MenuItem {
            text: "Clear all styles"
            onTriggered: { graph.styleManager.clear(); }
        }
        MenuItem {
            text: "Add new style"
            onTriggered: {  }
        }
        MenuItem {
            text: "Select node style"
            enabled: menu.targetNode !== undefined &&
                     menu.targetNode !== null
            onTriggered: {
                styleSelectionDialog.targetNode = menu.targetNode
                styleSelectionDialog.open()
            }
        }
        MenuItem {
            text: "Reset node style"
            enabled: menu.targetNode !== undefined && menu.targetNode !== null
            onTriggered: { menu.targetNode.style = null }
        }
        MenuItem {
            text: "Select edge style"
            enabled: menu.targetEdge !== undefined && menu.targetEdge !== null
            onTriggered: { styleSelectionDialog.open() }
        }
        MenuItem {
            text: "Reset edge style"
            enabled: menu.targetEdge !== undefined && menu.targetEdge !== null
            onTriggered: {  }
        }
    }
    Qan.GraphView {
        id: graphViewconfiguration
        anchors.fill: parent
        //graph       : graph
        navigable   : true
        graph       : Qan.Graph {
            id: graph
            objectName: "graph"
            enableConnectorDropNode: true
            Component.onCompleted: {
                var n1 = graph.insertNode()
                n1.label = "N1"
                var n2 = graph.insertNode()
                n2.label = "N2"
                var n3 = graph.insertNode()
                n3.label = "N3"
                graph.insertEdge( n1, n2 )
                graph.insertEdge( n2, n3 )
            }
            onNodeRightClicked: {
                menu.targetNode = node; menu.targetEdge = undefined
                var globalPos = node.mapToItem( graph, pos.x, pos.y )
                menu.x = globalPos.x; menu.y = globalPos.y
                menu.open()
            }
        } // Qan.Graph: graph
        onRightClicked: {
            menu.targetNode = undefined; menu.targetEdge = undefined
            var globalPos = mapToItem( graph, pos.x, pos.y )
            menu.x = globalPos.x; menu.y = globalPos.y
            menu.open()
        }
    }

    Pane {
        id: styleBrowser
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: parent.right; anchors.rightMargin: 15
        width: 280; height: 350
        Qan.StyleListView {
            anchors.fill: parent
            anchors.margins: 4
            model: graph.styleManager
            graph: graph
            onStyleClicked: { }
            onStyleDoubleClicked: { styleEditor.hilightStyle( style ) }
        }
    } // Frame: styleBrowser
    Pane {
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: styleBrowser.left; anchors.rightMargin: 15
        width: 280; height: 350
        ColumnLayout {
            id: layout
            anchors.fill: parent; /*anchors.margins: 4*/
            ComboBox {
                id: styleCb
                Layout.fillWidth: true
                displayText: "Nodes"
                model: graph.styleManager
                textRole: "itemLabel"
            }
            Qps.PropertiesEditor {
                id: styleEditor
                Layout.fillWidth: true; Layout.fillHeight: true
                model: graph.styleManager.getStyleAt( styleCb.currentIndex )
                flickableDirection: Flickable.HorizontalAndVerticalFlick
                function    hilightStyle( style ) { // Change selection to a given style and hilight it
                    styleCb.currentIndex = -1
                    styleCb.currentIndex = graph.styleManager.listReference.itemIndex( style )
                }
            }
        }
    } // Frame: styleEditor
}
