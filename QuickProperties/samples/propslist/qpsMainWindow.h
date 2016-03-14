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
// \date	2015 10 29
//-----------------------------------------------------------------------------

#ifndef qpsMainWindow_h
#define qpsMainWindow_h

// Qt headers
#include <QQuickView>

// QuickProperties headers
#include "../../src/QuickProperties2.h"

class RectangleStyle : public qps::Properties
{
    Q_OBJECT
public:
    RectangleStyle( QString label = QString( "" ), QObject* parent = 0 ) :
        qps::Properties( parent ),
        _label( label ),
        _color( QColor( Qt::blue ) ),
        _radius( 5. ),
        _borderWidth( 1 ),
        _borderColor( QColor( Qt::red ) )
    {
        monitorStaticProperties( );
        new qps::RealLimit( this, "radius", 0., 15., 0.2 );
        new qps::IntLimit( this, "borderWidth", 1, 5 );
        hideStaticProperties( 0 );
    }
    virtual ~RectangleStyle( ) { }
private:
    Q_DISABLE_COPY( RectangleStyle )

    // User defined "static" properties
public:
    Q_PROPERTY( QString  label READ getLabel WRITE setLabel NOTIFY labelChanged )
    QString     getLabel( ) const { return _label; }
    void        setLabel( QString label ) { _label = label; emit labelChanged( ); }
signals:
    void        labelChanged( );
protected:
    QString      _label;

public:
    Q_PROPERTY( QColor  color READ getColor WRITE setColor NOTIFY colorChanged )
    QColor      getColor( ) const { return _color; }
    void        setColor( QColor color ) { _color = color; emit colorChanged( ); }
signals:
    void        colorChanged( );
protected:
    QColor      _color;

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

public:
    Q_PROPERTY( QColor  borderColor READ getBorderColor WRITE setBorderColor NOTIFY borderColorChanged )
    QColor      getBorderColor( ) const { return _borderColor; }
    void        setBorderColor( QColor borderColor ) { _borderColor = borderColor; emit borderColorChanged( ); }
signals:
    void        borderColorChanged( );
protected:
    QColor      _borderColor;
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

