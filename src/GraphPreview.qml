/*
 Copyright (c) 2008-2022, Benoit AUTHEMAN All rights reserved.

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
import QtQuick 2.7
import QtQuick.Controls 2.13
//import QtGraphicalEffects   1.0

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan

/*! \brief Visual graph preview.
 *
 */
Control {
    id: graphPreview

    // PUBLIC /////////////////////////////////////////////////////////////////
    width: 200
    height: 113

    property alias source: navigablePreview.source
    property alias visibleWindowColor: navigablePreview.visibleWindowColor

    // Preview background panel opacity (default to 0.9).
    property alias previewOpactity: previewBackground.opacity

    // PUBLIC /////////////////////////////////////////////////////////////////
    padding: 0

    property real   previewSize: 0.15
    property    real graphRatio: graphView.containerItem.childrenRect.width / graphView.containerItem.childrenRect.height
    property    real previewRatio: graphView.width / graphView.height
    onGraphRatioChanged: updateNavigablePreviewSize()
    onPreviewRatioChanged: updateNavigablePreviewSize()

    function updateNavigablePreviewSize() {
        // Algorithm:
        // 1. Compute navigable preview height (nph) given graph width (gw) and graphRatio.
        // 2. If navigable preview height (nph) < preview height (ph), then use graphRatio to
        //    generate nph.
        // 3. Else compute navigable preview width using previewRatio and fix nph to ph.

        const pw = graphPreview.width
        const ph = graphPreview.height
        const gw = graphView.containerItem.childrenRect.width
        const gh = graphView.containerItem.childrenRect.height

        //console.error('')
        //console.error('graphRatio=' + graphRatio + '    previewRatio=' + previewRatio)
        //console.error('pw=' + pw + '   ph=' + ph)
        //console.error('gw=' + gw + '   gh=' + gh)

        // 1.
        let sw = pw / gw
        let sh = ph / gh
        let npw = 0.
        let nph = pw * graphRatio
        if (nph < ph) {
            // 2.
            npw = pw
        } else {
            // 3.
            nph = ph
            npw = ph * graphRatio
        }
        // If npw is larger than actual preview width (pw), scale nph
        if (npw > pw)
            nph = nph * (pw / npw)

        // Secure with boundary Check
        navigablePreview.width = Math.min(npw, pw)
        navigablePreview.height = Math.min(nph, ph)
    }

    opacity: 0.8
    hoverEnabled: true; ToolTip.visible: hovered; ToolTip.delay: 1500
    ToolTip.text: qsTr("Show parts of image that have actually been viewed with more than 100% zoom")
    z: 3    // Avoid tooltips beeing generated on top of preview
    Qan.Glow {
        anchors.fill: parent
        cached: true
        //glowRadius:  8
        //cornerRadius: 8
        spread: 0.5
        color: "lightgrey"
    }
    Pane {
        id: previewBackground
        anchors.fill: parent
        opacity: 0.9
        padding: 1
        clip: true
        Label {
            x: 4
            y: 2
            text: (graphView.zoom * 100).toFixed(1) + "%"
            font.pixelSize: 11
        }
        Qan.NavigablePreview {
            id: navigablePreview
            anchors.centerIn: parent
        }  // Qan.NavigablePreview
    }
}  // Control graph preview
