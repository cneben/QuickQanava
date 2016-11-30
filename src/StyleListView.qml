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
// This file is a part of the QuickQanava software library. Copyright 2014 Benoit AUTHEMAN.
//
// \file	StyleListView.qml
// \author	benoit@destrat.io
// \date	2015 10 18
//-----------------------------------------------------------------------------

import QtQuick          2.7
import QtQuick.Layouts  1.3
import QtQuick.Controls 2.0

import "qrc:/QuickProperties"   as Qps
import QuickProperties 2.0      as Qps

import QuickQanava 2.0          as Qan

/*! \brief Show a selectable list of style with a live style preview.
 *
 */
Qps.PropertiesListView {
    id: styleListView

    // Public:
    //! Specify the delegate desired height for the style preview items (default to 55).
    property real   previewHeight: 55
    property var    graph: undefined

    //! Hilight a specific style, so its get the current selection and appears at the center of the view.
    function    hilightStyle( style ) {
        if ( model &&
             style) {
            var styleIndex = model.getStyleIndex( style ) // Note 20151028: See StylesFilterModel::getStyleIndex()
            if ( styleIndex !== -1 )
                currentIndex = styleIndex
        }
    }

    //! Emmited whenever a style is clicked by the user.
    signal  styleClicked( var style )
    //! Emmited whenever a style is double clicked by the user.
    signal  styleDoubleClicked( var style )

    // Private:
    delegate: Component {
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
                Drag.dragType: Drag.Automatic
                Drag.hotSpot.x: 10; Drag.hotSpot.y: 10
            }
            MouseArea {
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
}
