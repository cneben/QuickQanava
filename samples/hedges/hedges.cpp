/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

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
// This file is a part of the QuickQanava software library.
//
// \file	hedges.cpp
// \author	benoit@qanava.org
// \date	2016 10 10
//-----------------------------------------------------------------------------

// QuickQanava headers
#include "./hedges.h"

// Qt headers
#include <QGuiApplication>
#include <QQuickStyle>

using namespace qan;

//-----------------------------------------------------------------------------
MainView::MainView( ) :
    QQuickView( )
{
    QuickQanava::initialize();
    setSource( QUrl( "qrc:/main.qml" ) );
}

int	main( int argc, char** argv )
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    MainView mainView;
    mainView.setResizeMode( QQuickView::SizeRootObjectToView );
    mainView.resize( 1024, 700 );
    mainView.show( );
    return app.exec( );
}
//-----------------------------------------------------------------------------

