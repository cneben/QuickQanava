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
// \file	qan40kSample.cpp
// \author	benoit@destrat.io
// \date	2017 01 29
//-----------------------------------------------------------------------------

// QuickQanava headers
#include "../../src/QuickQanava.h"

// Qt headers
#include <QScopedPointer>
#include <QApplication>
#include <QImage>
#include <QtQml>
#include <QQuickItem>
#include <QQuickStyle>
#include <QElapsedTimer>

// Topology sample headers
#include "./qan40kSample.h"

using namespace qan;


int	main( int argc, char** argv )
{
    QApplication app( argc, argv );
    QQuickStyle::setStyle("Material");

    QScopedPointer<QQmlApplicationEngine> engine{ new QQmlApplicationEngine{} };
    {
        QuickQanava::initialize();
    }
    engine->load(QUrl("qrc:/40k.qml"));

    {
        QPointer<qan::Graph> graph{nullptr};
        const auto rootObjects = engine->rootObjects();
        for ( const auto rootObject : rootObjects ) {
            graph = qobject_cast<qan::Graph*>( rootObject->findChild<QQuickItem*>( "graph" ) );
            if ( graph != nullptr )
                break;
        }
        qWarning() << "graph=" << graph.data();
        //auto defaultNode = graph->get
        if ( graph ) {
            unsigned int n = 0;
            QElapsedTimer t; t.start();
            //QImage image(":/qt4logo4k.png");
            QImage image(":/qt4logo1k.png");
            qWarning() << "image: width=" << image.width() << "\theight=" << image.height();
            qWarning() << "rendering " << ( image.width() * image.height() ) << " nodes";
            qreal defaultWidth{40.}, defaultHeight{30.};
            qreal xSpacing{5.}, ySpacing{3.};
            for ( int x = 0; x < image.width(); x++ ) {
                qreal nodeX = static_cast<qreal>(x) * ( defaultWidth + xSpacing ) + xSpacing;
                for ( int y = 0; y < image.height(); y++ ) {
                    qreal nodeY = static_cast<qreal>(y) * ( defaultHeight + ySpacing ) + ySpacing;
                    auto node = graph->insertNode();    // FIXME
                    node->setX(nodeX);
                    node->setY(nodeY);
                    node->setMinimumSize( {defaultWidth / 2., defaultHeight / 2.} );
                    node->setWidth(defaultWidth);
                    node->setHeight(defaultHeight);
                    //node->setLabel( QString::number(n++) );
                }
            }
            qWarning() << "Node components creation took " << t.elapsed() << "ms";
        }
    }

    auto e = app.exec( );
    qWarning() << "40k terminate with code " << e;
    return e;
}

