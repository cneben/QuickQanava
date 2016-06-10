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
// This file is a part of the QuickQanava software. Copyright 2015 Benoit AUTHEMAN.
//
// \file	NodeRectTemplate.qml
// \author	benoit@qanava.org
// \date	2015 11 30
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QuickQanava 2.0 as Qan

/*! \brief Default template component for building a custom rectangular qan::Node item.
 *
 * Node with custom content definition using "templates" is described in \ref qanavacustom
 */
Item {
    id: template

    implicitWidth: layout.implicitWidth
    implicitHeight: Math.max( layout.implicitHeight, parent.Layout.preferredHeight )

    property         var    node
    default property alias  children : contentLayout.children

    onNodeChanged: {
        if ( node != null && node != undefined )
            node.Drag.dragType = Drag.Internal
    }

    Rectangle {
        id: background
        anchors.fill: parent    // Background follow the content layout implicit size
        z: 1
        radius: 2
        color: node.style.backColor
        border.color: node.style.borderColor
        border.width: node.style.borderWidth
    }

    DropShadow {
        id: backgroundShadow
        anchors.fill: parent
        source: background
        horizontalOffset: node.style.shadowOffset.width
        verticalOffset: node.style.shadowOffset.height
        radius: 4
        samples: 16; smooth: true
        color: node.style.shadowColor
        visible: node.style.hasShadow
        transparentBorder: true
    }

    MouseArea {
        id: nodeDragArea
        anchors.fill: parent
        z: 2
        drag.target: ( node.draggable ? node : null )
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        smooth: true
        propagateComposedEvents: true
        hoverEnabled: true
        property bool nodeAcceptDrops: false
        drag.onActiveChanged: {
            if ( drag.active ) {
                nodeAcceptDrops = node.acceptDrops
                node.acceptDrops = false
            } else node.acceptDrops = nodeAcceptDrops
            // Node 20160104 Drag.active must be updated manually to initiate de new
            // Drag sequence when node is dragged
            node.Drag.active = node.draggable && drag.active
        }
        onPressed : {
            mouse.accepted = ( node.isInsideBoundingShape( Qt.point( mouse.x, mouse.y ) ) ? true : false )
            if ( mouse.accepted )
                node.nodeClicked( node, Qt.point( mouse.x, mouse.y ) )
        }
        // Quick Qanava node drag and drop to groups management
        onReleased: { node.dropNode( node.Drag.target ); }
        onPositionChanged: {
            if ( node.Drag.target !== null )
                node.proposeNodeDrop( node.Drag.target )
        }
        onClicked: {
            if ( mouse.button == Qt.LeftButton )
                node.nodeClicked( node, Qt.point( mouse.x, mouse.y ) )
            if ( mouse.button === Qt.RightButton )
                node.nodeRightClicked( node, Qt.point( mouse.x, mouse.y ) )
        }
        onDoubleClicked: { node.nodeDoubleClicked( node, Qt.point( mouse.x, mouse.y ) ) }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: background.radius
        z: 3
        Text {
            z: 4
            Layout.maximumWidth: template.parent.Layout.preferredWidth
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            text: node.label
            font: node.style.labelFont
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            elide: Text.ElideRight
            wrapMode: Text.Wrap
        }
        ColumnLayout {
            id: contentLayout
            z: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            //Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: contentLayout.children.length > 0  // Hide if the user has not added any content
        }
    }

    // Node intersection shape and symbol polygon management
    property var boundingShape: new Array( )
    onWidthChanged: { updateBoundingShape() }
    onHeightChanged: { updateBoundingShape() }
    function updateBoundingShape( ) {
        // Generate a standard rectangular bounding polygon
        var w = width /*- 1*/
        var h = height /*- 1*/
        boundingShape = new Array( )
        boundingShape.push( Qt.point( 1, 1 ) )
        boundingShape.push( Qt.point( w, 1 ) )
        boundingShape.push( Qt.point( w, h ) )
        boundingShape.push( Qt.point( 1, h ) )
        boundingShape.push( Qt.point( 1, 1 ) )
        node.setBoundingShape( boundingShape )
    }
    Component.onCompleted: { updateBoundingShape() }
}
