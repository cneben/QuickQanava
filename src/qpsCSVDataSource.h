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
// \file	qpsCSVDataSource.h
// \author	benoit@qanava.org
// \date	2015 12 26
//-----------------------------------------------------------------------------

#ifndef qpsCSVDataSource_h
#define qpsCSVDataSource_h

// Qt headers
#include <QNetworkAccessManager>    // YFProperties
#include <QBuffer>                  // YFProperties

// QuickProperties headers
#include "./qpsDataSource.h"
#include "./qpsTimedProperties.h"

namespace qps { // ::qps

/*! \brief Private thread worker object used internally in CSVDataSource.
 *
 */
class CSVWorker : public QObject
{
    Q_OBJECT
public:
    CSVWorker( );
public slots:
    void    operate( );
public:
    /*! \brief Set the CSV source file before calling operate().
     *
     * \param columnPropertyMap optional user specified column-property mapping (will override the mapping read in the header, can't be empty).
     */
    void    setSource( qps::TimedProperties* properties, QIODevice* csvFile, QMap< int, QString >* columnPropertyMap = nullptr );
signals:
    void    operateFinish( );
protected:
    QIODevice*                      _csvIoDevice;
    qps::TimedProperties*           _properties;
    //! Map CSV column index to time value map.
    QMap< int, qps::TimeValueMap* > _tvms;
};

/*! \brief Synchronous Comma Separated Value (.csv) data source.
 *
 * CSVDataSource only support a subset of CSV necessary to import timed values:
 * \li input file must have a valid header column.
 * \li column header names must correspond to a property name existing in the qps::TimedProperties \c properties object specified in source constructor.
 * (until you manually override this default behaviour with mapColumnToProperty()). If a property element can't be mapped to an existing property,
 * a dynamic property will be automatically created in the target \c properties.
 * \li the first column must consist of date/time data.
 *
 * CSVDataSource is asynchronous by default.
 *
 * Minimal code for loading a .CSV file containing a stock 'Open' values over time to a qps::TimedProperties subclass:
 * \code
 *   // Asynchronously load a local CSV stock file
 *   Stock* stock = new Stock( nullptr ); // A qps::TimedProperties subclasse defining custom "Open" Q_PROPERTY
 *   CSVDataSource* csvDataSource = new CSVDataSource( stock, nullptr );
 *   csvDataSource->setAsynchronous( true );
 *   csvDataSource->setSourceCsvFile( ":/euronext100.csv" );  // First column is date/time stamps, second CSV column is labelled "Open"
 *   stock->addDataSource( csvDataSource );
 *   stock->fetch( );   // Non blocking...
 *
 *   // Time value map for the "fetched" property is already available but will be filed in background (catch its update signal qps::TimeValueMap::inserted()...)
 *   TimeValueMap* tvm = yfStock->getTimeValueMap( "Open" );
 * \endcode
 *
 * \nosubgrouping
 */
class CSVDataSource : public qps::DataSource
{
    Q_OBJECT
public:
    //! Create a data source from a comma separated value (.csv) file.
    CSVDataSource( qps::TimedProperties* properties = nullptr, QString property = "", QObject* parent = nullptr );
    virtual ~CSVDataSource( );
private:
    Q_DISABLE_COPY( CSVDataSource )

public:
    /*! \brief Fetch data from a CSV file with options set from setCVSSource().
     *
     *  Any user column/property mapping set with mapColumnToProperty() will be cleared.
     */
    virtual bool    fetch( ) override;
protected slots:
    void            workerFinished();
private:
    CSVWorker*      _worker;

public:
    /*! \brief Force mapping of a CSV column to a specific property (even if the column header could be mapped to an existing property name).
     *
     * Any user set mapping is cleared after a call to fetch().
     *
     * Calling this method after a call to fetch() or while fetch() is running is not thread safe.
     */
    void                    mapColumnToProperty( int column, QString property ) { _columnPropertyMap.insert( column, property ); }
protected:
    QMap< int, QString >    _columnPropertyMap;

public:
    //! CSV source filename.
    Q_PROPERTY( QString sourceCsvFile READ getSourceCsvFile WRITE setSourceCsvFile NOTIFY sourceCsvFileChanged )
    QString         getSourceCsvFile( ) const { return _sourceCsvFile; }
    //! Set the CSV source filename before fetch() is called.
    void            setSourceCsvFile( QString sourceCsvFile );
signals:
    void            sourceCsvFileChanged( );
private:
    QString         _sourceCsvFile;
public:
    //! Set the CSV input stream before fetch() is called.
    void    setCSVSource( QIODevice* csvIoDevice );
private:
    QIODevice*          _csvIoDevice;
    QFileSystemWatcher  _csvFileWatcher;
private slots:
    //! Connected to CSV file watcher, generate a fetch when the file is modified.
    void    csvFileModified() { fetch( ); }
};

/*! \brief Sample Yahoo! Finance asynchronous HTTP stock datasource.
 *
 * \nosubgrouping
 */
class YFDataSource : public qps::DataSource
{
    Q_OBJECT
public:
    //! \param properties must inherit from qps::TimedProperties and have at least an "Open" double property (eventually High, Low, Close, Volume, Adj Close).
    YFDataSource( qps::TimedProperties* properties, QObject* parent = nullptr );
    virtual ~YFDataSource( );
private:
    Q_DISABLE_COPY( YFDataSource )

    /*! \name HTTP Interface *///----------------------------------------------
    //@{
public:
    Q_INVOKABLE virtual bool    fetch( ) override;

public:
    Q_PROPERTY( QString exchangeCode READ getExchangeCode WRITE setExchangeCode NOTIFY exchangeCodeChanged )
    QString         getExchangeCode( ) const { return _exchangeCode; }
    void            setExchangeCode( QString exchangeCode ) { _exchangeCode = exchangeCode; emit exchangeCodeChanged(); }
signals:
    void            exchangeCodeChanged( );
private:
    QString         _exchangeCode;

public:
    Q_PROPERTY( QString symbolCode READ getSymbolCode WRITE setSymbolCode NOTIFY symbolCodeChanged )
    QString         getSymbolCode( ) const { return _symbolCode; }
    void            setSymbolCode( QString symbolCode ) { _symbolCode = symbolCode; emit symbolCodeChanged(); }
signals:
    void            symbolCodeChanged( );
private:
    QString         _symbolCode;

public:
    Q_PROPERTY( QDateTime symbolTo READ getSymbolTo WRITE setSymbolTo NOTIFY symbolToChanged )
    QDateTime       getSymbolTo( ) const { return _symbolTo; }
    void            setSymbolTo( QDateTime symbolTo ) { _symbolTo = symbolTo; emit symbolToChanged(); }
signals:
    void            symbolToChanged( );
private:
    QDateTime       _symbolTo;

public:
    Q_PROPERTY( QDateTime symbolFrom READ getSymbolFrom WRITE setSymbolFrom NOTIFY symbolFromChanged )
    QDateTime       getSymbolFrom( ) const { return _symbolFrom; }
    void            setSymbolFrom( QDateTime symbolFrom ) { _symbolFrom = symbolFrom; emit symbolFromChanged(); }
signals:
    void            symbolFromChanged( );
private:
    QDateTime       _symbolFrom;

private:
    QNetworkAccessManager   _network;
    QNetworkReply*          _reply;
    QBuffer                 _httpData;
    CSVDataSource           _csvDs;
private slots:
    void    httpFinished( );
    void    httpReadyRead( );
    //@}
    //-------------------------------------------------------------------------
};

//! Static helper class for XML serialization of qps::CSVDataSource objects.
class CSVDataSourceXMLSerializer {
public:
    /*! Serialize \c dataSource properties to an XML stream current element properties.
     *
     *  \note Base qps::DataSource properties are also automatically serialized with a call to DataSourceXmlSerializer::sertializeOutProperties();
     *  \return false if an error occurs.
     */
    static bool serializeOutProperties( const CSVDataSource& csvDataSource, QXmlStreamWriter& stream );
    /*! Serialize \c dataSource properties from an XML stream current element properties.
     *
     *  \note Base qps::DataSource properties are also automatically serialized with a call to DataSourceXmlSerializer::sertializeInProperties();
     *  \return false if an error occurs.
     */
    static bool serializeInProperties( CSVDataSource& csvDataSource, QXmlStreamReader& stream );
};

//! Static helper class for XML serialization of qps::YFDataSource objects.
class YFDataSourceXMLSerializer {
public:
    /*! Serialize \c dataSource properties to an XML stream current element properties.
     *
     *  \note Base qps::DataSource properties are also automatically serialized with a call to DataSourceXmlSerializer::sertializeOutProperties();
     *  \return false if an error occurs.
     */
    static bool serializeOutProperties( const YFDataSource& yFdataSource, QXmlStreamWriter& stream );
    /*! Serialize \c dataSource properties from an XML stream current element properties.
     *
     *  \note Base qps::DataSource properties are also automatically serialized with a call to DataSourceXmlSerializer::sertializeInProperties();
     *  \return false if an error occurs.
     */
    static bool serializeInProperties( YFDataSource& yFdataSource, QXmlStreamReader& stream );
};

} // ::qps

QML_DECLARE_TYPE( qps::CSVDataSource )
QML_DECLARE_TYPE( qps::YFDataSource )

#endif // qpsCSVDataSource_h

