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
// \file    qpsLimit.h
// \author	benoit@qanava.org
// \date	2015 05 01
//-----------------------------------------------------------------------------

#ifndef qpsLimit_h
#define qpsLimit_h

// Qt headers
#include <QtQml>
#include <QtDebug>
#include <QDateTime>

// QuickProperties headers
// Nil

namespace qps { // ::qps

/*! \brief Model an abstract limit for a property element in qps::TimedProperties designed thanks to this limit's target property.
 *
 * Example of limits specification in a qps::TimedProperties constructor from C++ for this properties "property elements" \c content, \c radius and \c borderWidth:
 * \code
 * MyProperties( QObject* parent = 0 ) :
 *     qps::TimedProperties( parent ) //, ...
 * {
 *     new qps::StringLimit( this, "content", 40 );
 *     new qps::RealLimit( this, "radius", 0., 5., 0.2 );
 *     new qps::IntLimit( this, "borderWidth", 1, 15 ); // Will be destroyed from QObject parent destructor
 * }
 * \endcode
 * All properties referenced via 'target' parameter should have been declared with Q_PROPERTY macros.
 *
 * The QML version for the same code is:
 * \code
 * QpsProperties {
 *     property string content: "Rectangle content string"
 *     QpsStringLimit { target: "content"; maxChars: 40 }
 *
 *     property real   radius: 5
 *     QpsRealLimit { target: "borderWidth"; min: 0.; max: 5.; stepSize:0.2 }
 *
 *     property int    borderWidth: 3
 *     QpsIntLimit { target: "borderWidth"; min: 1; max: 15 }
 * }
 * \endcode
 * \nosubgrouping
 */
class Limit : public QObject
{
    /*! \name Limit Object Management *///-------------------------------------
    //@{
    Q_OBJECT
public:
    //! Parent object must be specified and must be a qps::TimedProperties* object, since this limit will apply on one of its internal property.
    Limit( QObject* parent = 0, QString target = QString( "" ) ) : QObject( parent ), _target( target ) { setTarget( target ); }
    virtual ~Limit( ) { }
private:
    Q_DISABLE_COPY( Limit )

public:
    /*! \brief Define the target for this limit, usually it is the name of a property member of qps::TimedProperties.
     *
     * Exemple:
     * \code
     * QpsProperties {
     *   property string content: "Rectangle content string"
     *   QpsStringLimit { target: "content"; maxChars: 40 }     // Target for this limit will be property "content"
     * }
     * \endcode
     */
    Q_PROPERTY( QString target READ getTarget WRITE setTarget NOTIFY targetChanged )
    //! \sa target.
    QString     getTarget( ) const { return _target; }
    //! \sa target.
    void        setTarget( QString target ) { _target = target; emit targetChanged( ); }
signals:
    //! \sa target.
    void        targetChanged( );
protected:
    //! \sa target.
    QString     _target;
    //@}
    //-------------------------------------------------------------------------
};

//! \brief Model a character length limit for string property in qps::TimedProperties.
/*!
    \sa qps::Limit::Limit()
    \nosubgrouping
*/
class StringLimit : public Limit
{
    /*! \name StringLimit Object Management *///-------------------------------
    //@{
    Q_OBJECT
public:
    //! Parent object must be specified and must be a qps::TimedProperties* object, since this limit will apply on one of its internal property.
    StringLimit( QObject* parent = 0, QString target = QString( "" ), int maxChars = -1 ) : Limit( parent, target ), _maxChars( -1 ) { setMaxChars( maxChars ); }
    virtual ~StringLimit( ) { }
private:
    Q_DISABLE_COPY( StringLimit )

public:
    //! Define a length limit in characters count for a string property in qps::TimedProperties (default to -1, ie no limitations).
    Q_PROPERTY( int maxChars READ getMaxChars WRITE setMaxChars NOTIFY maxCharsChanged )
    //! \sa maxChars.
    int         getMaxChars( ) const { return _maxChars; }
    //! \sa maxChars.
    void        setMaxChars( int maxChars ) { _maxChars = maxChars; emit maxCharsChanged( ); }
signals:
    //! \sa maxChars.
    void        maxCharsChanged( );
protected:
    //! \sa maxChars.
    int         _maxChars;
    //@}
    //-------------------------------------------------------------------------
};

//! Model a min/max limit for real property in qps::TimedProperties.
/*!
    \sa qps::Limit::Limit()
    \nosubgrouping
*/
class RealLimit : public Limit
{
    /*! \name RealLimit Object Management *///---------------------------------
    //@{
    Q_OBJECT
public:
    //! Parent object must be specified and must be a qps::TimedProperties* object, since this limit will apply on one of its internal property.
    RealLimit( QObject* parent = 0, QString target = QString( "" ), qreal min = -10000000000., qreal max = 1000000000000., qreal stepSize = 0.1 ) :
        Limit( parent, target ), _min( min ), _max( max ), _stepSize( stepSize ) { }
    virtual ~RealLimit( ) { }
private:
    Q_DISABLE_COPY( RealLimit )

public:
    //! Define the real number minimum value (must be explicitely set if a limit is defined).
    Q_PROPERTY( qreal min READ getMin WRITE setMin NOTIFY minChanged )
    //! \sa min.
    qreal       getMin( ) const { return _min; }
    //! \sa min.
    void        setMin( qreal min ) { _min = min; emit minChanged( ); }
signals:
    //! \sa min.
    void        minChanged( );
protected:
    //! \sa min.
    qreal       _min;

public:
    //! Define the real number maximum value (must be explicitely set defined if a limit is defined).
    Q_PROPERTY( qreal max READ getMax WRITE setMax NOTIFY maxChanged )
    //! \sa max.
    qreal       getMax( ) const { return _max; }
    //! \sa max.
    void        setMax( qreal max ) { _max = max; emit maxChanged( ); }
signals:
    //! \sa max.
    void        maxChanged( );
protected:
    //! \sa max.
    qreal      _max;

public:
    //! Define the step size for real delegate editor (default to 0.1).
    Q_PROPERTY( qreal stepSize READ getStepSize WRITE setStepSize NOTIFY stepSizeChanged )
    //! \sa stepSize.
    qreal       getStepSize( ) const { return _stepSize; }
    //! \sa stepSize.
    void        setStepSize( qreal stepSize ) { _stepSize = stepSize; emit stepSizeChanged( ); }
signals:
    //! \sa stepSize.
    void        stepSizeChanged( );
protected:
    //! \sa stepSize.
    qreal      _stepSize;
    //@}
    //-------------------------------------------------------------------------
};


//! Model a min/max limit for integer property in qps::TimedProperties.
/*!
    \sa qps::Limit::Limit()
    \nosubgrouping
*/
class IntLimit : public Limit
{
    /*! \name RealLimit Object Management *///---------------------------------
    //@{
    Q_OBJECT
public:
    //! Parent object must be specified and must be a qps::TimedProperties* object, since this limit will apply on one of its internal property.
    IntLimit( QObject* parent = 0, QString target = QString( "" ), int min = -100000000, int max = 1000000000 ) :
        Limit( parent, target ), _min( min ), _max( max ) { }
    virtual ~IntLimit( ) { }
private:
    Q_DISABLE_COPY( IntLimit )

public:
    //! Define the real number minimum value (must be explicitely set if a limit is defined).
    Q_PROPERTY( int min READ getMin WRITE setMin NOTIFY minChanged )
    //! \sa min.
    int         getMin( ) const { return _min; }
    //! \sa min.
    void        setMin( int min ) { _min = min; emit minChanged( ); }
signals:
    //! \sa min.
    void        minChanged( );
protected:
    //! \sa min.
    int       _min;

public:
    //! Define the real number maximum value (must be explicitely set defined if a limit is defined).
    Q_PROPERTY( int max READ getMax WRITE setMax NOTIFY maxChanged )
    //! \sa max.
    int         getMax( ) const { return _max; }
    //! \sa max.
    void        setMax( int max ) { _max = max; emit maxChanged( ); }
signals:
    //! \sa max.
    void        maxChanged( );
protected:
    //! \sa max.
    int         _max;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qps

QML_DECLARE_TYPE( qps::Limit )
QML_DECLARE_TYPE( qps::StringLimit )

#endif // qpsLimit_h

