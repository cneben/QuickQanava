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
<<<<<<< HEAD
=======
import "qrc:/QuickQanava"   as Qan
>>>>>>> dev

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

<<<<<<< HEAD
    /*onModelChanged: {
        if ( model &&               // Select the first "properties" when a new model is sets
             model.itemCount > 0 )
            currentIndex = 0
    }*/
    highlight: propertiesHighlightBar
    Component {
        id: propertiesHighlightBar
=======
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
>>>>>>> dev
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
<<<<<<< HEAD
            var styleIndex = model.getStyleIndex( style ) // Note 20151028: See StylesFilterModel::getStyleIndex()
            if ( styleIndex !== -1 )
                currentIndex = styleIndex
=======
            // FIXME QAN3
            console.debug( "FIXME QAN3" )
            //var styleIndex = model.getStyleIndex( style )
            //if ( styleIndex !== -1 )
            //    currentIndex = styleIndex
>>>>>>> dev
        }
    }

    //! Emmited whenever a style is clicked by the user.
    signal  styleClicked( var style )
    //! Emmited whenever a style is double clicked by the user.
    signal  styleDoubleClicked( var style )

    // Private:
    delegate: Component {
<<<<<<< HEAD
        Loader {
            property var    listView:           styleListView
            property var    styleIndex:         index
            property var    styleProperties:    itemData
            sourceComponent: selectStyleDelegate( itemData ? itemData.target : "",
                                                  itemData ? itemData.metaTarget : "" )
        }
    }
    function selectStyleDelegate( target, metaTarget ) {
        var styleTarget = ( metaTarget === "" ? target :
                                                metaTarget ) // Note 20160404: Use meta target by default
        switch ( styleTarget ) {
        case "qan::Node": return nodeStyleDelegate
        case "qan::Edge": return edgeStyleDelegate
        case "qan::HEdge": return edgeStyleDelegate
        default: break
        }
        return defaultStyleDelegate
    }
    Component {
        id: defaultStyleDelegate
        Item {
            width: styleListView.width; height: previewHeight
            Label {
                id: label;
                anchors.fill: parent;
                wrapMode: Text.Wrap;
                verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
                text: styleProperties ? "Style:" + styleProperties.name : ""
            }
            MouseArea {
                anchors.fill: parent
                onClicked:       { listView.currentIndex = styleIndex; styleClicked( styleProperties ) }
                onDoubleClicked: { listView.currentIndex = styleIndex; styleDoubleClicked( styleProperties ) }
            }
        }
    } // Component: defaultStyleDelegate
    Component {
        id: nodeStyleDelegate
        Item {
            id: nodeStyleDelegateWrapper
            width: styleListView.width;    height: previewHeight
            Item {
                id: nodeContainer
                anchors.fill: parent; anchors.leftMargin: 10; anchors.rightMargin: 10
                Component.onCompleted: {
                    if ( !styleProperties )
                        return;
                    var styleTarget = ( styleProperties.target.length === 0 ? styleProperties.metaTarget :
                                                                              styleProperties.target ) // Targeting either target or metaTarget
                    if ( styleListView.graph ) {
                        var node = graph.createNodeItem( styleTarget );
                        if ( node ) {
                            node.parent = nodeContainer
                            node.label = styleProperties.name
                            node.anchors.fill = nodeContainer; node.anchors.margins = 5
                            node.resizable = false

                            node.Layout.minimumWidth = width
                            node.Layout.minimumHeight = height

                            node.style = styleProperties
                            node.acceptDrops = false    // Don't allow style DnD inside style browser
                            node.dropable = false       // Concern only QuickQanava group drops, set to false
                        }
                    }
                }
                property var draggedNodeStyle: styleProperties  // Used from C++ for drop
                Drag.active: mouseArea.drag.active
                Drag.dragType: Drag.Automatic
                Drag.hotSpot.x: 10; Drag.hotSpot.y: 10
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                anchors.leftMargin: 15; anchors.rightMargin: 15
                anchors.topMargin: 5; anchors.bottomMargin: 5
                drag.target: nodeContainer
                onClicked:       { listView.currentIndex = styleIndex; styleClicked( styleProperties ) }
                onDoubleClicked: { listView.currentIndex = styleIndex; styleDoubleClicked( styleProperties ) }
            }
        }
    } // Component: nodeStyleDelegate
    Component {
        id: edgeStyleDelegate
        Item {
            id: edgeStyleDelegateWrapper
            width: styleListView.width;    height: previewHeight
            Item {
                id: edgeContainer
                anchors.fill: parent; anchors.leftMargin: 10; anchors.rightMargin: 10
                Component.onCompleted: {
                    if ( !styleProperties )
                        return;
                    var styleTarget = ( styleProperties.target.length === 0 ? styleProperties.metaTarget :
                                                                              styleProperties.target ) // Targeting either target or metaTarget
                    if ( styleListView.graph ) {
                        var edge = graph.createEdgeItem( styleTarget );
                        if ( edge ) {
                            edge.parent = edgeContainer
                            edge.label = styleProperties.name
                            edge.anchors.fill = edgeContainer; edge.anchors.margins = 5
                            edge.setLine( Qt.point( 15, previewHeight / 2 ),
                                          Qt.point( width - 15, previewHeight / 2 ) );
                            edge.style = styleProperties
                            edge.acceptDrops=false
                        }
                    }
                }
                property var draggedEdgeStyle: styleProperties      // Used from C++ for drop
                Drag.active: mouseArea.drag.active
=======
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
                    console.debug( "Qan.StyleListView.delegate.onCompleted():")
                    console.debug( "\tstyleItem=" + styleItem )
                    console.debug( "\tstyleComponent=" + styleComponent )
                    console.debug( "styleManager.getStyleComponent(itemData)=" + styleManager.getStyleComponent(styleItem) );
                    if ( !styleItem ||
                            !styleComponent ||
                            !styleListView.graph ) {
                        console.debug( "can't create delegate for styleItem=" + styleItem )
                        styleDelegate.visible = false
                        return
                    }
                    primitive = graph.createFromComponent( styleComponent, styleItem );
                    console.debug( "primitive=" + primitive )
                    if ( primitive ) {
                        primitive.parent = delegateItem
                        primitive.anchors.fill = delegateItem;
                        primitive.anchors.margins = 5
                        console.debug( "primitive.style=" + primitive.style )
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
>>>>>>> dev
                Drag.dragType: Drag.Automatic
                Drag.hotSpot.x: 10; Drag.hotSpot.y: 10
            }
            MouseArea {
<<<<<<< HEAD
                id: mouseArea
                anchors.fill: parent;
                anchors.leftMargin: 15; anchors.rightMargin: 15
                anchors.topMargin: 5; anchors.bottomMargin: 5
                drag.target: edgeContainer
                onClicked:       { listView.currentIndex = styleIndex; styleClicked( styleProperties ) }
                onDoubleClicked: { listView.currentIndex = styleIndex; styleDoubleClicked( styleProperties ) }
            }
            Label {
                id: label
                anchors.horizontalCenter: parent.horizontalCenter;  anchors.topMargin: 8
                text: styleProperties ? styleProperties.name : ""
            }
        }
    } // Component: edgeStyleDelegate
=======
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
>>>>>>> dev
}
