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

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "." as Qan

Item {
    anchors.fill: parent
    property alias nodeDelegate : graph.nodeDelegate

    Qan.Navigable {
        id: navigable
        anchors.fill: parent
        Qan.Graph {
            parent: navigable.containerItem
            id: graph
            objectName: "graph"
            anchors.fill: parent

            property var nodeDelegate: Qt.createComponent( "qrc:/QuickQanava/Node.qml" )

            Component.onCompleted: {
                var n1 = graph.insertNode( nodeDelegate )
                var n2 = graph.insertNode( nodeDelegate )
                var n3 = graph.insertNode( nodeDelegate )
                graph.insertEdge( edgeDelegate, n1, n2 )
                graph.insertEdge( edgeDelegate, n2, n3 )
            }
        } // Qan.Graph: graph
    } // Qan.Navigable: navigable

    RowLayout {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 300
        Button {
            text: "Load GML"
        }
        Button {
            text: "Save GML"
            onClicked: { graph.saveToGml( "test.gml" ) }
        }
        Button {
            text: "Load QAN"
        }
        Button {
            text: "Save Qan"
        }
        Button {
            text: "Insert Node"
            onClicked: { graph.insertNode( nodeDelegate ) }
        }
    }
}

