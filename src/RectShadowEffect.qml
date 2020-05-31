/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
// \file	RectShadowEffect.qml
// \author	benoit@destrat.io
// \date	2017 11 17
//-----------------------------------------------------------------------------

import QtQuick              2.7
import QtGraphicalEffects   1.0

import QuickQanava          2.0 as Qan

/*! \brief Node or group background component with solid fill, shadow effect and backOpacity style support
 *
 */
Item {
    // Public:
    property var    nodeItem: undefined

    // private:
    // Default settings for rect radius, shadow margin is the _maximum_ shadow radius (+vertical or horizontal offset).
    property real   borderWidth  : nodeItem.style.borderWidth
    property real   borderWidth2 : borderWidth / 2.
    property real   shadowOffset : nodeItem.style.effectOffset
    property real   shadowRadius : nodeItem.style.effectRadius
    property color  shadowColor  : nodeItem.style.effectColor

    Item {
        id: effectBackground
        x: 0; y: 0
        //z: -1   // Effect should be behind edges , docks and connectors...
        width: nodeItem.width + shadowOffset + shadowRadius
        height: nodeItem.height + shadowOffset + shadowRadius
        visible: false
        Rectangle {
            x: borderWidth2; y: borderWidth2
            width: nodeItem.width - borderWidth         // avoid aliasing artifacts at high scales.
            height: nodeItem.height - borderWidth
            radius: nodeItem.style.backRadius
            color: Qt.rgba(0, 0, 0, 1)
            antialiasing: true
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
        anchors.fill: effectBackground
        visible: false
        Rectangle {
            x: 0; y: 0
            width: nodeItem.width - borderWidth2
            height: nodeItem.height - borderWidth2
            radius: nodeItem.style.backRadius
            color: Qt.rgba(1, 0, 0, 1)
        }
    }
    OpacityMask {
        anchors.fill: effectBackground
        source: effectBackground
        maskSource: backgroundMask
        invert: true
    }
}
