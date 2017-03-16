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

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanva software. Copyright 2014 Benoit AUTHEMAN.
//
// \file	CanvasTemplate.qml
// \author	benoit@destrat.io
// \date	2015 11 30
//-----------------------------------------------------------------------------

import QtQuick              2.7
import QtQuick.Controls     2.0

/*! \brief Reusable component for visual edition of node and groups labels with on-demand loading.
 *
 * Initialize 'target' property with the node or group that should be edited, editor is invisible by default,
 * complex label edition component is loaded only when editor \c visible is set to true.
 *
 * \note LabelEditor is basically a loader with an internal edition component using
 * a Quick Controls 2 TextField that is loaded only when visible property is sets to true. Internal
 * editor is not unloaded when editor visibility is set to false.
 *
 * Exemple use on an user defined custom node:
 * \code
 * Qan.AbstractNode {
 *   id: rectNode
 *   width: 110; height: 50
 *   Connections {
 *    target: node
 *     onNodeDoubleClicked: labelEditor.visible = true
 *   }
 *   LabelEditor {
 *     id: nodeLabelEditor
 *     anchors.fill: parent
 *     anchors.margins: background.radius / 2
 *     target: parent.node
 *   }
 * }
 * \endcode
 *
 * More consistent sample is available in Qan.RectNodeTemplate component.
 */
Loader {
    id: labelEditorLoader
    visible: false

    //! Editor target node or group (or any Qan primitive with a \c label property).
    property var    target: undefined

    onVisibleChanged: {
        if ( visible && !item )
            sourceComponent = labelEditorComponent
        if ( item &&
             visible )
            showLabelEditor()
    }
    onItemChanged: {
        if ( item )             // If item is non null, it's creation has been request,
            showLabelEditor()   // force show label edition
    }
    function showLabelEditor() {
        if ( item ) {
            if ( !visible )
                visible = true
            item.forceActiveFocus()
            item.selectAll()
        }
    }
    Component {
        id: labelEditorComponent
        TextField {
            id: labelTextField
            anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right
            text: target ? target.label : ""
            onAccepted: {
                if ( target &&
                     text.length !== 0 )
                    target.label = text   // Do not allow empty labels
                focus = false;          // Release focus once the label has been edited
            }
            onEditingFinished: labelEditorLoader.visible = false
            onActiveFocusChanged: {
                if ( target &&
                     text !== target.label )  // Ensure that last edition text is removed
                    text = target.label       // for exemple if edition has been interrupted in a focus change
            }
        }
    }
}
