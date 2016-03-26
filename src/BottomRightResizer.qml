//-----------------------------------------------------------------------------
// This file is a part of the Holographe software. Copyright 2015 Benoit AUTHEMAN.
//
// \file	BottomRightResizer.qml
// \author	benoit@qanava.org
//-----------------------------------------------------------------------------

import QtQuick 2.5              // MouseArea
import QtQuick.Layouts 1.2

/*! Add a resize handler ont the bottom right of a target QML Item.

  To avoid binding loop between resizer and its target, it is necessary to
  define intermediate width/height properties to control target dimension:
  \code
    Item {
        id: content
        width: Math.max( groupWidth, minimumWidth )
        height: Math.max( groupHeight, minimumHeight )
        property real   minimumWidth: childrenRect.width + 10
        property real   minimumHeight: childrenRect.height + 10
        property real groupWidth: 150
        property real groupHeight: 80
        BottomRightResizer {
            target: content
            minimumSize: Qt.size( content.minimumWidth, content.minimumHeight )
            targetWidth: 150    // Initial size
            targetHeight: 80
            targetWidthName: "groupWidth"
            targetHeightName: "groupHeight"
        }
    }
  \endcode
 */
Item {
    id: resizer

    //! Resizer target.
    property var    target

    //! Width target property (usually target.width).
    property var targetWidth
    onTargetWidthChanged: {
        if ( !brResizerDragger.drag.active )
            brResizer.x = targetWidth
    }
    //! Width target property (usually target.height).
    property var targetHeight
    onTargetHeightChanged: {
        if ( !brResizerDragger.drag.active )
            brResizer.y = targetHeight
    }

    //! Target width property name (default to "width").
    property var    targetWidthName: "width"
    //! Target width property name (default to "height").
    property var    targetHeightName: "height"

    //! Corner resize handlers size (default to 8x8).
    property size   resizerSize: Qt.size( 9, 9 )
    //! Minimum size that can be "resized" with this resizer (default to 300x250).
    property size   minimumSize: Qt.size( 300, 250 )

    Binding {
        target: resizer.target
        property: resizer.targetWidthName; value: brResizer.center.x
        when: brResizerDragger.drag.active
    }
    Binding {
        target: resizer.target
        property: resizer.targetHeightName; value: brResizer.center.y
        when: brResizerDragger.drag.active
    }
    Rectangle {
        id: brResizer
        width: resizerSize.width; height: resizerSize.height
        radius: 3
        border.width: 3
        color: Qt.rgba( 0., 0., 0., 0.40 )
        property point center: Qt.point( x + width / 2, y + height / 2 )
        MouseArea {
            id: brResizerDragger
            anchors.fill: parent
            cursorShape: Qt.SizeFDiagCursor
            drag.target: brResizer
            drag.minimumX: minimumSize.width
            drag.minimumY: minimumSize.height
            preventStealing: true   // Otherwise with a parent scaled item, we don't get drag events
        }
    }
}
