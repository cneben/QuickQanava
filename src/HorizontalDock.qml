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
// \file	HorizontalDock.qml
// \author	benoit@destrat.io
// \date	2017 08 28
//-----------------------------------------------------------------------------
import QtQuick 2.2
import QtQuick.Layouts 1.1

import QuickQanava 2.0 as Qan

RowLayout {
    id: root
    spacing: 15
    z: 1.5 // Selection item z=1.0, dock must be on top of selection
    states: [
        State {
            name: "top"
            when: hostNodeItem && dockType === Qan.NodeItem.Top

            AnchorChanges {
                target: root
                anchors {
                    bottom: hostNodeItem.top
                    horizontalCenter: hostNodeItem.horizontalCenter
                }
            }

            PropertyChanges {
                target: root
                bottomMargin: root.bottomMargin
            }
        },
        State {
            name: "bottom"
            when: hostNodeItem && dockType === Qan.NodeItem.Bottom

            AnchorChanges {
                target: root
                anchors {
                    top: hostNodeItem.bottom
                    horizontalCenter: hostNodeItem.horizontalCenter
                }
            }

            PropertyChanges {
                target: root
                topMargin: root.topMargin
            }
        }
    ]

    property var hostNodeItem: undefined
    property int dockType: -1
    property int topMargin: 7
    property int bottomMargin: 7
}
