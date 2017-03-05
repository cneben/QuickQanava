//-----------------------------------------------------------------------------
// This file is a part of the QuickGeoGL software. Copyright (C) 2016 Benoit Autheman.
//
// \file	geometry.cpp
// \author	benoit@destrat.io
// \date	2016 09 26
//-----------------------------------------------------------------------------

// Qt headers
#include <QGuiApplication>
#include <QVariant>
#include <QQmlContext>

// QuickGeoGL headers
#include "QuickGeoGL"
#include "./geometry.h"

using namespace qgl;

MainView::MainView( ) :
    QQuickView( )
{
    QuickGeoGL::initialize();
    setSource( QUrl( "qrc:/geometry.qml" ) );
}

int	main( int argc, char** argv )
{
    QGuiApplication app(argc, argv);
    MainView mainView;
    mainView.setResizeMode( QQuickView::SizeRootObjectToView );
    mainView.resize( 850, 650 );
    mainView.show( );
    return app.exec( );
}
