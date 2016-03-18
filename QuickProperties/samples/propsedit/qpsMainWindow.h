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
// \file	qpsMainWindow.h
// \author	benoit@qanava.org
// \date	2015 06 20
//-----------------------------------------------------------------------------

#ifndef qpsMainWindow_h
#define qpsMainWindow_h

// Qt headers
#include <QQuickView>
#include <QGuiApplication>

// QuickProperties headers
#include "../../src/QuickProperties2.h"

class RectangleStyle : public qps::Properties
{
    Q_OBJECT
public:
    RectangleStyle( QObject* parent = 0 ) :
        qps::Properties( parent ),
        _content( "Content set from C++"),
        _radius( 5. ),
        _borderWidth( 1 )
    {
        new qps::StringLimit( this, "content", 40 );
        new qps::RealLimit( this, "radius", 0., 5., 0.2 );
        new qps::IntLimit( this, "borderWidth", 1, 15 );
    }
    virtual ~RectangleStyle( ) { }
private:
    Q_DISABLE_COPY( RectangleStyle )

    // User defined "static" properties
public:
    Q_PROPERTY( QString  content READ getContent WRITE setContent NOTIFY contentChanged )
    QString     getContent( ) const { return _content; }
    void        setContent( QString content ) { _content = content; emit contentChanged( ); }
signals:
    void        contentChanged( );
protected:
    QString      _content;

public:
    Q_PROPERTY( double  radius READ getRadius WRITE setRadius NOTIFY radiusChanged )
    double      getRadius( ) const { return _radius; }
    void        setRadius( double radius ) { _radius = radius; emit radiusChanged( ); }
signals:
    void        radiusChanged( );
protected:
    double      _radius;

public:
    Q_PROPERTY( int  borderWidth READ getBorderWidth WRITE setBorderWidth NOTIFY borderWidthChanged )
    int         getBorderWidth( ) const { return _borderWidth; }
    void        setBorderWidth( int borderWidth ) { _borderWidth = borderWidth; emit borderWidthChanged( ); }
signals:
    void        borderWidthChanged( );
protected:
    int         _borderWidth;
};

//! 
/*!
	\nosubgrouping
*/
class MainView : public QQuickView
{
    Q_OBJECT
public:
    MainView( QGuiApplication* application );
    virtual ~MainView( ) { }
private:
    Q_DISABLE_COPY( MainView )
};

#endif // qpsMainWindow_h

