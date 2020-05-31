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

import QtQuick              2.7
import QtQuick.Controls     2.0
import QtQuick.Layouts      1.3
import QtQuick.Dialogs      1.2

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "qrc:/QuickContainers" as Qps


ApplicationWindow {
    id: window
    visible: true
    width: 1280; height: 720    // MPEG - 2 HD 720p - 1280 x 720 16:9

    title: "QuickQanava 40k"

    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable   : true
        graph : Qan.Graph {
            id: topology
            objectName: "graph"
            anchors.fill: parent
            clip: true
            enableConnectorDropNode: true
            Component.onCompleted: {
            }
            onNodeRightClicked: { }
            onEdgeRightClicked: { }
        } // Qan.Graph: graph
    }
}
