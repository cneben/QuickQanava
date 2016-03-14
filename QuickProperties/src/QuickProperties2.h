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
// This file is a part of the QuickProperties2 library.
//
// \file    QuickProperties2.h
// \author	benoit@qanava.org
// \date	2012 02 08
//-----------------------------------------------------------------------------

#ifndef QuickProperties2_h
#define QuickProperties2_h

// Qt headers
// Nil

// QuickProperties headers
#include "./qpsProperties.h"
#include "./qpsTimedProperties.h"
#include "./qpsPropertiesList.h"
#include "./qpsLimit.h"
#include "./qpsContainerListModel.h"

namespace qps {

//! Abstract interface to manage dynamic language changes in QuickProperties.
/*! See http://qt-project.org/wiki/How_to_do_dynamic_translation_in_QML for an
 *  explanation of this hack and its credits...
 *  \nosubgrouping
 */
class AbstractTranslator : public QObject
{
    /*! \name Translator Object Management *///--------------------------------
    //@{
    Q_OBJECT
public:
    //! FIXME.
    AbstractTranslator( QObject* parent = nullptr ) : QObject( parent ) { }
    virtual ~AbstractTranslator( ) { }
private:
    Q_DISABLE_COPY( AbstractTranslator )
signals:
    void    languageChanged( );
    //@}
    //-------------------------------------------------------------------------
};

}

struct QuickProperties {
    static void initialize( QQmlEngine* engine = nullptr, qps::AbstractTranslator* abstractTranslator = nullptr ) {
        if ( engine != nullptr && abstractTranslator != nullptr )
            engine->rootContext( )->setContextProperty( "qpsTranslator", abstractTranslator );
        qmlRegisterType< qps::Properties >( "QuickProperties", 2, 0, "Properties");
        qmlRegisterType< qps::Limit >( "QuickProperties", 2, 0, "Limit");
        qmlRegisterType< qps::StringLimit >( "QuickProperties", 2, 0, "StringLimit");
        qmlRegisterType< qps::RealLimit >( "QuickProperties", 2, 0, "RealLimit");
        qmlRegisterType< qps::IntLimit >( "QuickProperties", 2, 0, "IntLimit");
    }
};

#endif // QuickProperties2_h

