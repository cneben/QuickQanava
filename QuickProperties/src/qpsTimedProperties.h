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
// \file    qpsTimedProperties.h
// \author	benoit@qanava.org
// \date	2015 10 21
//-----------------------------------------------------------------------------

#ifndef qpsTimedProperties_h
#define qpsTimedProperties_h

// Qt headers
#include <QtDebug>
#include <QDateTime>

// QuickProperties headers
#include "./qpsProperties.h"
#include "./qpsTimeValueMap.h"
#include "./qpsDataSource.h"

namespace qps { // ::qps

/*! \brief Provide a simple interface to Qt static and dynamic properties interface with support for property change over time.
 *
 * See the QuickProperties documentation for more information: \ref qpintro
 *
 * Note 20151021: This class is highly alpha...
 *
 *	\nosubgrouping
 */
class TimedProperties : public Properties
{
    /*! \name TimedProperties Object Management *///---------------------------
    //@{
    Q_OBJECT
public:
    TimedProperties( QObject* parent = 0 );
    virtual ~TimedProperties( );
private:
    Q_DISABLE_COPY( TimedProperties )
    //@}
    //-------------------------------------------------------------------------


    /*! \name Property Date Time Management *///-------------------------------
    //@{
public:
    Q_PROPERTY( QDateTime current READ getCurrent WRITE setCurrent NOTIFY currentChanged )
    //! Set the object properties current date (current must be: first <= current <= last).
    void        setCurrent( QDateTime current );
    //! Return the object properties current date or last date if current date has not been set.
    QDateTime   getCurrent( ) const;
signals:
    void        currentChanged( );
private:
    //! \copydoc qps::TimedProperties::current
    QDateTime   _current;

public:
    //! Oldest property time stamp, ie value entered with the oldest date and time.
    Q_PROPERTY( QDateTime   first READ getFirst WRITE setFirst NOTIFY firstChanged )
    QDateTime   getFirst( ) const { return _first; }
    void        setFirst( const QDateTime& first ) { _first = first; emit firstChanged( ); }
signals:
    void        firstChanged( );
private:
    //! \copydoc qps::TimedProperties::first
    QDateTime   _first;

public:
    //! Most recent property time stamp, ie date time of the value entered with for the most "recent" date and time.
    Q_PROPERTY( QDateTime   last READ getLast WRITE setLast NOTIFY lastChanged )
    QDateTime   getLast( ) const { return _last; }
    void        setLast( const QDateTime& last ) { _last = last; emit lastChanged( ); }
signals:
    void        lastChanged( );
private:
    //! \copydoc qps::TimedProperties::last
    QDateTime   _last;

protected slots:
    //! Called on TimeValueMap::updated() is emmited to update this timed properties \c first and \c last properties.
    void        updateFirstLast( );

public:
    /*! \brief Set a property value at a specific time stamp (first and last if the time stamp is outside the current begin and end time).
     *
     *  \param  propertyName    name of the static or dynamic property that should be affected.
     *  \param  propertyDateTime time stamp for the couple (propertyName / propertyValue ).
     *  \note If a non existing property name is specified, a dynamic property with the given name will first be created before the
     *        timed value is registered.
     */
    Q_INVOKABLE void                setProperty( QString propertyName, const QDateTime& propertyDateTime, QVariant propertyValue );
    //! Get a property value for a specific time stamp (it will be clipped to first and last properties values).
    Q_INVOKABLE QVariant            getProperty( QString propertyName, const QDateTime& propertyDateTime ) const;
    //! Get a property current value (ie \c last property value if \c current has not been set).
    Q_INVOKABLE virtual QVariant    getProperty( QString propertyName ) override;

    //! Generate a debug dump of all (datetime=value) records for a specific property.
    void                    dumpProperty( QDebug dbg, QString propertyName ) const;

    /*! \brief Return a time value map for a property with the given \c propertyName, maps are cached, the same unique map is always returned for the same property.
     *
     * \return a valid map of value over time for property \c propertyName, or nullptr if an error occurs.
     * \note Returned object ownership is forced to QmlEngine::CppOwnership.
     */
    Q_INVOKABLE qps::TimeValueMap*       getTimeValueMap( QString propertyName );

protected:
    QMap< QString, TimeValueMap* >   _propertiesTimeValueMap;
    //@}
    //-------------------------------------------------------------------------

    /*! \name QML Time Browser Interface *///----------------------------------
    //@{
public:
    //! Model 'currentTime' as a [0.;1.0] percentage between _first and _last time stamps (at 0%, current time is set to _first and so on up to 100% for _last)
    Q_PROPERTY( double  timeValue READ getTimeValue WRITE setTimeValue NOTIFY timeValueChanged )
    Q_INVOKABLE double  getTimeValue( ) const;
    Q_INVOKABLE void    setTimeValue( double tv );
signals:
    void        timeValueChanged( );
protected:
    //! \copydoc qps::TimedProperties::timeValue
    double      _timeValue;
    //@}
    //-------------------------------------------------------------------------

    /*! \name DataSource Management *///---------------------------------------
    //@{
public:
    //! Fetch data from the installed data sources to their corresponding properties.
    virtual bool    fetch( );
    //! Register a data source in this timed properties (this timed properties object get the ownership for \c datasource).
    void    addDataSource( qps::DataSource* dataSource );
private:
    QList< DataSource* >   _dataSources;
    //@}
    //-------------------------------------------------------------------------

    /*! \name XML Serialization *///-------------------------------------------
    //@{
protected:
    //! Serialize out time values information for a given property.
    void            serializeTimeValuesOut( QString propertyName, QXmlStreamWriter& stream );
protected:
    //! Serialize in time values information for a given property.
    void            serializeTimeValuesIn( QString propertyName, QXmlStreamReader& stream );
    //@}
    //-------------------------------------------------------------------------
};

//! Properties qDebug() stream operator.
QDebug      operator<<( QDebug dbg, const TimedProperties& p );

} // ::qps

QML_DECLARE_TYPE( qps::TimedProperties )

#endif // qpsTimedProperties_h

