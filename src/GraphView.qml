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

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library. Copyright 2015 Benoit AUTHEMAN.
//
// \file	Graph.qml
// \author	benoit@destrat.io
// \date	2015 08 01
//-----------------------------------------------------------------------------

import QtQuick              2.7

import QuickQanava          2.0 as Qan
import "qrc:/QuickQanava"   as Qan

/*! \brief Visual view for a Qan.Graph component.
 *
 *  Set the graph property to Qan.Graph{}.
<<<<<<< HEAD
=======
 *
 * Visual connection of nodes:
 *   \li Visual connection of nodes with VisualConnector is enabled by setting the \c enableVisualConnector property to \c true (default to false).
 *   \li Class name for the edges created by the visual node connector could be changed with property \c connectorEdgeClassName (default to qan::Edge).
 *   \li When an edge is created with the visual node connector, the signal \c edgeInsertedVisually with the newly inserted \c edge as an argument.
 *
>>>>>>> dev
 */
Qan.AbstractGraphView {
    id: graphView

    onClicked: graphView.focus = true           // User clicked outside a graph item, remove it's eventual active focus
    onRightClicked: graphView.focus = true
<<<<<<< HEAD
}
=======

    property alias  resizeHandlerColor: nodeResizer.handlerColor

    //! Color of the visual connector edge line.
    property color  connectorEdgeColor: Qt.rgba(0,0,0,1)
    //! Color of the visual connector draggable item.
    property color  connectorColor: "darkblue"
    //! Turn visual creation of hyper edges on or off (default to off).
    property bool   connectorHEdgeEnabled: false
    onConnectorChanged: {
        if ( graph && graph.connector ) {
            connectorEdgeColor = graph.connector.edgeColor
            connectorColor = graph.connector.connectorColor
            connectorHEdgeEnabled = graph.connector.hEdgeEnabled
        }
    }
    onConnectorEdgeColorChanged: {
        if ( graph && graph.connector )
            graph.connector.edgeColor = connectorEdgeColor
    }
    onConnectorColorChanged: {
        if ( graph && graph.connector )
            graph.connector.connectorColor = connectorColor
    }
    onConnectorHEdgeEnabledChanged: {
        if ( graph && graph.connector )
            graph.connector.hEdgeEnabled = connectorHEdgeEnabled
    }

    Qan.BottomRightResizer {
        id: nodeResizer
        parent: graph.containerItem
        visible: false
    }
    property real maxZ: -1.
    onNodeClicked: {
        if ( graph && node && node.item ) {
            maxZ = Math.max( node.item.z + 1, maxZ + 1 )
            node.item.z = maxZ + 1;
            if ( node.group )
                node.group.z = maxZ
            if ( graph.connector &&
                 graph.connectorEnabled )
                graph.connector.sourceNode = node;
            if ( nodeResizer &&
                 node.item.resizable ) {
                nodeResizer.parent = node.item
                nodeResizer.minimumTargetSize = node.item.minimumSize
                nodeResizer.target = node.item
                nodeResizer.visible = Qt.binding( function() { return nodeResizer.target.resizable; } )
                nodeResizer.z = node.item.z + 1.
                //nodeResizer.z = maxZ + 1000.
            } else {
                nodeResizer.target = null
                nodeResizer.visible = false
            }
        } else if ( graph ) {
            graph.connector.visible = false
            resizer.visible = false
        }
    }
    function updateGroupZ(group) {
        if ( group && group.item ) {
            maxZ = Math.max( group.item.z + 1, maxZ + 1 )
            group.item.z = maxZ + 1;
        }
    }
    onGroupClicked: updateGroupZ(group)
    onGroupRightClicked: updateGroupZ(group)
    onGroupDoubleClicked: updateGroupZ(group)
    Component.onCompleted: { }
    Component.onDestruction: { }
}

>>>>>>> dev
