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
// \file	qglPolyLine.h
// \author	benoit@destrat.io
// \date	2016 09 30
//-----------------------------------------------------------------------------

#ifndef qglPolyLine_h
#define qglPolyLine_h

// Qt headers
#include <QQuickItem>

// QuickGeoGL headers
#include "./qglSGPolyLineNode.h"

namespace qgl { // ::qgl

/*! Display a polyline, ie a line with multiple linked segment.
 *
 *
 * \nosubgrouping
 */
class PolyLine : public QQuickItem
{
    /*! \name PolyLine QuickItem *///----------------------------------------------
    //@{
    Q_OBJECT
public:
    explicit PolyLine( QQuickItem* parent = nullptr );
    virtual ~PolyLine() { /*_node destroyed by its SG parent*/ }
    PolyLine( const PolyLine& ) = delete;
    PolyLine& operator=(const PolyLine& ) = delete;

public:
    //! PolyLine points, default to an empty vector of points.
    inline auto         getPoints() const noexcept -> const QVector<QPointF>& { return _points; }
    Q_INVOKABLE void    setPoints( const QVariantList& points );
    auto                setPoints( const QVector<QPointF>& points ) noexcept -> void;
private:
    QVector<QPointF>    _points;

public:
    //! PolyLine width, default to 1.0.
    Q_PROPERTY( qreal lineWidth READ getLineWidth WRITE setLineWidth NOTIFY lineWidthChanged FINAL )
    inline auto getLineWidth() const noexcept -> qreal { return _lineWidth; }
    inline auto setLineWidth( qreal lineWidth ) noexcept -> void { _lineWidth = lineWidth; setDirty(WidthDirty); emit lineWidthChanged(); update(); }
private:
    qreal       _lineWidth{1.};
signals:
    void        lineWidthChanged();

public:
    //! PolyLine color, default to black.
    Q_PROPERTY( QColor color READ getColor WRITE setColor NOTIFY colorChanged FINAL )
    inline auto getColor() const noexcept -> QColor { return _color; }
    inline auto setColor( QColor color ) noexcept -> void { _color = color; setDirty(ColorDirty); emit colorChanged(); update(); }
private:
    QColor      _color{ 0, 0, 0, 255 };
signals:
    void        colorChanged();

public:
    /*! \brief Set to true to force this polyline to act as closed path, with last point automatically chained to first one, default to false.
     *
     * \warning Changing \c closed property dynamically is a costly operation since it involve modify
     * poly line geometry internally. To avoid unecessary updates, set the \c closed property to true
     * _before_ calling setPoints() method.
     */
    Q_PROPERTY( bool closed READ getClosed WRITE setClosed NOTIFY closedChanged FINAL )
    inline auto getClosed() const noexcept -> bool { return _closed; }
    auto        setClosed( bool closed ) noexcept -> void;
private:
    bool        _closed{false};
    /*! \brief Apply the current \c closed property to the internal geometry.
     *
     * If \c closed is true and the last vertex doesn't equal the first vertex, close the path by joining last vertex to first vertex.
     * If \c closed is false and the last vertex equals the first vertex, remove the last vertex to open the path.
     */
    inline          auto    applyClosing( bool closed ) -> void;
    static inline   bool    fuzzyComparePoints( const QPointF& p1, const QPointF& p2 );

signals:
    void        closedChanged();

protected:
    enum PolyLineDirtyFlag {
                Clean           = 0,
                GeometryDirty   = 2,
                WidthDirty      = 8,
                ColorDirty      = 16,
                Dirty           = GeometryDirty | WidthDirty | ColorDirty
              };
    Q_DECLARE_FLAGS( PolyLineDirtyFlags, PolyLineDirtyFlag )
protected:
    //! Returns the current flags.
    inline auto getDirtyFlags( ) -> PolyLineDirtyFlags { return _dirtyFlags; }
    //! Return true if flag \c flag is set to dirty.
    inline auto isDirty( PolyLineDirtyFlag flag ) const -> bool { return _dirtyFlags & flag; }
    //! Set flag \c flag to dirty.
    inline auto setDirty( PolyLineDirtyFlag flag ) -> void { _dirtyFlags |= flag; }
    //! Clear all flags and set the Clean flag.
    inline auto cleanDirtyFlags( ) -> void { _dirtyFlags = Clean; }
private:
    //! PolyLine current dirty flag.
    PolyLineDirtyFlags   _dirtyFlags;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Qt QML Scene Graph Interface *///--------------------------------
    //@{
protected:
    virtual QSGNode*    updatePaintNode( QSGNode*, UpdatePaintNodeData* ) override;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qgl

#endif  // qglPolyLine.h

