/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software. Copyright 2017 Benoit AUTHEMAN.
//
// \file	RectGradientShadowBackground.qml
// \author	benoit@destrat.io
// \date	2018 03 25
//-----------------------------------------------------------------------------

import QtQuick              2.7
import QtGraphicalEffects   1.0

import QuickQanava          2.0 as Qan

/*! \brief Node or group background component with gradient fill, shadow effect and backOpacity style support
 *
 */
Item {
    id: background

    // Public:
    property var    nodeItem: undefined

    //! Back color property, default to style.backColor, but available for user overidde.
    property color  backColor: nodeItem.style.backColor

    // private:
    // Default settings for rect radius, shadow margin is the _maximum_ shadow radius (+vertical or horizontal offset).
    property real   shadowOffset : nodeItem && nodeItem.style ? nodeItem.style.effectOffset : 4
    property real   shadowRadius : nodeItem && nodeItem.style ? nodeItem.style.effectRadius : 4
    property color  shadowColor  : nodeItem && nodeItem.style ? nodeItem.style.effectColor : Qt.rgba(0.1, 0.1, 0.1)
    property real   shadowMargin : (shadowRadius + shadowOffset) * 2

    Item {
        z: -1   // Effect should be behind edges , docks and connectors...
        id: effectBackground
        anchors.centerIn: parent
        width: nodeItem.width + shadowMargin; height: nodeItem.height + shadowMargin
        visible: false
        Rectangle {
            anchors.centerIn: parent
            width: nodeItem.width - 1.0;  height: nodeItem.height - 2.0 // Reduce rectangle used to generate shadow/glow to
            radius: nodeItem.style.backRadius                           // avoid aliasing artifacts at high scales.
            color: Qt.rgba(0, 0, 0, 1)
            antialiasing: true
            visible: false
            layer.enabled: true
            layer.effect: DropShadow {
                horizontalOffset: shadowOffset; verticalOffset: shadowOffset
                radius: shadowRadius; samples: 8
                color: shadowColor
                visible: nodeItem.style.effectEnabled
                transparentBorder: true
                cached: false
            }
        }
    }
    Item {
        id: backgroundMask
        anchors.centerIn: parent
        width: parent.width + shadowMargin; height: parent.height + shadowMargin
        visible: false
        Rectangle {
            anchors.centerIn: parent
            width: nodeItem.width - 0.5;  height: nodeItem.height - 0.5
            radius: nodeItem.style.backRadius
            color: Qt.rgba(0, 0, 0, 1)
            antialiasing: true
        }
    }
    OpacityMask {
        anchors.centerIn: parent
        width: parent.width + shadowMargin; height: parent.height + shadowMargin
        source: ShaderEffectSource { sourceItem: effectBackground; hideSource: false }
        maskSource: ShaderEffectSource { format: ShaderEffectSource.Alpha; sourceItem: backgroundMask; hideSource: false }
        invert: true
    }

    RectGradientBackground {
        anchors.fill: parent
        nodeItem: background.nodeItem
    }
}
