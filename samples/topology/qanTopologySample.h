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
// \file	qanTopologySample.h
// \author	benoit@qanava.org
// \date	2016 02 09
//-----------------------------------------------------------------------------

#ifndef qanTopologySample_h
#define qanTopologySample_h

// QuickQanava headers
#include <QuickQanava>
#include "../../src/qanProtoSerializer.h"

// QT headers
#include <QQuickView>

class MainView : public QQuickView
{
    Q_OBJECT
public:
    MainView( );
    virtual ~MainView( ) { }

private:
    QScopedPointer< qan::ProtoSerializer > _serializer;
};

#endif // qanTopologySample_h

