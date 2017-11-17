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
// This file is a part of the QuickQanava library. Copyright 2016 Benoit AUTHEMAN.
//
// \file	qanGrid.cpp
// \author	benoit@destrat.io
// \date	2017 01 29
//-----------------------------------------------------------------------------

// Std headers
#include <cmath>        // std::round
#include <iostream>

// Qt headers

// QuickQanava headers
#include "./qanGrid.h"

namespace qan {  // ::qan

/* Grid Object Management *///-------------------------------------------------
Grid::Grid( QQuickItem* parent ) : QQuickItem{parent}
{
    setEnabled(false);  // Prevent event stealing, grids are not interactive
}
//-----------------------------------------------------------------------------

/* Grid Management *///--------------------------------------------------------
void    Grid::setThickColor( QColor thickColor ) noexcept
{
    if ( thickColor != _thickColor ) {
        _thickColor = thickColor;
        emit thickColorChanged();
    }
}

void    Grid::setGridWidth( qreal gridWidth ) noexcept
{
    if ( gridWidth < 0.001 ) {
        qWarning() << "qan::Grid::setMinorWidth(): Warning, major width should be superior to 0.0";
        return;
    }
    if ( !qFuzzyCompare(1.0 + gridWidth, 1.0 + _gridWidth) ) {
        _gridWidth = gridWidth;
        emit gridWidthChanged();
    }
}

void    Grid::setGridScale( qreal gridScale ) noexcept
{
    if ( gridScale < 0.001 ) {
        qWarning() << "qan::Grid::setGridScale(): Warning, grid scale should be superior to 0.0";
        return;
    }
    if ( !qFuzzyCompare(1.0 + gridScale, 1.0 + _gridScale) ) {
        _gridScale = gridScale;
        emit gridScaleChanged();
        updateGrid();
    }
}

void    Grid::setGridMajor( int gridMajor ) noexcept
{
    if ( gridMajor < 1 ) {
        qWarning() << "qan::Grid::setGridMajor(): Warning, grid major should be superior or equal to 1";
        return;
    }
    if ( gridMajor != _gridMajor ) {
        _gridMajor = gridMajor;
        emit gridMajorChanged();
        updateGrid();
    }
}
//-----------------------------------------------------------------------------


/* OrthoGrid Object Management *///---------------------------------------------
OrthoGrid::OrthoGrid( QQuickItem* parent ) :
    Grid( parent )
{ /* Nil */ }

OrthoGrid::~OrthoGrid()
{
    // Delete the geometry component if it is not owned by QML
    if ( _geometryComponent != nullptr &&
         QQmlEngine::objectOwnership(_geometryComponent) == QQmlEngine::CppOwnership ) {
        _geometryComponent->deleteLater();
        _geometryComponent.clear();
    }
}
//-----------------------------------------------------------------------------

/* Grid Management *///--------------------------------------------------------
void    OrthoGrid::setGeometryComponent( QQmlComponent* geometryComponent ) noexcept
{
    if ( _geometryComponent &&
         _geometryComponent != geometryComponent ) {
        qWarning() << "qan::ORthoGrid::setGeometryComponent(): Component couldn't be initialized dynamically more than one time.";
        return;
    }

    if ( _geometryComponent != geometryComponent ) {
        if ( _geometryComponent != nullptr &&   // Shouldn't happen since dynamic components change are forbiden.
             QQmlEngine::objectOwnership(_geometryComponent) == QQmlEngine::CppOwnership ) {
            _geometryComponent->deleteLater();
            _geometryComponent.clear();
        }
        _geometryComponent = geometryComponent;
        emit geometryComponentChanged();
    }
}

bool    OrthoGrid::updateGrid(const QRectF& viewRect,
                              const QQuickItem& container,
                              const QQuickItem& navigable ) noexcept
{
    // PRECONDITIONS:
        // Grid item must be visible
        // View rect must be valid
        // _geometryComponent must have been set.
    if ( !isVisible() )   // Do not update an invisible grid
        return false;
    if ( !viewRect.isValid() )
        return false;
    if ( !_geometryComponent )
        return false;

    _viewRectCache = viewRect;      // Cache all arguments for an eventual updateGrid() call with
    _containerCache = const_cast<QQuickItem*>(&container);   // no arguments (for example on a qan::Grid property change)
    _navigableCache = const_cast<QQuickItem*>(&navigable);

    return true;
}

bool    OrthoGrid::updateGrid() noexcept
{
    if ( _containerCache &&
         _navigableCache &&
         _viewRectCache.isValid() ) // Update the grid with the last correct cached settings
        return updateGrid( _viewRectCache, *_containerCache, *_navigableCache );
    return false;
}
//-----------------------------------------------------------------------------


/* PointGrid Object Management *///--------------------------------------------
PointGrid::PointGrid( QQuickItem* parent ) :
    OrthoGrid( parent ) { /* Nil */ }

PointGrid::~PointGrid( )
{
    for ( auto& point : _points ) {
        if ( point != nullptr &&
             QQmlEngine::objectOwnership(point) == QQmlEngine::CppOwnership )
             point->deleteLater();
    }
    _points.clear();
}
//-----------------------------------------------------------------------------

/* Grid Management *///--------------------------------------------------------
bool    PointGrid::updateGrid(const QRectF& viewRect,
                              const QQuickItem& container,
                              const QQuickItem& navigable ) noexcept
{
    // PRECONDITIONS:
        // Base implementation should return true
    if ( !OrthoGrid::updateGrid(viewRect, container, navigable) )
        return false;

    const unsigned int gridMajor{static_cast<unsigned int>(getGridMajor())}; // On major thick every 10 minor thicks
    const qreal gridScale{getGridScale()};

    qreal containerZoom = container.scale();
    if ( qFuzzyCompare(1.0 + containerZoom, 1.0) )  // Protect against 0 zoom
        containerZoom = 1.0;
    const qreal adaptativeScale = containerZoom < 1. ? gridScale / containerZoom : gridScale;
    QPointF rectifiedTopLeft{ std::floor(viewRect.topLeft().x() / adaptativeScale) * adaptativeScale,
                              std::floor(viewRect.topLeft().y() / adaptativeScale) * adaptativeScale };
    QPointF rectifiedBottomRight{ ( std::ceil(viewRect.bottomRight().x() / adaptativeScale) * adaptativeScale ),
                                  ( std::ceil(viewRect.bottomRight().y() / adaptativeScale) * adaptativeScale )};
    QRectF rectified{rectifiedTopLeft, rectifiedBottomRight };

    //qDebug() << "rect.topLeft=" << rect.topLeft();
    //qDebug() << "rect.bottomRight=" << rect.bottomRight();
    //qDebug() << "adaptativeScale=" << adaptativeScale;
    //qDebug() << "rectified.topLeft=" << rectified.topLeft();
    //qDebug() << "rectified.bottomRight=" << rectified.bottomRight();

    // FIXME: check that the rounding do not generate the graphics glitchs...
    const unsigned int numPointsX = static_cast<unsigned int>(std::round(rectified.width() / adaptativeScale));
    const unsigned int numPointsY = static_cast<unsigned int>(std::round(rectified.height() / adaptativeScale));

    // Update points cache size
    const unsigned int pointsCount = static_cast<unsigned int>(numPointsX * numPointsY);
    if ( getGeometryComponent() &&
         _points.size() < pointsCount ) {
        unsigned int cachedPointsCount = static_cast<unsigned int>(_points.size());
        try {
            if ( _points.size() < pointsCount )     // Optimize append of new cached points items
                _points.resize(pointsCount);
        } catch (...) { return false; } // Might be std::bad_alloc or std::length_error

        auto gridContext = QQmlEngine::contextForObject(this);
        if ( gridContext == nullptr ) {
            qWarning() << "qan::PointGrid::updateGrid(): Error: No QML context available.";
            return false;
        }
        for ( auto p = cachedPointsCount; p < pointsCount; p++ ) {   // Initialize the necessary items
            const auto pointItem = qobject_cast<QQuickItem*>(getGeometryComponent()->create(gridContext));
            if ( pointItem != nullptr ) {
                pointItem->setParentItem(this);
                pointItem->setParent( this );
                _points[p] = pointItem;
            }
        }
    }
    if ( _points.size() < pointsCount )
        return false;

    // Generate grid points
    unsigned int p = 0;
    for ( unsigned int npx = 0; npx < numPointsX; npx++ ) {
        auto px = rectifiedTopLeft.x() + ( npx * adaptativeScale );
        bool isMajorColumn = qFuzzyCompare( 1. + std::fmod(px, gridMajor * adaptativeScale), 1. );
        for ( unsigned int npy = 0; npy < numPointsY; npy++ ) {
            auto py = rectifiedTopLeft.y() + ( npy * adaptativeScale );
            bool isMajorRow = qFuzzyCompare( 1. + std::fmod(py, gridMajor * adaptativeScale), 1. );
            _points[p]->setVisible(true);
            auto navigablePoint = container.mapToItem(&navigable, QPointF{px, py});
            _points[p]->setX(navigablePoint.x());
            _points[p]->setY(navigablePoint.y());
            _points[p]->setScale( isMajorColumn && isMajorRow ? 1.5 : 1.0 );
            p++;
        }
    }
    while ( p < _points.size() ) {
        if ( _points[p] != nullptr )
            _points[p]->setVisible(false);
        ++p;
    }
    return true;
}
//-----------------------------------------------------------------------------


/* LineGrid Object Management *///---------------------------------------------
LineGrid::LineGrid( QQuickItem* parent ) :
    OrthoGrid( parent ) { /* Nil */ }

LineGrid::~LineGrid( )
{
    for ( auto& line : _lines ) {
        if ( line != nullptr &&
             QQmlEngine::objectOwnership(line) == QQmlEngine::CppOwnership )
             line->deleteLater();
    }
    _lines.clear();
}
//-----------------------------------------------------------------------------

/* Grid Management *///--------------------------------------------------------
void    LineGrid::setGridShape( QObject* gridShape ) noexcept
{
    if ( _gridShape != gridShape ) {
        _gridShape = gridShape;
        emit gridShapeChanged();
    }
}

bool    LineGrid::updateGrid( const QRectF& viewRect,
                              const QQuickItem& container,
                              const QQuickItem& navigable ) noexcept
{
    // PRECONDITIONS:
        // Base implementation should return true
        // gridShape property should have been set
    if ( !OrthoGrid::updateGrid(viewRect, container, navigable) )
        return false;
    if ( !getGridShape() )
        return false;

    const unsigned int  gridMajor{static_cast<unsigned int>(getGridMajor())}; // On major thick every 10 minor thicks
    const qreal gridScale{getGridScale()};

    qreal containerZoom = container.scale();
    if ( qFuzzyCompare(1.0 + containerZoom, 1.0) )  // Protect against 0 zoom
        containerZoom = 1.0;
    const qreal adaptativeScale = containerZoom < 1. ? gridScale / containerZoom : gridScale;
    QPointF rectifiedTopLeft{ std::floor(viewRect.topLeft().x() / adaptativeScale) * adaptativeScale,
                              std::floor(viewRect.topLeft().y() / adaptativeScale) * adaptativeScale };
    QPointF rectifiedBottomRight{ ( std::ceil(viewRect.bottomRight().x() / adaptativeScale) * adaptativeScale ),
                                  ( std::ceil(viewRect.bottomRight().y() / adaptativeScale) * adaptativeScale )};
    QRectF rectified{rectifiedTopLeft, rectifiedBottomRight };

    //qDebug() << "rect.topLeft=" << rect.topLeft();
    //qDebug() << "rect.bottomRight=" << rect.bottomRight();
    //qDebug() << "adaptativeScale=" << adaptativeScale;
    //qDebug() << "rectified.topLeft=" << rectified.topLeft();
    //qDebug() << "rectified.bottomRight=" << rectified.bottomRight();

    const unsigned int numLinesX = static_cast<unsigned int>(std::round(rectified.width() / adaptativeScale));
    const unsigned int numLinesY = static_cast<unsigned int>(std::round(rectified.height() / adaptativeScale));

    // Update points cache size
    const unsigned int linesCount = numLinesX + numLinesY;
    if ( getGeometryComponent() &&
         _lines.size() < linesCount ) {
        unsigned int cachedLinesCount = static_cast<unsigned int>(_lines.size());
        try {
            if ( _lines.size() < linesCount )     // Optimize append of new cached points items
                _lines.resize(linesCount);
        } catch (...) { return false; } // Might be std::bad_alloc or std::length_error

        auto gridContext = QQmlEngine::contextForObject(getGridShape());
        if ( gridContext == nullptr ) {
            qWarning() << "qan::LineGrid::updateGrid(): Error: No QML context available.";
            return false;
        }
        for ( unsigned int p = cachedLinesCount; p < linesCount; p++ ) {   // Initialize the necessary items
            const auto point = qobject_cast<QObject*>(getGeometryComponent()->create(gridContext));
            if ( point != nullptr ) {
                point->setParent( getGridShape() );
                emit addLine(point);
                _lines[p] = point;
            }
        }
    }
    if ( _lines.size() < linesCount )
        return false;

    //for (auto& line : _lines)
    //    if ( line != nullptr )
    //        line->setProperty("visible", QVariant{false});

    const auto navigableRectified = container.mapRectToItem(&navigable, rectified);
    unsigned int p = 0; // Generate HORIZONTAL lines
    for ( unsigned int npx = 0; npx < numLinesX; ++npx ) {
        auto px = rectifiedTopLeft.x() + ( npx * adaptativeScale );
        bool isMajorColumn = qFuzzyCompare( 1. + std::fmod(px, gridMajor * adaptativeScale), 1. );
        auto navigablePoint = container.mapToItem(&navigable, QPointF{px, 0.});
        _lines[p]->setProperty( "visible", QVariant{true});
        _lines[p]->setProperty( "startX", QVariant{navigablePoint.x()});
        _lines[p]->setProperty( "startY", QVariant{navigableRectified.top()});
        _lines[p]->setProperty( "endX", QVariant{navigablePoint.x()});
        _lines[p]->setProperty( "endY", QVariant{navigableRectified.bottom()});
        _lines[p]->setProperty( "strokeWidth", QVariant{ isMajorColumn ? 2. : 1. } );
        ++p;
    }
    for ( unsigned int npy = 0; npy < numLinesY; ++npy ) {   // Generate VERTICAL lines
        auto py = rectifiedTopLeft.y() + ( npy * adaptativeScale );
        bool isMajorRow = qFuzzyCompare( 1. + std::fmod(py, gridMajor * adaptativeScale), 1. );
        auto navigablePoint = container.mapToItem(&navigable, QPointF{0., py});
        _lines[p]->setProperty( "visible", QVariant{true});
        _lines[p]->setProperty( "startX", QVariant{navigableRectified.left()});
        _lines[p]->setProperty( "startY", QVariant{navigablePoint.y()});
        _lines[p]->setProperty( "endX", QVariant{navigableRectified.right()});
        _lines[p]->setProperty( "endY", QVariant{navigablePoint.y()});
        _lines[p]->setProperty( "strokeWidth", QVariant{ isMajorRow ? 2. : 1. } );
        ++p;
    }
    while (p < _lines.size()) {    // Hide unconfigured cached points
        if ( _lines[p] != nullptr )
            _lines[p]->setProperty("visible", QVariant{false});
        ++p;
    }
    return true;
}
//-----------------------------------------------------------------------------

} // ::qan
