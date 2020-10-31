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

import QuickQanava 2.0 as Qan

/*! \brief Concrete component for qan::NavigablePreview interface.
 *
 */
Qan.AbstractNavigablePreview {
    id: preview

    // PUBLIC /////////////////////////////////////////////////////////////////

    //! Overlay item could be used to display a user defined item (for example an heat map image) between the background and the current visible window rectangle.
    property var    overlay : overlayItem

    //! Color for the visible window rect border (default to red).
    property color  visibleWindowColor: Qt.rgba(1, 0, 0, 1)

    //! Show or hide the target navigable content as a background image (default to true).
    property alias  backgroundPreviewVisible: sourcePreview.visible

    // PRIVATE ////////////////////////////////////////////////////////////////
    function updatePreviewSourceRect(rect) {
        if (!source)
            return
        console.error('NavigablePreview.updatePreviewSourceRect(): source=' + source)
        console.error('NavigablePreview.updatePreviewSourceRect(): source.containerItem=' + source.containerItem)
        if (preview.source &&     // Manually update shader effect source source rect
            preview.source.containerItem &&
            sourcePreview.sourceItem === preview.source.containerItem ) {
            var cr = preview.source.containerItem.childrenRect
            console.error('NavigablePreview.updatePreviewSourceRect(): cr=' + cr)
            if (cr.width > 0 && cr.height > 0)
                sourcePreview.sourceRect = cr
        }
    }

    onSourceChanged: {
        if (source &&
            source.containerItem) {
            resetVisibleWindow()

            // Monitor source changes
            source.containerItem.onWidthChanged.connect(updateVisibleWindow)
            source.containerItem.onHeightChanged.connect(updateVisibleWindow)
            source.containerItem.onScaleChanged.connect(updateVisibleWindow)
            source.containerItem.onXChanged.connect(updateVisibleWindow)
            source.containerItem.onYChanged.connect(updateVisibleWindow)
            source.containerItem.onChildrenRectChanged.connect(updatePreviewSourceRect)

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
        live: true; recursive: false
        sourceItem: source.containerItem
        textureSize: Qt.size(width, height)
    }

    // Reset visibleWindow rect to preview dimension (taking rectangle border into account)
    function    resetVisibleWindow() {
        const border = visibleWindow.border.width
        const border2 = visibleWindow.border.width * 2
        visibleWindow.x = border
        visibleWindow.y = border
        visibleWindow.width = preview.width - border2
        visibleWindow.height = preview.height - border2
    }

    function    updateVisibleWindow() {
        if (!preview)
            return
        if (!source) {  // Reset the window when source is invalid
            preview.resetVisibleWindow()
            return
        }
        var containerItem = source.containerItem
        if (!containerItem) {
            preview.resetVisibleWindow()
            return
        }
        var containerItemCr = containerItem.childrenRect
        if (containerItemCr.width < preview.source.width && // If scene size is stricly inferior to preview size
            containerItemCr.height < preview.source.height) {         // reset the preview window
            //preview.resetVisibleWindow()
            //return
            containerItemCr.width = preview.source.width
            containerItemCr.height = preview.source.height
        }
        if (containerItemCr.width < 0.01 ||        // Do not update without a valid children rect
            containerItemCr.height < 0.01) {
            preview.resetVisibleWindow()
            return
        }
        if (containerItemCr.width < containerItemCr.width ||        // Reset the visible window is the whole containerItem content
            containerItemCr.height < containerItemCr.height) {       // is smaller than graph view
            preview.resetVisibleWindow()
            return
        }
        if (containerItemCr.width < preview.width && // If scene size is stricly inferior to preview size
            containerItemCr.height < preview.height) {         // reset the preview window
            preview.resetVisibleWindow()
            return
        }
        const border = visibleWindow.border.width
        const border2 = border * 2.
        var windowTopLeft = source.mapToItem(containerItem, 0, 0)
        var windowBottomRight = source.mapToItem(containerItem, source.width, source.height)

        var previewXRatio = preview.width / containerItemCr.width
        var previewYRatio = preview.height / containerItemCr.height

        visibleWindow.x = previewXRatio * (windowTopLeft.x - containerItemCr.x) + border
        visibleWindow.y = previewYRatio * (windowTopLeft.y - containerItemCr.y) + border
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
}

