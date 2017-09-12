/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

