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
#include <QCoreApplication>
#include <QScopedPointer>

// QPS headers
#include "../../src/QuickProperties2"
#include "qpsBasic.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QScopedPointer< ObjectStaticProps > o{ new ObjectStaticProps() };
    QScopedPointer< qps::Properties >p{ new qps::Properties() };
    p->setTarget( o.data() );
    o->setProperty( "test", QVariant( 42 ) );
    qDebug() << *p;

    return a.exec();
}
