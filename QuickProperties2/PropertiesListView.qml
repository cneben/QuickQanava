/*
    This file is part of QuickProperties2 library.

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
// This file is a part of the QuickProperties software. Copyright 2015 Benoit AUTHEMAN.
//
// \file	PropertiesListView.qml
// \author	benoit@qanava.org
// \date	2015 06 19
//-----------------------------------------------------------------------------

import QtQuick 2.2
//import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4

/*! \brief Display a read-only list of Qps.Properties or qps::TimedProperties object from a qps::PropertiesList model.
 *
 */
ListView {
    id: propertiesListView

    clip: true
    flickableDirection : Flickable.VerticalFlick
    spacing: 4

    //! A defaut delegate is provided by QuickProperties.
    delegate: propertiesDefaultDelegate

    highlight: propertiesHighlightBar
    highlightFollowsCurrentItem: false
    focus: true

    //! Component text color (default to black).
    property color  textColor: Qt.rgba( 0., 0., 0., 1. )
    //! List view default selection bar color (default to "lightsteelblue").
    property color  hilightColor: "lightsteelblue"
    //! List view default selection bar opacity (default to 85%).
    property real   hilightOpacity: 0.85
    //! Hilight rectangle corners radius (default to 2.0).
    property real   hilightRadius: 2

    //! Read only currently selected properties object (may be undefined).
    property var    current: undefined

    //! Emmitted when a properties is selected with a click in this list view.
    signal propertiesClicked( var properties )
    //! Emmitted when a properties is selected with a double-click in this list view.
    signal propertiesDoubleClicked( var properties )

    onModelChanged: {   // Select the first "properties" when a new model is sets
        if ( model.itemCount > 0 )
            currentIndex = 0
    }
    onCurrentIndexChanged: {    // Change current "properties" when the selection change
        if ( currentIndex >= 0 )
            current = model.itemAt( currentIndex )
    }

    Component {
        id: propertiesDefaultDelegate
        Item {
            id: wrapper
            width: wrapper.ListView.view.width - 2
            height: 30
            Text {
                anchors.verticalCenter: parent.verticalCenter;
                width: 100
                text: item.label
                color: propertiesListView.textColor
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    wrapper.ListView.view.currentIndex = index
                    propertiesListView.propertiesClicked( item )
                }
                onDoubleClicked: {
                    wrapper.ListView.view.currentIndex = index
                    propertiesListView.propertiesDoubleClicked( item )
                }
            }
        }
    }

    Component {
        id: propertiesHighlightBar
        Rectangle {
            x: propertiesListView.currentItem.x
            y: propertiesListView.currentItem.y

            width: propertiesListView.currentItem.width
            height: propertiesListView.currentItem.height

            color: propertiesListView.hilightColor
            opacity: propertiesListView.hilightOpacity
            radius: hilightRadius

            Behavior on x { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
            Behavior on y { SpringAnimation { duration: 150; spring: 1.8; damping: 0.12 } }
        }
    }
}
