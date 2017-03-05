/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

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
// This file is a part of the QuickQanava library. Copyright 2016 Benoit AUTHEMAN.
//
// \file	qanPointGrid.cpp
// \author	benoit@destrat.io
// \date	2017 01 29
//-----------------------------------------------------------------------------

// Std headers
#include <cmath>        // std::round
#include <iostream>

// Qt headers

// QuickQanava headers
#include "./qanPointGrid.h"

namespace qan {  // ::qan

/* Grid Object Management *///-------------------------------------------------
Grid::Grid( QQuickItem* parent ) : QQuickItem{parent} { }
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


/* PointGrid Object Management *///--------------------------------------------
PointGrid::PointGrid( QQuickItem* parent ) :
    Grid( parent )
{
    setEnabled(false); // Force disabling events handling
}

PointGrid::~PointGrid( )
{
    if ( _pointComponent )
        _pointComponent->deleteLater();
}
//-----------------------------------------------------------------------------

/* Grid Management *///--------------------------------------------------------
void    PointGrid::setPointComponent( QQmlComponent* pointComponent ) noexcept
{
    if ( _pointComponent != pointComponent ) {
        if ( _pointComponent )
            _pointComponent->deleteLater();
        _pointComponent = pointComponent;
        emit pointComponentChanged();
        // FIXME clear _points and create new cached points item with the new component
        //for ( const auto pointItem : qAsConst(_points) ) {}
    }
}

void    PointGrid::updateGrid(const QRectF& viewRect,
                              const QQuickItem& container,
                              const QQuickItem& navigable ) noexcept
{
    if ( !isVisible() )   // Do not update an invisible grid
        return;
    if ( !viewRect.isValid() )
        return;

    _viewRectCache = viewRect;      // Cache all input for an eventual updateGrid() call with
    _containerCache = const_cast<QQuickItem*>(&container);   // no arguments (for example on a qan::Grid property change)
    _navigableCache = const_cast<QQuickItem*>(&navigable);

    int gridMajor{getGridMajor()}; // On major thick every 10 minor thicks
    qreal gridScale{getGridScale()};

    qreal containerZoom = container.scale();
    if ( qFuzzyCompare(1.0 + containerZoom, 1.0) )  // Protect against 0 zoom
        containerZoom = 1.0;
    qreal adaptativeScale = containerZoom < 1. ? gridScale / containerZoom : gridScale;
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

    const int numPointsX = static_cast<int>(std::round(rectified.width() / adaptativeScale));
    const int numPointsY = static_cast<int>(std::round(rectified.height() / adaptativeScale));

    // Update points cache size
    int pointsCacheSize = numPointsX * numPointsY;
    if ( _pointComponent &&
         _points.size() < pointsCacheSize ) {
        int cachedPointsCount = _points.size();
        if ( _points.size() < pointsCacheSize )     // Optimize append of new cached points items
            _points.reserve( pointsCacheSize );
        auto gridContext = QQmlEngine::contextForObject(this);
        for ( int p = cachedPointsCount; p < pointsCacheSize; p++ ) {   // Initialize the necessary items
            const auto point = _pointComponent->create(gridContext);
            const auto pointItem = qobject_cast<QQuickItem*>(point);
            pointItem->setParentItem(this);
            pointItem->setParent( this );
            _points.append(pointItem);
        }
    }

    // Generate grid points
    int pCount = 0;
    for ( int npx = 0; npx < numPointsX; npx++ ) {
        auto px = rectifiedTopLeft.x() + ( npx * adaptativeScale );
        bool isMajorColumn = qFuzzyCompare( 1. + std::fmod(px, gridMajor * adaptativeScale), 1. );
        for ( int npy = 0; npy < numPointsY; npy++ ) {
            auto py = rectifiedTopLeft.y() + ( npy * adaptativeScale );
            bool isMajorRow = qFuzzyCompare( 1. + std::fmod(py, gridMajor * adaptativeScale), 1. );
            _points[pCount]->setVisible(true);
            auto navigablePoint = container.mapToItem(&navigable, QPointF{px, py});
            _points[pCount]->setX(navigablePoint.x());
            _points[pCount]->setY(navigablePoint.y());
            _points[pCount]->setScale( isMajorColumn && isMajorRow ? 1.5 : 1.0 );
            pCount++;
        }
    }
    //qDebug() << "\tConfigured points count=" << pCount;
    for ( int p = pCount; p < _points.size(); p++ )    // Hide unconfigured cached points
        _points[p]->setVisible(false);
}

void    PointGrid::updateGrid() noexcept
{
    if ( _containerCache &&
         _navigableCache &&
         _viewRectCache.isValid() ) // Update the grid with the last correct cached settings
        updateGrid( _viewRectCache, *_containerCache, *_navigableCache );
}
//-----------------------------------------------------------------------------

} // ::qan
