/*
    This file is part of QuickQanava library.

    Copyright (c) 2008-2018 Benoit AUTHEMAN

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

import QtQuick 2.7
import QtQuick.Controls 2.13
import QtGraphicalEffects   1.0

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
    RectangularGlow {
        anchors.fill: parent
        cached: true
        glowRadius:  8
        cornerRadius: 8
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
