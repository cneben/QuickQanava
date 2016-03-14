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
// This file is a part of the Qanava software. Copyright 2015 Benoit AUTHEMAN.
//
// \file	QanRectNode.qml
// \author	benoit@qanava.org
// \date	2015 06 16
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QuickQanava 2.0 as Qan
import "." as Qan

Qan.AbstractNode {
    id: rectNode
    Layout.preferredWidth: 110
    Layout.preferredHeight: 50
    width: Layout.preferredWidth
    height: Layout.preferredHeight
    Qan.RectNodeTemplate {
        anchors.fill: parent
        node : rectNode
    }
}
