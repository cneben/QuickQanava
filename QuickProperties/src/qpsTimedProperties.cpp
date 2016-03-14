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
// \file	qpsTimedProperties.cpp
// \author	benoit@qanava.org
// \date	2015 10 21
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "./qpsTimedProperties.h"

// QT headers
#include <QtGlobal>
#include <QVariant>
#include <QFont>
#include <QQmlEngine>

namespace qps { // ::qps


/* TimedProperties Object Management *///--------------------------------------
TimedProperties::TimedProperties( QObject* parent ) :
    Properties( parent ),
    _current( QDateTime( ) ),
    _first( QDateTime( ) ),
    _last( QDateTime( ) ),
    _timeValue( 1.0 )   // Default to last date (ie 100% of the object properties time interval)
{

}

TimedProperties::~TimedProperties( )
{
    // Delete properties time value maps
    QList< TimeValueMap* > timeValueMaps = _propertiesTimeValueMap.values( );   // Note 20151224: values() construct a new list, it is safe to delete wihtout removing from the source map
    while ( !timeValueMaps.isEmpty( ) )
        delete timeValueMaps.takeFirst( );
    _propertiesTimeValueMap.clear( );
    while ( !_dataSources.isEmpty( ) )
        delete _dataSources.takeFirst( );
}
//-----------------------------------------------------------------------------


/* Property Date Time Management *///------------------------------------------
void    TimedProperties::setCurrent( QDateTime current )
{
    // Current must be between _first and _last or it will be clipped to one of these two values
    if ( current > _last )
        _current = _last;
    else if ( current < _first )
        _current = _first;
    else
        _current = current;

    emit currentChanged( );

    // FIXME 20150708
    // Change properties affected by current time
        // For each property
            // Get QMetaProperty
                // Call meta property notify signal
}

/*!
 * \return can return an invalid (ie !isValid()) QDateTime if current has not been set and last property is invalid, otherwise current == last.
 */
QDateTime   TimedProperties::getCurrent( ) const
{
    if ( !_current.isValid( ) && _last.isValid( ) )
        return _last;
    return _current;
}

void        TimedProperties::updateFirstLast( )
{
    qDebug( ) << "TimedProperties::updateFirstLast( )";
    QDateTime first;
    QDateTime last;
    foreach ( TimeValueMap* tvm, _propertiesTimeValueMap ) {
        if ( tvm->isEmpty() )
            continue;
        QDateTime tvmFirst = tvm->firstKey( ); // Safe since isEmpty() has been checked first
        QDateTime tvmLast = tvm->lastKey( ); // Safe since isEmpty() has been checked first
        if ( !first.isValid() )
            first = tvmFirst;
        if ( !last.isValid() )
            last = tvmLast;

        if ( tvmFirst < first )
            first = tvmFirst;
        if ( tvmLast > last )
            last = tvmLast;
    }
    if ( first.isValid() )
        setFirst( first );
    if ( last.isValid() )
        setLast( last );
}

void        TimedProperties::setProperty( QString propertyName, const QDateTime& propertyDateTime, QVariant propertyValue )
{
    if ( !hasProperty( propertyName ) )
        addDynamicProperty( propertyName, propertyValue );
    if ( hasProperty( propertyName ) ) // Test is property exists or new property creation was successfull
    {
        TimeValueMap* timeValueMap = nullptr;
        if ( !_propertiesTimeValueMap.contains( propertyName ) )
        {
            timeValueMap = new TimeValueMap( nullptr ); // Manual destruction
            connect( timeValueMap, &qps::TimeValueMap::updated, this, &qps::TimedProperties::updateFirstLast );
            _propertiesTimeValueMap.insert( propertyName, timeValueMap );
        }
        else timeValueMap = _propertiesTimeValueMap.value( propertyName, nullptr );

        Q_ASSERT( timeValueMap != nullptr );
        timeValueMap->qpsInsert( propertyDateTime, propertyValue );

        // First and last initialization
        if ( !_last.isValid( ) ) {
            _last = propertyDateTime;
            qps::Properties::setProperty( propertyName, propertyValue );    // If value is the last one, update current value
        }
        if ( !_first.isValid( ) )
            _first = propertyDateTime;

        if ( propertyDateTime > _last ) { // Update object date time interval
            _last = propertyDateTime;
            qps::Properties::setProperty( propertyName, propertyValue );    // If value is the last one, update current value
        }
        else if ( propertyDateTime < _first )
            _first = propertyDateTime;
    }
}

/*!
 * \return an invalid QVariant if a property for the given propertyName can't be found.
 */
QVariant    TimedProperties::getProperty( QString propertyName, const QDateTime& propertyDateTime ) const
{
    if ( _propertiesTimeValueMap.contains( propertyName ) )
    {
        TimeValueMap* timeValueMap = _propertiesTimeValueMap.value( propertyName, nullptr );
        Q_ASSERT( timeValueMap != nullptr );

        if ( timeValueMap->size( ) <= 0 )
            return QVariant( ); // Secure next tests against returning a valid result, do not remove

        // Note (lowerBound() Qt Doc): Returns an iterator pointing to the first item with key key in the map. If the map contains
        //                             no item with key key, the function returns an iterator to the nearest item with a greater key.
        // So: if i.key equals propertyDateTime, just return the value, otherwise, since we want the immediatly lower key, use the --
        //     operator on the iterator, or use the first or last.
        TimeValueMap::const_iterator i = timeValueMap->lowerBound( propertyDateTime );
        if ( i == timeValueMap->end( ) )
            return ( !timeValueMap->isEmpty() ? timeValueMap->last( ) : QVariant( ) );
        else if ( i.key( ).isValid( ) && i.key( ) == propertyDateTime )
            return i.value( );
        else
        {
            if ( i == timeValueMap->end( ) )
                return timeValueMap->last( );
            else if ( i == timeValueMap->begin( ) )
                return timeValueMap->first( );
            else
                return (--i).value( );
        }
    }
    return QVariant( );
}

QVariant    TimedProperties::getProperty( QString propertyName )
{
    // FIXME: 20150616 hack
    return property( propertyName.toLatin1( ) );
}

void        TimedProperties::dumpProperty( QDebug dbg, QString propertyName ) const
{
    QDebugStateSaver saver( dbg ); Q_UNUSED( saver );
    if ( _propertiesTimeValueMap.contains( propertyName ) )
    {
        QMap< QDateTime, QVariant >* timeValueMap = _propertiesTimeValueMap.value( propertyName, nullptr );
        Q_ASSERT( timeValueMap != nullptr );
        foreach ( QDateTime dateTime, timeValueMap->keys( ) )
            dbg.nospace( ) << "\t" << dateTime << ":" << timeValueMap->value( dateTime, QVariant( ) ) << "\n";
    }
    else
        dbg << "no timed values.\n";
}

TimeValueMap*   TimedProperties::getTimeValueMap( QString propertyName )
{
    if ( propertyName.isNull( ) || propertyName.isEmpty( ) )
        return nullptr;
    // Look for an existing time value map for the requested property, or create it if the property exists without time data
    TimeValueMap* timeValueMap = _propertiesTimeValueMap.value( propertyName, nullptr );
    if ( timeValueMap == nullptr && hasProperty( propertyName ) ) {
        timeValueMap = new TimeValueMap( nullptr );
        connect( timeValueMap, &qps::TimeValueMap::updated, this, &qps::TimedProperties::updateFirstLast );
        _propertiesTimeValueMap.insert( propertyName, timeValueMap );
    }
    if ( timeValueMap != nullptr )
        QQmlEngine::setObjectOwnership( timeValueMap, QQmlEngine::CppOwnership );
    return timeValueMap;
}
//-----------------------------------------------------------------------------

/* QML Time Browser Interface *///---------------------------------------------
double     TimedProperties::getTimeValue( ) const
{
    return _timeValue;
}
void    TimedProperties::setTimeValue( double tv )
{
    Q_ASSERT( tv <= 1.00001 && tv >= 0. );
    _timeValue = tv;
    emit timeValueChanged( );

    if ( qFuzzyCompare( 1. + tv, 1. + 1. ) )  // Force 1. value to _last to avoid rounding problems
        setCurrent( _last );
    else
    {
        qint64 startToLastDelay = _first.msecsTo( _last );
        Q_ASSERT( startToLastDelay > 0 );
        setCurrent( _first.addMSecs( startToLastDelay * tv ) );
    }
}
//-----------------------------------------------------------------------------

/* DataSource Management *///--------------------------------------------------
bool    TimedProperties::fetch( )
{
    bool r = true;
    foreach ( qps::DataSource* dataSource, _dataSources )
        r = ( dataSource->fetch( ) ? r : false );
    return r;
}

void    TimedProperties::addDataSource( qps::DataSource* dataSource )
{
    Q_ASSERT( dataSource != nullptr );
    if ( !_dataSources.contains( dataSource ) )
        _dataSources.append( dataSource );
}
//-----------------------------------------------------------------------------

/* XML Serialization *///------------------------------------------------------
void    TimedProperties::serializeTimeValuesOut( QString propertyName, QXmlStreamWriter& stream )
{
    QMap< QDateTime, QVariant >* timeValueMap;
    timeValueMap = _propertiesTimeValueMap.value( propertyName, nullptr );
    if ( timeValueMap != nullptr )
    {
        stream.writeStartElement( "values" );           // <values>
        QList< QDateTime > dateKeys = timeValueMap->keys( );
        QList< QVariant > dateValues = timeValueMap->values( );
        for ( int i = 0; i < dateKeys.size( ); i++ )
        {
            stream.writeStartElement( "dtv" );              // <dtv>  DTV = Date Time Value
            stream.writeAttribute( "d", dateKeys.at( i ).toString( ) );
            stream.writeAttribute( "t", QString::number( dateValues.at( i ).type( ) ) );
            stream.writeAttribute( "v", dateValues.at( i ).toString( ) );
            stream.writeEndElement( );                      // </dtv>
        }
        stream.writeEndElement( );                      // </values>
    }
}

void    TimedProperties::serializeTimeValuesIn( QString propertyName, QXmlStreamReader& stream )
{
    qDebug( ) << "Reading property " << propertyName << " values" << "\n";
    while ( stream.readNextStartElement( ) )
    {
        qDebug( ) << "reading property " << propertyName << " values DTV " << stream.name( ) << "\n";
        if ( stream.name( ) == QString( "dtv" ) )   // <dtv>
        {
            QString date = stream.attributes( ).value( "d" ).toString( );
            QVariant::Type type = ( QVariant::Type )stream.attributes( ).value( "t" ).toInt( );
            QVariant value;
            switch ( type )
            {
            case QVariant::Type::Int:
                value = QVariant( stream.attributes( ).value( "v" ).toInt( ) );
                break;
            case QVariant::Type::Double:
                value = QVariant( stream.attributes( ).value( "v" ).toInt( ) );
                break;
            default:
                value = QVariant( );
                break;
            }

            if ( value.isValid( ) )
                setProperty( propertyName, QDateTime::fromString( date ), value );

            stream.skipCurrentElement( );   // Jump to the next DTV
        }                                           // </dtv>
        else
            stream.skipCurrentElement( );
    }
}
//-----------------------------------------------------------------------------

QDebug  operator<<( QDebug dbg, const TimedProperties& p )
{
    QDebugStateSaver saver( dbg ); Q_UNUSED( saver );  // Automatically save dbg settings, and restore them at destruction

    dbg.nospace( ) << "Properties: " << p.objectName( )<< "\n";
    dbg << "First date time stamp=\t" << p.getFirst( ) << "\n";
    dbg << "Last date time stamp=\t" << p.getLast( ) << "\n";

    int pCount = p.metaObject( )->propertyCount( );
    for ( int i = 0; i < pCount; ++i )
    {
        QMetaProperty metaProperty = p.metaObject( )->property( i );
        const char* propertyName = metaProperty.name( );
        QVariant propertyValue = p.property( propertyName );

        dbg.nospace( ) << propertyName << "=" << propertyValue.toString( ) << "(current) \n";
        p.dumpProperty( dbg.nospace( ), propertyName );
    }
    // Serialize dynamic properties
    QList< QByteArray > dynamicProperties = p.dynamicPropertyNames( );
    for ( int d = 0; d < dynamicProperties.size( ); d++ )
    {
        QString propertyName = dynamicProperties.at( d );
        QVariant propertyValue = p.property( propertyName.toLatin1( ) );
        dbg.nospace( ) << propertyName << "=" << propertyValue.toString( ) << "(current) \n";
        p.dumpProperty( dbg.nospace( ), propertyName );
    }

    return dbg;
}

} // ::qps


