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
// \file    qpsTimeValueMap.h
// \author  benoit@qanava.org
// \date    2015 10 21
//-----------------------------------------------------------------------------

#ifndef qpsTimeValueMap_h
#define qpsTimeValueMap_h

// STD headers
#include <algorithm>

// Qt headers
#include <QtDebug>
#include <QtQml>
#include <QDateTime>
#include <QReadWriteLock>

// QuickProperties headers
// Nil

namespace qps { // ::qps

/*! \brief Thread safe and QObject based QMap< QDateTime, QVariant > used to store a value over time.
 *
 * TimeValueMap can be used in two different mode:
 * \li using standard QMap<> methods to manage (insert/remove) key/value pairs, followed by a manual call
 * to qpsUpdate() to force update of view connected to this map : it is the faster way for large data modifications.
 * \li using qps*() methods (or ts*() methods): a signal corresponding to the modification operation will be emmitted
 * for each atomic operation, ensuring that a view connected to this map will be updated accordingly in an
 * efficient way (for example an update following an append() operation might be faster than an insert()).
 *
 * TimeValueMap is thread safe when accessed with ts*() methods instead of QMap or qps*() methods.
 *
 * \nosubgrouping
 */
class TimeValueMap : public QObject, public QMap< QDateTime, QVariant >
{
    /*! \name TimeValueMap Object Management *///------------------------------
    //@{
    Q_OBJECT
public:
    TimeValueMap( QObject* parent = nullptr ) : QObject( parent ), _maxValue( ), _minValue( ) { }
    virtual ~TimeValueMap( ) { }
private:
    Q_DISABLE_COPY( TimeValueMap )

public:
    //! \brief Shortcut to this map read/write lock QReadWriteLock::tryLockForRead().
    bool    tryLockForRead( );
    //! \brief Shortcut to this map read/write lock QReadWriteLock::lockForRead().
    void    lockForRead( );
    //! \brief Shortcut to this map read/write lock QReadWriteLock::unlock().
    void    unlock( );
private:
    mutable QReadWriteLock  _lock;

public:
    //! Apply std::minmax_element to this map and return the result.
    std::pair< TimeValueMap::iterator, TimeValueMap::iterator > getMinMax( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Mapping Interface *///-------------------------------------------
    //@{
public:
    /*! \brief Force emission of update() signal, should be manually called when the map is modified using non qps*() methods.
     *
     * \note calling this method will also update map minimum and maximum values, it could be quite expensive (O(n)).
     */
    Q_INVOKABLE void    qpsUpdate( );

    /*! \brief Clear the complete time value map and emit the cleared() signal after internal clear.
     *
     * Reentrant, but not thread-safe. Use tsClear() if thread safety is needed.
     */
    Q_INVOKABLE void    qpsClear( );
    /*! \brief Thread safe version of qpsClear( ).
     *
     * Reentrant and thread-safe.
     * \sa qpsClear( )
     */
    Q_INVOKABLE void    tsClear( );

    /*! \brief Shortcut to QMap< QDateTime, QVariant >::insert(), emit the inserted() signal after insertion.
     *
     * If \c key is > to lastKey(), insertion will be handled as an "append" operation, and the appended() signal will
     * be casted instead of inserted().
     *
     * If \c key is already contained in the map, it current value will b replaced by \c value, and the modified() signal
     * will be casted instead of inserted().
     *
     * Reentrant, but not thread-safe.
     * \sa tsInsert()
     */
    void    qpsInsert( const QDateTime& key, const QVariant& value );
    /*! \brief Thread safe version of qpsInsert().
     *
     * Reentrant and thread-safe.
     * \sa qpsInsert()
     */
    void    tsInsert( const QDateTime& key, const QVariant& value );

    /*! \brief Shortcut to QMap< QDateTime, QVariant >::remove(), emit the removed() signal after removing key and its associed value.
     *
     * Reentrant, but not thread-safe.
     * \sa tsRemove()
     */
    int     qpsRemove( const QDateTime& key );
    /*! \brief Thread safe version of qpsRemove().
     *
     * Reentrant and thread-safe.
     * \sa qpsRemove()
     */
    int     tsRemove( const QDateTime& key );

signals:
    /*! \brief Emitted when the map data has changed and that any connected view should be totally updated (usually after a call to qpsUpdate()).
     *
     */
    void    updated( );

    /*! \brief Emitted when the underlining time value map is cleared using qpsClear().
     *
     * \note The removed() signal will not be called when the time value map is cleared using qpsClear(), it
     * as it is often much more efficient to delete a complete chunk of data than performing atomic deletion.
     */
    void    cleared( );

    /*! \brief Emitted after the (d, v) couple modification.
     */
    void    modified( QDateTime d, QVariant v );

    /*! \brief Emitted after the (d, v) couple insertion.
     */
    void    inserted( QDateTime d, QVariant v );

    /*! \brief Emitted after the (d, v) couple suppression.
     */
    void    removed( QDateTime d, QVariant v );

    /*! \brief Emmited after a time value couple insertion in the map, when time is the latest time
     *  key (ie it has been appended as the last value since keys are ordered).
     *
     * \note If appended() is emmited, time value map is necessarilly not empty (ie this.isEmpty == false).
     *
     * For a given \c map, use the following code to get the value "targetted" by this signal:
     * \code
     * TimeValueMap map;
     * QVariant v = map.last(); // Safe because map.isEmpty() can't be true
     * \endcode
     *
     * \note When this signal is emmited, property \c end of the associed qps::TimedProperties is not already updated.
     */
    void    appended( );

public:
    /*! \brief Get the maximum value registered in the map using qpsInsert() or tsInsert().
     *
     * \warning returned QVariant will be invalid if pairs of key/value are not inserted using qpsInsert() or tsInsert().
     */
    QVariant    getMaxValue( ) { return _maxValue; }
    /*! \brief Get the minimum value registered in the map using qpsInsert() or tsInsert().
     *
     * \warning returned QVariant will be invalid if pairs of key/value are not inserted using qpsInsert() or tsInsert().
     */
    QVariant    getMinValue( ) { return _minValue; }
protected:
    Q_INVOKABLE void    updateMinMax( );
    QVariant    _maxValue;
    QVariant    _minValue;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qps

QML_DECLARE_TYPE( qps::TimeValueMap )

#endif // qpsTimeValueMap_h

