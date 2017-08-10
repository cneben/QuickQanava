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
// \file	qglArrow.h
// \author	benoit@destrat.io
// \date	2016 09 29
//-----------------------------------------------------------------------------

#ifndef qglArrow_h
#define qglArrow_h

// Std headers
#include <memory>

// Qt headers
#include <QQuickItem>

// QuickGeoGL headers
#include "./qglLine.h"
#include "./qglConvexPolygon.h"

namespace qgl { // ::qgl

class Arrow : public QQuickItem
{
    /*! \name Arrow QuickItem*///----------------------------------------------
    //@{
    Q_OBJECT
public:
    explicit Arrow( QQuickItem* parent = nullptr );
    virtual ~Arrow();
    Arrow( const Arrow& ) = delete;
    Arrow& operator=(const Arrow& ) = delete;

public:
    //! Arrow color, default to black.
    Q_PROPERTY( QColor color READ getColor WRITE setColor NOTIFY colorChanged FINAL )
    inline auto             getColor() const noexcept -> QColor { return _color; }
    Q_INVOKABLE auto        setColor( QColor color ) noexcept -> void;
private:
    QColor      _color{ 0, 0, 0, 255 };
signals:
    void        colorChanged();

public:
    enum CapStyle {
                NoCap       = 0,
                ArrowCap    = 4,
                CircleCap   = 8
              };
    Q_ENUM( CapStyle )
public:
    //! Line source point, default to {0.,0.}.
    Q_PROPERTY( QPointF p1 READ getP1 WRITE setP1 NOTIFY p1Changed FINAL )
    inline auto         getP1() const noexcept -> QPointF { return _p1; }
    auto                setP1( QPointF source ) noexcept -> void;
private:
    QPointF     _p1{0., 0.};
    bool        _p1Valid{false};
signals:
    void        p1Changed();

public:
    //! Line destination point, default to {0.,0.}.
    Q_PROPERTY( QPointF p2 READ getP2 WRITE setP2 NOTIFY p2Changed FINAL )
    inline auto         getP2() const noexcept -> QPointF { return _p2; }
    auto                setP2( QPointF destination ) noexcept -> void;
private:
    QPointF     _p2{0., 0.};
    bool        _p2Valid{false};
signals:
    void        p2Changed();

public:
    //! Line width, default to 1.0.
    Q_PROPERTY( qreal lineWidth READ getLineWidth WRITE setLineWidth NOTIFY lineWidthChanged FINAL )
    inline auto getLineWidth() const noexcept -> qreal { return _line->getLineWidth(); }
    auto        setLineWidth( qreal lineWidth ) noexcept -> void;
signals:
    void        lineWidthChanged();

public:
    //! Arrow source cap style, default to \c NoCap.
    Q_PROPERTY( CapStyle p1CapStyle READ getP1CapStyle WRITE setP1CapStyle NOTIFY p1CapStyleChanged FINAL )
    inline auto         getP1CapStyle() const noexcept -> CapStyle { return _p1CapStyle; }
    auto                setP1CapStyle( CapStyle ) noexcept -> void;
private:
    CapStyle    _p1CapStyle{NoCap};
signals:
    void        p1CapStyleChanged();

protected:
    //! Update internal arrow geometry when either \c p1, \c p2 or cap size or style change.
    auto        updateGeometry() noexcept -> void;
private:
    static constexpr    qreal MinLength = 0.00001;
    static constexpr    qreal Pi = 3.141592653;
    static constexpr    qreal TwoPi = 2. * Pi;

public:
    //! Arrow destination cap style, default to \c ArrowCap.
    Q_PROPERTY( CapStyle p2CapStyle READ getP2CapStyle WRITE setP2CapStyle NOTIFY p2CapStyleChanged FINAL )
    inline auto         getP2CapStyle() const noexcept -> CapStyle { return _p2CapStyle; }
    Q_INVOKABLE auto    setP2CapStyle( CapStyle ) noexcept -> void;
private:
    CapStyle    _p2CapStyle{ArrowCap};
signals:
    void        p2CapStyleChanged();

public:
    /*! \brief Source cap size, arrow base size for \c ArrowCap, circle radius for \c CircleCap, default to 8.0.
     *
     * \note Arrow protect against \c p1CapSize beeing inferior to \c lineWidth.
     */
    Q_PROPERTY( qreal p1CapSize READ getP1CapSize WRITE setP1CapSize NOTIFY p1CapSizeChanged FINAL )
    inline auto         getP1CapSize() const noexcept -> qreal { return _p1CapSize; }
    Q_INVOKABLE auto    setP1CapSize( qreal capSize ) noexcept -> void;
private:
    qreal       _p1CapSize{8.0};
signals:
    void        p1CapSizeChanged();

public:
    /*! \brief Destination cap size, arrow base size for \c ArrowCap, circle radius for \c CircleCap, default to 8.0.
     *
     * \note Arrow protect against \c p2CapSize beeing inferior to \c lineWidth.
     */
    Q_PROPERTY( qreal p2CapSize READ getP2CapSize WRITE setP2CapSize NOTIFY p2CapSizeChanged FINAL )
    inline auto         getP2CapSize() const noexcept -> qreal { return _p2CapSize; }
    Q_INVOKABLE auto    setP2CapSize( qreal capSize ) noexcept -> void;
private:
    qreal       _p2CapSize{8.0};
signals:
    void        p2CapSizeChanged();

protected:
    /*! Take into account actual Cap settings and create or remove the necessary components.
     *
     * \note Should be called when either \c p2CapStyle, \c p1CapStyle or \c p2CapSize are modified.
     */
    auto        updateCapStyle() noexcept -> void;

protected:
    qgl::Line*          _line{nullptr};
    qgl::ConvexPolygon* _p1Arrow{nullptr};
    qgl::ConvexPolygon* _p2Arrow{nullptr};
    //@}
    //-------------------------------------------------------------------------
};

} // ::qgl

#endif  // qglArrow.h
