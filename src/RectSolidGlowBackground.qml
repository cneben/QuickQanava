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
// \file	RectSolidGlowBackground.qml
// \author	benoit@destrat.io
// \date	2018 03 23
//-----------------------------------------------------------------------------

import QtQuick              2.7
import QtGraphicalEffects   1.0

import QuickQanava          2.0 as Qan

/*! \brief Node or group background component with solid fill, glow effect and backOpacity style support
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
    property real   glowRadius: nodeItem && nodeItem.style ? nodeItem.style.effectRadius : 15
    property color  glowColor: nodeItem && nodeItem.style ? nodeItem.style.effectColor : Qt.rgba(0,0,0, 0.45)
    property real   effectMargin: glowRadius * 2.

    Item {
        id: fakeBackground
        z: -1   // Effect should be behind edges , docks and connectors...
        x: -glowRadius; y: -glowRadius
        width: nodeItem.width + effectMargin; height: nodeItem.height + effectMargin
        visible: false
        Rectangle {
            x: glowRadius + 0.5 ; y: glowRadius + 0.5
            width: nodeItem.width - 1.      // Avoid aliasing artifacts for mask at high scales.
            height: nodeItem.height - 1.
            radius: nodeItem.style.backRadius
            color: Qt.rgba(0, 0, 0, 1)
            antialiasing: true
            layer.enabled: true
            layer.effect: Glow {
                color: background.glowColor
                radius: background.glowRadius;     samples: Math.min( 15, background.glowRadius * 0.75)
                spread: 0.25;   transparentBorder: true;    cached: false
                visible: nodeItem.style.effectEnabled
            }
        }
    }
    Item {
        id: backgroundMask
        x: -glowRadius; y: -glowRadius
        width: nodeItem.width + effectMargin; height: nodeItem.height + effectMargin
        visible: false
        Rectangle {
            x: glowRadius + 0.5; y: glowRadius + 0.5
            width: nodeItem.width - 1.;  height: nodeItem.height - 1.
            radius: nodeItem.style.backRadius
            color: Qt.rgba(0,0,0,1)
            antialiasing: true
        }
    }
    OpacityMask {
        x: -glowRadius; y: -glowRadius
        width: nodeItem.width + effectMargin; height: nodeItem.height + effectMargin
        source: ShaderEffectSource { sourceItem: fakeBackground; hideSource: false }
        maskSource: ShaderEffectSource { format: ShaderEffectSource.Alpha; sourceItem: backgroundMask; hideSource: false }
        invert: true
    }

    RectSolidBackground {
        anchors.fill: parent
        nodeItem: background.nodeItem
    }
}
