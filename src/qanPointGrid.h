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
// \file	qanPointGrid.h
// \author	benoit@destrat.io
// \date	2017 01 29
//-----------------------------------------------------------------------------

#ifndef qanPointGrid_h
#define qanPointGrid_h

// Qt headers
#include <QtQml>
#include <QQuickItem>

namespace qan {  // ::qan

/*! \brief Abstract grid interface compatbile with qan::Navigable::grid.
 *
 * A grid could be disabled by setting it's \c visible property to false.
 *
 * // FIXME: code sample.
 *
 * \nosubgrouping
 */
class Grid : public QQuickItem
{
    /*! \name Grid Interface *///----------------------------------------------
    //@{
    Q_OBJECT
public:
    explicit Grid( QQuickItem* parent = nullptr );
    virtual ~Grid() { }
    Grid( const Grid& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Grid Management *///---------------------------------------------
    //@{
public:
    //! Update the grid for a given \c viewRect in \c container coordinate system and project to \c navigable CS.
    virtual void    updateGrid(const QRectF& viewRect,
                               const QQuickItem& container,
                               const QQuickItem& navigable ) noexcept { Q_UNUSED(viewRect); Q_UNUSED(container); Q_UNUSED(navigable);}
protected:
    //! Update the grid using cached settings when a grid property change.
    virtual void    updateGrid() noexcept { }

public:
    //! Color for major thicks (usually a point with qan::PointGrid), default to \c darkgrey.
    Q_PROPERTY( QColor thickColor READ getThickColor WRITE setThickColor NOTIFY thickColorChanged FINAL )
    void            setThickColor( QColor thickColor ) noexcept;
    inline QColor   getThickColor() const noexcept { return _thickColor; }
signals:
    void            thickColorChanged();
private:
    QColor          _thickColor{ 169,169,169 };     // darkgray/darkgrey: #A9A9A9 rgb(169,169,169)

public:
    //! Grid thicks width (width for lines, diameter for points), default to 3.0.
    Q_PROPERTY( qreal gridWidth READ getGridWidth WRITE setGridWidth NOTIFY gridWidthChanged FINAL )
    void            setGridWidth( qreal gridWidth ) noexcept;
    inline qreal    getGridWidth() const noexcept { return _gridWidth; }
signals:
    void            gridWidthChanged();
private:
    qreal           _gridWidth{ 3. };

public:
    //! Grid "scale" define how much points or thicks will be drawn, for a value of 100.0, a thicks will be drawn every 100. width or height intervals (at 1.0 zoom).
    Q_PROPERTY( qreal gridScale READ getGridScale WRITE setGridScale NOTIFY gridScaleChanged FINAL )
    void            setGridScale( qreal gridScale ) noexcept;
    inline qreal    getGridScale() const noexcept { return _gridScale; }
signals:
    void            gridScaleChanged();
private:
    qreal           _gridScale{ 100. };

public:
    //! A major thick (or point or line) will be drawn every \c gridMajor thicks (for a \c gridScale of 100 with \c gridMajor set to 5, a major thick will be drawn every 500. points).
    Q_PROPERTY( int gridMajor READ getGridMajor WRITE setGridMajor NOTIFY gridMajorChanged FINAL )
    void            setGridMajor( int gridMajor ) noexcept;
    inline int      getGridMajor() const noexcept { return _gridMajor; }
signals:
    void            gridMajorChanged();
private:
    int             _gridMajor{ 5 };
    //@}
    //-------------------------------------------------------------------------
};


/*! \brief Draw an (orthogonal) adaptative grid of point on a qan::Navigable \c overlay or \c underlay.
 *
 * A valid QML Component must be provided by the user to visualize points, for example:
 * \code
 *  Qan.Navigable {
 *    navigable: true
 *    grid: Qan.PointGrid {
 *      id: pointGrid
 *      pointComponent: Component {
 *        Rectangle {
 *          smooth: true
 *          width: pointGrid.gridWidth; height: width   // Binding to pointGrid could be removed for maximum efficiency if no runtime changes are needed
 *          radius: width/2.
 *          color: pointGrid.thickColor
 *        }
 *    }
 *  }
 * \endcode
 *
 * \warning As of 2017/01/30 it is not possible to change pointComponent dynamically.
 *
 * \nosubgrouping
 */
class PointGrid : public Grid
{
    /*! \name PointGrid Object Management *///---------------------------------
    //@{
    Q_OBJECT
public:
    explicit PointGrid( QQuickItem* parent = nullptr );
    virtual ~PointGrid();
    PointGrid( const PointGrid& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Grid Management *///---------------------------------------------
    //@{
public:
    Q_PROPERTY( QQmlComponent* pointComponent READ getPointComponent WRITE setPointComponent NOTIFY pointComponentChanged FINAL )
    void                    setPointComponent( QQmlComponent* pointComponent ) noexcept;
    inline QQmlComponent*   getPointComponent() noexcept { return _pointComponent.data(); }
signals:
    void                    pointComponentChanged();
private:
    QPointer< QQmlComponent > _pointComponent{ nullptr };

public:
    virtual void    updateGrid(const QRectF& viewRect,
                               const QQuickItem& container,
                               const QQuickItem& navigable ) noexcept override;
protected:
    virtual void    updateGrid() noexcept override;

private:
    //! View rect cache (might be invalid before updateGrid() is called).
    QRectF                  _viewRectCache;
    //! Cache for container targetted by this grid is used (might be nullptr before updateGrid() is called).
    QPointer<QQuickItem>    _containerCache;
    //! Cache for navigable where this grid is used (might be nullptr before updateGrid() is called).
    QPointer<QQuickItem>    _navigableCache;
    QVector<QQuickItem*>    _points;
    //@}
    //-------------------------------------------------------------------------
};

}  // ::qan

QML_DECLARE_TYPE( qan::Grid );
QML_DECLARE_TYPE( qan::PointGrid );

#endif // qanPointGrid_h


