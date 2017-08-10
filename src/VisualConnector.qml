/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
 *       with setSource() (or Qan.Graph.setConnectorSource() for default connector).
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
    /*! \brief Set the connector source node (ie the node that will display the control and that will be used as 'source' for edge creation).
     *
     * \note this item will automatically be reparented to host node, and displayed in host node top right.
     */
    function setSource( sourceNode ) {
        if ( sourceNode &&
             sourceNode.item ) {
            visualConnector.sourceNode = sourceNode
            visualConnector.parent = sourceNode.item
            visualConnector.x = Qt.binding( function(){ return sourceNode.item.width + leftMargin } )
            visualConnector.z = Qt.binding( function(){ return sourceNode.item.z + 1. } )
            if ( edgeItem )
                edgeItem.sourceItem = sourceNode.item
            if ( connectorItem ) {
                connectorItem.parent = visualConnector
                connectorItem.state = "NORMAL"
                connectorItem.visible = true
            }
            visualConnector.visible = true
        } else {
            visualConnector.visible = false
            if( visualConnector.edgeItem )
                visualConnector.edgeItem.visible = false
            if( visualConnector.connectorItem )
                visualConnector.connectorItem.visible = false
        }
    }
    onSourceNodeChanged: setSource(sourceNode)

    onVisibleChanged: {     // Note 20170323: Necessary for custom connectorItem until they are reparented to this
        if ( connectorItem )
            connectorItem.visible = visible && sourceNode   // Visible only if visual connector is visible and a valid source node is set
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
                connectorItem.state = "HILIGHT"
            }
        }
    }
    connectorItem: Rectangle {
        id: defaultConnectorItem
        parent: visualConnector
        anchors.fill: parent
        visible: false
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
                    else requestEdgeCreation(src, dst.edge)
                }
            }
            if ( createdEdge ) // Notify user of the edge creation
                createdEdge.color = Qt.binding( function() { return visualConnector.edgeColor; } )
                edgeInserted( createdEdge );
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

