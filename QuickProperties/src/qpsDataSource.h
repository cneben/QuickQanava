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
// \file	qpsDataSource.h
// \author	benoit@qanava.org
// \date	2015 12 24
//-----------------------------------------------------------------------------

#ifndef qpsDataSource_h
#define qpsDataSource_h

// Qt headers
#include <QTQml>
#include <QThread>
#include <QTime>
#include <QTimer>

// QuickProperties headers
// Nil

namespace qps { // ::qps

class TimedProperties;

/*! \brief Abstract data source for a QObject dynamic or static property.
 *
 * To specialize a data source, subclass and re-implement the fetch( ) method (don't forget
 * to call super DataSource::fetch() method at the beginning of your specialized
 * version).
 *
 * \nosubgrouping
 */
class DataSource : public QObject
{
    /*! \name DataSource Object Management *///--------------------------------
    //@{
    Q_OBJECT
public:
    /*! \brief Create a qps::DataSource for property \c property of a \c target QObject.
     *
     * A newly created DataSource object is invalid if either the \c target QObject or target
     * \c property does not exists (otherwise, it is considered valid).
     *
     * \code
     * // qps::ConcreteDataSource beeing a concrete class implementing qps::DataSource interface
     * qps::ConcreteDataSource* d = new qps::ConcreteDataSource( target, "myProperty" );
     * Q_ASSERT( d->isValid() );
     * \endcode
     */
    DataSource( TimedProperties* properties = nullptr, QString property = "", QObject* parent = nullptr );
    virtual ~DataSource( );
private:
    Q_DISABLE_COPY( DataSource )
    //@}
    //-------------------------------------------------------------------------

    /*! \name QuickProperties Data Interface *///------------------------------
    //@{
protected:
    TimedProperties*    getProperties( ) { return _properties; }
    QString             getProperty( ) { return _property; }
private:
    TimedProperties*    _properties;
    QString             _property;

public:
    /*! Fetch the data from the source to the destination property.
     *
     * Any overriding method should call this base method at the start of its implementation and interrupt fetch if
     * false is returned:
     *
     * \code
     * bool    ConcreteDataSource::fetch( )
     * {
     *   if ( !DataSource::fetch( ) )
     *      return false;
     *    // Concrete implementation ...
     *   return true;
     * }
     * \endcode
     *
     * \return true if fetch is successfull, false otherwise (either target object or property has been destroyed, or isValid() return false).
     * \note Trying to fetch an invalid property will return false.
     * \note \c fetching read-only property is automatically set to true.
     */
    Q_INVOKABLE virtual bool    fetch( );
signals:
    void        fetchFinish();

public:
    //! True for a valid source, false otherwise.
    Q_PROPERTY( bool valid READ isValid WRITE setValid NOTIFY validChanged )
    //! \copydoc qps::DataSource::valid
    bool            isValid( ) const { return _valid; }
    //! \copydoc qps::DataSource::valid
    void            setValid( bool valid ) { _valid = valid; emit validChanged( ); }
signals:
    //! Emitted whenever \c valid property change.
    void            validChanged( );
private:
    //! \copydoc qps::DataSource::valid
    bool            _valid;

public:
    //! Set to true to enable "auto-fetching" of data with a delay of \a autoFetchDelay (default to false).
    Q_PROPERTY( bool autoFetch READ getAutoFetch WRITE setAutoFetch NOTIFY autoFetchChanged )
    //! \copydoc qps::DataSource::autoFetch
    bool            getAutoFetch( ) const { return _autoFetch; }
    //! Setting \c autoFetch to true will restart the internal fetch timer (ie, next auto fetch will occurs _autoFetchDelay ms after the call).
    void            setAutoFetch( bool autoFetch );
signals:
    //! Emitted whenever \c autoFetch property change.
    void            autoFetchChanged( );
private:
    //! \copydoc qps::DataSource::autoFecth
    bool            _autoFetch;
    QTimer          _autoFetchTimer;
private slots:
    //! Called when \c autoFetch is enabled after autoFetchDelay elapsed.
    void            autoFetchTimeout( );

public:
    //! Get the \c autoFetch value rounded to the nearest minute.
    Q_INVOKABLE int getAutoFetchDelayInMin( ) { return qRound( QTime( ).msecsTo( _autoFetchDelay ) / 60000. ); }
    //! Data source delay between two "auto-fetch" if data auto fetching has been set (default to 20 minutes).
    Q_PROPERTY( QTime autoFetchDelay READ getAutoFetchDelay WRITE setAutoFetchDelay NOTIFY autoFetchDelayChanged )
    //! \copydoc qps::DataSource::autoFetchDelay
    QTime           getAutoFetchDelay( ) const { return _autoFetchDelay; }
    //! \copydoc qps::DataSource::autoFetchDelay
    void            setAutoFetchDelay( QTime autoFetchDelay ) { _autoFetchDelay = autoFetchDelay; emit autoFetchDelayChanged( ); }
signals:
    //! Emitted whenever \c autoFetchDelay property change.
    void            autoFetchDelayChanged( );
private:
    //! \copydoc qps::DataSource::autoFetchDelay
    QTime           _autoFetchDelay;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Asynchronous Data Interface *///---------------------------------
    //@{
public:
    /*! \brief Let the user force either synchronous or asynchronous data loading (default to false).
     *
     * This property should be modified before fetch() is called.
     */
    Q_PROPERTY( bool asynchronous READ isAsynchronous WRITE setAsynchronous NOTIFY asynchronousChanged )
    //! \copydoc qps::DataSource::asynchronous
    bool            isAsynchronous( ) const { return _asynchronous; }
    //! \copydoc qps::DataSource::asynchronous
    void            setAsynchronous( bool asynchronous ) { _asynchronous = asynchronous; emit asynchronousChanged( ); }
signals:
    //! Emitted whenever \c asynchronous property change.
    void            asynchronousChanged( );
private:
    //! \copydoc qps::DataSource::asynchronous
    bool            _asynchronous;

public:
    /*! \brief Specifies if some data is currently beeing fetched (read-only, default to false).
     *
     */
    Q_PROPERTY( bool fetching READ isFetching WRITE setFetching NOTIFY fetchingChanged )
    //! \copydoc qps::DataSource::fetching
    bool            isFetching( ) const { return _fetching; }
    //! \copydoc qps::DataSource::fetching
    void            setFetching( bool fetching ) { _fetching = fetching; emit fetchingChanged( ); }
signals:
    //! Emitted whenever \c fetching property change.
    void            fetchingChanged( );
private:
    //! \copydoc qps::DataSource::fetching
    bool            _fetching;

protected:
    /*! \brief Set a worker object in a data source child thread for fetching asynchronous datas.
     *
     * DataSource get ownership for \c worker, any previously installed worker is deleted.
     *
     * \param worker must have an \c operate() slot containing the code that must executed asynchronously.
     */
    void        operateWorker( QObject* worker );
signals:
    //! The worker object sets in operateWorker() is "run" when this signal is emmited automatically at the end of operateWorker() (user can consider this signal private).
    void        operate( );
private:
    //! Thread object used internally to fetch asynchronous data.
    QThread*    _workerThread;
    //@}
    //-------------------------------------------------------------------------
};

//! Static helper class for XML serialization of qps::DataSource objects.
class DataSourceXMLSerializer {
public:
    /*! Serialize \c dataSource properties to an XML stream current element properties.
     *
     *  \return false if an error occurs.
     */
    static bool serializeOutProperties( const DataSource& dataSource, QXmlStreamWriter& stream );
    /*! Serialize \c dataSource properties from an XML stream current element properties.
     *
     *  \return false if an error occurs.
     */
    static bool serializeInProperties( DataSource& dataSource, QXmlStreamReader& stream );
};

} // ::qps

QML_DECLARE_TYPE( qps::DataSource )

#endif // qpsDataSource_h

