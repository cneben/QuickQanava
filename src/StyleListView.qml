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
// This file is a part of the QuickQanava software library. Copyright 2014 Benoit AUTHEMAN.
//
// \file	StyleListView.qml
// \author	benoit@destrat.io
// \date	2015 10 18
//-----------------------------------------------------------------------------

import QtQuick          2.7
import QtQuick.Layouts  1.3
import QtQuick.Controls 2.0

import QuickQanava      2.0 as Qan
import "qrc:/QuickQanava"   as Qan

/*! \brief Show a selectable list of style with a live style preview.
 *
 */
ListView {
    id: styleListView

    clip: true
    flickableDirection : Flickable.VerticalFlick
    spacing: 4
    highlightFollowsCurrentItem: false
    focus: true

    //! List view default selection bar color (default to "lightsteelblue").
    property color  hilightColor: "lightsteelblue"
    //! List view default selection bar opacity (default to 85%).
    property real   hilightOpacity: 0.85
    //! Hilight rectangle corners radius (default to 2.0).
    property real   hilightRadius: 2

    // Public:
    //! Specify the delegate desired height for the style preview items (default to 55).
    property real   previewHeight: 55
    property var    graph: undefined

    property var styleManager: undefined
    onStyleManagerChanged: {
        if ( styleManager ) {
            model = styleManager.styles
            if ( model &&               // Select the first "properties" when a new model is sets
                 model.itemCount > 0 )
                currentIndex = 0
        } else model = null
    }
    highlight: Component {
        Rectangle {
            x: styleListView.currentItem.x;         y: styleListView.currentItem.y
            width: styleListView.currentItem.width; height: styleListView.currentItem.height
            color: styleListView.hilightColor
            opacity: styleListView.hilightOpacity
            radius: hilightRadius
            Behavior on x { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
            Behavior on y { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
        }
    }
    //! Hilight a specific style, so its get the current selection and appears at the center of the view.
    function    hilightStyle( style ) {
        if ( model &&
             style ) {
            // FIXME QAN3
            console.debug( "FIXME QAN3" )
            //var styleIndex = model.getStyleIndex( style )
            //if ( styleIndex !== -1 )
            //    currentIndex = styleIndex
        }
    }

    //! Emmited whenever a style is clicked by the user.
    signal  styleClicked( var style )
    //! Emmited whenever a style is double clicked by the user.
    signal  styleDoubleClicked( var style )

    // Private:
    delegate: Component {
        Item {
            id: styleDelegate
            width: styleListView.width;    height: previewHeight
            property var    styleItem:      itemData
            property var    styleComponent: styleManager.getStyleComponent(itemData)
            visible: styleItem && styleComponent
            property var primitive: undefined

            Item {
                id: delegateItem
                anchors.fill: parent
                anchors.leftMargin: 15; anchors.rightMargin: 15
                anchors.topMargin: 5; anchors.bottomMargin: 5
                z: 1
                Component.onCompleted: {
                    if ( !styleItem ||
                            !styleComponent ||
                            !styleListView.graph ) {
                        console.debug( "can't create delegate for styleItem=" + styleItem )
                        styleDelegate.visible = false
                        return
                    }
                    primitive = graph.createFromComponent( styleComponent, styleItem );
                    if ( primitive ) {
                        primitive.parent = delegateItem
                        primitive.anchors.fill = delegateItem;
                        primitive.anchors.margins = 5
                        if ( primitive.objectName === "qan::NodeItem" ) {
                            primitive.resizable = false
                            primitive.Layout.minimumWidth = width
                            primitive.Layout.minimumHeight = height
                            primitive.selectable = false
                            primitive.acceptDrops = false    // Don't allow style DnD inside style browser
                            primitive.droppable = false      // Concern only QuickQanava group drops, set to false
                            primitive.draggable = false      // Concern only QuickQanava group drops, set to false
                            var delegateLabel = Qt.createQmlObject( 'import QtQuick 2.7;import QtQuick.Controls 2.0; Label { anchors.centerIn: parent; text: "Node Style" }', primitive )
                            if ( delegateLabel )
                                delegateLabel.text = qsTr( "Node Style" )
                            primitive.nodeClicked.connect(onNodeClicked)
                            primitive.nodeDoubleClicked.connect(onNodeDoubleClicked)
                            primitive.style = styleItem
                        } else if ( primitive.objectName === "qan::EdgeItem" ) {
                            primitive.setLine( Qt.point( 15, height / 1.5 ),
                                              Qt.point( width - 15, height / 1.5 ) );
                            primitive.acceptDrops = false
                            var delegateLabel = Qt.createQmlObject( 'import QtQuick 2.7;import QtQuick.Controls 2.0; Label { anchors.centerIn: parent; text: "Edge Style" }', primitive )
                            if ( delegateLabel )
                                delegateLabel.text = qsTr( "Edge Style" )
                        } else if ( primitive.objectName === "qan::GroupItem" ) {
                            primitive.selectable = false
                            primitive.draggable = false
                            styleDelegate.visible = false  // Do not show group styles (for the moment 20170317)
                        }
                    }
                    function onNodeClicked(node) { styleListView.currentIndex = index; styleListView.styleClicked( styleItem ) }
                    function onNodeDoubleClicked(node) { styleListView.currentIndex = index; styleListView.styleDoubleClicked( styleItem ) }
                } // Component.onCompleted()
                property var draggedStyle: styleItem // Used from C++ for detecting style drop (see qan::DraggableCtrl<>::handleDropEvent())
                Drag.active: delegateMouseArea.drag.active
                Drag.dragType: Drag.Automatic
                Drag.hotSpot.x: 10; Drag.hotSpot.y: 10
            }
            MouseArea {
                z: 2
                id: delegateMouseArea
                anchors.fill: parent
                drag.target: delegateItem
                //drag.target: primitive
                onClicked:       { styleListView.currentIndex = index; styleListView.styleClicked( styleItem ) }
                onDoubleClicked: { styleListView.currentIndex = index; styleListView.styleDoubleClicked( styleItem ) }
            }
        } // Item: delegate
    } // Component: delegate
}
