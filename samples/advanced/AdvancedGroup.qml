/*
 Copyright (c) 2008-2024, Benoit AUTHEMAN All rights reserved.

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

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library. Copyright 2024 Benoit AUTHEMAN.
//
// \file	AdvancedNode.qml
// \author	benoit@destrat.io
// \date	2023 11 17
//-----------------------------------------------------------------------------

import QtQuick              2.8
import QtQuick.Controls     2.1
import QtQuick.Layouts      1.3

import QuickQanava as Qan

Qan.GroupItem {
    id: advancedGroup

    minimumSize: Qt.size(150., 100.)
    width: 200
    height: 150

    default property alias children : template
    container: template.container   // See qan::GroupItem::container property documentation
    labelEditorVisible : template.labelEditorVisible
    //expandButtonVisible : template.expandButtonVisible

    Qan.RectGroupTemplate {
        id: template
        anchors.fill: parent
        groupItem: parent
        z: 1

        RowLayout {
            anchors.bottom: parent.top
            anchors.bottomMargin: 2
            anchors.right: parent.right
            anchors.rightMargin: 4

            ToolButton {
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                icon.source: 'qrc:/collapse-parents.png'
                //visible: advancedGroup.node && advancedGroup.node.inNodes.length > 0
                property bool collapsed: false
                checked: collapsed
                checkable: true
                onClicked: {
                    advancedGroup.collapseAncestors(collapsed)
                    collapsed = !collapsed
                }
            }
            ToolButton {
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                icon.source: 'qrc:/collapse-childs.png'
                //visible: advancedGroup.node && advancedGroup.node.outNodes.length > 0
                property bool collapsed: false
                checked: collapsed
                checkable: true
                onClicked: {
                    advancedGroup.collapseChilds(collapsed)
                    collapsed = !collapsed
                }
            }
        }
    }
}
