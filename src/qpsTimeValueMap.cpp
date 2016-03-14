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
// \file	qpsTimeValueMap.cpp
// \author	benoit@qanava.org
// \date	2015 10 21
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "./qpsTimeValueMap.h"

namespace qps { // ::qps

/* TimeValueMap Object Management *///-----------------------------------------
bool    TimeValueMap::tryLockForRead( )
{
    return _lock.tryLockForRead( );
}

void    TimeValueMap::lockForRead( )
{
    _lock.lockForRead( );
}

void    TimeValueMap::unlock( )
{
    _lock.unlock( );
}

bool qpsCmpDoubleVariant(   const QVariant& a,
                            const QVariant& b ) {
    return a.toDouble( ) < b.toDouble( );
}

std::pair< TimeValueMap::iterator, TimeValueMap::iterator > TimeValueMap::getMinMax( )
{
    return std::minmax_element( begin(), end(), qpsCmpDoubleVariant );
}
//-----------------------------------------------------------------------------

/* Mapping Interface *///------------------------------------------------------
void    TimeValueMap::qpsUpdate( )
{
    updateMinMax( );
    emit updated();
}

void    TimeValueMap::qpsClear( )
{
    clear( );
    _minValue = QVariant( );
    _maxValue = QVariant( );
    emit cleared( );
}

void    TimeValueMap::tsClear( )
{
    _lock.lockForWrite( );
    clear( );
    _minValue = QVariant( );
    _maxValue = QVariant( );
    _lock.unlock( );
    emit cleared( );
}

void    TimeValueMap::qpsInsert( const QDateTime& key, const QVariant& value )
{
    if ( !_minValue.isValid() || value < _minValue )
        _minValue = value;
    if ( !_maxValue.isValid() || value > _maxValue )
        _maxValue = value;
    bool valueAppended = ( !isEmpty( ) && key > lastKey( ) ? true : false );
    bool valueModified = contains( key );
    insert( key, value );
    if ( valueAppended )
        emit appended( );
    else if ( valueModified )
        emit modified( key, value );
    else
        emit inserted( key, value );
}

void    TimeValueMap::tsInsert( const QDateTime& key, const QVariant& value )
{
    if ( !_minValue.isValid() || value < _minValue )
        _minValue = value;
    if ( !_maxValue.isValid() || value > _maxValue )
        _maxValue = value;

    bool valueAppended = ( !isEmpty( ) && key > lastKey( ) ? true : false );
    bool valueModified = contains( key );

    _lock.lockForWrite( );
    insert( key, value );
    _lock.unlock( );

    if ( valueAppended )
        emit appended( );
    else if ( valueModified )
        emit modified( key, value );
    else
        emit inserted( key, value );
}

int    TimeValueMap::qpsRemove( const QDateTime& key )
{
    int r = -1;
    if ( key.isValid() ) {
        QVariant v = value( key );
        r = remove( key );
        if ( v == _minValue || v == _maxValue )
            updateMinMax( );
        emit removed( key, v );
    }
    return r;
}

int     TimeValueMap::tsRemove( const QDateTime& key )
{
    int r = -1;
    if ( key.isValid() ) {
        _lock.lockForWrite( );
        QVariant v = value( key );
        r = remove( key );
        if ( v == _minValue || v == _maxValue )
            updateMinMax( );
        _lock.unlock( );
        emit removed( key, v );
    }
    return r;
}

void    TimeValueMap::updateMinMax( )
{
    const_iterator i = begin( );
    for ( ; i != end( ); ++i ) {
        QVariant v = i.value();
        if ( !_minValue.isValid() || v < _minValue )
            _minValue = v;
        if ( !_maxValue.isValid() || v > _maxValue )
            _maxValue = v;
    }
}
//-----------------------------------------------------------------------------

} // ::qps


