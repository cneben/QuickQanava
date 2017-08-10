/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

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
// This file is a part of the QuickContainers library. Copyright 2014 Benoit AUTHEMAN.
//
// \file	styleEditor.qml
// \author	benoit@destrat.io
// \date	2015 05 02
//-----------------------------------------------------------------------------

import QtQuick          2.7
import QtQuick.Layouts  1.3
import QtQuick.Controls 2.0
import QtQuick.Dialogs  1.2

import QuickContainers  1.0 as Qcm

/*! \brief Graphical QuickControls2 editor for qan::Style edition.
 *
 */
ListView {
    id: styleEditor

    property alias  style: objectModel.target
    Qcm.ObjectModel {
        id: objectModel
    }
    model: objectModel

    // Used as a cache in font and color dialogs
    property var    currentPropName         : undefined
    property var    currentPropData         : undefined
    property real   maxPropertyLabelWidth   : 25
    property real   preferredPropertyHeight : 35

    clip: true
    flickableDirection : Flickable.HorizontalAndVerticalFlick
    spacing: 10

    delegate: Component {
        Loader {
            width: styleEditor.width
            height: preferredPropertyHeight
            property string propName: propertyName
            property string propType: propertyType
            property var    propData: propertyData
            property var    propModel: model
            sourceComponent: selectPropertyDelegate( propType )
        }
    }
    Component.onCompleted: { }

    ColorDialog {
        id: colorDialog
        title: qsTr( "Select a color" )
    }
    FontDialog {
        id: fontDialog
        title: qsTr( "Select a font" )
        onAccepted: { objectModel.setProperty( currentPropName, fontDialog.font ); }
        onRejected: { }
        modality: Qt.ApplicationModal
    }

    Component {
        id: defaultDelegate
        RowLayout {
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Flickable {
                Layout.fillWidth: true; Layout.fillHeight: true
                flickableDirection: Flickable.HorizontalFlick
                clip: true
                Label {
                    width: contentWidth; height: preferredPropertyHeight
                    verticalAlignment: Text.AlignVCenter
                    maximumLineCount: 1
                    text: propData + " #" + propType
                }
            }
        }
    }

    Component {
        id: stringDelegate
        RowLayout {
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            TextField {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: propData
                //maximumLength: delegateOptions ? delegateOptions.maxChars : -1
                Keys.onReturnPressed: {
                    objectModel.setProperty( propName, displayText );
                    text = displayText
                }
                Keys.onEnterPressed: {
                    objectModel.setProperty( propName, displayText );
                    text = displayText
                }
            }
        }
    }

    Component {
        id: doubleDelegate
        RowLayout {
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Slider {
                id: realValueSlider
                //visible: !delegateOptions  ||
                //         delegateOptions.type === Qps.RealOptions.Slider
                Layout.alignment: Qt.AlignVCenter; Layout.fillWidth: true

                //from: delegateOptions ? delegateOptions.min : -1000000.
                //to: delegateOptions ? delegateOptions.max : 1000000.
                // FIXME QC2
                //stepSize: ( delegateOptions !== null ? delegateOptions.stepSize : 0.1 )
                value: propData
                onValueChanged: { objectModel.setProperty( propName, value ); }
                onPositionChanged: { objectModel.setProperty( propName, from + position * (to - from ) ); }
            }
            SpinBox {
                visible: !realValueSlider.visible
                Layout.alignment: Qt.AlignVCenter; Layout.fillWidth: false
                Layout.preferredHeight: preferredPropertyHeight

                //from: delegateOptions ? delegateOptions.min : -1000000.
                //to: delegateOptions ? delegateOptions.max : 1000000.
                // FIXME QC2
                //stepSize: ( delegateOptions !== null ? delegateOptions.stepSize : 0.1 )
                //decimals: 2
                value: propData
                onValueChanged: { objectModel.setProperty( propName, value ); }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: intDelegate
        RowLayout {
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Slider {
                Layout.preferredHeight: 35

                id: intValueSlider
                //visible: delegateOptions ||
                //         delegateOptions.type === Qps.IntOptions.Slider
                Layout.alignment: Qt.AlignVCenter; Layout.fillWidth: true
                //from: delegateOptions ? delegateOptions.min : -1000000.
                //to: delegateOptions ? delegateOptions.max : 1000000.
                stepSize: 1
                value: propData
                onValueChanged: { objectModel.setProperty( propName, value ); }
                onPositionChanged: { objectModel.setProperty( propName, from + position * (to - from ) ); }
            }
            SpinBox {
                visible: !intValueSlider.visible
                Layout.alignment: Qt.AlignVCenter; Layout.fillWidth: false
                Layout.preferredHeight: preferredPropertyHeight
                //from: delegateOptions ? delegateOptions.min : -1000000
                //to: delegateOptions ? delegateOptions.max : 1000000
                stepSize: 1
                value: propData
                onValueChanged: { objectModel.setProperty( propName, value ); }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: colorDelegate
        RowLayout {
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Rectangle {
                id: colorRectangle
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: false; Layout.preferredWidth: 45
                Layout.preferredHeight: preferredPropertyHeight - 5
                Layout.topMargin: 3; Layout.bottomMargin: 3
                color: propData
                radius: 3; border.width: 1; border.color: Qt.lighter( color, 1.2 )

            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: 35
                Layout.preferredHeight: 25
                text: "..."
                onClicked: {
                    currentPropName = propName;
                    colorDialog.color = propData;
                    colorDialog.accepted.connect( onColorDialogAccepted )
                    colorDialog.rejected.connect( onColorDialogRejected )
                    colorDialog.open()
                }
                function onColorDialogAccepted() {
                    objectModel.setProperty( currentPropName, colorDialog.color )
                    colorRectangle.color = colorDialog.color; // Note 20160719: Force color update, since it can't be handled by the qcm::Objectmodel abstract model
                    colorDialog.accepted.disconnect( onColorDialogAccepted )
                    colorDialog.rejected.disconnect( onColorDialogRejected )
                }
                function onColorDialogRejected() {
                    colorDialog.accepted.disconnect( onColorDialogAccepted )
                    colorDialog.rejected.disconnect( onColorDialogRejected )
                }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: sizeDelegate
        RowLayout {
            Label {
                anchors.verticalCenter: parent.verticalCenter;
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            SpinBox {
                id: valueX
                Layout.preferredHeight: preferredPropertyHeight
                value: propData.width
                onValueChanged: { objectModel.setProperty( propName, Qt.size( valueX.value, valueY.value ) ) }
            }
            Label { anchors.verticalCenter: parent.verticalCenter; text: " x "}
            SpinBox {
                id: valueY
                Layout.preferredHeight: preferredPropertyHeight
                //decimals: 2
                //stepSize: 0.1
                value: propData.height
                onValueChanged: { objectModel.setProperty( propName, Qt.size( valueX.value, valueY.value ) ) }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: fontDelegate
        RowLayout {
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Label {
                Layout.alignment: Qt.AlignVCenter
                id: sampleText
                text: fontDialog.font.family + " " + fontDialog.font.pixelSize + "px"
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: 35
                Layout.preferredHeight: 25
                text: "..."
                onClicked: { currentPropName = propName; fontDialog.font = propData; fontDialog.visible = true }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: boolDelegate
        RowLayout {
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Switch {
                id: boolSwitch
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredHeight: preferredPropertyHeight
                Layout.maximumHeight: preferredPropertyHeight
                checked: propData
                onCheckedChanged: { objectModel.setProperty( propName, boolSwitch.checked ); }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: enumDelegate
        RowLayout {
            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            ComboBox {
                id: enumCombo
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: 75
                model: style.getEnumValueLabels( propName );
                currentIndex: propData
                onActivated: {
                    currentIndex = index;
                    objectModel.setProperty( propName, index );
                }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    function selectPropertyDelegate( typeName ) {
        switch ( typeName ) {
        case "QString": return stringDelegate
        case "double": return doubleDelegate
        case "int": return intDelegate
        case "QColor": return colorDelegate
        case "QSizeF": return sizeDelegate
        case "QFont": return fontDelegate
        case "bool":return boolDelegate
        }
        return defaultDelegate
    }
} // ListView

