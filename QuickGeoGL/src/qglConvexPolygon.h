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
// \file	qglConvexPolygon.h
// \author	benoit@destrat.io
// \date	2016 09 24
//-----------------------------------------------------------------------------

#ifndef qglConvexPolygon_h
#define qglConvexPolygon_h

// Std headers
#include <memory>

// Qt headers
#include <QQuickItem>
#include <QPolygonF>

// QuickGeoGL headers
#include "./qglConvexPolygonBorder.h"
#include "./qglPolyLine.h"
#include "./qglSGConvexPolygonNode.h"

namespace qgl { // ::qgl

/*! Internaly used as a convex polygon background item.
 *
 * \nosubgrouping
 */
class ConvexPolygonBackground : public QQuickItem
{
    /*! \name ConvexPolygonBackground QuickItem*///----------------------------
    //@{
    Q_OBJECT
public:
    explicit ConvexPolygonBackground( QQuickItem* parent = nullptr );
    virtual ~ConvexPolygonBackground() { }
    ConvexPolygonBackground( const ConvexPolygonBackground& ) = delete;
    ConvexPolygonBackground& operator=(const ConvexPolygonBackground& ) = delete;

public:
    //! Append a point to this convex polygon from QML (points should be inserted ordered either clockwise or counter clockwise).
    void        appendPoint( QPointF p );

public:
    //! FIXME.
    Q_PROPERTY( QPolygonF polygon READ getPolygon WRITE setPolygon NOTIFY polygonChanged FINAL )
    inline auto     getPolygon() const noexcept -> QPolygonF { return _polygon; }
    auto            setPolygon( QPolygonF polygon ) noexcept -> void;
    //! Set polygon points from QML (points should be inserted ordered either clockwise or counter clockwise).
    Q_INVOKABLE void    setPolygon( const QVariantList& polygon );

private:
    QPolygonF   _polygon{};
signals:
    void        polygonChanged();

public:
    //! Polygon color, default to black.
    Q_PROPERTY( QColor color READ getColor WRITE setColor NOTIFY colorChanged FINAL )
    inline auto     getColor() const noexcept -> QColor { return _color; }
    inline auto     setColor( QColor color ) noexcept -> void { _color = color; setDirty(ColorDirty); emit colorChanged(); update(); }
private:
    QColor      _color{ 0, 0, 0, 255 };
signals:
    void        colorChanged();

public:
    enum ConvexPolygonDirtyFlag {
                //! Everything is clean, no update is necessary.
                Clean               = 0,
                PointAddedDirty     = 2,
                PointRemovedDirty   = 4,
                GeometryDirty       = 8,
                ColorDirty          = 16,
                BorderWidthDirty    = 32,
                BorderColorDirty    = 64,
                Dirty               = GeometryDirty | ColorDirty | BorderWidthDirty | BorderColorDirty
              };
    Q_DECLARE_FLAGS( ConvexPolygonDirtyFlags, ConvexPolygonDirtyFlag )
protected:
    //! Returns the current flags.
    inline auto getDirtyFlags( ) -> ConvexPolygonDirtyFlags { return _dirtyFlags; }
    //! Return true if flag \c flag is set to dirty.
    inline auto isDirty( ConvexPolygonDirtyFlag flag ) const -> bool { return _dirtyFlags & flag; }
    //! Set flag \c flag to dirty.
    inline auto setDirty( ConvexPolygonDirtyFlag flag ) -> void { _dirtyFlags |= flag; }
    //! Clear all flags and set the Clean flag.
    inline auto cleanDirtyFlags( ) -> void { _dirtyFlags = Clean; }
private:
    //! ConvexPolygon current dirty flag.
    ConvexPolygonDirtyFlags   _dirtyFlags;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Qt QML Scene Graph Interface *///--------------------------------
    //@{
protected:
    virtual QSGNode*        updatePaintNode( QSGNode*, UpdatePaintNodeData* ) override;
    //@}
    //-------------------------------------------------------------------------
};


/*! Closed convex polygon with a solid fill color and antialiased borders.
 *
 * Convex polygon points should be ordered, either clockwise or counterclockwise and the polygon path
 * must be closed: last polygon point coordinates should equals first point one.
 *
 * It is up to the user to ensure theses limitations are met when geometry is feed into
 * the polygon, otherwise behaviour at rendering is undefined.
 *
 * \nosubgrouping
 */
class ConvexPolygon : public QQuickItem
{
    /*! \name ConvexPolygon QuickItem*///--------------------------------------------
    //@{
    Q_OBJECT
public:
    explicit ConvexPolygon( QQuickItem* parent = nullptr );
    virtual ~ConvexPolygon();
    ConvexPolygon( const ConvexPolygon& ) = delete;
    ConvexPolygon& operator=(const ConvexPolygon& ) = delete;
protected slots:
    void    onWidthChanged();
    void    onHeightChanged();

public:
    //! Append a point to this convex polygon from QML (points should be inserted ordered either clockwise or counter clockwise).
    Q_INVOKABLE void    appendPoint( QPointF p );

public:
    //! FIXME.
    Q_PROPERTY( QPolygonF polygon READ getPolygon WRITE setPolygon NOTIFY polygonChanged FINAL )
    inline auto     getPolygon() const noexcept -> QPolygonF { return _background->getPolygon(); }
    auto            setPolygon( QPolygonF polygon ) noexcept -> void;
    //! Set polygon points from QML (points should be inserted ordered either clockwise or counter clockwise).
    Q_INVOKABLE void    setPolygon( const QVariantList& polygon );
signals:
    void            polygonChanged();

public:
    //! Polygon color, default to black.
    Q_PROPERTY( QColor color READ getColor WRITE setColor NOTIFY colorChanged FINAL )
    inline auto     getColor() const noexcept -> QColor { return _background->getColor(); }
    auto            setColor( QColor color ) noexcept -> void;
signals:
    void            colorChanged();

public:
    //! Polygon color, default to black.
    Q_PROPERTY( QQuickItem* background READ getBackground CONSTANT FINAL )
    inline auto     getBackground() const noexcept -> QQuickItem* { return _background; }
private:
    //! Polygon background.
    ConvexPolygonBackground*    _background{nullptr};
    //@}
    //-------------------------------------------------------------------------

    /*! \name Polygon Border *///----------------------------------------------
    //@{
public:
    //! Convex polygon border settings.
    Q_PROPERTY(ConvexPolygonBorder* border READ getBorder CONSTANT )
    inline auto     getBorder() -> ConvexPolygonBorder* { return &_border; }
private:
    ConvexPolygonBorder     _border;
    inline auto             updateBorderLine() noexcept -> void;
protected slots:
    void    borderColorChanged() noexcept;
    void    borderWidthChanged() noexcept;
    void    borderVisibleChanged() noexcept;
private:
    qgl::PolyLine*  _borderLine{nullptr};
    //@}
    //-------------------------------------------------------------------------
};

} // ::qgl

#endif  // qglConvexPolygon.h

