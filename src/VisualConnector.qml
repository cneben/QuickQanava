/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
 *       or port by setting property \c sourceNode or \c sourcePort (or Qan.Graph.setConnectorSource() for default connector).
 */
Qan.Connector {
    id: visualConnector
    width: radius * 2;  height: radius * 2
    x: parent.width + connectorMargin;  y: topMargin

    visible: false
    selectable: false
    clip: false; antialiasing: true

    // Public
    //! Edge color (default to black).
    property color  edgeColor: Qt.rgba(0,0,0,1)

    //! Connector control radius (final diameter will be radius x 2).
    property real   radius: 8

    //! Connector color (default to dodgerblue).
    property color  connectorColor: "dodgerblue"

    //! Default connector line width (default to 2.0).
    property real   connectorLineWidth: 2

    //! Maximum connector line width, used to visually hilight that te current drag "target" can be connected (default to 5.0).
    property real   connectorHilightLineWidth: 4

    //! Margin added between source (node or port) and this connector.
    property real   connectorMargin: 7

    //! Use that property with custom connector item to ensure they are anchored under default connector item.
    property real   topMargin: 0

    //! True when the connector item is currently dragged.
    property bool   connectorDragged: dropDestArea.drag.active

    edgeComponent: Component{ Qan.Edge{} }
    onEdgeColorChanged: {
        if (edgeItem)
            edgeItem.color = edgeColor
    }

    // Private properties

    /*! \brief Internally used to reset correct connector position according to current node
     *  or port configuration, also restore position bindings to source.
     */
    function configureConnectorPosition() {
        if ( sourcePort ) {
            switch ( sourcePort.dockType ) {
            case Qan.NodeItem.Left:
                visualConnector.x = Qt.binding( function(){ return -width - connectorMargin } )
                visualConnector.y = Qt.binding( function(){ return ( sourcePort.height - visualConnector.height ) / 2 } )
                visualConnector.z = Qt.binding( function(){ return sourcePort.z + 1. } )
                break;
            case Qan.NodeItem.Top:
                visualConnector.x = Qt.binding( function(){ return ( sourcePort.width + connectorMargin ) } )
                visualConnector.y = Qt.binding( function(){ return ( sourcePort.height - visualConnector.height ) / 2 } )
                visualConnector.z = Qt.binding( function(){ return sourcePort.z + 1. } )
                break;
            case Qan.NodeItem.Right:
                visualConnector.x = Qt.binding( function(){ return sourcePort.width + connectorMargin } )
                visualConnector.y = Qt.binding( function(){ return ( sourcePort.height - visualConnector.height ) / 2 } )
                visualConnector.z = Qt.binding( function(){ return sourcePort.z + 1. } )
                break;
            case Qan.NodeItem.Bottom:
                visualConnector.x = Qt.binding( function(){ return ( sourcePort.width + connectorMargin ) } )
                visualConnector.y = Qt.binding( function(){ return ( sourcePort.height - visualConnector.height ) / 2 } )
                visualConnector.z = Qt.binding( function(){ return sourcePort.z + 1. } )
                break;
            }
        } else if ( sourceNode ) {
            visualConnector.x = Qt.binding( function(){ return sourceNode.item.width + connectorMargin } )
            visualConnector.y = -visualConnector.height / 2 + visualConnector.topMargin
            visualConnector.z = Qt.binding( function(){ return sourceNode.item.z + 1. } )
        }
    }

    onSourcePortChanged: configureConnectorPosition()
    onSourceNodeChanged: configureConnectorPosition()

    onVisibleChanged: {     // Note 20170323: Necessary for custom connectorItem until they are reparented to this
        if ( connectorItem )
            connectorItem.visible = visible && sourceNode   // Visible only if visual connector is visible and a valid source node is set
    }

    Drag.active: dropDestArea.drag.active
    Drag.dragType: Drag.Internal
    Drag.onTargetChanged: { // Hilight a target node
        if ( Drag.target ) {
            visualConnector.z = Drag.target.z + 1
            if ( connectorItem )
                connectorItem.z = Drag.target.z + 1
        }
        if ( !Drag.target &&
             connectorItem ) {
            connectorItem.state = "NORMAL"
        } else {
            if ( sourceNode &&
                 sourceNode.item &&
                 Drag.target === sourceNode.item ) {    // Do not create a circuit on source node
                 connectorItem.state = "NORMAL"
            } else if ( ( Drag.target.node &&
                          ( Drag.target.connectable === Qan.NodeItem.Connectable ||
                            Drag.target.connectable === Qan.NodeItem.InConnectable    ) ) ||             // Hilight only on a node target OR an edge target IF hyper edge creation is enabled
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
            if ( connectorItem.state === "HILIGHT" ) {
                connectorReleased(visualConnector.Drag.target)
                configureConnectorPosition()
            } else {
                edgeItem.visible = false
                configureConnectorPosition()
            }
        }
        onPressed : {
            mouse.accepted = true
            connectorPressed()
        }
    } // MouseArea: dropDestArea
}

