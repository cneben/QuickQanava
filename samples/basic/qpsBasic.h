/*
    This file is part of QuickProperties2 library.

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

// Qt headers
#include <QObject>

class ObjectStaticProps : public QObject
{
    Q_OBJECT
public:
    ObjectStaticProps( QObject* parent = nullptr ) : QObject{ parent } { }

public:
    Q_PROPERTY( qreal dummyReal READ getDummyReal WRITE setDummyReal NOTIFY dummyRealChanged )
    qreal       getDummyReal( ) const { return _dummyReal; }
    void        setDummyReal( qreal dummyReal ) { _dummyReal = dummyReal; emit dummyRealChanged(); }
protected:
    qreal       _dummyReal = 0.;
signals:
    void        dummyRealChanged( );
};
