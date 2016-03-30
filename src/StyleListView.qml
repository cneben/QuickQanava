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
// \author	benoit@qanava.org
// \date	2015 10 18
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

import "qrc:/QuickProperties2" as Qps
import QuickProperties 2.0 as Qps

import QuickQanava 2.0 as Qan

/*! \brief Show a selectable list of style with a live style preview.
 *
 */
Qps.PropertiesListView {
    id: styleListView
    clip: true

    // Public:
    //! Specify the delegate desired height (default to list view width).
    property real   desiredDelegateWidth: styleListView.width

    //! Specify the delegate desired height (default to 55).
    property real   desiredDelegateHeight: 55

    //! Read only property containing the currently selected style (may be undefined).
    property var    selectedStyle: current

    //! Color used for the text inside this creator (default to black).
    property color  textColor: Qt.rgba( 0., 0., 0., 1.0)

    //! Hilight a specific style, so its get the current selection and appears at the center of the view.
    function    hilightStyle( style ) {
        currentIndex = model.getStyleIndex( style ) // Note 20151028: See StylesFilterModel::getStyleIndex()
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
            sourceComponent: selectStyleDelegate( itemData != null ? itemData.target : "",
                                                  itemData != null ? itemData.metaTarget : "" )
        }
    }

    function selectStyleDelegate( target, metaTarget ) {
        var styleTarget = ( target === "" || target.length == 0 ? metaTarget : target )
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
            property var delegateStyle : styleProperties
            onDelegateStyleChanged: {
                label.text = ""
                if ( delegateStyle != undefined && delegateStyle != null ) {
                    label.text = "Style:" + styleProperties.name
                }
            }
            width: desiredDelegateWidth
            height: desiredDelegateHeight
            Text { id: label; anchors.fill: parent; color: "black"; wrapMode: Text.Wrap; verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter }
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
            width: desiredDelegateWidth
            height: desiredDelegateHeight
            Item {
                id: nodeContainer
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                Component.onCompleted: {
                    if ( styleProperties == undefined || styleProperties == null )
                        return;
                    var styleTarget = ( styleProperties.target.length == 0 ? styleProperties.metaTarget :
                                                                             styleProperties.target ) // Targeting either target or metaTarget
                    var node = graph.createNodeItem( styleTarget );
                    node.parent = nodeContainer
                    node.label = styleProperties.name
                    node.anchors.fill = nodeContainer;
                    node.anchors.margins = 5

                    node.Layout.minimumWidth = width
                    node.Layout.minimumHeight = height

                    node.style = styleProperties
                    node.acceptDrops = false // Don't allow style DnD inside style browser
                    node.dropable = false     // Concern only QuickQanava group drops, set to false
                }
                property var draggedNodeStyle: styleProperties
                Drag.active: mouseArea.drag.active
                Drag.dragType: Drag.Automatic
                Drag.hotSpot.x: 10
                Drag.hotSpot.y: 10
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
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
            width: desiredDelegateWidth
            height: desiredDelegateHeight
            property var delegateStyle : styleProperties
            onDelegateStyleChanged: {
                label.text = ""
                edge.label = ""
                if ( delegateStyle != undefined && delegateStyle != null ) {
                    label.text = styleProperties.name
                    edge.label = styleProperties.name
                }
            }
            Qan.Edge {
                id: edge
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                label: styleProperties.name
                onWidthChanged: { setLine( Qt.point( 10, desiredDelegateHeight / 2 ),
                                           Qt.point( width - 10, desiredDelegateHeight / 2 ) ) }
                style: styleProperties
                property var draggedEdgeStyle: styleProperties
                Drag.active: mouseArea.drag.active
                Drag.dragType: Drag.Automatic
                Drag.hotSpot.x: 10
                Drag.hotSpot.y: 10
                acceptDrops: false // Don't allow style DnD inside style browser
            }
            Text {
                id: label
                x: ( edgeStyleDelegateWrapper.width - contentWidth ) / 2
                y: 3
                text: styleProperties.name
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                drag.target: edge
                onClicked:       { listView.currentIndex = styleIndex; styleClicked( styleProperties ) }
                onDoubleClicked: { listView.currentIndex = styleIndex; styleDoubleClicked( styleProperties ) }
            }
            Component.onCompleted: {
                console.debug( "edgeStyleDelegate() created: style.name=" + styleProperties.name )
            }
        }
    } // Component: edgeStyleDelegate
}
