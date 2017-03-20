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
// \file	CustomNode.qml
// \author	benoit@destrat.io
// \date	2015 08 01
//-----------------------------------------------------------------------------

import QtQuick              2.8
import QtQuick.Controls     2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts      1.3
import QtGraphicalEffects   1.0

import QuickQanava          2.0 as Qan

Qan.NodeItem {
    id: roundNode
    width: 60; height: 60
    minimumSize: Qt.size(60,60)
    x: 15;      y: 15
    Rectangle {
        id: background
        z: 1
        Material.onThemeChanged: {
            console.debug( "Material.theme=" + Material.theme )
            console.debug( "Material.foreground=" + Material.foreground )
            console.debug( "Material.background=" + Material.background )
        }
        anchors.fill: parent
        radius: width / 2; color: "white"
        border.color: Material.accent; border.width: 2
    }
    property color nodeColor: Qt.rgba( style.backColor.r, style.backColor.g, style.backColor.b, 0.2 )
    property color backColor: Material.background//Material.background
    onBackColorChanged: {
        console.debug( "backColor=" + backColor )
        console.debug( "tinted=" + Qt.tint( roundNode.backColor, roundNode.nodeColor ) )
    }
    LinearGradient {
        anchors.fill: parent
        z: 2
        source: background
        start: Qt.point(0.,0.)
        end: Qt.point(background.width, background.height)
        gradient: Gradient {
            id: backGrad
            GradientStop { position: 0.0; color: roundNode.nodeColor }
            GradientStop {
                position: 1.0;
                //color: Qt.tint( roundNode.backColor, roundNode.nodeColor )
                color: Qt.tint( roundNode.nodeColor, roundNode.backColor )
            }
        }
    }

    Label {
        text: "youpi"
        z: 3
        anchors.centerIn: parent
    }
    Glow {
        z: 0
        source: background
        anchors.fill: parent
        color: Material.theme === Material.Light ? Qt.lighter( Material.foreground ) : Qt.darker( Material.foreground )
        radius: 12;     samples: 15
        spread: 0.25;   transparentBorder: true
    }
}
