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

    width: radius * 2 * radiusHilightMag
    height: radius * 2 * radiusHilightMag
    x: parent.width + 2
    y: 0

    visible: false
    clip: false
    label: "Connector Drop Node"
    // FIXME 20160210
    //serializable: false // Don't serialize "helper control node"

    // Public
    //! Connector control radius (final diameter will be radius x 2).
    property real radius: 5

    //! Connector control will be expanded up to radius * raduiusHilightMag to hilight a "connectable" drag target (default to 2.).
    property real radiusHilightMag: 2.

    //! Connector color (default to Qt.DarkBlue).
    property color connectorColor: "darkblue"

    //! Default connector line width (default to 2.0).
    property real connectorLineWidth: 2

    //! Maximum connector line width, used to visually hilight that te current drag "target" can be connected (default to 5.0).
    property real connectorHilightLineWidth: 5

    //! Qan.Graph where this connector drop node is registered
    property var graph : undefined

    /*! \brief Set the connector target (is the node that will display the control and that will be used as 'source' for edge creation).
     *
     * \note this item will automatically be reparented to host node, and displayed in host node top right.
     */
    function setHostNode( hostNode ) {
        visible = ( hostNode != null && hostNode != undefined )
        parent = hostNode
        sourceNode = hostNode
    }

    // Private properties
    acceptDrops: false  // Base qan::Node should not accept drop, or control will "drop on itself" !

    Drag.active: dropDestArea.drag.active
    Drag.dragType: Drag.Internal

    Drag.onTargetChanged: { // Hilight a target node
        if ( Drag.target != null && Drag.target == sourceNode ) {
            nodeSymbol.state = "NORMAL"
            if ( dummyEdge != undefined )
                dummyEdge.visible = false
        }
        else {
            if ( Drag.target != undefined )
                parent.z = Drag.target.z + 1
            nodeSymbol.state = ( Drag.target != undefined ) ? "HILIGHT" : "NORMAL"
            if ( dummyEdge != undefined ) {
                dummyEdge.visible = true
                dummyEdge.z = parent.z  // Edge should be always on top
            }
        }
    }

    Canvas {
        id: nodeSymbol
        anchors.fill: parent
        z: 1
        state: "NORMAL"
        clip: false
        property var radius : connectorDropNode.radius
        property var lineWidth: connectorDropNode.connectorLineWidth
        states: [
            State { name: "NORMAL"; PropertyChanges { target: nodeSymbol; lineWidth: connectorDropNode.connectorLineWidth; radius : connectorDropNode.radius } },
            State { name: "HILIGHT"
                PropertyChanges {
                    target: nodeSymbol; lineWidth: connectorDropNode.connectorHilightLineWidth;
                    radius : ( nodeSymbol.width / 2 ) - connectorDropNode.connectorHilightLineWidth / 1.8  // 1.8 to add an "antialisaing margin"
                }
            }
        ]
        smooth: true
        antialiasing: true
        transitions: [
            Transition {
                from: "NORMAL"; to: "HILIGHT"; PropertyAnimation { target: nodeSymbol; properties: "lineWidth, radius"; duration: 100 }
            },
            Transition {
                from: "HILIGHT"; to: "NORMAL"; PropertyAnimation { target: nodeSymbol; properties: "lineWidth, radius"; duration: 150 }
            } ]

        onRadiusChanged: requestPaint()
        onPaint: {
            var ctx = nodeSymbol.getContext( "2d" )
            ctx.clearRect( 0, 0, width, height )
            ctx.lineWidth = lineWidth
            ctx.strokeStyle = connectorDropNode.connectorColor
            ctx.beginPath( )
            var center = Qt.point( width / 2, height / 2 )
            ctx.ellipse( center.x - radius, center.y - radius,
                        radius * 2, radius * 2 )
            ctx.stroke( )
        }
    }
    property var sourceNode
    onSourceNodeChanged: {
        if ( visible && connectorDropNode.parent != null ) {
            connectorDropNode.x = connectorDropNode.parent.width + 2
            connectorDropNode.y = 0
        }
    }
    property var dummyEdge: undefined
    MouseArea {
        id: dropDestArea
        anchors.fill: parent
        drag.target: connectorDropNode
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        enabled: true
        onReleased: {
            if ( connectorDropNode.graph == undefined )
                return
            var src = connectorDropNode.sourceNode
            var dst = connectorDropNode.Drag.target
            if ( src != undefined && dst != undefined && dst != null ) {
                console.debug( "connectorDropNode.graph.isNode( dst )=" + connectorDropNode.graph.isNode( dst ) );
                console.debug( "connectorDropNode.graph.hasEdge( src, dst )=" + connectorDropNode.graph.hasEdge( src, dst ) );
                if ( connectorDropNode.graph.isNode( dst ) &&
                     !connectorDropNode.graph.hasEdge( src, dst ) ) {
                    connectorDropNode.graph.insertEdge( src, dst )
                }
            }
            // Restore original position
            connectorDropNode.x = connectorDropNode.parent.width + 2
            connectorDropNode.y = 0
            if ( connectorDropNode.dummyEdge != undefined ) {
                connectorDropNode.graph.removeEdge( connectorDropNode.dummyEdge )
                connectorDropNode.dummyEdge = undefined
            }
        }
        onPressed : {
            mouse.accepted = true
            if ( !connectorDropNode.graph.hasEdge( connectorDropNode.sourceNode, connectorDropNode ) ) {
                if ( connectorDropNode.dummyEdge != undefined )
                    connectorDropNode.graph.removeEdge( connectorDropNode.dummyEdge )
                connectorDropNode.dummyEdge = connectorDropNode.graph.insertEdge( connectorDropNode.sourceNode, connectorDropNode )
                connectorDropNode.dummyEdge.z = connectorDropNode.sourceNode.z
            }
        }
    }
    // QuickQanava Qan.Node interface
    signal  nodeClicked( var node, var p )
    signal  nodeDoubleClicked( var node, var p )
    signal  nodeRightClicked( var node, var p )
}

