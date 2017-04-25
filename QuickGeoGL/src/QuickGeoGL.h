/*
    This file is part of QuickGeoGL library.

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
// This file is a part of the QuickGeoGL software. Copyright (C) 2016 Benoit Autheman.
//
// \file    QuickGeoGL.h
// \author	benoit@destrat.io
// \date	2016 09 24
//-----------------------------------------------------------------------------

#ifndef QuickGeoGL_h
#define QuickGeoGL_h

// Qt headers
#include <QQmlEngine>   // qmlRegisterType()

// QuickGeoGL headers
#include "./qglConvexPolygon.h"
#include "./qglConvexPolygonBorder.h"
#include "./qglLine.h"
#include "./qglPolyLine.h"
#include "./qglArrow.h"

struct QuickGeoGL {
    static void initialize( ) {
        qmlRegisterType< qgl::ConvexPolygon >( "QuickGeoGL", 1, 0, "ConvexPolygon");
        qmlRegisterType< qgl::ConvexPolygonBorder >( "QuickGeoGL", 1, 0, "ConvexPolygonBorder");
        qmlRegisterType< qgl::Line >( "QuickGeoGL", 1, 0, "Line");
        qmlRegisterType< qgl::PolyLine >( "QuickGeoGL", 1, 0, "PolyLine");
        qmlRegisterType< qgl::Arrow >( "QuickGeoGL", 1, 0, "Arrow");
    }
};

#endif // QuickGeoGL_h

