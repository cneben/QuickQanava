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
// \file	qpsSerializer.cpp
// \author	benoit@qanava.org
// \date	2016 01 20
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "./qpsSerializer.h"

// STD headers
#include <iostream>

// Qt headers
#include <QColor>

namespace qps { // ::qps

auto ProtoSerializer::serializeOut( const qps::Properties& properties,
                                    qps::pb::Properties& pbProperties ) -> void
{
    std::cout << "ProtoSerializer::serializeOut()..." << std::endl;
    pbProperties.set_object_name( properties.objectName().toStdString() );
    //pbProperties.set_hidden_static_properties_count( properties.);

    // Serialize static properties
    int pCount = properties.metaObject( )->propertyCount( );
    pbProperties.set_static_property_count( pCount );
    for ( int i = 0; i < pCount; ++i ) {
        auto pbProperty = pbProperties.add_properties();
        QMetaProperty metaProperty = properties.metaObject( )->property( i );
        QVariant v = properties.property( metaProperty.name( ) );
        QString vs;
        switch ( v.type( ) ) {
        case QVariant::Type::SizeF: {
            QSizeF s = v.toSizeF( );
            QTextStream vss( &vs );
            vss << s.width( ) << " x " << s.height( );
            break;
        }
        default:
            vs = v.toString( );
        }
        pbProperty->set_name( std::string( metaProperty.name() ) );
        pbProperty->set_type( v.type() );
        pbProperty->set_value( vs.toStdString() );
    }

    // Serialize dynamic properties
    QList< QByteArray > dynamicProperties = properties.dynamicPropertyNames( );
    pbProperties.set_static_property_count( dynamicProperties.size() );
    for ( int d = 0; d < dynamicProperties.size( ); d++ ) {
        auto pbProperty = pbProperties.add_properties();
        QString propertyName = dynamicProperties.at( d );
        QVariant v = properties.property( propertyName.toLatin1( ) );
        QString vs;
        switch ( v.type( ) ) {
        case QVariant::Type::SizeF: {
            QSizeF s = v.toSizeF( );
            QTextStream vss( &vs );
            vss << s.width( ) << " x " << s.height( );
            break;
        }
        default:
            vs = v.toString( );
        }

        pbProperty->set_name( propertyName.toStdString() );
        pbProperty->set_type( v.type() );
        pbProperty->set_value( vs.toStdString() );
    }
}

auto ProtoSerializer::serializeIn( const qps::pb::Properties& pbProperties,
                                   qps::Properties& properties ) -> void
{
    for ( auto& pbProperty : pbProperties.properties() ) {
        QVariant value;
        switch ( pbProperty.type() ) {
        case QVariant::Double: {
            QString v( QString::fromStdString( pbProperty.value() ) );
            value = v.toDouble();
        }
            break;
        case QVariant::Bool: {
            QString v( QString::fromStdString( pbProperty.value() ) );
            value = ( v == "true" );
        }
            break;
        case QVariant::Int: {
            QString v( QString::fromStdString( pbProperty.value() ) );
            value = v.toInt();
        }
            break;
        case QVariant::String: {
            value = QString::fromStdString( pbProperty.value() );
        }
            break;
        case QVariant::Color: {
            QColor c( QString::fromStdString( pbProperty.value() ) );
            if ( c.isValid( ) )
                value = QVariant::fromValue< QColor >( c );
        }
            break;
        case QVariant::SizeF: {
                QString s = QString::fromStdString( pbProperty.value() );
                QTextStream ts( &s );
                qreal w; ts >> w;
                QString separator; ts >> separator;
                qreal h; ts >> h;
                value = QVariant::fromValue< QSizeF >( QSizeF( w, h ) );
            }
            break;
        default:
            std::cerr << "qps::ProtoSerializer::serializeIn(): Warning: Unknown property with type=" << pbProperty.type() << " name=" << pbProperty.name() << std::endl;
            break;
        }
        if ( value.isValid() )
            properties.setProperty( QString::fromStdString( pbProperty.name() ),
                                    value );
    }
}

} // ::qps



