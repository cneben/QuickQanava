import QtQuick          2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts  1.3

import QuickQanava 2.0 as Qan

ApplicationWindow {
    visible: true
    width: 1024
    height: 800
    title: qsTr("Navigable Sample")

    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: tabBar
            Layout.fillWidth: true; Layout.fillHeight: false
            TabButton { text: qsTr("Grid") }
            TabButton { text: qsTr("Image") }
        }
        StackLayout {
            Layout.fillWidth: true; Layout.fillHeight: true
            currentIndex: tabBar.currentIndex
            Item { Loader { anchors.fill: parent; source: "qrc:/grid.qml"} }
            Item { Loader { anchors.fill: parent; source: "qrc:/image.qml"} }
        }
    }
}
