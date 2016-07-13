/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

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
// This file is a part of the QuickQanava software. Copyright 2015 Benoit AUTHEMAN.
//
// \file	NodeRectTemplate.qml
// \author	benoit@destrat.io
// \date	2015 11 30
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QuickQanava 2.0 as Qan

/*! \brief Default template component for building a custom rectangular qan::Node item.
 *
 * Node with custom content definition using "templates" is described in \ref qanavacustom
 */
Item {
    id: template
    property         var    node
    default property alias  children : contentLayout.children

    Rectangle {
        id: background
        anchors.fill: parent    // Background follow the content layout implicit size
        z: 1
        radius: 2
        color: node.style.backColor
        border.color: node.style.borderColor
        border.width: node.style.borderWidth
    }
    DropShadow {
        id: backgroundShadow
        anchors.fill: parent
        source: background
        horizontalOffset: node.style.shadowOffset.width
        verticalOffset: node.style.shadowOffset.height
        radius: 4; samples: 8
        color: node.style.shadowColor
        visible: node.style.hasShadow
        transparentBorder: true
    }

    Connections {
        target: node
        onNodeDoubleClicked: {
            nodeLabel.visible = false
            nodeLabelEditor.forceActiveFocus();
            nodeLabelEditor.selectAll()
        }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: background.radius
        z: 3
        Text {
            id: nodeLabel
            z: 4
            Layout.fillWidth: true
            Layout.fillHeight: contentLayout.children.length === 0
            Layout.preferredHeight: contentHeight
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            text: node.label
            font: node.style.labelFont
            horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
            maximumLineCount: 3 // Must be set, otherwise elide don't work and we end up with single line text
            elide: Text.ElideRight; wrapMode: Text.Wrap
        }
        TextInput {
            id: nodeLabelEditor
            z: 5
            Layout.preferredWidth: nodeLabel.width
            Layout.preferredHeight: nodeLabel.height
            text: node.label
            onAccepted: {
                if ( text.length == 0 )
                    text = node.label  // Do not allow empty labels
                else node.label = text
                focus = false;  // Release focus once the label has been edited
                nodeLabel.visible = true
            }
            onEditingFinished: { visible = false; }
            onActiveFocusChanged: {
                if ( !activeFocus )
                    nodeLabel.visible = true;
            }
            visible: !nodeLabel.visible
        }
        Item {
            id: contentLayout
            z: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true;
            Layout.fillHeight: true
            visible: contentLayout.children.length > 0  // Hide if the user has not added any content
        }
    }
}
