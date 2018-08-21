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

import QtQuick 2.7

import QuickQanava 2.0 as Qan

/*! \brief Concrete component for qan::NavigablePreview interface.
 *
 */
Qan.AbstractNavigablePreview {
    id: preview
    clip: true

    //! Overlay item could be used to display a user defined item (for example an heat map image) between the background and the current visible window rectangle.
    property var    overlay : overlayItem

    //! Color for the visible window rect border (default to red).
    property color  visibleWindowColor: Qt.rgba(1, 0, 0, 1)

    //! Show or hide the target navigable content as a background image (default to true).
    property alias  backgroundPreviewVisible: sourcePreview.visible

    Item {
        id: voidItem
        visible: false
    }
    Connections {
        id: sourceMonitor
        onWidthChanged: updatevisibleWindow()
        onHeightChanged: updatevisibleWindow()
        onZoomChanged: updatevisibleWindow()
        onChildrenRectChanged: {
            if ( preview.source &&     // Manually update shader effect source source rect
                 preview.source.containerItem &&
                 sourcePreview.sourceItem === preview.source.containerItem ) {
                var cr = preview.source.containerItem.childrenRect
                if ( cr.width > 0 && cr.height > 0 )
                    sourcePreview.sourceRect = cr
            }
        }
    }
    Connections {
        id: containerItemMonitor
        target: voidItem
        onXChanged: updatevisibleWindow()
        onYChanged: updatevisibleWindow()
        onScaleChanged: updatevisibleWindow()
        onWidthChanged: updatevisibleWindow()
        onHeightChanged: updatevisibleWindow()
    }
    onSourceChanged: {
        if ( source &&
             source.containerItem ) {
            containerItemMonitor.target = source.containerItem
            sourceMonitor.target = source
            sourcePreview.sourceItem = source.containerItem
            var cr = preview.source.containerItem.childrenRect
            if ( cr.width > 0 && cr.height > 0 )
                sourcePreview.sourceRect = cr
        } else sourcePreview.sourceItem = undefined
        updatevisibleWindow()
    }
    ShaderEffectSource {
        id: sourcePreview
        anchors.fill: parent
        live: true; recursive: false
        sourceItem: source.containerItem
        textureSize: Qt.size(width, height)
    }
    function updatevisibleWindow() {
        if ( !source )
            return
        var containerItem = source.containerItem
        if ( !containerItem )
            return;
        var containerItemCr = containerItem.childrenRect
        if ( containerItemCr.width < 0.01 ||        // Do not update without a valid children rect
             containerItemCr.height < 0.01 )
            return;
        var windowTopLeft = source.mapToItem(containerItem, 0, 0)
        var windowBottomRight = source.mapToItem(containerItem, source.width, source.height)
        var previewXRatio = preview.width / containerItemCr.width
        var previewYRatio = preview.height / containerItemCr.height
        var borderHalf = visibleWindow.border.width / 2.
        visibleWindow.x = ( windowTopLeft.x * previewXRatio ) + borderHalf
        visibleWindow.y = ( windowTopLeft.y * previewYRatio ) + borderHalf
        visibleWindow.width = ( ( windowBottomRight.x - windowTopLeft.x ) * previewXRatio ) - visibleWindow.border.width
        visibleWindow.height = ( ( windowBottomRight.y - windowTopLeft.y )  * previewYRatio ) - visibleWindow.border.width
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
        border.color: visibleWindowColor; border.width: 2
    }
    MouseArea {
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
    }
}

