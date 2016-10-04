/*
    This file is part of QuickGeoGL library.

    Copyright (C) 2016 Benoit AUTHEMAN

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
// This file is a part of the QuickGeoGL software. Copyright (C) 2016 Benoit Autheman.
//
// \file	qglConvexPolygonBorder.h
// \author	benoit@destrat.io
// \date	2016 10 01
//-----------------------------------------------------------------------------

#ifndef qglConvexPolygonBorder_h
#define qglConvexPolygonBorder_h

// Qt headers
#include <QObject>
#include <QColor>

namespace qgl { // ::qgl

/*! Internally used to define \ConvexPolygon.border property.
 *
 * \internal
 * \nosubgrouping
 */
class ConvexPolygonBorder : public QObject
{
    Q_OBJECT
public:
    explicit ConvexPolygonBorder( QObject* parent = nullptr ) : QObject{ parent } { }
    virtual ~ConvexPolygonBorder() { }
    ConvexPolygonBorder( const ConvexPolygonBorder& ) = delete;
    ConvexPolygonBorder& operator=(const ConvexPolygonBorder& ) = delete;

public:
    //! Polygon border visibility, default to true (visible).
    Q_PROPERTY( bool visible READ getVisible WRITE setVisible NOTIFY visibleChanged FINAL )
    inline auto     getVisible() const noexcept -> bool { return _visible; }
    inline auto     setVisible( bool visible ) noexcept -> void { _visible = visible; emit visibleChanged(); }
private:
    bool            _visible{true};
signals:
    void            visibleChanged();

public:
    //! Polygon border color, default to black.
    Q_PROPERTY( QColor color READ getColor WRITE setColor NOTIFY colorChanged FINAL )
    inline auto     getColor() const noexcept -> QColor { return _color; }
    inline auto     setColor( QColor color ) noexcept -> void { _color = color; emit colorChanged(); }
private:
    QColor      _color{ 0, 0, 0, 255 };
signals:
    void        colorChanged();

public:
    //! Polygon border width, default to 0.0.
    Q_PROPERTY( qreal width READ getWidth WRITE setWidth NOTIFY widthChanged FINAL )
    inline auto getWidth() const noexcept -> qreal { return _width; }
    inline auto setWidth( qreal width ) noexcept -> void { _width = width; emit widthChanged(); }
private:
    qreal       _width{1.0};
signals:
    void        widthChanged();
};

} // ::qgl

//QML_DECLARE_TYPE(qgl::ConvexPolygonBorder*);

#endif  // qglConvexPolygonBorder.h

