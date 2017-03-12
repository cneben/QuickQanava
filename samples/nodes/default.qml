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

Qan.GraphView {
    id: graphView
    anchors.fill: parent
    navigable   : true
    graph: Qan.Graph {
        id: graph
        Component.onCompleted: {
            var n1 = graph.insertNode()
            n1.label = "test"
            n1.ToolTip.text = "testttt"
        }
        onNodeClicked: { notifyUser( "Node <b>" + node.label + "</b> clicked" ) }
        onNodeRightClicked: { notifyUser( "Node <b>" + node.label + "</b> right clicked" ) }
        onNodeDoubleClicked: { notifyUser( "Node <b>" + node.label + "</b> double clicked" ) }
    }
    ToolTip { id: toolTip; timeout: 2500 }
    function notifyUser(message) { toolTip.text=message; toolTip.open() }
}  // Qan.GraphView

