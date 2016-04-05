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
// This file is a part of the QuickQanva software. Copyright 2014 Benoit AUTHEMAN.
//
// \file	NodeCanvasTemplate.qml
// \author	benoit@qanava.org
// \date	2015 11 30
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QuickQanava 2.0 as Qan


/*! \brief Default component template for building a custom qan::Node Item with an arbitrary geometry drawn into a JS Canvas2D.
 *
 * Node with custom content definition using "templates" is described in \ref qanavacustom
 */
Item {
    id: template

    implicitWidth: nodeLayout.implicitWidth
    implicitHeight: nodeLayout.implicitHeight

    property         var    node
    property         alias  symbol: nodeSymbol.sourceComponent
    default property alias  children : templateContentLayout.children

    Drag.active: node.dropable && nodeDragArea.drag.active
    Drag.dragType: Drag.Internal
    onNodeChanged: {
        if ( node != null && node != undefined )
            node.Drag.dragType = Drag.Internal
    }
    function requestPaint( ) {
        if ( nodeSymbol.item != null )
            nodeSymbol.item.requestPaint( );
    }
    Loader {
        id: nodeSymbol
        anchors.fill: parent
        onItemChanged: {
            if ( item != null )
                backgroundShadow.source = item
        }
    }
    DropShadow {    // Effect source property set in nodeSymbol Loader onItemChanged()
        id: backgroundShadow
        anchors.fill: parent
        horizontalOffset: node.style.shadowOffset.width
        verticalOffset: node.style.shadowOffset.height
        radius: 8.0
        samples: 16
        smooth: true
        color: node.style.shadowColor
        visible: node.style.hasShadow
        transparentBorder: true
    }

    ColumnLayout {
        id: nodeLayout
        anchors.fill: parent
        z: 3
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Text {
            id: nodeLabel
            Layout.fillWidth: true;
            Layout.maximumWidth: template.parent.Layout.preferredWidth
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.margins: 5
            verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
            text: node.label
            font: node.style.labelFont
            wrapMode: Text.Wrap
            elide: Text.ElideRight
        }
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            id: templateContentLayout
            z: 4
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: templateContentLayout.children.length > 0  // Hide if the user has not added any content
        }
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
        onPressed : {
            //console.debug( "onPressed() pos=" + mouse.x + "," + mouse.y + "  accepted=" + node.isInsideIntersectionShape( Qt.point( mouse.x, mouse.y ) ) );
            mouse.accepted = ( node.isInsideBoundingShape( Qt.point( mouse.x, mouse.y ) ) ? true : false )
            if ( mouse.accepted )
                node.nodeClicked( node, Qt.point( mouse.x, mouse.y ) )
        }
        onReleased: { node.dropNode( node.Drag.target ); } // Quick Qanava node drag and drop to groups management
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

    // Node intersection shape and symbol polygon management
    onWidthChanged: {
        if ( nodeSymbol.item != null )
            nodeSymbol.item.updateSymbolPolygon()
    }
    onHeightChanged: {
        if ( nodeSymbol.item != null )
            nodeSymbol.item.updateSymbolPolygon()
    }
    Component.onCompleted: {
        if ( nodeSymbol.item != null )
            nodeSymbol.item.updateSymbolPolygon()
    }
}
