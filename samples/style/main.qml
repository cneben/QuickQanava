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
import "qrc:/QuickProperties" as Qps

Item {
    anchors.fill: parent
    property alias nodeDelegate : graph.nodeDelegate

    Dialog {
        id: styleSelectionDialog
        visible: false
        title: "Select Node Style"
        property var targetNode: undefined
        property var styleToApply: undefined
        contentItem: Item {
            implicitWidth: 350; implicitHeight: 300
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 4
                Qan.StyleListView {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    id: nodeStyleList
                    model: graph.styleManager.nodeStylesModel
                    onStyleDoubleClicked: { styleSelectionDialog.styleToApply = style }
                    onStyleClicked: { styleSelectionDialog.styleToApply = style }
                }
                Button {
                    Layout.alignment: Qt.AlignRight
                    text: "Apply"
                    enabled: styleSelectionDialog.styleToApply != undefined
                    onClicked: {
                        if ( styleSelectionDialog.styleToApply != undefined &&
                             styleSelectionDialog.targetNode != undefined )
                                styleSelectionDialog.targetNode.style = styleSelectionDialog.styleToApply
                        styleSelectionDialog.targetNode = undefined
                        styleSelectionDialog.styleToApply = undefined
                        styleSelectionDialog.close()
                    }
                }
            }
        }
    } // Dialog: styleSelectionDialog
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
        property var targetNode: undefined
        property var targetEdge: undefined
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
            enabled: menu.targetNode != undefined
            onTriggered: { styleSelectionDialog.targetNode = menu.targetNode; styleSelectionDialog.open() }
        }
        MenuItem {
            text: "Reset node style"
            enabled: menu.targetNode != undefined
            onTriggered: { menu.targetNode.style = null }
        }
        MenuItem {
            text: "Select edge style"
            enabled: menu.targetEdge != undefined
            onTriggered: { styleSelectionDialog.open() }
        }
        MenuItem {
            text: "Reset edge style"
            enabled: menu.targetEdge != undefined
            onTriggered: {  }
        }
    }

    Qan.Graph {
        id: graph
        objectName: "graph"
        anchors.fill: parent
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
            console.debug( "hasEdge(n1,n2)=" + graph.hasEdge( n1, n2 ) )
        }
        onNodeRightClicked: { menu.targetNode = node; menu.popup() }
        onRightClicked: { menu.targetNode = undefined; menu.targetEdge = undefined; menu.popup() }
    } // Qan.Graph: graph


    Rectangle {
        id: styleBrowser
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: parent.right; anchors.rightMargin: 15
        width: 250; height: 350
        color: "lightblue"; border.color: "black"; border.width: 2
        Qan.StyleListView {
            anchors.fill: parent
            anchors.margins: 4
            model: graph.styleManager
            onStyleClicked: { }
            onStyleDoubleClicked: { styleEditorDialog.hilightStyle( style ) }
        }
    } // Rectangle: styleEditor

    Rectangle {
        id: styleEditorDialog
        anchors.top: parent.top;     anchors.topMargin: 15
        anchors.right: styleBrowser.left; anchors.rightMargin: 15
        width: 250; height: 350
        color: "lightblue"; border.color: "black"; border.width: 2

        function    hilightStyle( style ) { // Change selection to a given style and hilight it
            styleSelectionCb.currentIndex = -1
            styleSelectionCb.currentIndex = graph.styleManager.itemIndex( style )
        }
        ColumnLayout {
            id: layout
            anchors.fill: parent; anchors.margins: 4
            ComboBox {
                id: styleSelectionCb
                Layout.fillWidth: true
                editText: "Nodes"
                model: graph.styleManager
                textRole: "itemLabel"
                onCurrentIndexChanged: {
                    if ( currentIndex >= 0 )
                        styleEditor.properties = graph.styleManager.getStyleAt( currentIndex )
                }
            }
            Qps.PropertiesEditor {
                id: styleEditor
                Layout.fillWidth: true; Layout.fillHeight: true
            }
        }
    } // Rectangle: styleEditor
}
