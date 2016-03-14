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
// This file is a part of the QuickProperties2 library. Copyright 2014 Benoit AUTHEMAN.
//
// \file	PropertiesView.qml
// \author	benoit@qanava.org
// \date	2015 05 02
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2

/*! \brief Display a read-only list of property from a QpsProperties or qps::TimedProperties object.
 *
 */
ListView {
    id: propertiesListView
    clip: true
    flickableDirection : Flickable.HorizontalAndVerticalFlick
    spacing: 10
    delegate: Item {
        width: propertiesListView.width
        height: propertyDescription.contentHeight
        Text { id: propertyDescription; text: propertyName + "=" + propertyData }
    }
}
