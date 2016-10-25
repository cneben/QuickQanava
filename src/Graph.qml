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

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library. Copyright 2015 Benoit AUTHEMAN.
//
// \file	Graph.qml
// \author	benoit@destrat.io
// \date	2015 08 01
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan

/*! \brief Graph is the QML view for qan::Graph
 *
 * Visual connection of nodes:
 *   \li Visual connection of nodes with ConnectorDropNode is enabled by setting the \c enableConnectorDropNode property to \c true (default to false).
 *   \li Class name for the edges created by the visual node connector could be changed with property \c connectorDropNodeEdgeClassName (default to qan::Edge).
 *   \li When an edge is created with the visual node connector, the signal \c edgeInsertedVisually with the newly inserted \c edge as an argument.
 */
Qan.AbstractGraph {
    id: graph

    // Public:
    //! Default delegate for qan::Node and Qan.Node nodes.
    property Component  nodeDelegate: Qt.createComponent( "qrc:/QuickQanava/Node.qml" )
    //! Default delegate for qan::Edge and Qan.Edge edges.
    property Component  edgeDelegate: Qt.createComponent( "qrc:/QuickQanava/Edge.qml" )
    //! Default delegate for qan::Group and Qan.Group groups.
    property Component  groupDelegate: Qt.createComponent( "qrc:/QuickQanava/Group.qml" )

    property alias      resizeHandlerColor: nodeResizer.handlerColor

    //! Set to true to enable visual edge creation via a droppable connector control (default to true).
    property bool   enableConnectorDropNode: false
    //! Modify to create edge with custom class name with connector drop node (default to qan::Edge).
    property string connectorDropNodeEdgeClassName: "qan::Edge"
    property color  connectorDropNodeEdgeColor: Qt.rgba(0,0,0,1)

    // Private:
    property Component  connectorDropNodeComponent: Qt.createComponent( "qrc:/QuickQanava/ConnectorDropNode.qml" )
    onConnectorDropNodeComponentChanged: {
        connectorDropNodeComponent.edgeClassName = connectorDropNodeEdgeClassName
    }
    property var        connectorDropNode: undefined
    property color      connectorDropNodeColor: "darkblue"

    //! Turn visual creation of hyper edges on or off (default to off).
    property bool       visualHEdgeCreationEnabled: false
    onEnableConnectorDropNodeChanged: {
        if ( !enableConnectorDropNode &&
             connectorDropNode &&
             graph.hasNode( connectorDropNode ) )
            graph.removeNode( connectorDropNode )

        if ( enableConnectorDropNode &&
             !connectorDropNode &&
             connectorDropNodeComponent ) {
            connectorDropNode = graph.insertNode( graph.connectorDropNodeComponent )
            if ( connectorDropNode ) {
                connectorDropNode.edgeClassName = connectorDropNodeEdgeClassName
                connectorDropNode.visible = false
                connectorDropNode.graph = graph
                connectorDropNode.edgeColor = Qt.binding( function() { return graph.connectorDropNodeEdgeColor; } )
                connectorDropNode.connectorColor = Qt.binding( function() { return graph.connectorDropNodeColor; } )
                connectorDropNode.hEdgeCreationEnabled = Qt.binding( function() { return graph.visualHEdgeCreationEnabled; } )
            }
        }
    }
    signal  edgeInsertedVisually( var edge );

    Qan.BottomRightResizer {
        id: nodeResizer
        visible: false
    }
    property real maxZ: -1.
    onNodeClicked: {
        if ( !node ) {
            connectorDropNode.visible = false
            resizer.visible = false
        }
        maxZ = Math.max( node.z + 1, maxZ + 1 )
        node.z = maxZ + 1;
        if ( node.qmlGetGroup() )
            node.qmlGetGroup.z = maxZ
        if ( connectorDropNode )
            connectorDropNode.setHostNode( node );
        if ( node.resizable ) {
            nodeResizer.parent = node
            nodeResizer.minimumTargetSize = node.minimumSize
            nodeResizer.target = node
            nodeResizer.visible = true

        } else
            nodeResizer.visible = false
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
    Component.onCompleted: {
        graph.registerNodeDelegate( "qan::Node", nodeDelegate )
        graph.registerEdgeDelegate( "qan::Edge", edgeDelegate )
        graph.registerGroupDelegate( "qan::Group", groupDelegate )
        graph.addControlNode( connectorDropNode )
    }
    Component.onDestruction: {
        graph.removeControlNode( connectorDropNode )
    }
}
