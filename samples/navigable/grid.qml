import QtQuick          2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts  1.3

import QuickQanava 2.0 as Qan

Item {
    Qan.Navigable {
        id: navigable
        anchors.fill: parent
        clip: true
        navigable: true

        grid: Qan.PointGrid {
            id: pointGrid
            pointComponent: Component {
                Rectangle {
                    smooth: true
                    width: pointGrid.gridWidth
                    height: width
                    radius: width/2.
                    color: pointGrid.thickColor
                    //width: 2; height: 2; radius: 1; color: pointGrid.thickColor //"darkgrey"
                }
            }

        }
        Rectangle {
            parent: navigable.containerItem
            x: 100; y: 100
            width: 50; height: 25
            color: "lightblue"
        }
        Rectangle {
            parent: navigable.containerItem
            x: 300; y: 100
            width: 50; height: 25
            color: "red"
        }
        Rectangle {
            parent: navigable.containerItem
            x: 300; y: 300
            width: 50; height: 25
            color: "green"
        }
        Rectangle {
            parent: navigable.containerItem
            x: 100; y: 300
            width: 50; height: 25
            color: "blue"
        }
    } // Qan.Navigable

    RowLayout {
        CheckBox {
            text: "Grid Visible"
            checked: pointGrid.visible
            onCheckedChanged: pointGrid.visible = checked
        }
        Label { text: "Grid Scale:" }
        ComboBox {
            textRole: "key"
            model: ListModel {
                ListElement { key: "50";    value: 50 }
                ListElement { key: "100";   value: 100 }
                ListElement { key: "150";   value: 150 }
            }
            currentIndex: 1 // Default to 100
            onActivated: {
                var gridScale = model.get(currentIndex).value
                if ( gridScale )
                    pointGrid.gridScale = gridScale
            }
        }
        Label { Layout.leftMargin: 25; text: "Grid Major:" }
        SpinBox {
            from: 1;    to: 10
            value: pointGrid.gridMajor
            onValueChanged: pointGrid.gridMajor = value
        }
        Label { Layout.leftMargin: 25; text: "Point size:" }
        SpinBox {
            from: 1;    to: 10
            value: pointGrid.gridWidth
            onValueChanged: pointGrid.gridWidth = value
        }
    }
}
