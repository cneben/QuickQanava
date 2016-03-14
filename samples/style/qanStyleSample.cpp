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
// \file	qanStyleSample.cpp
// \author	benoit@qanava.org
// \date	2016 02 09
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "../../QuickProperties2/QuickProperties2.h"

// QuickQanava headers
#include "../../src/QuickQanava.h"
#include "../../src/qanProtoSerializer.h"
#include "./qanStyleSample.h"

// Qt headers
#include <QGuiApplication>

using namespace qan;

//-----------------------------------------------------------------------------
MainView::MainView( ) :
    QQuickView( ),
    _serializer{ new qan::ProtoSerializer() }
{
    QScopedPointer< qps::AbstractTranslator > translator{ new qps::AbstractTranslator() };
    QuickProperties::initialize( engine(), translator.data() );
    QuickQanava::initialize();

    engine()->rootContext( )->setContextProperty( "qanSerializer", _serializer.data() );

    setSource( QUrl( "qrc:/main.qml" ) );
    qan::Graph* g = qobject_cast< qan::Graph* >( rootObject( )->findChild< QQuickItem* >( "graph" ) );
    Q_ASSERT( g != nullptr );

    qan::StyleManager& styleManager = g->styleManager();
    styleManager.generateDefaultStyles();
    qan::NodeStyle* nodeStyle = styleManager.createNodeStyle( "Blue Node", "qan::Node" );
    nodeStyle->setBackColor( QColor( 0, 0, 255 ) );
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

