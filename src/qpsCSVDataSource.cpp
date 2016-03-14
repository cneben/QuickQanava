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
// \file	qpsCSVDataSource.cpp
// \author	benoit@qanava.org
// \date	2015 12 26
//-----------------------------------------------------------------------------

// Qt headers
#include <QVariant>

// QuickProperties headers
#include "./qpsCSVDataSource.h"

namespace qps { // ::qps

/* CSV Data Source Worker Management *///--------------------------------------
CSVWorker::CSVWorker( ) :
    _csvIoDevice( nullptr ),
    _properties( nullptr )
{

}

void    CSVWorker::operate( )
{
    Q_ASSERT( _properties != nullptr );
    if ( _csvIoDevice == nullptr ||
         !_csvIoDevice->isReadable( ) ) {
        qDebug( ) << "qps::CSVWorker::operate(): CSV input stream is nullptr or not readable.";
        emit operateFinish( );
        return;
    }
    if ( _tvms.size( ) < 1 ) {  // Error in set source, there must be at least one tvm associed with first column
        emit operateFinish( );
        return;
    }
    // Force destination time value maps clearing
    QMap< int, qps::TimeValueMap* >::iterator tvmIter = _tvms.begin( );
    while ( tvmIter != _tvms.end() ) {
        tvmIter.value()->tsClear();
        ++tvmIter;
    }

    QTextStream csvStream( _csvIoDevice );
    csvStream.seek( 0 );    // Skip header
    csvStream.readLine( );
    while ( !csvStream.atEnd( ) ) {
        QString l = csvStream.readLine( );
        if ( l.isEmpty( ) )
            break;

        QStringList values = l.split( "," );
        if ( values.length() > 0 ) {
            // Read the date at column 0 ...
            QDateTime t = QDateTime::fromString( values.at( 0 ), Qt::ISODate );
            if ( t.isValid( ) ) {
                // ... then set the value for properties of next columns (>=1) to their respective time value map at the correct time
                for ( int c = 1; c < values.length( ); c++ ) {
                    qps::TimeValueMap* tvm = _tvms.value( c - 1, nullptr );
                    if ( tvm != nullptr ) {
                        double v = values.at( c ).toDouble( );
                        if ( !qIsNaN( v ) )
                            tvm->qpsInsert( t, v );
                    }
                }
            }
        }
    }

    // Force tvm update() calls
    foreach ( qps::TimeValueMap* tvm, _tvms )
        tvm->qpsUpdate( );
    emit operateFinish( );
}

void    CSVWorker::setSource( qps::TimedProperties* properties, QIODevice* csvIoDevice, QMap< int, QString >* columnPropertyMap )
{
    _csvIoDevice = csvIoDevice;
    _properties = properties;

    Q_ASSERT( _properties != nullptr );
    if ( _csvIoDevice == nullptr ||
         !_csvIoDevice->isReadable( ) ) {
        qDebug( ) << "qps::CSVWorker::operate(): CSV input stream is not nullptr or not readable.";
        return;
    }
    QTextStream csvStream( _csvIoDevice );
    csvStream.flush();
    if ( !_csvIoDevice->isSequential( ) )
        csvStream.seek( 0 );    // If IODevice is a QBuffer, current position might already been at end

    QString header = csvStream.readLine( );
    qDebug( ) << "header=" << header;
    if ( header.isNull( ) || header.isEmpty( ) ) {
        qDebug( ) << "qps::CSVWorker::operate(): Can't read CSV file header.";
        return;
    }
    QStringList columns = header.split( "," );
    if ( columns.size( ) < 2 ) {
        qDebug( ) << "qps::CSVWorker::operate(): CSV file must have at least two columns.";
        return;
    }
    Q_ASSERT( _tvms.size( ) == 0 );
    for ( int c = 1; c < columns.size( ); c++ ) {
        QString column = columns.at( c );
        if ( column.isNull() || column.isEmpty( ) ) {
            qDebug( ) << "qps::CSVWorker::operate(): Can't read CSV file column " << c;
            return;
        }
        qps::TimeValueMap* columnTvm = _properties->getTimeValueMap( column );
        if ( columnTvm == nullptr ) {   // Create a dynamic property for the column
            _properties->addDynamicProperty( column, QVariant( 1.0 ) );
            columnTvm = _properties->getTimeValueMap( column );
        }
        qDebug( ) << "Column " << column << "  TVM=" << columnTvm;
        if ( columnTvm != nullptr )
            _tvms.insert( c - 1, columnTvm );
    }

    // Override CSV header mapping if the user has specified a custom mapping for some columns
    if ( columnPropertyMap != nullptr ) {
        QMap< int, QString >::const_iterator columnPropertyIter = columnPropertyMap->begin( );
        while ( columnPropertyIter != columnPropertyMap->end( ) ) {
            qps::TimeValueMap* columnTvm = _properties->getTimeValueMap( columnPropertyIter.value() );
            if ( columnTvm == nullptr ) {   // Create a dynamic property for the column
                _properties->addDynamicProperty( columnPropertyIter.value(), QVariant( 1.0 ) );
                columnTvm = _properties->getTimeValueMap( columnPropertyIter.value() );
            }
            if ( columnTvm != nullptr )
                _tvms.insert( columnPropertyIter.key(), columnTvm );
            ++columnPropertyIter;
        }
    }

    if ( _tvms.size( ) != columns.size( ) - 1 ) {    // -1, because there is no time value map associed with the first "Date" column
        qDebug( ) << "qps::CSVWorker::operate(): Warning all columns in CSV file can't be mapped to properties property elements names.";
    }
}
//-----------------------------------------------------------------------------


/* CSV Data Source Management *///---------------------------------------------
CSVDataSource::CSVDataSource( TimedProperties* properties, QString property, QObject* parent ) :
    DataSource( properties, property, parent ),
    _worker( nullptr ),
    _csvIoDevice( nullptr )
{
    setAsynchronous( true );
    connect( &_csvFileWatcher, &QFileSystemWatcher::fileChanged, this, &qps::CSVDataSource::csvFileModified );
}

CSVDataSource::~CSVDataSource( )
{
    qDebug() << "CSVDataSource::~CSVDataSource( )";
    if ( _worker != nullptr ) {
        _worker->deleteLater();
        _worker = nullptr;
    }
    if ( _csvIoDevice != nullptr ) {
        _csvIoDevice->close( );
        // FIXME 20151227 Heap crash on win64?
        //delete _csvIoDevice;
    }
}

void    CSVDataSource::setSourceCsvFile( QString sourceCsvFile )
{
    Q_ASSERT( !sourceCsvFile.isEmpty( ) && "qps::CSVDataSource::setCSVSource(): cvsFileName can't be empty." );

    QFileInfo csvFi( sourceCsvFile );
    if ( csvFi.exists() ) {
        QStringList watchedFiles = _csvFileWatcher.files();
        if ( watchedFiles.size() > 0 )
            _csvFileWatcher.removePaths( watchedFiles );
        _csvFileWatcher.addPath( sourceCsvFile );

        QFile* csvFile = new QFile();
        csvFile->setFileName( sourceCsvFile );
        if ( csvFile->open( QFile::ReadOnly ) )
        {
            _sourceCsvFile = sourceCsvFile;
            setCSVSource( csvFile );
            emit sourceCsvFileChanged( );
        }
         else qDebug() << "qps::CSVDataSource::setCSVSource(): can't open " << sourceCsvFile;
    } else qDebug() << "qps::CSVDataSource::setCSVSource(): " << sourceCsvFile << " does not exists";
}

bool    CSVDataSource::fetch( )
{
    QException e;
    try {
        if ( !DataSource::fetch( ) )
            throw e;

        if ( _csvIoDevice == nullptr || !_csvIoDevice->isOpen( ) ) {
            qDebug( ) << "qps::CSVDataSource::fetch(): trying to fetch data from a closed CSV file.";
            throw e;
        }
    } catch (...) {
        setValid( false );
        setFetching( false );
        emit fetchFinish( );
    }

    _worker = new CSVWorker( );
    _worker->setSource( getProperties( ), _csvIoDevice, &_columnPropertyMap );
    connect( _worker, &CSVWorker::operateFinish, this, &CSVDataSource::workerFinished );
    operateWorker( _worker );
    return true;
}

void    CSVDataSource::workerFinished( )
{
    qDebug( ) << "CSVDataSource::workerFinished()";
    if ( _csvIoDevice->isOpen() )
        _csvIoDevice->close();
    if ( _worker != nullptr ) {
        delete _worker;
        _worker = nullptr;
    }
    setFetching( false );
    emit fetchFinish( );
}

void    CSVDataSource::setCSVSource( QIODevice* csvIoDevice )
{
    Q_ASSERT( csvIoDevice != nullptr );

    if ( _csvIoDevice != nullptr &&
         _csvIoDevice != csvIoDevice ) { // Protect against multiple call with the same IODevice
        _csvIoDevice->close( );
        delete _csvIoDevice;
        _csvIoDevice = nullptr;
    }
    _csvIoDevice = csvIoDevice;
    if ( csvIoDevice->isReadable( ) ) {
        _csvIoDevice = csvIoDevice;
        setValid( true );
    }
}
//-----------------------------------------------------------------------------


/* YFDataSource Object Management *///-----------------------------------------
YFDataSource::YFDataSource( qps::TimedProperties* properties, QObject* parent ) :
    DataSource( properties, "", parent ),
    _symbolCode( "" ),
    _symbolTo( QDateTime::currentDateTime( ) ),
    _symbolFrom( _symbolTo.addYears( -1 ) ),
    _reply( nullptr ),
    _csvDs( properties, "", parent )
{
    _csvDs.setAsynchronous( false );    // YFDataSource is already asynchronous, parsing CSV synchronously cause almost no freeze
}

YFDataSource::~YFDataSource( )
{
    if ( _reply != nullptr ) {  // Clean an eventual existing HTTP request
        _reply->deleteLater( );
        _reply = nullptr;
    }
}
//-------------------------------------------------------------------------

/* YF Data Source HTTP Interface *///------------------------------------------
bool    YFDataSource::fetch( )
{
    if ( !_symbolFrom.isValid() ||
         !_symbolTo.isValid() ||
         _symbolCode.isEmpty() ||
         _symbolCode.isNull() ) {
        setValid( false );
        return false;
    }
    QException e;   // Ok since everything is catched next
    try {
        DataSource::fetch( );   // Update 'valid'
        if ( !isValid( ) )
            throw e;

        QString startDay    = "b=" + QString::number( _symbolFrom.date().day()       ) + "&amp;";
        QString startMonth  = "a=" + QString::number( _symbolFrom.date().month() - 1 ) + "&amp;"; // Month-1
        QString startYear   = "c=" + QString::number( _symbolFrom.date().year()      ) + "&amp;";
        QString endDay      = "e=" + QString::number( _symbolTo.date().day()         ) + "&amp;";
        QString endMonth    = "d=" + QString::number( _symbolTo.date().month() - 1   ) + "&amp;"; // Month-1
        QString endYear     = "f=" + QString::number( _symbolTo.date().year()        ) + "&amp;";
        QString undef       = "g=d&amp;";
        QString stock       = "s=" + _symbolCode;
        QString serverUrl = "http://ichart.finance.yahoo.com/table.csv?";
        QString symbolUrl = serverUrl + startMonth + startDay + startYear + endMonth + endDay + endYear + undef + stock;
        qDebug( ) << "symbolUrl=" << symbolUrl;

        // Clean the HTTP related stuff, it might be second or more call to fetch...
        if ( _httpData.isOpen( ) )
            _httpData.close( );
        if ( _reply != nullptr ) {
            delete _reply;
            _reply = nullptr;
        }

        _httpData.open( QIODevice::ReadWrite );
        _reply = _network.get( QNetworkRequest( symbolUrl ) );
        if ( _reply == nullptr ) {
            qDebug( ) << "qps::YFProperties::fetch( ): HTTP GET error, no reply for " << symbolUrl;
            throw e;
        }
        connect( _reply, &QNetworkReply::finished, this, &YFDataSource::httpFinished );
        connect( _reply, &QIODevice::readyRead, this, &YFDataSource::httpReadyRead );
    } catch (...) {
        setValid( false );
        setFetching( false );
        emit fetchFinish( );
        return false;
    }
    return true;
}

void    YFDataSource::httpFinished( )
{
    QException e;   // Ok since everything is catched next
    try {
        if ( _reply == nullptr ) {
            qDebug( ) << "qps::YFProperties::httpFinished( ): error, can't get a reply to the HTTP request.";
            throw e;
        }

        if ( _reply->error( ) ) {
            qDebug( ) << "qps::YFProperties::httpFinished( ): error, HTTP request reply is incorrect.";
            _reply->deleteLater( ); // Clean the temporary reply
            _reply = nullptr;
            throw e;
        }

        // If the HTTP server answer with a redirection, follow it.
        const QVariant redirectionTarget = _reply->attribute( QNetworkRequest::RedirectionTargetAttribute );
        if ( !redirectionTarget.isNull( ) ) {
            qDebug( ) << "qps::YFProperties::httpFinished( ): Error: HTTP server answer with a redirection.";
            throw e;
        }
    } catch (...) {
        setValid( false );
        setFetching( false );
        emit fetchFinish( );
        return;
    }
    // If there is no reported error, try to read the result CSV data (synchronously)
    _csvDs.setAsynchronous( false );
    _csvDs.setCSVSource( &_httpData );
    _csvDs.fetch( );
    _reply->deleteLater( );
    _reply = nullptr;

    setFetching( false );
    emit fetchFinish( );
}

void    YFDataSource::httpReadyRead( )
{
    // Note QT DOCS:
    // This slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if ( _httpData.isOpen( ) && _reply != nullptr )
    {
        int w = _httpData.write( _reply->readAll( ) );
        qDebug( ) << "qps::YFProperties::httpReadyRead(): " << w << " bytes fetched...";
        qDebug( ) << "_httpData.size=" << _httpData.size( );
        if ( w == -1 )
            qDebug( ) << "qps::YFProperties::httpReadyRead( ): httpData write error.";
    }
}
//-----------------------------------------------------------------------------


/* CSV DataSource XML Serialization *///---------------------------------------
bool CSVDataSourceXMLSerializer::serializeOutProperties( const CSVDataSource& csvDataSource, QXmlStreamWriter& stream )
{
    DataSourceXMLSerializer::serializeOutProperties( csvDataSource, stream );
    stream.writeAttribute( "CSVDsourceCsvFile", csvDataSource.getSourceCsvFile() );
    return true;
}

bool CSVDataSourceXMLSerializer::serializeInProperties( CSVDataSource& csvDataSource, QXmlStreamReader& stream )
{
    DataSourceXMLSerializer::serializeInProperties( csvDataSource, stream );
    QString csvSourceFile = stream.attributes().value( "CSVDsourceCsvFile" ).toString();
    if ( !csvSourceFile.isEmpty( ) )
        csvDataSource.setSourceCsvFile( csvSourceFile );
    return true;
}
//-----------------------------------------------------------------------------


/* CSV DataSource XML Serialization *///---------------------------------------
bool YFDataSourceXMLSerializer::serializeOutProperties( const YFDataSource& yfDataSource, QXmlStreamWriter& stream )
{
    DataSourceXMLSerializer::serializeOutProperties( yfDataSource, stream );
    stream.writeAttribute( "YFDSsymbolCode", yfDataSource.getSymbolCode() );
    stream.writeAttribute( "YFDSexchangeCode", yfDataSource.getExchangeCode() );
    stream.writeAttribute( "YFDSsymbolFrom", yfDataSource.getSymbolFrom( ).toString() );
    stream.writeAttribute( "YFDSsymbolTo", yfDataSource.getSymbolTo( ).toString() );
    return true;
}

bool YFDataSourceXMLSerializer::serializeInProperties( YFDataSource& yfDataSource, QXmlStreamReader& stream )
{
    DataSourceXMLSerializer::serializeInProperties( yfDataSource, stream );
    yfDataSource.setSymbolCode( stream.attributes().value( "YFDSsymbolCode" ).toString() );
    yfDataSource.setSymbolCode( stream.attributes().value( "YFDSexchangeCode" ).toString() );
    yfDataSource.setSymbolFrom( QDateTime::fromString( stream.attributes().value( "YFDSsymbolFrom" ).toString() ) );
    yfDataSource.setSymbolTo( QDateTime::fromString( stream.attributes().value( "YFDSsymbolTo" ).toString() ) );
    return true;
}
//-----------------------------------------------------------------------------

} // ::qps
