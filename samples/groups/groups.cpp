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
// \file	groups.cpp
// \author	benoit@qanava.org
// \date	2016 03 23
//-----------------------------------------------------------------------------

// Qt headers
#include <QGuiApplication>
#include <QtQml>
#include <QQuickStyle>

// QuickQanava headers
#include <QuickQanava>

using namespace qan;

//-----------------------------------------------------------------------------
int	main( int argc, char** argv )
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    QuickQanava::initialize();
    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/groups.qml"));
    return app.exec();
}
//-----------------------------------------------------------------------------

