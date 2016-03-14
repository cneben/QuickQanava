/*
    This file is part of QuickProperties library.

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
// This file is a part of the QuickProperties software. Copyright 2015 Benoit Autheman Copyright 2014 Benoit AUTHEMAN.
//
// \file	QpsPropertyCreator.qml
// \author	benoit@qanava.org
// \date	2015 10 20
//-----------------------------------------------------------------------------

import QtQuick 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import "qrc:/QuickProperties"

/*! \brief High level UI component acting as a factory for individual "property" element in a QpsProperties or qps::TimedProperties object.
 *
 */
Item {
    id: propertiesEditor

    //! QpsProperties object where this component should create new property.
    property var    properties
    //! Color used for this component text.
    property color  textColor: Qt.rgba( 0., 0., 0., 1. )
    //! Emmited whenever a new property is created in 'properties' by this component (usually when the user click on "create").
    signal  propertyCreated( var propertyName )

    //! Component will expand to desired size if its content has a smaller implicit size.
    Layout.preferredWidth: Math.max( layout.implicitWidth, 250 )
    Layout.preferredHeight: Math.max( layout.implicitHeight, 100 )

    function validateDialog( ) {
        var validateCreation = false
        var warningText = ""
        if ( nameTf.text != "" )
        {
            if ( !properties.hasProperty( nameTf.text ) )
                validateCreation = true
            else
                warningText = qsTr( "Property name already exists" ) + qpsTranslator.languageChange
        }
        createWarning.text = warningText
        createButton.enabled = validateCreation
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: 2
        Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
        Layout.fillWidth: true

        Text {
            Layout.fillWidth: true; Layout.fillHeight: false
            text: qsTr( "Name:" ) + qpsTranslator.languageChange
            color: textColor
        }
        TextField {
            id: nameTf
            Layout.fillWidth: true; Layout.fillHeight: false
            placeholderText: qsTr( "Enter new property name" ) + qpsTranslator.languageChange
            onTextChanged: { validateDialog( ) }
        }
        Text {
            Layout.fillWidth: true; Layout.fillHeight: false
            text: qsTr( "Type:" ) + qpsTranslator.languageChange
            color: textColor
        }
        ComboBox {
            id: typeCb
            Layout.fillWidth: true; Layout.fillHeight: false
            model: [ "Integer", "String", "Color", "Size" ]
        }
        Text {
            Layout.fillHeight: false
            Layout.minimumHeight: 1
            Layout.preferredHeight: ( text === "" ? 1 : contentHeight )
            id: createWarning
            text: ""
            font.italic: true
            font.pointSize: 7
            color: textColor
        }
        RowLayout {
            Item { Layout.fillWidth: true } // Space eater
            Button {
                id: createButton
                Layout.alignment: Qt.AlignRight
                text: qsTr( "Create" ) + qpsTranslator.languageChange
                onClicked: {
                    if ( typeCb.currentIndex == 0 )
                        properties.addDynamicProperty( nameTf.text, 0 )
                    else if ( typeCb.currentIndex == 1 )
                        properties.addDynamicProperty( nameTf.text, "" )
                    else if ( typeCb.currentIndex == 2 )
                        properties.addDynamicProperty( nameTf.text, Qt.black )

                    propertyCreated( nameTf.text )
                    typeCb.currentIndex = 0
                    nameTf.text = ""
                }
            }
        }
    }   // ColumnLayout: layout
}
