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
    /*! Set to true to force this polyline to act as closed path, with last point automatically chained to first one, default to false.
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
    /*! Apply the current \c closed property to the internal geometry.
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

