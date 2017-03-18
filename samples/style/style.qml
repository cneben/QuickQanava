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

import QtQuick          2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts  1.3
import QtQuick.Dialogs  1.2

import QuickQanava          2.0 as Qan
import "."                  as Qan
import "qrc:/QuickQanava"   as Qan

ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720
    title: "Style sample"
    Pane { anchors.fill: parent }

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
            /*Label {
                text: "Select Node Style:"
                font.bold: true
            }
            Qan.StyleListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                id: nodeStyleList
                graph: graph
                model: graph.styleManager.styles
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
            }*/
        }
    } // Popup: styleSelectionDialog
    Menu {
        id: menu
        title: "Main Menu"
        closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
        property var targetNode: undefined
        property var targetEdge: undefined
        onClosed: { targetNode = undefined; targetEdge = undefined }
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
        navigable   : true
        graph       : Qan.Graph {
            id: graph
            objectName: "graph"
            connectorEnabled: true
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
                /*menu.targetNode = node; menu.targetEdge = undefined
                var globalPos = node.mapToItem( graph, pos.x, pos.y )
                menu.x = globalPos.x; menu.y = globalPos.y
                menu.open()*/
            }
        } // Qan.Graph: graph
        onRightClicked: {
            /*menu.targetNode = undefined; menu.targetEdge = undefined
            var globalPos = mapToItem( graph, pos.x, pos.y )
            menu.x = globalPos.x; menu.y = globalPos.y
            menu.open()*/
        }
    }
    Frame {
        id: styleBrowser
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: parent.right; anchors.rightMargin: 15
        width: 280; height: 350
        Qan.StyleListView {
            anchors.fill: parent
            anchors.margins: 4
            styleManager: graph.styleManager
            graph: graph
            onStyleClicked: { }
            //onStyleDoubleClicked: { styleEditor.hilightStyle( style ) }
        }
    } // Frame: styleBrowser
    Frame {
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: styleBrowser.left; anchors.rightMargin: 15
        width: 280; height: 350
        /*ColumnLayout {
            id: layout
            anchors.fill: parent
            ComboBox {
                id: styleCb
                Layout.fillWidth: true
                displayText: "Select a Style"
                model: graph.styleManager.styles
                textRole: "itemLabel"
            }
            Qan.StyleEditor {
                id: styleEditor
                Layout.fillWidth: true; Layout.fillHeight: true
                style: graph.styleManager.getStyleAt( styleCb.currentIndex )
                flickableDirection: Flickable.HorizontalAndVerticalFlick
                function    hilightStyle( style ) { // Change selection to a given style and hilight it
                    styleCb.currentIndex = -1
                    styleCb.currentIndex = graph.styleManager.styles.listReference.itemIndex( style )
                }
            }
        }*/
    } // Frame: styleEditor
}
