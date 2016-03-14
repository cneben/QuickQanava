/*
    This file is part of QuickProperties2 library.

    Copyright (C) 2016  Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickProperties2 library. Copyright 2014 Benoit AUTHEMAN.
//
// \file	PropertiesEditor.qml
// \author	benoit@qanava.org
// \date	2015 05 02
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import "qrc:/QuickProperties2" as Qps

/*! \brief Graphical editor for Qps.Properties or qps::TimedProperties objects (allow edition of individual property elements inside properties).
 *
 */
ColumnLayout {
    id: propertiesEditor
    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter

    //! Qps.Properties or qps::TimedProperties object that should be edited in this component.
    property var    properties: undefined
    /*! \brief Define the color that should be used for this component text.
     *
     * Default to black */
    property color  textColor: Qt.rgba( 0., 0., 0., 1. )

    // Used as a cache in font and color dialogs
    property var currentPropName: null
    property var currentPropData: null
    property real maxPropertyLabelWidth: 25

    ListView {
        id: propertiesListView
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        flickableDirection : Flickable.HorizontalAndVerticalFlick
        spacing: 4
        model: properties

        delegate: Component {
            Loader {
                width: propertiesListView.width // Delegate expand to listview width, height is delegate implicit height
                property string propName: propertyName
                property string propType: propertyType
                property bool propIsEnum: propertyIsEnum
                property var propData: propertyData
                property var delegateLimit : propertyLimit
                property var propModel: model
                sourceComponent: selectPropertyDelegate( propertyType, propertyIsEnum )
            }
        }
        Component.onCompleted: { }
    }

    ColorDialog {
        id: colorDialog
        title: qsTr( "Select a color" ) + qpsTranslator.change
        onAccepted: {
            console.debug( "Set color property: " + currentPropName + "   with properties=" + properties )
            if ( properties != undefined )
                properties.setProperty( currentPropName, colorDialog.color )
        }
        onRejected: { }
        modality: Qt.ApplicationModal
    }

    FontDialog {
        id: fontDialog
        title: qsTr( "Select a font" ) + qpsTranslator.change
        onAccepted: {
            if ( properties != undefined )
                properties.setProperty( currentPropName, fontDialog.font );
        }
        onRejected: { }
        modality: Qt.ApplicationModal
    }

    Component {
        id: propertyDefaultDelegate
        RowLayout {
            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName
                color: textColor
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: propData + " #" + propType
                color: textColor
            }
        }
    }

    Component {
        id: propertyStringDelegate
        RowLayout {
            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                color: textColor
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            TextField {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: propData
                maximumLength: ( delegateLimit != null ? delegateLimit.maxChars : -1 )
                Keys.onReturnPressed: {
                    if ( properties != undefined ) {
                        properties.setProperty( propName, displayText );
                        text = displayText
                    }
                }
                Keys.onEnterPressed: {
                    if ( properties != undefined ) {
                        properties.setProperty( propName, displayText );
                        text = displayText
                    }
                }
            }
        }
    }

    Component {
        id: propertyDoubleDelegate
        RowLayout {
            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                color: textColor
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            SpinBox {
                id: doubleValue
                Layout.alignment: Qt.AlignVCenter; Layout.fillWidth: false

                minimumValue: ( delegateLimit != null ? delegateLimit.min : -1000000. )
                maximumValue: ( delegateLimit != null ? delegateLimit.max : 1000000. )
                stepSize: ( delegateLimit != null ? delegateLimit.stepSize : 0.1 )
                decimals: 2
                value: propData
                onValueChanged: {
                    if ( properties != undefined )
                        properties.setProperty( propName, value );
                }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: propertyIntDelegate
        RowLayout {
            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                color: textColor
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            SpinBox {
                id: doubleValue
                Layout.alignment: Qt.AlignVCenter; Layout.fillWidth: false
                minimumValue: ( delegateLimit != null ? delegateLimit.min : -1000000 )
                maximumValue: ( delegateLimit != null ? delegateLimit.max : 1000000 )
                stepSize: 1
                decimals: 0
                value: propData
                onValueChanged: {
                    if ( properties != undefined )
                        properties.setProperty( propName, value );
                }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: propertyColorDelegate
        RowLayout {
            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                color: textColor
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Rectangle {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: 45
                Layout.fillHeight: true
                color: propData
            }
            Button {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: 20
                text: "..."
                onClicked: { currentPropName = propName; colorDialog.color = propData; colorDialog.visible = true }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: propertySizeDelegate
        RowLayout {
            Text {
                anchors.verticalCenter: parent.verticalCenter;
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                color: textColor
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            SpinBox {
                id: valueX
                decimals: 2
                stepSize: 0.1
                value: propData.width
                onValueChanged: {
                    if ( properties != undefined )
                        properties.setProperty( propName, Qt.size( valueX.value, valueY.value ) )
                }
            }
            Text { anchors.verticalCenter: parent.verticalCenter; text: " x " ; color: textColor}
            SpinBox {
                id: valueY
                decimals: 2
                stepSize: 0.1
                value: propData.height
                onValueChanged: {
                    if ( properties != undefined )
                        properties.setProperty( propName, Qt.size( valueX.value, valueY.value ) )
                }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: propertyFontDelegate
        RowLayout {
            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                color: textColor
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Text {
                Layout.alignment: Qt.AlignVCenter
                id: sampleText
                text: fontDialog.font.family + " " + fontDialog.font.pixelSize + "px"
                color: textColor
            }
            Button {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: 20
                text: "..."
                onClicked: { currentPropName = propName; fontDialog.font = propData; fontDialog.visible = true }

            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: propertyBoolDelegate
        RowLayout {
            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                color: textColor
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            Switch {
                id: boolSwitch
                Layout.alignment: Qt.AlignVCenter
                checked: propData
                onCheckedChanged: {
                    if ( properties != undefined )
                        properties.setProperty( propName, boolSwitch.checked );
                }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    Component {
        id: propertyEnumDelegate
        RowLayout {
            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: maxPropertyLabelWidth
                text: propName + " : "
                color: textColor
                onTextChanged: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
                Component.onCompleted: { maxPropertyLabelWidth = Math.max( contentWidth, maxPropertyLabelWidth ) }
            }
            ComboBox {
                id: enumCombo
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: 75
                model: properties.getEnumValueLabels( propName );
                currentIndex: propData
                onActivated: {
                    currentIndex = index;
                    if ( properties != undefined )
                        properties.setProperty( propName, index );
                }
            }
            Item { Layout.fillWidth: true }    // Space eater
        }
    }

    function selectPropertyDelegate( typeName, isEnum ) {
        if ( isEnum )
            return propertyEnumDelegate
        else {
            switch ( typeName ) {
            case "QString": return propertyStringDelegate
            case "double": return propertyDoubleDelegate
            case "int": return propertyIntDelegate
            case "QColor": return propertyColorDelegate
            case "QSizeF": return propertySizeDelegate
            case "QFont": return propertyFontDelegate
            case "bool":return propertyBoolDelegate
            }
        }
        return propertyDefaultDelegate
    }
}

