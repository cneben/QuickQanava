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

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "." as Qan

Item {
    anchors.fill: parent
    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        graph       : topology
        navigable   : true
        Qan.Graph {
            id: topology
            objectName: "graph"
            anchors.fill: parent
            enableConnectorDropNode: true
            visualHEdgeCreationEnabled: true
            Component.onCompleted: {
                var n1 = topology.insertNode( )
                n1.label = "N1"
                n1.x = 10; n1.y = 50
                var n2 = topology.insertNode( )
                n2.label = "N2"
                n2.x = 300; n2.y = 50
                var n3 = topology.insertNode( )
                n3.label = "N3"
                n3.x = 150; n3.y = 250
                var n4 = topology.insertNode( )
                n4.label = "N4"
                n4.x = 300; n4.y = 250
                var e1 = topology.insertEdge( n1, n2 )
                topology.insertEdge( n3, e1 )
            }
            onEdgeRightClicked: {
                var globalPos = edge.mapToItem( topology, pos.x, pos.y )
                edgeMenu.x = globalPos.x; edgeMenu.y = globalPos.y
                edgeMenu.targetEdge = edge; edgeMenu.open()
            }
        } // Qan.Graph: topology
    } // Qan.GraphView: graphView

    Menu {
        id: edgeMenu
        title: "Edge Menu"
        property var targetEdge: undefined
        closePolicy: Popup.CloseOnPressOutside
        MenuItem {
            text: "Remove edge"
            enabled: edgeMenu.targetEdge !== undefined
            onTriggered: {
                if ( edgeMenu.targetEdge )
                    topology.removeEdge( edgeMenu.targetEdge )
                edgeMenu.targetEdge = undefined
            }
        }
    }

    CheckBox {
        text: "Enable visual creation of hEdges"
        checked: topology.visualHEdgeCreationEnabled
        onClicked: { topology.visualHEdgeCreationEnabled = !topology.visualHEdgeCreationEnabled }
    }
} // Item: main window

