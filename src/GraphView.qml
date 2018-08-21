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

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library. Copyright 2015 Benoit AUTHEMAN.
//
// \file	Graph.qml
// \author	benoit@destrat.io
// \date	2015 08 01
//-----------------------------------------------------------------------------

import QtQuick          2.7
import QtQuick.Shapes   1.0

import QuickQanava          2.0 as Qan

/*! \brief Visual view for a Qan.Graph component.
 *
 *  Set the graph property to Qan.Graph{}.
 *
 * Visual connection of nodes:
 *   \li Visual connection of nodes with VisualConnector is enabled by setting the \c enableVisualConnector property to \c true (default to false).
 *   \li Class name for the edges created by the visual node connector could be changed with property \c connectorEdgeClassName (default to qan::Edge).
 *   \li When an edge is created with the visual node connector, the signal \c edgeInsertedVisually with the newly inserted \c edge as an argument.
 *
 */
Qan.AbstractGraphView {
    id: graphView
    Qan.LineGrid { id: lineGrid }
    grid: lineGrid

    property real maxZ: -1.    // Node management ////////////////////////////////////////////////////////

    property color  gridThickColor: grid ? grid.thickColor : lineGrid.thickColor
    onGridThickColorChanged: {
        if ( grid )
            grid.thickColor = gridThickColor
    }

    property color  resizeHandlerColor: Qt.rgba(0.117, 0.564, 1.0)  // dodgerblue=rgb( 30, 144, 255)
    property real   resizeHandlerOpacity: 1.0
    property real   resizeHandlerRadius: 4.0
    property real   resizeHandlerWidth: 4.0
    property size   resizeHandlerSize: "9x9"

    Qan.BottomRightResizer {
        id: nodeResizer
        parent: graph.containerItem
        visible: false

        opacity: resizeHandlerOpacity
        handlerColor: resizeHandlerColor
        handlerRadius: resizeHandlerRadius
        handlerWidth: resizeHandlerWidth
        handlerSize: resizeHandlerSize
    }
    Qan.BottomRightResizer {
        id: groupResizer
        parent: graph.containerItem
        visible: false

        opacity: resizeHandlerOpacity
        handlerColor: resizeHandlerColor
        handlerRadius: resizeHandlerRadius
        handlerWidth: resizeHandlerWidth
        handlerSize: resizeHandlerSize
    }

    // View Click management //////////////////////////////////////////////////
    onClicked: {
        // Hide resizers when view background is clicked
        nodeResizer.target = null
        nodeResizer.visible = false
        groupResizer.target = null
        groupResizer.visible = false

        graphView.focus = true           // User clicked outside a graph item, remove it's eventual active focus
    }
    onRightClicked: graphView.focus = true

    // Port management ////////////////////////////////////////////////////////
    onPortClicked: {
        if ( graph &&
             port ) {
            if (port.node)    // Force port host node on top
                sendToTop(port.node)
            if (graph.connector &&
                graph.connectorEnabled)
                graph.connector.sourcePort = port
        } else if (graph) {
            graph.connector.visible = false
        }
    }
    onPortRightClicked: { }

    // Node management ////////////////////////////////////////////////////////
    function    sendToTop(node) {
        if (node) {
            if (node.item) {
                maxZ = Math.max( node.item.z + 1, maxZ + 1 )
                node.item.z = maxZ + 1;
            }
            if ( node.group )
                sendGroupToTop(node.group)
        }
    }

    // Dynamically handle currently selected node item onRatioChanged() signal
    Connections { // and update nodeResizer ratio policy (selected node is nodeResizer target)
        id: nodeItemRatioWatcher
        target: null
        onRatioChanged: {
            if ( nodeResizer &&
                 target &&
                 nodeResizer.target === target ) {
                nodeResizer.preserveRatio = target.ratio > 0.
                nodeResizer.ratio = target.ratio
            }
        }
    }

    onNodeClicked: {
        if ( graph &&
             node &&
             node.item ) {
            sendToTop(node)
            if ( graph.connector &&
                 graph.connectorEnabled &&
                 ( node.item.connectable === Qan.NodeItem.Connectable ||
                   node.item.connectable === Qan.NodeItem.OutConnectable ) ) {      // Do not show visual connector if node is not visually "connectable"
                graph.connector.sourceNode = node
                // FIXME: connector remove that....
                graph.connector.y = -graph.connector.height / 2
            }
            if ( node.item.resizable ) {
                nodeItemRatioWatcher.target = node.item
                nodeResizer.parent = node.item
                nodeResizer.target = null   // Note: set resizer target to null _before_ settings minimum target size
                nodeResizer.minimumTargetSize = node.item.minimumSize   // to avoid old target beeing eventually resized to new target min size...
                nodeResizer.target = node.item
                nodeResizer.visible = Qt.binding( function() { return nodeResizer.target.resizable; } )
                nodeResizer.z = node.item.z + 4.    // We want resizer to stay on top of selection item and ports.
                nodeResizer.preserveRatio = (node.item.ratio > 0.)
                if (node.item.ratio > 0. ) {
                    nodeResizer.ratio = node.item.ratio
                    nodeResizer.preserveRatio = true
                } else
                    nodeResizer.preserveRatio = false
            } else {
                nodeResizer.target = null
                nodeResizer.visible = false
            }
        } else if ( graph ) {
            nodeItemRatioWatcher.target = null
            graph.connector.visible = false
            resizer.visible = false
        }
    }

    // Group management ///////////////////////////////////////////////////////
    function sendGroupToTop(group) {
        if ( group && group.item ) {
            maxZ = Math.max( group.item.z + 1, maxZ + 1 )
            group.item.z = maxZ + 1;
        }
    }

    onGroupClicked: {
        sendGroupToTop(group)

        // FIXME: fast exit on incorrect argument...
        if ( !graph )
            return
        if ( !group || !(group.item) )
            return
        if ( !group.item.container ) {
            console.error("Qan.GraphView.onGroupClicked(): Trying to add a bottom right resizer to a group with invalid container property")
            return
        }

        if ( group.item.resizable ) {
            groupResizer.parent = group.item
            groupResizer.target = null   // Note: set resizer target to null _before_ settings minimum target size
            groupResizer.minimumTargetSize = Qt.binding( function() {
                return Qt.size( Math.max( group.item.minimumSize.width, group.item.container.childrenRect.x + group.item.container.childrenRect.width + 10 ),
                                Math.max( group.item.minimumSize.height, group.item.container.childrenRect.y + group.item.container.childrenRect.height + 10 ) )
            } )
            groupResizer.target = group.item
            // Do not show resizer when group is collapsed
            groupResizer.visible = Qt.binding( function() { return (!groupResizer.target.collapsed) && groupResizer.target.resizable; } )
            groupResizer.z = group.item.z + 4.    // We want resizer to stay on top of selection item and ports.
            groupResizer.preserveRatio = false
        } else {
            groupResizer.target = null
            groupResizer.visible = false
        } // group.item.resizable
    }

    onGroupRightClicked: sendGroupToTop(group)
    onGroupDoubleClicked: sendGroupToTop(group)
} // Qan.GraphView

