//-----------------------------------------------------------------------------
// This file is a part of the Holographe software. Copyright 2015 Benoit AUTHEMAN.
//
// \file	BottomRightResizer.qml
// \author	benoit@qanava.org
//-----------------------------------------------------------------------------

import QtQuick 2.5              // MouseArea
import QtQuick.Layouts 1.2

/*! Add a resize handler ont the bottom right of a target QML Item.

  FIXME: Rewrite documentation.

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
    property var    target: undefined
    onTargetChanged: {
        if ( target != undefined )
            setResizerSize( target.width, target.height )
    }
    Connections {
        target: resizer.target
        onWidthChanged: {
            if ( target !== undefined )
                setResizerSize( target.width, target.height ) }
    }
    Connections {
        target: resizer.target
        onHeightChanged: {
            if ( target !== undefined )
                setResizerSize( target.width, target.height ) }
    }

    //! Corner resize handlers size (default to 8x8).
    property size   resizerSize: Qt.size( 9, 9 )
    //! Minimum size that can be "resized" with this resizer (default to 300x250).
    property size   minimumSize: Qt.size( 300, 250 )
    onMinimumSizeChanged: {
        var minWidth = Math.max( minimumSize.width, resizerCenter.x )
        var minHeight = Math.max( minimumSize.height, resizerCenter.y )
        setResizerSize( minWidth, minHeight )
    }

    property point resizerCenter: Qt.point( resizerSize.width / 2, resizerSize.height / 2 )
    onResizerCenterChanged: {
        if ( !brResizerDragger.drag.active ) {
            var resizerWidth2 = resizerSize.width / 2
            if ( !almostEqual( brResizer.x, resizerCenter.x - resizerWidth2 ) )
                brResizer.x = resizerCenter.x - resizerWidth2
            var resizerHeight2 = resizerSize.height / 2
            if ( !almostEqual( brResizer.y, resizerCenter.y - resizerHeight2 ) )
                brResizer.y = resizerCenter.y - resizerHeight2
        }
        if ( target !== undefined ) {
            if ( !almostEqual( target.width, resizerCenter.x ) )
                target.width = resizerCenter.x
            if ( !almostEqual( target.height, resizerCenter.y ) )
                target.height = resizerCenter.y;
        }
    }

    function almostEqual( a, b ) {
        return Math.round( a * 100 ) / 100 === Math.round( b * 100 ) / 100
    }
    function setResizerSize( width, height ) {
        if ( !almostEqual( resizerCenter.x, width ) )
            resizerCenter.x = width;
        if ( !almostEqual( resizerCenter.y, height ) )
            resizerCenter.y = height;
    }

    Rectangle {
        id: brResizer
        width: resizerSize.width; height: resizerSize.height
        radius: 3; border.width: 3; color: Qt.rgba( 0., 0., 0., 0.40 )
        onXChanged: {
            var resizerWidth2 = resizerSize.width / 2
            if ( !almostEqual( resizerCenter.x, x + resizerWidth2 ) ) // Avoid binding loops
                resizerCenter.x = x + resizerWidth2
        }
        onYChanged: {
            var resizerHeight2 = resizerSize.height / 2
            if ( !almostEqual( resizerCenter.y, y + resizerHeight2 ) ) // Avoid binding loops
                resizerCenter.y = y + resizerHeight2
        }
        MouseArea {
            id: brResizerDragger
            anchors.fill: parent
            cursorShape: Qt.SizeFDiagCursor
            drag.target: brResizer
            drag.minimumX: resizer.minimumSize.width
            drag.minimumY: resizer.minimumSize.height
            preventStealing: true   // Otherwise with a parent scaled item, we don't get drag events
        }
    }
}
