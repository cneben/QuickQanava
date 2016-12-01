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
// This file is a part of the QuickQanava software library.
//
// \file	qanStyleSample.cpp
// \author	benoit@qanava.org
// \date	2016 02 09
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "../../QuickProperties/src/QuickProperties.h"

// QuickQanava headers
#include "../../src/QuickQanava.h"
#include "./qanStyleSample.h"

// Qt headers
#include <QApplication>
#include <QQuickStyle>

using namespace qan;

//-----------------------------------------------------------------------------
MainView::MainView( ) :
    QQuickView{}
{
    QScopedPointer< qps::AbstractTranslator > translator{ new qps::AbstractTranslator() };
    QuickProperties::initialize( engine(), translator.data() );
    QuickQanava::initialize();
#ifdef QUICKQANAVA_HAS_PROTOBUF
    _serializer = new qan::ProtoSerializer{};
    engine()->rootContext( )->setContextProperty( "qanSerializer", _serializer );
#endif

    setSource( QUrl( "qrc:/main.qml" ) );
    qan::Graph* graph = qobject_cast< qan::Graph* >( rootObject( )->findChild< QQuickItem* >( "graph" ) );
    if ( graph != nullptr ) {
        auto& styleManager = graph->styleManager();
        styleManager.generateDefaultStyles();
        qan::NodeStyle* nodeStyle = styleManager.createNodeStyle( "Blue Node", "qan::Node" );
        nodeStyle->setBackColor( QColor( 0, 0, 255 ) );
    }
}
//-----------------------------------------------------------------------------

int	main( int argc, char** argv )
{
    QQuickStyle::setStyle("Material");
    QApplication app(argc, argv);

    MainView mainView;
    mainView.setResizeMode( QQuickView::SizeRootObjectToView );
    mainView.resize( 1024, 700 );
    mainView.show( );

    return app.exec( );
}

