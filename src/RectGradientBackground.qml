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
// \file	RectGradientBackground.qml
// \author	benoit@destrat.io
// \date	2018 03 25
//-----------------------------------------------------------------------------

import QtQuick              2.7
import QtGraphicalEffects   1.0

import QuickQanava          2.0 as Qan

/*! \brief Node or group background component with gradient fill, no effect and backOpacity style support
 *
 */
Item {
    // Public:
    property var    nodeItem: undefined

    // Note: Using an item to separate render tree of background with a gradient effect and foreground (mainly
    // rectangle border), to ensure that rectangle border is always rasterized even at high scale and never
    //  batched/cached with gradient effect SG node...

    anchors.fill: parent
    Rectangle {
        id: background
        anchors.fill: parent
        radius: nodeItem.style.backRadius
        color: Qt.rgba(0, 0, 0, 1)  // Force black, otherwise, effect does not reasterize gradient pixels
        border.width: 0             // Do not draw border, just the background gradient (border is drawn in foreground)
        antialiasing: true
        opacity: nodeItem.style.backOpacity

        layer.enabled: true
        layer.effect:    LinearGradient {
            start:  Qt.point(0.,0.)
            end:    Qt.point(background.width, background.height)
            cached: false
            gradient: Gradient {
                GradientStop { position: 0.0; color: nodeItem.style.baseColor }
                GradientStop { position: 1.0;  color: nodeItem.style.backColor }
            }
        }
    }
    Rectangle {
        id: foreground
        anchors.fill: parent    // Background follow the content layout implicit size
        radius: nodeItem.style.backRadius
        color: Qt.rgba(0, 0, 0, 0)  // Fully transparent
        border.color: nodeItem.style.borderColor
        border.width: nodeItem.style.borderWidth
        antialiasing: true
        // Note: Do not enable layer to avoid aliasing at high scale
    }
}
