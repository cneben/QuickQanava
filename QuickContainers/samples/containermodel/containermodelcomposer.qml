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

import QtQuick          2.7
import QtQuick.Controls          2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts  1.3

import QuickProperties          2.0 as Qps
import "qrc:/QuickProperties"       as Qps

import ContainerModelSample 1.0

Item {
    id: mainVindow
    anchors.fill: parent

    Component {
        id: dummyComponent
        Dummy { }
    }

    Pane {
        id: paneDummies1
        anchors.top: parent.top; anchors.left: parent.left
        anchors.leftMargin: 25; anchors.topMargin: 15
        width: 250; height: 300
        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            Text {  Layout.maximumWidth: parent.width; font.bold: true; wrapMode: Text.WrapAnywhere
                    text:"qps::ContainerModel<QVector,Dummy*>" }
            RowLayout {
                Button {
                    text:"+"
                    onClicked: {
                        var dummy = dummyComponent.createObject();
                        dummy.label = "New Label #" + dummies1.itemCount
                        dummy.number = dummies1.itemCount + 42
                        dummies1.listReference.append(dummy);
                    }
                }
                Button {
                    text:"-"
                    onClicked: { dummies1.listReference.remove(dummies1.listReference.itemAt(dummies1List.currentIndex)) }
                }
            }
            Text { text:"Item count=" + dummies1.itemCount; font.bold:true }
            ListView {
                id: dummies1List
                Layout.fillWidth: true; Layout.fillHeight: true
                highlight: Rectangle {
                    visible: ListView.view.currentItem !== null && ListView.view.currentItem !== undefined
                    width: ListView.view.currentItem.width;     height: ListView.view.currentItem.height
                    x: ListView.view.currentItem.x;             y: ListView.view.currentItem.y
                    color: Material.accent; opacity: 0.3; radius: 4
                    Behavior on x { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
                    Behavior on y { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
                }
                highlightFollowsCurrentItem: false
                model: dummies1
                delegate: Item {
                    width: ListView.view.width; height: 35
                    MouseArea { anchors.fill: parent; onClicked: dummies1List.currentIndex = index; }
                    Text {
                        anchors.fill: parent;
                        text: itemData.label + " " + itemData.number
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    } // Pane m1

    Pane {
        id: paneDummies2
        anchors.top: parent.top; anchors.left: paneDummies1.right
        anchors.leftMargin: 25; anchors.topMargin: 15
        width: 250; height: 300
        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            Text {  Layout.maximumWidth: parent.width; font.bold: true; wrapMode: Text.WrapAnywhere
                    text:"qps::ContainerModel<QVector,Dummy*>" }
            RowLayout {
                Button {
                    text:"+"
                    onClicked: {
                        var dummy = dummyComponent.createObject();
                        dummy.label = "New Label #" + dummies2.itemCount
                        dummy.number = dummies2.itemCount + 42
                        dummies2.listReference.append(dummy);
                    }
                }
                Button {
                    text:"-"
                    onClicked: { dummies2.listReference.remove(dummies2.listReference.itemAt(dummies2List.currentIndex)) }
                }
            }
            Text { text:"Item count=" + dummies2.itemCount; font.bold:true }
            ListView {
                id: dummies2List
                Layout.fillWidth: true; Layout.fillHeight: true
                model: dummies2
                highlight: Rectangle {
                    visible: ListView.view.currentItem !== null && ListView.view.currentItem !== undefined
                    width: ListView.view.currentItem.width; height: ListView.view.currentItem.height
                    x: ListView.view.currentItem.x; y: ListView.view.currentItem.y
                    color: Material.accent; opacity: 0.3; radius: 4
                    Behavior on x { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
                    Behavior on y { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
                }
                highlightFollowsCurrentItem: false
                delegate: Item {
                    width: ListView.view.width; height: 35
                    MouseArea { anchors.fill: parent; onClicked: dummies2List.currentIndex = index }
                    Text {
                        anchors.fill: parent;
                        text: itemData.label + " " + itemData.number
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    } // Pane m2

    Pane {
        id: paneDummies12
        anchors.top: parent.top; anchors.left: paneDummies2.right
        anchors.leftMargin: 25; anchors.topMargin: 15
        width: 300; height: 300
        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            Text {  Layout.maximumWidth: parent.width; font.bold: true; wrapMode: Text.WrapAnywhere
                    text:"qps::ContainerModelComposer(m1, m2)" }
            Text { text:"Item count=" + dummies12.itemCount; font.bold:true }
            ListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                model: dummies12
                delegate: Text { text: itemData.label + " " + itemData.number }
            }
        }
    } // Pane m1m2


    //--------------------------------------------------------------------
    // HETEROGENEOUS COMPOSED MODELS TEST
    //--------------------------------------------------------------------

    Pane {
        id: paneM1
        anchors.top: paneDummies2.bottom; anchors.left: parent.left
        anchors.leftMargin: 25; anchors.topMargin: 15
        width: 250; height: 300
        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            Text {  Layout.maximumWidth: parent.width; font.bold: true; wrapMode: Text.WrapAnywhere
                    text:"qps::ContainerModel<QVector,std::weak_ptr<QA>>" }
            RowLayout {
                Button {
                    text: "Clear"
                    onClicked: {
                        onClicked: { m1.listReference.clear(); }
                    }
                }
                Button {
                    text:"-"
                    onClicked: { m1.listReference.remove(m1.listReference.itemAt(m1List.currentIndex)) }
                }
            }
            Text { text:"Item count=" + m1.itemCount; font.bold:true }
            ListView {
                id: m1List
                Layout.fillWidth: true; Layout.fillHeight: true
                highlight: Rectangle {
                    visible: ListView.view.currentItem !== null && ListView.view.currentItem !== undefined
                    width: ListView.view.currentItem.width;     height: ListView.view.currentItem.height
                    x: ListView.view.currentItem.x;             y: ListView.view.currentItem.y
                    color: Material.accent; opacity: 0.3; radius: 4
                    Behavior on x { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
                    Behavior on y { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
                }
                highlightFollowsCurrentItem: false
                model: m1
                delegate: Item {
                    width: ListView.view.width; height: 35
                    MouseArea { anchors.fill: parent; onClicked: m1List.currentIndex = index; }
                    Text {
                        anchors.fill: parent;
                        text: itemData ? "objectName=" + itemData.objectName + " dummyReal=" + itemData.dummyReal :
                                         "undefined"
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    } // Pane m1

    Pane {
        id: paneM2
        anchors.top: paneM1.top; anchors.left: paneM1.right
        anchors.leftMargin: 25; anchors.topMargin: 0
        width: 250; height: 300
        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            Text {  Layout.maximumWidth: parent.width; font.bold: true; wrapMode: Text.WrapAnywhere
                    text:"qps::ContainerModel<QVector,,std::weak_ptr<QA>>" }
            RowLayout {
                Button {
                    text:"Clear"
                    onClicked: { m2.listReference.clear(); }
                }
                Button {
                    text:"-"
                    onClicked: { m2.listReference.remove(m2.listReference.itemAt(m2List.currentIndex)) }
                }
            }
            Text { text:"Item count=" + m2.itemCount; font.bold:true }
            ListView {
                id: m2List
                Layout.fillWidth: true; Layout.fillHeight: true
                model: m2
                highlight: Rectangle {
                    visible: ListView.view.currentItem !== null && ListView.view.currentItem !== undefined
                    width: ListView.view.currentItem.width; height: ListView.view.currentItem.height
                    x: ListView.view.currentItem.x; y: ListView.view.currentItem.y
                    color: Material.accent; opacity: 0.3; radius: 4
                    Behavior on x { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
                    Behavior on y { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
                }
                highlightFollowsCurrentItem: false
                delegate: Item {
                    width: ListView.view.width; height: 35
                    MouseArea { anchors.fill: parent; onClicked: m2List.currentIndex = index }
                    Text {
                        anchors.fill: parent;
                        text: itemData ? "objectName=" + itemData.objectName + " dummyReal=" + itemData.dummyReal :
                                         "undefined"
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    } // Pane m2

    Pane {
        id: paneM1m2
        anchors.top: paneM1.top; anchors.left: paneM2.right
        anchors.leftMargin: 25; anchors.topMargin: 0
        width: 300; height: 300
        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            Text {  Layout.maximumWidth: parent.width; font.bold: true; wrapMode: Text.WrapAnywhere
                    text:"qps::ContainerModelComposer(m1, m2)" }
            Button {
                text: "Swap m2/m3"
                onClicked: { m1m2.listReference.m2 = ( m1m2.listReference.m2 === m2 ? m3 : m2 ) }
            }
            Text { text:"Item count=" + m1m2.itemCount; font.bold:true }
            ListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                model: m1m2
                delegate: Text { text: itemData ? "objectName=" + itemData.objectName + " dummyReal=" + itemData.dummyReal :
                                                  "undefined" }
            }
        }
    } // Pane m1m2
}

