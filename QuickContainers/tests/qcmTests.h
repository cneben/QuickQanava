/*
    This file is part of QuickContainers library.

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
// This file is a part of QuickContainers library.
//
// \file	qpsTests.h
// \author	benoit@qanava.org
// \date	2016 02 08
//-----------------------------------------------------------------------------

// Std headers
#include <memory>
#include <fstream>
#include <cstring>  // std::memcpy
#include <type_traits>

// Qt headers
#include <QObject>
#include <QColor>
#include <QScopedPointer>
#include <QSizeF>

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// QuickContainers headers
#include <QuickContainers>

class ObjectStaticProps : public QObject
{
    Q_OBJECT
public:
    ObjectStaticProps( QObject* parent = nullptr ) : QObject{ parent } { }

public:
    Q_PROPERTY( qreal dummyReal READ getDummyReal WRITE setDummyReal NOTIFY dummyRealChanged )
    qreal       getDummyReal( ) const { return _dummyReal; }
    void        setDummyReal( qreal dummyReal ) { _dummyReal = dummyReal; emit dummyRealChanged(); }
protected:
    qreal       _dummyReal{ 0. };
signals:
    void        dummyRealChanged( );
};

class Props : public qcm::ObjectModel
{
    Q_OBJECT
public:
    Props( QObject* parent = nullptr ) : qcm::ObjectModel{ parent } { }

public:
    Q_PROPERTY( qreal staticReal READ getStaticReal WRITE setStaticReal NOTIFY staticRealChanged )
    qreal       getStaticReal( ) const { return _staticReal; }
    void        setStaticReal( qreal staticReal ) { _staticReal = staticReal; emit staticRealChanged(); }
protected:
    qreal       _staticReal = 0.;
signals:
    void        staticRealChanged( );

public:
    Q_PROPERTY( bool staticBool READ getStaticBool WRITE setStaticBool NOTIFY staticBoolChanged )
    bool        getStaticBool( ) const { return _staticBool; }
    void        setStaticBool( bool staticBool ) { _staticBool = staticBool; emit staticBoolChanged(); }
protected:
    bool        _staticBool = true;
signals:
    void        staticBoolChanged( );

public:
    Q_PROPERTY( int staticInt READ getStaticInt WRITE setStaticInt NOTIFY staticIntChanged )
    int         getStaticInt( ) const { return _staticInt; }
    void        setStaticInt( int staticInt ) { _staticInt = staticInt; emit staticIntChanged(); }
protected:
    int        _staticInt = true;
signals:
    void        staticIntChanged( );

public:
    Q_PROPERTY( QString staticString READ getStaticString WRITE setStaticString NOTIFY staticStringChanged )
    QString     getStaticString( ) const { return _staticString; }
    void        setStaticString( QString staticString ) { _staticString = staticString; emit staticStringChanged(); }
protected:
    QString     _staticString = QString( "" );
signals:
    void        staticStringChanged( );

public:
    Q_PROPERTY( QColor staticColor READ getStaticColor WRITE setStaticColor NOTIFY staticColorChanged )
    QColor      getStaticColor( ) const { return _staticColor; }
    void        setStaticColor( QColor staticColor ) { _staticColor = staticColor; emit staticColorChanged(); }
protected:
    QColor      _staticColor = QColor( 0, 255, 0, 255 );
signals:
    void        staticColorChanged( );

public:
    Q_PROPERTY( QSizeF staticSizeF READ getStaticSizeF WRITE setStaticSizeF NOTIFY staticSizeFChanged )
    QSizeF      getStaticSizeF( ) const { return _staticSizeF; }
    void        setStaticSizeF( QSizeF staticSizeF ) { _staticSizeF = staticSizeF; emit staticSizeFChanged(); }
protected:
    QSizeF      _staticSizeF = QSizeF( 0., 0. );
signals:
    void        staticSizeFChanged( );
};

