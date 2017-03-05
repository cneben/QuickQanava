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
// \file	qanProgressSample.cpp
// \author	benoit@qanava.org
// \date	2016 02 09
//-----------------------------------------------------------------------------

// QuickContainers headers
#include "../../QuickContainers/src/QuickContainers.h"

// QuickQanava headers
#include "../../src/QuickQanava.h"
#include "qanProgressSample.h"

// Qt headers
#include <QGuiApplication>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace qan;

void    consumePhase( gtpo::IProgressNotifier& notifier )
{
    for ( int i = 0; i < 100; ++i ) {
        notifier.setProgress( 0.01 * i );
        QThread::msleep( 15 );
    }
}

void    Consumer::consume( qan::ProgressNotifier* notifier )
{
    if ( notifier == nullptr )
        return;
    notifier->reset();
    notifier->reserveSubProgress( 2 );

    try {
        gtpo::IProgressNotifier& subNotifier1 = notifier->takeSubProgress();
        subNotifier1.reserveSubProgress( 2 );

        gtpo::IProgressNotifier& subNotifier2 = notifier->takeSubProgress();
        subNotifier2.reserveSubProgress( 3 );

        notifier->beginProgress();
        {
            gtpo::IProgressNotifier& subNotifier11 = subNotifier1.takeSubProgress( "Sub progress 1 - phase1" );
            consumePhase( subNotifier11 );

            gtpo::IProgressNotifier& subNotifier12 = subNotifier1.takeSubProgress( "Sub Progress 1 - phase2" );
            consumePhase( subNotifier12 );
        }
        {
            gtpo::IProgressNotifier& subNotifier21 = subNotifier2.takeSubProgress( "Sub progress 2 - phase1");
            consumePhase( subNotifier21 );

            gtpo::IProgressNotifier& subNotifier22 = subNotifier2.takeSubProgress( "Sub progress 2 - phase2" );
            consumePhase( subNotifier22 );

            gtpo::IProgressNotifier& subNotifier23 = subNotifier2.takeSubProgress( "Sub progress 3 - phase3" );
            consumePhase( subNotifier23 );
        }
    } catch ( std::exception e ) { std::cerr << "Consumer::concume(): Error while reporting progress: " << e.what() << std::endl; }
    notifier->endProgress();    // Calling enProgress() is not mandatory on intermediate sub progress.
}

//-----------------------------------------------------------------------------
MainView::MainView( ) :
    QQuickView( )
{
    QuickContainers::initialize();
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

