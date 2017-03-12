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

/*! \brief Graph is the QML view for qan::Graph
 *
 * Visual connection of nodes:
 *   \li Visual connection of nodes with VisualConnector is enabled by setting the \c enableVisualConnector property to \c true (default to false).
 *   \li Class name for the edges created by the visual node connector could be changed with property \c connectorEdgeClassName (default to qan::Edge).
 *   \li When an edge is created with the visual node connector, the signal \c edgeInsertedVisually with the newly inserted \c edge as an argument.
 */
Qan.AbstractGraph {
    id: graph

    property alias  resizeHandlerColor: nodeResizer.handlerColor

    //! Color of the visual connector edge line.
    property color  connectorEdgeColor: Qt.rgba(0,0,0,1)
    //! Color of the visual connector draggable item.
    property color  connectorColor: "darkblue"
    //! Turn visual creation of hyper edges on or off (default to off).
    property bool   connectorHEdgeEnabled: false
    onConnectorChanged: {
        if ( connector ) {
            connectorEdgeColor = connector.edgeColor
            connectorColor = connector.connectorColor
            connectorHEdgeEnabled = connector.hEdgeEnabled
        }
    }
    onConnectorEdgeColorChanged: {
        if ( connector )
            connector.edgeColor = connectorEdgeColor
    }
    onConnectorColorChanged: {
        if ( connector )
            connector.connectorColor = connectorColor
    }
    onConnectorHEdgeEnabledChanged: {
        if ( connector )
            connector.hEdgeEnabled = connectorHEdgeEnabled
    }

    Qan.BottomRightResizer {
        id: nodeResizer
        visible: false
    }
    property real maxZ: -1.
    onNodeClicked: {
        if ( node ) {
            maxZ = Math.max( node.z + 1, maxZ + 1 )
            node.z = maxZ + 1;
            if ( node.qmlGetGroup() )
                node.qmlGetGroup.z = maxZ
            if ( connector )
                connector.sourceNode = node;
            if ( node.resizable ) {
                // FIXME QAN3
                /*nodeResizer.parent = node
                nodeResizer.minimumTargetSize = node.minimumSize
                nodeResizer.target = node
                nodeResizer.visible = true
                */
            } else
                nodeResizer.visible = false
        } else {
            connector.visible = false
            resizer.visible = false
        }
    }
    onGroupClicked: {
        maxZ = Math.max(group.z, maxZ)
        group.z = maxZ + 1;
    }
    onGroupRightClicked: {
        maxZ = Math.max(group.z, maxZ)
        group.z = maxZ + 1;
    }
    onGroupDoubleClicked: {
        maxZ = Math.max(group.z, maxZ)
        group.z = maxZ + 1;
    }
    Component.onCompleted: { }
    Component.onDestruction: { }
}
