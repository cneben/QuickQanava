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
// This file is a part of the QuickQanava software library. Copyright 2015 Benoit AUTHEMAN.
//
// \file	RectNode.qml
// \author	benoit@destrat.io
// \date	2017 03 17
//-----------------------------------------------------------------------------

import QtQuick              2.8
import QtQuick.Controls     2.1
import QtQuick.Layouts      1.3
import QtGraphicalEffects   1.0

import QuickQanava 2.0 as Qan

Qan.NodeItem {
    id: rectNode
    width: 160; height: 180
    x: 150; y: 15
    Rectangle {
        anchors.fill: parent
        radius: 10; color: "blue"
        border.color: "violet"; border.width: 2
    }
}
