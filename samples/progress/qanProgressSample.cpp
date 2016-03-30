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
// \file	qanProgressSample.cpp
// \author	benoit@qanava.org
// \date	2016 02 09
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "../../QuickProperties/src/QuickProperties2.h"

// QuickQanava headers
#include "../../src/QuickQanava.h"
#include "qanProgressSample.h"

// Qt headers
#include <QGuiApplication>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace qan;

void    consumePhase( gtpo::IProgressNotifier* notifier )
{
    for ( int i = 0; i < 100; ++i ) {
        notifier->setPhaseProgress( 0.01 * i );
        QThread::msleep( 15 );
    }
}

void    Consumer::consume( qan::ProgressNotifier* notifier )
{
    if ( notifier == nullptr )
        return;
    notifier->reset();
    notifier->setPhaseCount( 3 );
    gtpo::IProgressNotifier* subNotifier = notifier->createSubProgress();
    subNotifier->setPhaseCount( 2 );
    notifier->beginProgress();

    subNotifier->beginProgress();
    subNotifier->beginPhase( "Sub progress - phase1" );
    consumePhase( subNotifier );
    subNotifier->beginPhase( "Sub Progress - phase2" );
    consumePhase( subNotifier );
    subNotifier->endProgress();

    notifier->beginPhase( "Super progress - phase1");
    consumePhase( notifier );
    notifier->beginPhase( "Super progress - phase2" );
    consumePhase( notifier );
    notifier->beginPhase( "Super progress - phase3" );
    consumePhase( notifier );

    notifier->endProgress();
}

//-----------------------------------------------------------------------------
MainView::MainView( ) :
    QQuickView( )
{
    QScopedPointer< qps::AbstractTranslator > translator{ new qps::AbstractTranslator() };
    QuickProperties::initialize( engine(), translator.data() );
    QuickQanava::initialize();

    qmlRegisterType< ::Consumer >( "QuickQanava", 2, 0, "Consumer");
    engine()->rootContext( )->setContextProperty( "consumer", _consumer.data() );

    setSource( QUrl( "qrc:/main.qml" ) );
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

