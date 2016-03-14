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
// \file	qpsMainWindow.cpp
// \author	benoit@qanava.org
// \date	2015 10 18
//-----------------------------------------------------------------------------

// Qt headers
#include <QGuiApplication>
#include <QQmlContext>

// QuickProperties headers
#include "./qpsMainWindow.h"

using namespace qps;

//-----------------------------------------------------------------------------
MainView::MainView( QGuiApplication* ) :
    QQuickView( )
{
    QScopedPointer< qps::AbstractTranslator > translator{ new qps::AbstractTranslator() };
    QuickProperties::initialize( engine(), translator.data() );

    RectangleStyle* cppRectangleStyle = new RectangleStyle( this );
    rootContext( )->setContextProperty( "cppRectangleStyle", cppRectangleStyle );

    setSource( QUrl( "qrc:/propsedit.qml" ) );
}
//-----------------------------------------------------------------------------

int	main( int argc, char** argv )
{
    QGuiApplication app(argc, argv);

    MainView mainView( &app );
    mainView.setResizeMode( QQuickView::SizeRootObjectToView );
    mainView.resize( 1084, 530 );
    mainView.show( );

    return app.exec( );
}
