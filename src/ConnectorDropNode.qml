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

import "qrc:/QuickQanava" as Qan
import QuickQanava 2.0 as Qan

/*! \brief Droppable node control that allow direct mouse/touch edge creation between nodes.
 *
 * \image html ConnectorDropNode.png
 *
 * Example use in QML:
 * \code
 * Qan.Graph {
 *   enableConnectorDropNode: true
 * }
 * \endcode
 *
 * \note ConnectorDropNode visible property is automatically set to false until it has been associed to an existing "host" node
 *       with setHostNode().
 */
Qan.AbstractNode {
    id: connectorDropNode

    width: radius * 2
    height: radius * 2
    x: parent.width + xMargin
    y: 0

    visible: false
    clip: false
    label: "Connector Drop Node"
    serializable: false // Don't serialize "helper control node"
    selectable: false
    antialiasing: true

    // Public
    //! Edge color (default to black).
    property color  edgeColor: Qt.rgba(0,0,0,1)

    //! Connector control radius (final diameter will be radius x 2).
    property real   radius: 7

    //! Connector color (default to Qt.DarkBlue).
    property color  connectorColor: "darkblue"

    //! Default connector line width (default to 2.0).
    property real   connectorLineWidth: 2

    //! Maximum connector line width, used to visually hilight that te current drag "target" can be connected (default to 5.0).
    property real   connectorHilightLineWidth: 4

    //! Enable or disable visual creation of hyper edges.
    property bool   hEdgeCreationEnabled: false

    //! Qan.Graph where this connector drop node is registered
    property var    graph : undefined

    property real   xMargin: 7

    /*! \brief Set the connector target (is the node that will display the control and that will be used as 'source' for edge creation).
     *
     * \note this item will automatically be reparented to host node, and displayed in host node top right.
     */
    function setHostNode( hostNode ) {
        visible = ( hostNode !== null && hostNode !== undefined )
        parent = hostNode
        sourceNode = hostNode
        if ( hostNode )   // Force drop node position updates
            x = Qt.binding( function(){ return hostNode.width + xMargin } )
        else x = 0
        connectionSymbol.state = "NORMAL"
    }

    // Private properties
    acceptDrops: false  // Base qan::Node should not accept drop, or control will "drop on itself" !

    Drag.active: dropDestArea.drag.active
    Drag.dragType: Drag.Internal

    Drag.onTargetChanged: { // Hilight a target node
        if ( !Drag.target )
            connectionSymbol.state = "NORMAL"
        else if ( Drag.target &&
                   Drag.target === sourceNode ) {       // Do not create a circuit on source node
            connectionSymbol.state = "NORMAL"
            if ( dummyEdge )
                dummyEdge.visible = false
        }
        else if ( Drag.target &&                        // Hilight only on a node target OR an edge target IF hyper edge creation is enabled
                  ( graph.isNode(Drag.target) ||
                    ( connectorDropNode.hEdgeCreationEnabled && graph.isEdge(Drag.target) ) ) )
                 {
                     parent.z = Drag.target.z + 1
                     connectionSymbol.state = "HILIGHT"
                     if ( dummyEdge ) {
                         dummyEdge.visible = true
                         dummyEdge.z = parent.z  // Edge should be always on top
                     }
                 }
    }

    Rectangle {
        id: connectionSymbol
        anchors.fill: parent
        z: 15
        state: "NORMAL"
        color: Qt.rgba(0, 0, 0, 0)
        radius : width / 2.
        smooth: true
        antialiasing: true
        property real   borderWidth: connectorDropNode.connectorLineWidth
        border.color: connectorDropNode.connectorColor
        border.width: borderWidth
        states: [
            State { name: "NORMAL";
                PropertyChanges {
                    target: connectionSymbol;
                    borderWidth: connectorDropNode.connectorLineWidth;
                    scale: 1.0
                }
            },
            State { name: "HILIGHT"
                PropertyChanges {
                    target: connectionSymbol;
                    borderWidth: connectorDropNode.connectorHilightLineWidth;
                    scale: 1.7
                }
            }
        ]
        transitions: [
            Transition {
                from: "NORMAL"; to: "HILIGHT"; PropertyAnimation { target: connectionSymbol; properties: "borderWidth, scale"; duration: 100 }
            },
            Transition {
                from: "HILIGHT"; to: "NORMAL"; PropertyAnimation { target: connectionSymbol; properties: "borderWidth, scale"; duration: 150 }
            } ]
    }
    property var sourceNode
    onSourceNodeChanged: {
        if ( visible && connectorDropNode.parent !== null ) {
            connectorDropNode.x = connectorDropNode.parent.width + xMargin
            connectorDropNode.y = 0
            connectionSymbol.state = "NORMAL"
        }
    }
    //! Modify this property to create edge with a specific class name (default to qan::Edge).
    property var edgeClassName: "qan::Edge"
    property var dummyEdge: undefined
    MouseArea {
        id: dropDestArea
        anchors.fill: parent
        drag.target: connectorDropNode
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        enabled: true
        onReleased: {
            if ( !connectorDropNode.graph )
                return
            var src = connectorDropNode.sourceNode
            var dst = connectorDropNode.Drag.target
            var edge = null
            if ( src && dst ) {
                if ( connectorDropNode.graph.isNode( dst ) ) {
                    if ( !connectorDropNode.graph.hasEdge( src, dst ) )     // Do not insert parrallel edgse
                        edge = connectorDropNode.graph.insertEdge( edgeClassName, src, dst )
                } else if ( connectorDropNode.hEdgeCreationEnabled &&
                            connectorDropNode.graph.isEdge( dst ) ) {           // Do not create an hyper edge on an hyper edge
                    if ( !connectorDropNode.graph.hasEdge( src, dst ) &&
                            !connectorDropNode.graph.isHyperEdge( dst ) )
                        edge = connectorDropNode.graph.insertEdge( edgeClassName, src, dst )
                }
            }
            if ( edge ) {    // Notify graph user of the edge creation
                edge.color = Qt.binding( function() { return connectorDropNode.edgeColor; } )
                connectorDropNode.graph.edgeInsertedVisually( edge )
            }
            // Restore original position
            connectionSymbol.state = "NORMAL"
            connectorDropNode.x = connectorDropNode.parent.width + xMargin
            connectorDropNode.y = 0
            if ( connectorDropNode.dummyEdge ) {
                connectorDropNode.graph.removeEdge( connectorDropNode.dummyEdge )
                connectorDropNode.dummyEdge = undefined
            }
        }
        onPressed : {
            mouse.accepted = true
            //if ( !connectorDropNode.graph.hasEdge( connectorDropNode.sourceNode, connectorDropNode ) ) {
                if ( connectorDropNode.dummyEdge ) {
                    connectorDropNode.graph.removeEdge( connectorDropNode.dummyEdge )
                    connectorDropNode.dummyEdge = undefined
                }
                connectorDropNode.dummyEdge = connectorDropNode.graph.insertEdge( edgeClassName, connectorDropNode.sourceNode, connectorDropNode )
                connectorDropNode.dummyEdge.color = Qt.binding( function() { return connectorDropNode.edgeColor; } )
                connectorDropNode.dummyEdge.z = connectorDropNode.sourceNode.z
            //}
        }
    }
    // QuickQanava Qan.Node interface
    signal  nodeClicked( var node, var p )
    signal  nodeDoubleClicked( var node, var p )
    signal  nodeRightClicked( var node, var p )
}

