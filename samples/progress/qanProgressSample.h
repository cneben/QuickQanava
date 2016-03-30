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
// \file	qanProgressSample.h
// \author	benoit@qanava.org
// \date	2016 03 29
//-----------------------------------------------------------------------------

#ifndef qanProgressSample_h
#define qanProgressSample_h

// QuickQanava headers
#include <QuickQanava>

// QT headers
#include <QQuickView>

class Consumer : public QObject
{
    Q_OBJECT
public:
    Consumer( QObject* parent = nullptr ) : QObject( parent ) { }
    Q_INVOKABLE void    consume( qan::ProgressNotifier* notifier );
};

class MainView : public QQuickView
{
    Q_OBJECT
public:
    MainView( );
    virtual ~MainView( ) { }

protected:
    QScopedPointer< ::Consumer > _consumer{ new Consumer() };
};

QML_DECLARE_TYPE( Consumer )

#endif // qanProgressSample_h

