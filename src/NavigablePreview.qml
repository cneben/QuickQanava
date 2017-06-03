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

import "qrc:/QuickQanava" as Qan
import QuickQanava 2.0 as Qan

Qan.AbstractNavigablePreview {
    id: preview
    clip: true
    Item {
        id: voidItem
        visible: false
    }
    Connections {
        id: sourceMonitor
        onWidthChanged: updateVisibleArea()
        onHeightChanged: updateVisibleArea()
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
        onXChanged: updateVisibleArea()
        onYChanged: updateVisibleArea()
        onScaleChanged: updateVisibleArea()
        onWidthChanged: updateVisibleArea()
        onHeightChanged: updateVisibleArea()
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
        updateVisibleArea()
    }
    ShaderEffectSource {
        id: sourcePreview
        anchors.fill: parent
        live: true; recursive: false
        sourceItem: source.containerItem
        textureSize: Qt.size(width, height)
    }
    function updateVisibleArea() {
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
        var borderHalf = visibleArea.border.width / 2.
        visibleArea.x = ( windowTopLeft.x * previewXRatio ) + borderHalf
        visibleArea.y = ( windowTopLeft.y * previewYRatio ) + borderHalf
        visibleArea.width = ( ( windowBottomRight.x - windowTopLeft.x ) * previewXRatio ) - visibleArea.border.width
        visibleArea.height = ( ( windowBottomRight.y - windowTopLeft.y )  * previewYRatio ) - visibleArea.border.width
    }
    Rectangle {
        id: visibleArea
        color: Qt.rgba(0, 0, 0, 0)
        smooth: true;   antialiasing: true
        border.color: "red"; border.width: 2
    }
    MouseArea {
        id: visibleAreaDragger
        anchors.fill: parent
        drag.onActiveChanged: {
            console.debug("dragging to:" + visibleArea.x + ":" + visibleArea.y );
            if ( source ) {
            }
        }
        drag.target: visibleArea
        drag.threshold: 1.      // Note 20170311: Avoid a nasty delay between mouse position and dragged item position
        // Do not allow dragging outside preview area
        drag.minimumX: 0; drag.maximumX: Math.max(0, preview.width - visibleArea.width)
        drag.minimumY: 0; drag.maximumY: Math.max(0, preview.height - visibleArea.height)
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        enabled: true
        onReleased: {
        }
        onPressed : {
        }
    }
}

