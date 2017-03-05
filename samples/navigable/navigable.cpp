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
// This file is a part of the QuickQanava software.
//
// \file	navigable.cpp
// \author	benoit@qanava.org
// \date	2016 01 14
//-----------------------------------------------------------------------------

// Qt headers
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

// QuickQanava headers
#include "../../src/qanNavigable.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    qmlRegisterType< qan::Navigable >( "QuickQanava", 2, 0, "Navigable");
    qmlRegisterType< qan::Grid >( "QuickQanava", 2, 0, "Grid");
    qmlRegisterType< qan::PointGrid >( "QuickQanava", 2, 0, "PointGrid");
    QQmlApplicationEngine engine;
    engine.load( QUrl( QStringLiteral( "qrc:/navigable.qml" ) ) );
    return app.exec();
}
