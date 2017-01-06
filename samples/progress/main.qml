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

import QtQuick 2.5
import QtQuick.Extras 1.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
import "qrc:/QuickContainers" as Qps

Item {
    id: window
    anchors.fill: parent
    Text {
        id: infoText
        text: "This sample demonstrate use of Qan.ProgressNotifier:
               \t-A custom progress dialog reflect progress properties from Qan.ProgressNotifier."
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: infoText.contentHeight + 15
        text: "Launch progress notifier"
        onClicked: {
            consumer.consume( progressNotifier );
        }
    }

    Qan.ProgressNotifier {
        id: progressNotifier
        onShowProgress: { progressDialog.visible = true; }
        onHideProgress: { progressDialog.visible = false; }
    }
    Item {
        id: progressDialog
        anchors.fill: parent
        visible: false
        onVisibleChanged: console.debug( "progressDialog.visible=" + visible )
        Rectangle {     // Background
            anchors.fill: parent
            color: "black"
            opacity: 0.3
        }
        ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 220
            ProgressBar {
                Layout.fillWidth: true; Layout.fillHeight: false
                value: progressNotifier.progress
                Text {
                    anchors.centerIn: parent
                    text: Math.round( progressNotifier.progress * 100. ) + " %"
                }
            }
            ProgressBar {
                Layout.fillWidth: true; Layout.fillHeight: false
                value: progressNotifier.phaseProgress
                onValueChanged: { indeterminate = value < 0. }
                Text {
                    anchors.centerIn: parent
                    text: Math.round( progressNotifier.phaseProgress * 100. ) + " %"
                }
            }
            Text {
                Layout.alignment: Layout.Center
                text: progressNotifier.phaseLabel
            }
        }
    }
}
