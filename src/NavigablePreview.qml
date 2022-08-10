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

import QuickQanava 2.0 as Qan

/*! \brief Concrete component for qan::NavigablePreview interface.
 *
 */
Qan.AbstractNavigablePreview {
    id: preview
    clip: true

    // PUBLIC /////////////////////////////////////////////////////////////////

    //! Overlay item could be used to display a user defined item (for example an heat map image) between the background and the current visible window rectangle.
    property var    overlay: overlayItem

    //! Color for the visible window rect border (default to red).
    property color  visibleWindowColor: Qt.rgba(1, 0, 0, 1)

    //! Show or hide the target navigable content as a background image (default to true).
    property alias  backgroundPreviewVisible: sourcePreview.visible

    // PRIVATE ////////////////////////////////////////////////////////////////
    onSourceChanged: {
        if (source &&
            source.containerItem) {
            resetVisibleWindow()

            // Monitor source changes
            source.containerItem.onWidthChanged.connect(updatePreview)
            source.containerItem.onHeightChanged.connect(updatePreview)
            source.containerItem.onScaleChanged.connect(updatePreview)
            source.containerItem.onXChanged.connect(updatePreview)
            source.containerItem.onYChanged.connect(updatePreview)
            source.containerItem.onChildrenRectChanged.connect(updatePreview)

            sourcePreview.sourceItem = source.containerItem
            var cr = preview.source.containerItem.childrenRect
            if (cr.width > 0 && cr.height > 0)
                sourcePreview.sourceRect = cr
        } else
            sourcePreview.sourceItem = undefined
        updateVisibleWindow()
    }

    ShaderEffectSource {
        id: sourcePreview
        anchors.fill: parent
        anchors.margins: 0
        live: true
        recursive: false
        textureSize: Qt.size(width, height)
    }

    function updatePreview() {
        if (!source)
            return

        const r = computeSourceRect()
        if (r &&
            r.width > 0. &&
            r.height > 0)
            sourcePreview.sourceRect = r

        updateVisibleWindow(r)
    }

    function computeSourceRect(rect) {
        if (!source)
            return undefined
        if (!preview.source ||
            !preview.source.containerItem ||
            sourcePreview.sourceItem !== preview.source.containerItem)
            return undefined

        console.error('computeSourceRect()')

        // Project preview.source size (0, 0, width, height) to
        // preview.source.containerItem to apply zoom.

        // 1. If projected view rect is not inside scene rect, use view rect.
        // 2. Otherwise use scene rect
        const viewRect = preview.source.mapToItem(preview.source.containerItem,
                                                  Qt.rect(0, 0,
                                                          preview.source.width,
                                                          preview.source.height))
        let cr = preview.source.containerItem.childrenRect
        console.error('cr=' + viewRect)
        console.error('viewRect=' + viewRect)
        console.error('viewRect in cr =' + preview.rectInside(cr, viewRect))
        if (!preview.rectInside(cr, viewRect))
            cr = viewRect
        return cr
    }

    // Reset visibleWindow rect to preview dimension (taking rectangle border into account)
    function    resetVisibleWindow() {
        const border = visibleWindow.border.width
        const border2 = border * 2
        visibleWindow.x = border
        visibleWindow.y = border
        visibleWindow.width = preview.width - border2
        visibleWindow.height = preview.height - border2
    }

    function    updateVisibleWindow(r) {
        // r is previewed rect in source.containerItem Cs
        if (!preview)
            return
        console.error('updateVisibleWindow()')
        if (!source) {  // Reset the window when source is invalid
            preview.resetVisibleWindow()
            return
        }
        var containerItem = source.containerItem
        if (!containerItem) {
            preview.resetVisibleWindow()
            return
        }
        console.error('r=' + r)
        console.error('preview.source.width=' + preview.source.width)
        console.error('preview.source.height=' + preview.source.height)

        if (r.width < preview.source.width && // If scene size is stricly inferior to preview size
            r.height < preview.source.height) {         // reset the preview window
            r.width = preview.source.width
            r.height = preview.source.height
        }
        if (r.width < 0.01 ||        // Do not update without a valid children rect
            r.height < 0.01) {
            preview.resetVisibleWindow()
            return
        }
        if (r.width < r.width ||        // Reset the visible window is the whole containerItem content
            r.height < r.height) {       // is smaller than graph view
            preview.resetVisibleWindow()
            return
        }
        if (r.width < preview.width && // If scene size is stricly inferior to preview size
            r.height < preview.height) {         // reset the preview window
            preview.resetVisibleWindow()
            return
        }
        const border = visibleWindow.border.width
        const border2 = border * 2.
        var windowTopLeft = source.mapToItem(containerItem, 0, 0)
        var windowBottomRight = source.mapToItem(containerItem, source.width, source.height)

        var previewXRatio = preview.width / r.width
        var previewYRatio = preview.height / r.height

        visibleWindow.x = previewXRatio * (windowTopLeft.x - r.x) + border
        visibleWindow.y = previewYRatio * (windowTopLeft.y - r.y) + border
        visibleWindow.width = previewXRatio * Math.abs(windowBottomRight.x - windowTopLeft.x) - border2
        visibleWindow.height = previewYRatio * Math.abs(windowBottomRight.y - windowTopLeft.y) - border2

        visibleWindowChanged(Qt.rect(visibleWindow.x / preview.width,     visibleWindow.y / preview.height,
                                     visibleWindow.width / preview.width, visibleWindow.height / preview.height),
                             source.zoom);
    }
    Item {
        id: overlayItem
        anchors.fill: parent; anchors.margins: 0
    }
    Rectangle {
        id: visibleWindow
        color: Qt.rgba(0, 0, 0, 0)
        smooth: true;   antialiasing: true
        border.color: visibleWindowColor;
        border.width: 2
    }
    // Not active on 20201027
    /*MouseArea {
        id: visibleWindowDragger
        anchors.fill: parent
        drag.onActiveChanged: {
            console.debug("dragging to:" + visibleWindow.x + ":" + visibleWindow.y );
            if ( source ) {
            }
        }
        drag.target: visibleWindow
        drag.threshold: 1.      // Note 20170311: Avoid a nasty delay between mouse position and dragged item position
        // Do not allow dragging outside preview area
        drag.minimumX: 0; drag.maximumX: Math.max(0, preview.width - visibleWindow.width)
        drag.minimumY: 0; drag.maximumY: Math.max(0, preview.height - visibleWindow.height)
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        enabled: true
        onReleased: {
        }
        onPressed : {
        }
    }*/
}  // Qan.AbstractNavigablePreview: preview
