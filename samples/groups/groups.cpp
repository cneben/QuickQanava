/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the Qanava software.
//
// \file	groups.cpp
// \author	benoit@qanava.org
// \date	2016 03 23
//-----------------------------------------------------------------------------

// QuickQanava headers
#include "./groups.h"

// Qt headers
#include <QGuiApplication>

using namespace qan;

//-----------------------------------------------------------------------------
MainView::MainView( ) :
    QQuickView( )
{
    QuickQanava::initialize();

    setSource( QUrl( "qrc:/main.qml" ) );
    qan::Graph* g = qobject_cast< qan::Graph* >( rootObject( )->findChild< QQuickItem* >( "graph" ) );
    Q_ASSERT( g != nullptr );
}
//-----------------------------------------------------------------------------

int	main( int argc, char** argv )
{
    QGuiApplication app(argc, argv);

    MainView mainView;
    mainView.setResizeMode( QQuickView::SizeRootObjectToView );
    mainView.resize( 1024, 700 );
    mainView.show( );

    return app.exec( );
}

