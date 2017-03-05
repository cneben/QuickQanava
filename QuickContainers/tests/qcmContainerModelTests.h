/*
    This file is part of QuickContainers library.

    Copyright (C) 2016  Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of QuickContainers library.
//
// \file	qpsContainerModelTests.h
// \author	benoit@qanava.org
// \date	2016 02 08
//-----------------------------------------------------------------------------

// Qt headers

// QuickContainers headers
#include <QuickContainers>

class QA : public QObject
{
    Q_OBJECT
public:
    QA( QObject* parent = nullptr ) : QObject{ parent }  {
        setObjectName("::QA");
    }

public:
    Q_PROPERTY( qreal dummyReal READ getDummyReal WRITE setDummyReal NOTIFY dummyRealChanged )
    qreal       getDummyReal( ) const { return _dummyReal; }
    void        setDummyReal( qreal dummyReal ) { _dummyReal = dummyReal; emit dummyRealChanged(); }
protected:
    qreal       _dummyReal{ 42. };
signals:
    void        dummyRealChanged( );
};


class QB : public QObject
{
    Q_OBJECT
public:
    QB( QObject* parent = nullptr ) : QObject{ parent }  {
        setObjectName("::QB");
    }
public:
    Q_PROPERTY( qreal dummyReal READ getDummyReal WRITE setDummyReal NOTIFY dummyRealChanged )
    qreal       getDummyReal( ) const { return _dummyReal; }
    void        setDummyReal( qreal dummyReal ) { _dummyReal = dummyReal; emit dummyRealChanged(); }
protected:
    qreal       _dummyReal{ 43. };
signals:
    void        dummyRealChanged( );
};

