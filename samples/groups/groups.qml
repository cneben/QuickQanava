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

import QtQuick                   2.8
import QtQuick.Controls          2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts           1.3

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "." as Qan

ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720
    title: "Custom nodes sample"
    Pane { anchors.fill: parent }
    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        graph: Qan.Graph {
            id: topology
            connectorEnabled: true
            objectName: "graph"
            anchors.fill: parent
            onGroupClicked: { console.debug( "Group " + group.label + " clicked" ) }
            onGroupDoubleClicked: { console.debug( "Group " + group.label + " double clicked" ) }
            onGroupRightClicked: { console.debug( "Group " + group.label + " right clicked" ) }
            Component.onCompleted: {
                var n1 = topology.insertNode( )
                n1.label = "N1"
                var n2 = topology.insertNode( )
                n2.label = "N2"
                var n3 = topology.insertNode( )
                n3.label = "N3"
                topology.insertEdge( n1, n2 )
                topology.insertEdge( n2, n3 )
            }
        } // Qan.Graph: graph

        RowLayout {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 300
            Button {
                text: "No Layout Group"
                onClicked: {
                    var gg = topology.insertGroup()
                    gg.z = 0
                    if ( gg )
                        gg.label = "Group"
                }
            }
            Button {
                text: "Insert Node"
                onClicked: {
                    var n = topology.insertNode( )
                    if ( n )
                        n.label = "Node"
                }
            }
        }
    } // Qan.GraphView
}

