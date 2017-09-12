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
// \file	qglLine.h
// \author	benoit@destrat.io
// \date	2016 09 29
//-----------------------------------------------------------------------------

#ifndef qglLine_h
#define qglLine_h

// Qt headers
#include <QQuickItem>
#include <QLineF>

// QuickGeoGL headers
#include "./qglSGLineNode.h"

namespace qgl { // ::qgl

class Line : public QQuickItem
{
    /*! \name Line QuickItem *///----------------------------------------------
    //@{
    Q_OBJECT
public:
    explicit Line( QQuickItem* parent = nullptr );
    virtual ~Line() { /*_node destroyed by its SG parent*/ }
    Line( const Line& ) = delete;
    Line& operator=(const Line& ) = delete;

public:
    //! Line p1 point, default to {0.,0.}.
    Q_PROPERTY( QPointF p1 READ getP1 WRITE setP1 NOTIFY p1Changed FINAL )
    inline auto getP1() const noexcept -> QPointF { return _line.p1(); }
    auto        setP1( QPointF p1 ) noexcept -> void;

    //! Line p2 point, default to {0.,0.}.
    Q_PROPERTY( QPointF p2 READ getP2 WRITE setP2 NOTIFY p2Changed FINAL )
    inline auto getP2() const noexcept -> QPointF { return _line.p2(); }
    auto        setP2( QPointF p2 ) noexcept -> void;

    //! Shortcut to internal QLineF length() method.
    inline      qreal length() const { return _line.length(); }
signals:
    void        p1Changed();
    void        p2Changed();
private:
    QLineF      _line{0., 0., 0., 0.};
    static constexpr    qreal MinLength = 0.00001;

public:
    //! Line width, default to 1.0.
    Q_PROPERTY( qreal lineWidth READ getLineWidth WRITE setLineWidth NOTIFY lineWidthChanged FINAL )
    inline auto getLineWidth() const noexcept -> qreal { return _lineWidth; }
    inline auto setLineWidth( qreal lineWidth ) noexcept -> void { _lineWidth = lineWidth; setDirty(WidthDirty); emit lineWidthChanged(); update(); }
private:
    qreal       _lineWidth{1.};
signals:
    void        lineWidthChanged();

public:
    //! Line color, default to black.
    Q_PROPERTY( QColor color READ getColor WRITE setColor NOTIFY colorChanged FINAL )
    inline auto             getColor() const noexcept -> QColor { return _color; }
    Q_INVOKABLE inline auto setColor( QColor color ) noexcept -> void { _color = color; setDirty(ColorDirty); emit colorChanged(); update(); }
private:
    QColor      _color{ 0, 0, 0, 255 };
signals:
    void        colorChanged();

protected:
    enum LineDirtyFlag {
                //! Everything is clean, no update is necessary.
                Clean       = 0,
                P1Dirty     = 4,
                P2Dirty     = 8,
                WidthDirty  = 16,
                ColorDirty  = 32,
                P1Valid     = 64,
                P2Valid     = 128,
                Dirty       = P1Dirty | P2Dirty | WidthDirty | ColorDirty
              };
    Q_DECLARE_FLAGS( LineDirtyFlags, LineDirtyFlag )
protected:
    //! Returns the current flags.
    inline auto getDirtyFlags( ) -> LineDirtyFlags { return _dirtyFlags; }
    //! Return true if flag \c flag is set to dirty.
    inline auto isDirty( LineDirtyFlag flag ) const -> bool { return _dirtyFlags & flag; }
    //! Set flag \c flag to dirty.
    inline auto setDirty( LineDirtyFlag flag ) -> void { _dirtyFlags |= flag; }
    //! Set flag \c flag to true.
    inline auto setLineFlag( LineDirtyFlag flag ) -> void { _dirtyFlags |= flag; }
    //! Return true if flag \c flag is set.
    inline auto getLineFlag( LineDirtyFlag flag ) const -> bool { return _dirtyFlags & flag; }
    //! Clear all flags and set the Clean flag.
    inline auto cleanDirtyFlags( ) -> void { _dirtyFlags = Clean; }
private:
    //! Line current dirty flag.
    LineDirtyFlags   _dirtyFlags;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Qt QML Scene Graph Interface *///--------------------------------
    //@{
protected:
    virtual QSGNode*    updatePaintNode( QSGNode*, UpdatePaintNodeData* ) override;
private:
    SGLineNode*         _node{nullptr};
    //@}
    //-------------------------------------------------------------------------
};

} // ::qgl

#endif  // qglLine.h

