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
// This file is a part of the QuickQanva software. Copyright 2014 Benoit AUTHEMAN.
//
// \file	NodeCanvasTemplate.qml
// \author	benoit@destrat.io
// \date	2015 11 30
//-----------------------------------------------------------------------------

import QtQuick              2.7
import QtQuick.Layouts      1.3
import QtGraphicalEffects   1.0

import QuickQanava          2.0 as Qan


/*! \brief Default component template for building a custom qan::Node Item with an arbitrary geometry drawn into a JS Canvas2D.
 *
 * Node with custom content definition using "templates" is described in \ref qanavacustom
 */
Item {
    id: template
    property         var    nodeItem : undefined
    property         alias  symbol   : nodeSymbol.sourceComponent
    default property alias  children : templateContentLayout.children

    function requestPaint( ) {
        if ( nodeSymbol.item )
            nodeSymbol.item.requestPaint( );
    }
    onNodeItemChanged: {
        if ( nodeItem )
            nodeUpdateBoundingShape.target = nodeItem
    }
    Loader {    // Node symbol is node background
        id: nodeSymbol
        anchors.fill: parent
        onItemChanged: {
            if ( item )
                backgroundShadow.source = item
        }
    }
    DropShadow {    // Effect source property set in nodeSymbol Loader onItemChanged()
        id: backgroundShadow
        anchors.fill: parent
        horizontalOffset: nodeItem.node.style.shadowOffset.width
        verticalOffset: nodeItem.node.style.shadowOffset.height
        radius: 8.0
        samples: 16
        smooth: true
        color: nodeItem.node.style.shadowColor
        visible: nodeItem.node.style.hasShadow
        transparentBorder: true
    }

    ColumnLayout {
        id: nodeLayout
        anchors.fill: parent
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        visible: !labelEditor.visible
        Text {
            id: nodeLabel
            Layout.fillWidth: true;
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.margins: 5
            width: parent.width
            verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
            text: nodeItem.node.label
            font: nodeItem.node.style.labelFont
            wrapMode: Text.Wrap;    elide: Text.ElideRight; maximumLineCount: 4
        }
        Item {
            id: templateContentLayout
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true; Layout.fillHeight: true
            visible: templateContentLayout.children.length > 0  // Hide if the user has not added any content
        }
    }
    Connections {
        target: nodeItem
        onNodeDoubleClicked: labelEditor.visible = true
    }
    NodeLabelEditor {
        id: labelEditor
        anchors.fill: parent
        anchors.margins: 4
        node: parent.nodeItem
    }
    // Bounding shape management
    Connections {
        id: nodeUpdateBoundingShape
        onUpdateBoundingShape: {
            if ( nodeSymbol.item )
                nodeSymbol.item.updateSymbolPolygon()
        }
    }
    Component.onCompleted: {
        if ( nodeSymbol.item )
            nodeSymbol.item.updateSymbolPolygon()
    }
}
