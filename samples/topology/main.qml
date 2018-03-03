/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.1

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan

ApplicationWindow {
    id: window
    visible: true
    width: 1280
    height: 720 // MPEG - 2 HD 720p - 1280 x 720 16:9
    title: "Topology test"
    Pane {
        anchors.fill: parent
        padding: 0
    }
    Menu {
        id: menu
        title: "Main Menu"
        property var targetNode: undefined
        property var targetEdge: undefined
        MenuItem {
            text: "Clear Graph"
            onTriggered: topology.clear()
        }
        MenuItem {
            text: "Insert Node"
            onTriggered: {
                var n = topology.insertNode()
                centerItem(n.item)
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
                if (menu.targetNode !== undefined)
                    topology.removeNode(menu.targetNode)
                menu.targetNode = undefined
            }
        }
        MenuItem {
            text: "Remove edge"
            enabled: menu.targetEdge !== undefined
            onTriggered: {
                if (menu.targetEdge !== undefined)
                    topology.removeEdge(menu.targetEdge)
                menu.targetEdge = undefined
            }
        }
        MenuItem {
            text: "Insert Group"
            onTriggered: {
                var n = topology.insertGroup()
                centerItem(n.item)
                n.label = "Group #" + topology.getGroupCount()
            }
        }
        MenuItem {
            text: "Add Left port"
            enabled: menu.targetNode !== undefined
            onTriggered: {
                var inPort = topology.insertPort(menu.targetNode,
                                                 Qan.NodeItem.Left)
                inPort.label = "LPORT"
            }
        }
        MenuItem {
            text: "Add Top port"
            enabled: menu.targetNode !== undefined
            onTriggered: topology.insertPort(menu.targetNode,
                                             Qan.NodeItem.Top, "IN")
        }
        MenuItem {
            text: "Add Right port"
            enabled: menu.targetNode !== undefined
            onTriggered: topology.insertPort(menu.targetNode,
                                             Qan.NodeItem.Right, "RPORT")
        }
        MenuItem {
            text: "Add Bottom port"
            enabled: menu.targetNode !== undefined
            onTriggered: topology.insertPort(menu.targetNode,
                                             Qan.NodeItem.Bottom, "IN")
        }
    }

    Menu {
        id: menuRmPort
        title: "Port Menu"
        property var targetPort: undefined

        MenuItem {
            text: "Remove port"
            enabled: menuRmPort.targetPort !== undefined
            onTriggered: {
                if (menuRmPort.targetPort !== undefined)
                    topology.removePort(menuRmPort.targetPort.node,
                                        menuRmPort.targetPort)
                menuRmPort.targetPort = undefined
            }
        }
    }

    function centerItem(item) {
        if (!item || !window.contentItem)
            return
        var windowCenter = Qt.point(
                    (window.contentItem.width - item.width) / 2.,
                    (window.contentItem.height - item.height) / 2.)
        var graphNodeCenter = window.contentItem.mapToItem(
                    graphView.containerItem, windowCenter.x, windowCenter.y)
        item.x = graphNodeCenter.x
        item.y = graphNodeCenter.y
    }

    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        graph: topology
        navigable: true
        resizeHandlerColor: Material.accent
        gridThickColor: Material.theme === Material.Dark ? "#4e4e4e" : "#c1c1c1"

        Qan.FaceGraph {
            id: topology
            objectName: "graph"
            anchors.fill: parent
            clip: true
            connectorEnabled: true
            selectionColor: Material.accent
            connectorColor: Material.accent
            connectorEdgeColor: Material.accent
            onConnectorEdgeInserted: {
                if (edge)
                    edge.label = "My edge"
            }
            property Component faceNodeComponent: Qt.createComponent(
                                                      "qrc:/FaceNode.qml")
            onNodeClicked: {
                portsListView.model = node.item.ports
            }
            onNodeRightClicked: {
                var globalPos = node.item.mapToItem(topology, pos.x, pos.y)
                menu.x = globalPos.x
                menu.y = globalPos.y
                menu.targetNode = node
                menu.open()
            }
            onEdgeRightClicked: {
                if (!edge)
                    return
                var globalPos = edge.mapToItem(topology, pos.x, pos.y)
                menu.x = globalPos.x
                menu.y = globalPos.y
                menu.targetEdge = edge
                menu.open()
            }
            onPortRightClicked: {
                var globalPos = port.parent.mapToItem(topology, pos.x, pos.y)
                menuRmPort.x = globalPos.x
                menuRmPort.y = globalPos.y
                menuRmPort.targetPort = port
                menuRmPort.open()
            }

            Component.onCompleted: {
                defaultEdgeStyle.lineWidth = 3
                defaultEdgeStyle.lineColor = Qt.binding(function () {
                    return Material.foreground
                })
                defaultNodeStyle.shadowColor = Qt.binding(function () {
                    return Material.theme === Material.Dark ? Qt.darker(
                                                                  Material.foreground) : Qt.darker(
                                                                  Material.foreground)
                })
                defaultNodeStyle.backColor = Qt.binding(function () {
                    return Material.theme === Material.Dark ? Qt.lighter(
                                                                  Material.background) : Qt.lighter(
                                                                  Material.background)
                })
                defaultGroupStyle.backColor = Qt.binding(function () {
                    return Material.theme === Material.Dark ? Qt.lighter(
                                                                  Material.background,
                                                                  1.3) : Qt.darker(
                                                                  Material.background,
                                                                  1.1)
                })
                var bw1 = topology.insertFaceNode()
                bw1.image = "qrc:/faces/BW1.jpg"
                bw1.item.x = 150
                bw1.item.y = 55
                var bw1p1 = topology.insertPort(bw1, Qan.NodeItem.Right)
                bw1p1.label = "P#1"
                var bw1p2 = topology.insertPort(bw1, Qan.NodeItem.Bottom)
                bw1p2.label = "P#2"
                var bw1p3 = topology.insertPort(bw1, Qan.NodeItem.Bottom)
                bw1p3.label = "P#3"

                var bw2 = topology.insertFaceNode()
                bw2.image = "qrc:/faces/BW2.jpg"
                bw2.item.x = 45
                bw2.item.y = 250
                var bw2p1 = topology.insertPort(bw2, Qan.NodeItem.Top)
                bw2p1.label = "P#1"

                var bw3 = topology.insertFaceNode()
                bw3.image = "qrc:/faces/BW3.jpg"
                bw3.item.x = 250
                bw3.item.y = 250
                var bw3p1 = topology.insertPort(bw3, Qan.NodeItem.Top)
                bw3p1.label = "P#1"

                var js1 = topology.insertFaceNode()
                js1.image = "qrc:/faces/JS1.jpg"
                js1.item.x = 500
                js1.item.y = 55
                var js1p1 = topology.insertPort(js1, Qan.NodeItem.Left)
                js1p1.label = "P#1"
                var js1p2 = topology.insertPort(js1, Qan.NodeItem.Bottom)
                js1p2.label = "P#2"
                var js1p3 = topology.insertPort(js1, Qan.NodeItem.Bottom)
                js1p3.label = "P#3"
                var js1p4 = topology.insertPort(js1, Qan.NodeItem.Top)
                js1p4.label = "P#4"

                var js2 = topology.insertFaceNode()
                js2.image = "qrc:/faces/JS2.jpg"
                js2.item.x = 500
                js2.item.y = -155
                var js2p1 = topology.insertPort(js2, Qan.NodeItem.Bottom)
                js2p1.label = "P#1"

                var vd1 = topology.insertFaceNode()
                vd1.image = "qrc:/faces/VD1.jpg"
                vd1.item.x = 400
                vd1.item.y = 350
                var vd1p1 = topology.insertPort(vd1, Qan.NodeItem.Top)
                vd1p1.label = "P#1"
                var vd1p2 = topology.insertPort(vd1, Qan.NodeItem.Bottom)
                vd1p2.label = "P#2"
                var vd1p3 = topology.insertPort(vd1, Qan.NodeItem.Bottom)
                vd1p3.label = "P#3"

                var vd2 = topology.insertFaceNode()
                vd2.image = "qrc:/faces/VD2.jpg"
                vd2.item.x = 200
                vd2.item.y = 600
                var vd2p1 = topology.insertPort(vd2, Qan.NodeItem.Top)
                vd2p1.label = "P#1"

                var vd3 = topology.insertFaceNode()
                vd3.image = "qrc:/faces/VD3.jpg"
                vd3.item.x = 400
                vd3.item.y = 600
                var vd3p1 = topology.insertPort(vd3, Qan.NodeItem.Top)
                vd3p1.label = "P#1"

                var dd1 = topology.insertFaceNode()
                dd1.image = "qrc:/faces/DD1.jpg"
                dd1.item.x = 650
                dd1.item.y = 350
                var dd1p1 = topology.insertPort(dd1, Qan.NodeItem.Top)
                dd1p1.label = "P#1"
                var dd1p2 = topology.insertPort(dd1, Qan.NodeItem.Bottom)
                dd1p2.label = "P#2"
                var dd1p3 = topology.insertPort(dd1, Qan.NodeItem.Bottom)
                dd1p3.label = "P#3"

                var dd2 = topology.insertFaceNode()
                dd2.image = "qrc:/faces/DD2.jpg"
                dd2.item.x = 650
                dd2.item.y = 600
                var dd2p1 = topology.insertPort(dd2, Qan.NodeItem.Top)
                dd2p1.label = "P#1"

                var dd3 = topology.insertFaceNode()
                dd3.image = "qrc:/faces/DD3.jpg"
                dd3.item.x = 800
                dd3.item.y = 600
                var dd3p1 = topology.insertPort(dd3, Qan.NodeItem.Top)
                dd3p1.label = "P#1"

                // Generate random connections
                var e = topology.insertEdge(bw1, js1)
                topology.bindEdgeSource(e, bw1p1)
                topology.bindEdgeDestination(e, js1p1)

                e = topology.insertEdge(bw2, bw1)
                topology.bindEdgeSource(e, bw2p1)
                topology.bindEdgeDestination(e, bw1p2)
                e = topology.insertEdge(bw3, bw1)
                topology.bindEdgeSource(e, bw3p1)
                topology.bindEdgeDestination(e, bw1p3)

                e = topology.insertEdge(js1, js2)
                topology.bindEdgeSource(e, js1p4)
                topology.bindEdgeDestination(e, js2p1)

                e = topology.insertEdge(js1, vd1)
                topology.bindEdgeSource(e, js1p2)
                topology.bindEdgeDestination(e, vd1p1)

                e = topology.insertEdge(js1, dd1)
                topology.bindEdgeSource(e, js1p3)
                topology.bindEdgeDestination(e, dd1p1)

                e = topology.insertEdge(dd2, dd1)
                topology.bindEdgeSource(e, dd2p1)
                topology.bindEdgeDestination(e, dd1p2)

                e = topology.insertEdge(dd3, dd1)
                topology.bindEdgeSource(e, dd3p1)
                topology.bindEdgeDestination(e, dd1p3)

                e = topology.insertEdge(vd2, vd1)
                topology.bindEdgeSource(e, vd2p1)
                topology.bindEdgeDestination(e, vd1p2)

                e = topology.insertEdge(vd3, vd1)
                topology.bindEdgeSource(e, vd3p1)
                topology.bindEdgeDestination(e, vd1p3)
            }
        } // Qan.Graph: graph
        onRightClicked: {
            var globalPos = graphView.mapToItem(topology, pos.x, pos.y)
            menu.x = globalPos.x
            menu.y = globalPos.y
            menu.targetNode = undefined
            menu.targetEdge = undefined
            menu.open()
        }
    }
    Label {
        text: "Right click for main menu:
\t- Add content with Add Node or Add Face Node entries.
\t- Use the DnD connector to add edges between nodes."
    }
    Item {
        id: edgeList
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 15
        width: 200
        height: 300
        Frame {
            anchors.fill: parent
            opacity: 0.8
            padding: 0
            Pane {
                anchors.fill: parent
            }
        } // Background
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            Label {
                Layout.margins: 3
                text: "Edges:"
                font.bold: true
                horizontalAlignment: Text.AlignLeft
            }
            ListView {
                id: edgesList
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: topology.edges
                spacing: 4
                focus: true
                flickableDirection: Flickable.VerticalFlick
                highlight: Rectangle {
                    x: 0
                    y: (edgesList.currentItem != null ? edgesList.currentItem.y : 0)
                    width: edgesList.width
                    height: (edgesList.currentItem != null ? edgesList.currentItem.height : 100)
                    color: Material.accent
                    opacity: 0.7
                    radius: 3
                    Behavior on y {
                        SpringAnimation {
                            duration: 200
                            spring: 2
                            damping: 0.1
                        }
                    }
                }
                delegate: Item {
                    id: edgeDelegate
                    width: ListView.view.width
                    height: edgeLayout.implicitHeight
                    ColumnLayout {
                        anchors.fill: parent
                        id: edgeLayout
                        property string srcLabel: ""
                        property string dstLabel: ""
                        property var edgeItemData: itemData
                        onEdgeItemDataChanged: {
                            if (itemData && itemData.item) {
                                if (itemData.item.sourceItem
                                        && itemData.item.sourceItem.node)
                                    srcLabel = itemData.item.sourceItem.node.label
                                if (itemData.item.destinationItem
                                        && itemData.item.destinationItem.node)
                                    dstLabel = itemData.item.destinationItem.node.label
                                else if (itemData.item.destinationItem
                                         && itemData.item.destinationItem.node)
                                    dstLabel = itemData.item.destinationEdge.edge.label
                            } else {
                                srcLabel = ""
                                dstLabel = ""
                            }
                        }
                        Label {
                            text: "Label: " + itemData.label
                        }
                        Label {
                            text: "  Src: " + parent.srcLabel
                        }
                        Label {
                            text: "  Dst: " + parent.dstLabel
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            edgeDelegate.ListView.view.currentIndex = index
                        }
                    }
                }
            }
            ComboBox {
                Layout.fillWidth: true
                Layout.fillHeight: false
                model: topology.edges
                textRole: "itemLabel"
            }
        }
    } // edgeList

    Item {
        id: nodeList
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.right: portList.left
        anchors.rightMargin: 15
        width: 200
        height: 300
        Frame {
            anchors.fill: parent
            opacity: 0.8
            padding: 0
            Pane {
                anchors.fill: parent
            }
        } // Background
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            Label {
                Layout.margins: 3
                text: "Nodes:"
                font.bold: true
                horizontalAlignment: Text.AlignLeft
            }
            ListView {
                id: nodesListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: topology.nodes
                spacing: 4
                focus: true
                flickableDirection: Flickable.VerticalFlick
                highlightFollowsCurrentItem: false
                highlight: Rectangle {
                    x: 0
                    y: nodesListView.currentItem.y
                    width: nodesListView.width
                    height: nodesListView.currentItem.height
                    color: Material.accent
                    opacity: 0.7
                    radius: 3
                    Behavior on y {
                        SpringAnimation {
                            duration: 200
                            spring: 2
                            damping: 0.1
                        }
                    }
                }
                delegate: Item {
                    id: nodeDelegate
                    width: ListView.view.width
                    height: 30
                    Label {
                        id: nodeLabel
                        text: "Label: " + itemData.label
                    }
                    MouseArea {
                        anchors.fill: nodeDelegate
                        onClicked: {
                            nodesListView.currentIndex = index
                        }
                    }
                }
            }
            ComboBox {
                Layout.fillWidth: true
                Layout.fillHeight: false
                model: topology.nodes
                textRole: "itemLabel"
            }
        }
    } // nodeList

    Item {
        id: portList
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.right: edgeList.left
        anchors.rightMargin: 15
        width: 200
        height: 300
        Frame {
            anchors.fill: parent
            opacity: 0.8
            padding: 0
            Pane {
                anchors.fill: parent
            }
        } // Background
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            Label {
                Layout.margins: 3
                text: "Selected Node's Ports:"
                font.bold: true
                horizontalAlignment: Text.AlignLeft
            }
            ListView {
                id: portsListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                spacing: 4
                focus: true
                flickableDirection: Flickable.VerticalFlick
                highlightFollowsCurrentItem: false
                highlight: Rectangle {
                    x: 0
                    y: portsListView.currentItem.y
                    width: portsListView.width
                    height: portsListView.currentItem.height
                    color: Material.accent
                    opacity: 0.7
                    radius: 3
                    Behavior on y {
                        SpringAnimation {
                            duration: 200
                            spring: 2
                            damping: 0.1
                        }
                    }
                }
                delegate: Item {
                    id: portDelegate
                    width: ListView.view.width
                    height: 30
                    Label {
                        id: portLabel
                        text: "Label: " + itemData.label
                    }
                    MouseArea {
                        anchors.fill: portDelegate
                        onClicked: {
                            portsListView.currentIndex = index
                        }
                    }
                }
            }
        }
    } // portList

    Button {
        id: autoPosButton
        anchors.top: portList.bottom
        anchors.topMargin: 15
        anchors.left: portList.left
        width: 110
        height: 50
        text: "AutoPos"

        onClicked: topology.autoPositionNodes()
    }

    ColorDialog {
        id: selectionColorDialog
        title: "Selection hilight color"
        onAccepted: {
            topology.selectionColor = color
        }
    }
    Item {
        id: selectionView
        anchors.top: nodeList.bottom
        anchors.topMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 15
        width: 250
        height: 280
        Frame {
            anchors.fill: parent
            opacity: 0.8
            padding: 0
            Pane {
                anchors.fill: parent
            }
        } // Background
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            Label {
                Layout.margins: 3
                text: "Selection:"
                font.bold: true
                horizontalAlignment: Text.AlignLeft
            }
            ListView {
                id: selectionListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: topology.selectedNodes
                spacing: 4
                focus: true
                flickableDirection: Flickable.VerticalFlick
                highlightFollowsCurrentItem: false
                highlight: Rectangle {
                    x: 0
                    y: (selectionListView.currentItem
                        !== null ? selectionListView.currentItem.y : 0)
                    width: selectionListView.width
                    height: selectionListView.currentItem.height
                    color: Material.accent
                    opacity: 0.7
                    radius: 3
                    Behavior on y {
                        SpringAnimation {
                            duration: 200
                            spring: 2
                            damping: 0.1
                        }
                    }
                }
                delegate: Item {
                    id: selectedNodeDelegate
                    width: ListView.view.width
                    height: 30
                    Label {
                        text: "Label: " + itemData.label
                    }
                    MouseArea {
                        anchors.fill: selectedNodeDelegate
                        onClicked: {
                            selectedNodeDelegate.ListView.view.currentIndex = index
                        }
                    }
                }
            }
            RowLayout {
                Layout.margins: 2
                Label {
                    text: "Policy:"
                }
                Item {
                    Layout.fillWidth: true
                }
                ColumnLayout {
                    CheckBox {
                        Layout.preferredHeight: 25
                        height: 15
                        autoExclusive: true
                        text: "NoSelection"
                        checked: topology.selectionPolicy === Qan.Graph.NoSelection
                        onCheckedChanged: {
                            if (checked)
                                topology.selectionPolicy = Qan.Graph.NoSelection
                        }
                    }
                    CheckBox {
                        Layout.preferredHeight: 25
                        height: 15
                        autoExclusive: true
                        text: "SelectOnClick"
                        checked: topology.selectionPolicy === Qan.Graph.SelectOnClick
                        onCheckedChanged: {
                            if (checked)
                                topology.selectionPolicy = Qan.Graph.SelectOnClick
                        }
                    }
                    CheckBox {
                        Layout.preferredHeight: 25
                        height: 15
                        autoExclusive: true
                        text: "SelectOnCtrlClick"
                        checked: topology.selectionPolicy === Qan.Graph.SelectOnCtrlClick
                        onCheckedChanged: {
                            if (checked)
                                topology.selectionPolicy = Qan.Graph.SelectOnCtrlClick
                        }
                    }
                }
            }
            RowLayout {
                Layout.margins: 2
                Label {
                    text: "Color:"
                }
                Item {
                    Layout.fillWidth: true
                }
                Rectangle {
                    Layout.preferredWidth: 25
                    Layout.preferredHeight: 25
                    color: topology.selectionColor
                    radius: 3
                    border.width: 1
                    border.color: Qt.lighter(topology.selectionColor)
                }
                Button {
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 30
                    text: "..."
                    onClicked: {
                        selectionColorDialog.color = topology.selectionColor
                        selectionColorDialog.open()
                    }
                }
            }
            RowLayout {
                Layout.margins: 2
                Label {
                    text: "Weight:"
                }
                Slider {
                    Layout.preferredHeight: 20
                    Layout.fillWidth: true
                    from: 1.0
                    to: 15.
                    stepSize: 0.1
                    value: topology.selectionWeight
                    onValueChanged: {
                        topology.selectionWeight = value
                    }
                }
            }
            RowLayout {
                Layout.margins: 2
                Label {
                    text: "Margin:"
                }
                Slider {
                    Layout.preferredHeight: 20
                    Layout.fillWidth: true
                    from: 1.0
                    to: 15.
                    stepSize: 0.1
                    value: topology.selectionMargin
                    onValueChanged: {
                        topology.selectionMargin = value
                    }
                }
            }
        }
    } // selectionView
    RowLayout {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        CheckBox {
            text: qsTr("Dark")
            checked: ApplicationWindow.contentItem.Material.theme === Material.Dark
            onClicked: ApplicationWindow.contentItem.Material.theme
                       = checked ? Material.Dark : Material.Light
        }
        RowLayout {
            Layout.margins: 2
            Label {
                text: "Edge type:"
            }
            Item {
                Layout.fillWidth: true
            }
            ComboBox {
                model: ["Straight", "Curved"]
                enabled: defaultEdgeStyle !== undefined
                currentIndex: defaultEdgeStyle.lineType === Qan.EdgeStyle.Straight ? 0 : 1
                onActivated: {
                    if (index == 0)
                        defaultEdgeStyle.lineType = Qan.EdgeStyle.Straight
                    else if (index == 1)
                        defaultEdgeStyle.lineType = Qan.EdgeStyle.Curved
                }
            }
        } // RowLayout: edgeType
    }
}
