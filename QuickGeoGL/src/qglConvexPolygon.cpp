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
// \file	qglConvexPolygon.cpp
// \author	benoit@destrat.io
// \date	2016 09 24
//-----------------------------------------------------------------------------

// Qt headers
#include <QQuickItem>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

// QuickGeoGL headers
#include "qglConvexPolygon.h"
#include "qglSGConvexPolygonNode.h"

namespace qgl { // ::qgl

/* ConvexPolygon Object Management *///----------------------------------------------
ConvexPolygonBackground::ConvexPolygonBackground( QQuickItem* parent ) :
    QQuickItem{ parent }
{
    setFlag( ItemHasContents, true );
}

void    ConvexPolygonBackground::appendPoint( QPointF p )
{
    auto polygon{_polygon};
    polygon.append( p );
    setPolygon( polygon );
}

auto     ConvexPolygonBackground::setPolygon( QPolygonF polygon ) noexcept -> void
{
    _polygon = polygon;
    setDirty(GeometryDirty);
    emit polygonChanged();
    update();
}

void    ConvexPolygonBackground::setPolygon( const QVariantList& polygon )
{
    _polygon.resize(polygon.size());
    int p{0};
    for ( const auto& point: polygon )
        _polygon[p++] = point.toPointF();
    setDirty(GeometryDirty);
    emit polygonChanged();
    update();
}
//-----------------------------------------------------------------------------

/* Qt QML Scene Graph Interface *///-------------------------------------------
QSGNode* ConvexPolygonBackground::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* )
{
    qgl::SGConvexPolygonNode* polygonNode = reinterpret_cast< qgl::SGConvexPolygonNode* >( oldNode );
    if ( polygonNode == nullptr ) {
        polygonNode = new qgl::SGConvexPolygonNode{};
        setDirty( Dirty );
    }

    if ( polygonNode != nullptr ) {
        if ( isDirty( ConvexPolygonBackground::GeometryDirty ) ) {
            polygonNode->updateGeometry(_polygon);
        }
        if ( isDirty( ConvexPolygonBackground::ColorDirty ) ) {
            // FIXME: move that to SG node
            auto material = static_cast<QSGFlatColorMaterial*>(polygonNode->material());
            if ( material != nullptr ) {
                material->setColor( _color );
                polygonNode->markDirty( QSGNode::DirtyMaterial );
            }
        }
        cleanDirtyFlags();
    }
    return polygonNode;
}
//-----------------------------------------------------------------------------


/* ConvexPolygon Object Management *///----------------------------------------------
ConvexPolygon::ConvexPolygon( QQuickItem* parent ) :
    QQuickItem{ parent },
    _background{ new ConvexPolygonBackground{this} }
{
    _background->setParentItem(this);
    _background->setWidth( width() );
    _background->setHeight( height() );
    connect( &_border, &ConvexPolygonBorder::visibleChanged, this, &ConvexPolygon::borderVisibleChanged );
    connect( &_border, &ConvexPolygonBorder::colorChanged, this, &ConvexPolygon::borderColorChanged );
    connect( &_border, &ConvexPolygonBorder::widthChanged, this, &ConvexPolygon::borderWidthChanged );

    connect( this, &QQuickItem::widthChanged, this, &ConvexPolygon::onWidthChanged );
    connect( this, &QQuickItem::heightChanged, this, &ConvexPolygon::onHeightChanged );
}

ConvexPolygon::~ConvexPolygon()
{
    /* _background destroyed by its SG parent */
    /* _borderLine destroyed by its SG parent */
}

void    ConvexPolygon::onWidthChanged()
{
    if ( _background != nullptr )
        _background->setWidth( width() );
    if ( _borderLine != nullptr )
        _borderLine->setHeight( height() );
}

void    ConvexPolygon::onHeightChanged()
{
    if ( _background != nullptr )
        _background->setHeight( height() );
    if ( _borderLine != nullptr )
        _borderLine->setHeight( height() );
}

void    ConvexPolygon::appendPoint( QPointF p )
{
    _background->appendPoint( p );
    updateBorderLine();
}

auto     ConvexPolygon::setPolygon( QPolygonF polygon ) noexcept -> void
{
    _background->setPolygon( polygon );
    emit polygonChanged();
    updateBorderLine();
}

void    ConvexPolygon::setPolygon( const QVariantList& polygon )
{
    QPolygonF qPolygon(polygon.size());
    int p{0};
    for ( const auto& point: polygon )
        qPolygon[p++] = point.toPointF();
    _background->setPolygon( qPolygon );
    emit polygonChanged();
    updateBorderLine();
}

auto     ConvexPolygon::setColor( QColor color ) noexcept -> void
{
    if ( _borderLine != nullptr )
        _borderLine->setColor( color );
    if ( _background != nullptr)
        _background->setColor(color);
    emit colorChanged();
}
//-----------------------------------------------------------------------------

/* Polygon Border *///----------------------------------------------
auto    ConvexPolygon::updateBorderLine() noexcept -> void
{
    if ( !_background ||
         _background->getPolygon().size() < 1 ) // Do not initialize border until there is points in the polygon
        return;
    if ( _borderLine == nullptr ) {
        try {
            _borderLine = new qgl::PolyLine(nullptr);
            _borderLine->setParentItem(this);
            _borderLine->setVisible(true);
            _borderLine->setColor( _border.getColor() );
            _borderLine->setLineWidth( _border.getWidth() );
            _borderLine->setWidth( width() );
            _borderLine->setHeight( height() );
            _borderLine->setZ( _background->z() + 1. );
            _borderLine->setClosed(true);
        } catch (...) { }
    }
    if ( _borderLine != nullptr )
        _borderLine->setPoints( _background->getPolygon() );
}

void    ConvexPolygon::borderColorChanged() noexcept
{
    updateBorderLine();
    if ( _borderLine != nullptr )
        _borderLine->setColor(_border.getColor());
}

void    ConvexPolygon::borderWidthChanged() noexcept
{
    updateBorderLine();
    if ( _borderLine != nullptr )
        _borderLine->setLineWidth(_border.getWidth());
}

void    ConvexPolygon::borderVisibleChanged() noexcept
{
    if ( _borderLine != nullptr )
        _borderLine->setVisible(_border.getVisible());
}
//-----------------------------------------------------------------------------

} // ::qgl
