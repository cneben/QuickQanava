/*
    This file is part of QuickContainers library.

    Copyright (C) 2016  Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickContainers software. Copyright 2015 Benoit AUTHEMAN.
//
// \file	PropertiesListView.qml
// \author	benoit@destrat.io
// \date	2015 06 19
//-----------------------------------------------------------------------------

import QtQuick          2.7
import QtQuick.Layouts  1.3
import QtQuick.Controls 2.0

/*! \brief Display a read-only list of Qps.Properties or qcm::TimedProperties object from a qcm::ObjectmodelList model.
 *
 */
ListView {
    id: propertiesListView

    clip: true
    flickableDirection : Flickable.VerticalFlick
    spacing: 4

    //! A defaut delegate is provided by QuickContainers.
    delegate: propertiesDefaultDelegate

    highlight: propertiesHighlightBar
    highlightFollowsCurrentItem: false
    focus: true

    //! List view default selection bar color (default to "lightsteelblue").
    property color  hilightColor: "lightsteelblue"
    //! List view default selection bar opacity (default to 85%).
    property real   hilightOpacity: 0.85
    //! Hilight rectangle corners radius (default to 2.0).
    property real   hilightRadius: 2

    //! Emmitted when a properties is selected with a click in this list view.
    signal propertiesClicked( var properties )
    //! Emmitted when a properties is selected with a double-click in this list view.
    signal propertiesDoubleClicked( var properties )
    onModelChanged: {
        if ( model &&               // Select the first "properties" when a new model is sets
             model.itemCount > 0 )
            currentIndex = 0
    }

    Component {
        id: propertiesDefaultDelegate
        Item {
            id: wrapper
            width: wrapper.ListView.view.width
            height: 35
            Label {
                anchors.verticalCenter: parent.verticalCenter;
                text: itemData.label
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    wrapper.ListView.view.currentIndex = index
                    propertiesListView.propertiesClicked( itemData )
                }
                onDoubleClicked: {
                    wrapper.ListView.view.currentIndex = index
                    propertiesListView.propertiesDoubleClicked( itemData )
                }
            }
        }
    }
    Component {
        id: propertiesHighlightBar
        Rectangle {
            x: propertiesListView.currentItem.x;         y: propertiesListView.currentItem.y
            width: propertiesListView.currentItem.width; height: propertiesListView.currentItem.height
            color: propertiesListView.hilightColor
            opacity: propertiesListView.hilightOpacity
            radius: hilightRadius
            Behavior on x { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
            Behavior on y { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
        }
    }
}
