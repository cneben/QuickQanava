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

import QtQuick 2.7

import "qrc:/QuickQanava" as Qan
import QuickQanava 2.0 as Qan

/*! \brief Droppable node control that allow direct mouse/touch edge creation between nodes.
 *
 * \image html VisualConnector.png
 *
 * Example use in QML:
 * \code
 * Qan.GraphView {
 *   graph: Qan.Graph {
 *     connectorEnabled: true
 *   }
 * }
 * \endcode
 *
 * \note VisualConnector visible property is automatically set to false until it has been associed to an existing "host" node
 *       with setHostNode().
 */
Qan.Connector {
    id: visualConnector
    width: radius * 2;  height: radius * 2
    x: parent.width + leftMargin;  y: topMargin

    visible: false
    selectable: false
    clip: false; antialiasing: true

    // Public
    //! Edge color (default to black).
    property color  edgeColor: Qt.rgba(0,0,0,1)

    //! Connector control radius (final diameter will be radius x 2).
    property real   radius: 8

    //! Connector color (default to Qt.DarkBlue).
    property color  connectorColor: "darkblue"

    //! Default connector line width (default to 2.0).
    property real   connectorLineWidth: 2

    //! Maximum connector line width, used to visually hilight that te current drag "target" can be connected (default to 5.0).
    property real   connectorHilightLineWidth: 4

    //! Enable or disable visual creation of hyper edges.
    property bool   hEdgeEnabled: false

    property real   leftMargin: 7

    //! Use that property with custom connector item to ensure they are anchored under default connector item.
    property real   topMargin: 0

    //! True when the connector item is currently dragged.
    property bool   connectorDragged: dropDestArea.drag.active

    /*! \brief When set to true, connector use qan::Graph::createEdge() to generate edges, when set to false, signal
        requestEdgeCreation() is emmited instead to allow user to create custom edges.
    */
    property bool   createDefaultEdge: true

    edgeComponent: Component{ Qan.Edge{} }

    onEdgeColorChanged: {
        if (edgeItem)
            edgeItem.color = edgeColor
    }

    // Private properties
    /*! \brief Set the connector target (is the node that will display the control and that will be used as 'source' for edge creation).
     *
     * \note this item will automatically be reparented to host node, and displayed in host node top right.
     */
    function setHostNode( hostNode ) {
        if ( hostNode ) {
            parent = hostNode
            if ( hostNode )  // Force drop node position updates
                x = Qt.binding( function(){ return hostNode.width + leftMargin } )
            else x = 0
            if ( connectorItem )
                connectorItem.state = "NORMAL"
        } else
            visible = false
    }

    Drag.active: dropDestArea.drag.active
    Drag.dragType: Drag.Internal
    Drag.onTargetChanged: { // Hilight a target node
        //console.debug( "Drag.target=" + Drag.target )
        if ( Drag.target )
            visualConnector.z = Drag.target.z + 1
        if ( !Drag.target && connectorItem ) {
            connectorItem.state = "NORMAL"
        } else {
            if ( sourceNode &&
                 sourceNode.item &&
                 Drag.target === sourceNode.item ) {    // Do not create a circuit on source node
                 connectorItem.state = "NORMAL"
            } else if ( Drag.target.node ||             // Hilight only on a node target OR an edge target IF hyper edge creation is enabled
                        ( hEdgeEnabled && Drag.target.edge ) )
            {
                parent.z = Drag.target.z + 1
                connectorItem.state = "HILIGHT"
                if ( edgeItem ) {
                    edgeItem.visible = true
                    edgeItem.z = parent.z  // Edge should be always on top
                }
            }
        }
    }
    connectorItem: Rectangle {
        id: defaultConnectorItem
        parent: visualConnector
        anchors.fill: parent
        z: 15
        state: "NORMAL"
        color: Qt.rgba(0, 0, 0, 0)
        radius: width / 2.
        smooth: true;   antialiasing: true
        property real   borderWidth: visualConnector.connectorLineWidth
        border.color: visualConnector.connectorColor
        border.width: borderWidth
        states: [
            State { name: "NORMAL";
                PropertyChanges { target: defaultConnectorItem; borderWidth: visualConnector.connectorLineWidth; scale: 1.0 }
            },
            State { name: "HILIGHT"
                PropertyChanges { target: defaultConnectorItem; borderWidth: visualConnector.connectorHilightLineWidth; scale: 1.7 }
            }
        ]
        transitions: [
            Transition {
                from: "NORMAL"; to: "HILIGHT"; PropertyAnimation { target: defaultConnectorItem; properties: "borderWidth, scale"; duration: 100 }
            },
            Transition {
                from: "HILIGHT"; to: "NORMAL"; PropertyAnimation { target: defaultConnectorItem; properties: "borderWidth, scale"; duration: 150 }
            } ]
    }
    onSourceNodeChanged: {
        if ( sourceNode.item )
            setHostNode(sourceNode.item)
    }
    MouseArea {
        id: dropDestArea
        anchors.fill: parent
        drag.target: visualConnector
        drag.threshold: 1.      // Note 20170311: Avoid a nasty delay between mouse position and dragged item position
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        enabled: true
        onReleased: {
            // Restore original position
            connectorItem.state = "NORMAL"
            visualConnector.x = visualConnector.parent.width + leftMargin
            visualConnector.y = topMargin
            if ( edgeItem )
                edgeItem.visible = false
            if ( !visualConnector.graph )
                return
            var src = visualConnector.sourceNode
            var dst = visualConnector.Drag.target
            var createdEdge = null
            if ( graph && src && dst ) {
                if ( dst.node &&
                     !graph.hasEdge( src, dst.node ) ) {     // Do not insert parrallel edgse
                    if ( createDefaultEdge )
                        createdEdge = graph.insertEdge( src, dst.node )
                    else requestEdgeCreation(src, dst.node)
                }
                else if ( hEdgeEnabled &&
                         dst.edge &&
                          !graph.hasEdge( src, dst.edge ) &&
                          !dst.isHyperEdge() ) {            // Do not create an hyper edge on an hyper edge
                    if ( createDefaultEdge )
                        createdEdge = graph.insertEdge( src, dst.edge )
                    else requestEdgeCreation(src, dst.node)
                }
            }
            if ( createdEdge ) {    // Notify graph user of the edge creation
                createdEdge.color = Qt.binding( function() { return visualConnector.edgeColor; } )
                edgeInserted( createdEdge );
                //if ( visualConnector.graph )
                //    visualConnector.graph.connectorEdgeInserted( createdEdge )
            }
        }
        onPressed : {
            mouse.accepted = true
            if ( graph &&
                 edgeItem &&
                 sourceNode &&
                 sourceNode.item ) {
                parent: graph.containerItem;
                edgeItem.graph = graph
                edgeItem.sourceItem = sourceNode.item
                edgeItem.destinationItem = visualConnector
                edgeItem.visible = true
            }
        }
    }
}

