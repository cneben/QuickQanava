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
// \file	qpsDataSource.cpp
// \author	benoit@qanava.org
// \date	2015 12 21
//-----------------------------------------------------------------------------

// Qt headers
#include <QVariant>

// QuickProperties headers
#include "./qpsDataSource.h"
#include "./qpsTimedProperties.h"

namespace qps { // ::qps

/* DataSource Object Management *///-------------------------------------------
DataSource::DataSource( TimedProperties* properties, QString property, QObject* parent ) :
    QObject( parent ),
    _properties( properties ),
    _property( property ),
    _valid( false ),
    _autoFetch( false ),
    _autoFetchDelay( QTime( 0, 20 ) ),  // Auto fetch delay default to 20mn
    _asynchronous( false ),
    _fetching( false ),
    _workerThread( nullptr )
{
    setValid( false );
    if ( properties != nullptr ) {
        setValid( true );
    }
    // Configure auto fetch timer
    connect( &_autoFetchTimer, &QTimer::timeout, this, &qps::DataSource::autoFetchTimeout );
    _autoFetchTimer.setSingleShot( false );
    _autoFetchTimer.stop( );
}

DataSource::~DataSource( )
{
    if ( _properties != nullptr )  // Probably already done in QObject
        disconnect( _properties, 0, this, 0 );

    if ( _workerThread != nullptr ) {
        _workerThread->quit( );
        _workerThread->wait( );
        delete _workerThread;
    }
}
//-----------------------------------------------------------------------------

/* QuickProperties Data Interface *///-----------------------------------------
bool    DataSource::fetch( )
{
    if ( isFetching() ) // Protect against a fetch request while the previous one is till running
        return false;
    if ( _properties == nullptr ) {
        qDebug( ) << "qps::DataSource::fetch(): can't fetch data to a null properties wrapper object.";
        setValid( false );
        return false;
    }
    setValid( true );
    setFetching( true );
    return true;
}

void    DataSource::setAutoFetch( bool autoFetch )
{
    _autoFetch = autoFetch;
    if ( _autoFetch && _autoFetchDelay.isValid( ) ) {
        _autoFetchTimer.setInterval( qAbs( QTime().msecsTo( _autoFetchDelay ) ) );
        _autoFetchTimer.start( );
    }
    else _autoFetchTimer.stop( );
    emit autoFetchChanged( );
}

void    DataSource::autoFetchTimeout( )
{
    if ( _autoFetch )   // Autofetch might have changed since timer has been started
        fetch( );
}
//-----------------------------------------------------------------------------

/* Asynchronous Data Interface *///--------------------------------------------
void    DataSource::operateWorker( QObject* worker )
{
    Q_ASSERT( worker != nullptr && "qps::DataSource::operateWorker(): can't operate a nullptr worker object" );

    connect( this, SIGNAL( operate() ), worker, SLOT( operate() ) );
    if ( isAsynchronous( ) ) {
        if ( _workerThread != nullptr ) {   // An existing running thread is destroyed to force a clean new one creation
            _workerThread->quit( );
            _workerThread->wait( );
            _workerThread->deleteLater( );
            _workerThread = nullptr;
        }
        if ( _workerThread == nullptr )
            _workerThread = new QThread( this );

        if ( _workerThread != nullptr ) {
            worker->moveToThread( _workerThread );
            connect( _workerThread, &QThread::finished, worker, &QObject::deleteLater );
            _workerThread->start( );
            emit operate( );
        }
    }
    else
        emit operate( );
}
//-----------------------------------------------------------------------------


/* DataSource SML Serialization *///-------------------------------------------
bool DataSourceXMLSerializer::serializeOutProperties( const DataSource& dataSource, QXmlStreamWriter& stream )
{
    stream.writeAttribute( "DSautoFetch", dataSource.getAutoFetch() ? "1" : "0" );
    stream.writeAttribute( "DSautoFetchDelay", dataSource.getAutoFetchDelay().toString() );
    stream.writeAttribute( "DSasynchronous", dataSource.isAsynchronous() ? "1" : "0" );
    return true;
}

bool DataSourceXMLSerializer::serializeInProperties( DataSource& dataSource, QXmlStreamReader& stream )
{
    bool error( false );
    bool dsAutoFetch = ( stream.attributes().value( "DSautoFetch" ).toInt( &error ) == 1 );
    if ( !error )
        dataSource.setAutoFetch( dsAutoFetch );
    QTime dsAutoFetchDelay = ( QTime::fromString( stream.attributes().value( "DSautoFetchDelay" ).toString( ) ) );
    if ( dsAutoFetchDelay.isValid() )
        dataSource.setAutoFetchDelay( dsAutoFetchDelay );
    bool dsAsynchronous = ( stream.attributes().value( "DSasynchronous" ).toInt( &error ) == 1 );
    if ( !error )
        dataSource.setAsynchronous( dsAsynchronous );
    return true;
}
//-----------------------------------------------------------------------------

} // ::qps
