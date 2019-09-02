/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
// \file	qanGrid.h
// \author	benoit@destrat.io
// \date	2017 01 29
//-----------------------------------------------------------------------------

#pragma once

// Qt headers
#include <QtQml>
#include <QQuickItem>

namespace qan {  // ::qan

/*! \brief Abstract grid interface compatbile with qan::Navigable::grid.
 *
 * A grid could be disabled by setting it's \c visible property to false.
 *
 * \nosubgrouping
 */
class Grid : public QQuickItem
{
    /*! \name Grid Interface *///----------------------------------------------
    //@{
    Q_OBJECT
public:
    explicit Grid(QQuickItem* parent = nullptr);
    virtual ~Grid() override = default;
    Grid(const Grid&) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Grid Management *///---------------------------------------------
    //@{
public:
    /*! \brief Update the grid for a given \c viewRect in \c container coordinate system and project to \c navigable CS.
     *
     * Default implementation return false and should _not_ be called in concrete
     * implementation
     *
     * \return \c false return value for incorrect arguments or grid configuration.
     */
    virtual bool    updateGrid(const QRectF& viewRect,
                               const QQuickItem& container,
                               const QQuickItem& navigable ) noexcept;
protected:
    /*!\brief  Update the grid using cached settings when a grid property change.
     *
     * Default implementation return false, do not call base in overridden members.
     */
    virtual bool    updateGrid() noexcept;

public:
    //! Color for major thicks (usually a point with qan::PointGrid), default to \c lightgrey.
    Q_PROPERTY( QColor thickColor READ getThickColor WRITE setThickColor NOTIFY thickColorChanged FINAL )
    void            setThickColor( QColor thickColor ) noexcept;
    inline QColor   getThickColor() const noexcept { return _thickColor; }
signals:
    void            thickColorChanged();
private:
    QColor          _thickColor{211,211,211};    // lightgrey #D3D3D3 / rgb(211,211,211)

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


/*! \brief Abstract grid with orthogonal geometry.
 *
 * \warning Changing \c geometryComponent dynamically is impossible and sill issue
 * a warning.
 *
 * \nosubgrouping
 */
class OrthoGrid : public Grid
{
    /*! \name OrthoGrid Object Management *///---------------------------------
    //@{
    Q_OBJECT
public:
    explicit OrthoGrid(QQuickItem* parent = nullptr);
    virtual ~OrthoGrid() override;
    OrthoGrid(const OrthoGrid&) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Grid Management *///---------------------------------------------
    //@{
public:
    //! Component used to generate grid concrete layout: usually a rounded Rectangle for point grid or a Qt Quick ShapePath for LineGrid.
    Q_PROPERTY( QQmlComponent* geometryComponent READ getGeometryComponent WRITE setGeometryComponent NOTIFY geometryComponentChanged FINAL )
    //! \copydoc geometryComponent
    void                    setGeometryComponent( QQmlComponent* geometryComponent ) noexcept;
    //! \copydoc geometryComponent
    inline QQmlComponent*   getGeometryComponent() noexcept { return _geometryComponent.data(); }
signals:
    //! \copydoc geometryComponent
    void                    geometryComponentChanged();
private:
    //! \copydoc geometryComponent
    QPointer< QQmlComponent > _geometryComponent{ nullptr };

public:
    virtual bool    updateGrid(const QRectF& viewRect,
                               const QQuickItem& container,
                               const QQuickItem& navigable ) noexcept override;
protected:
    virtual bool    updateGrid() noexcept override;

private:
    //! View rect cached updated in updateGrid(), allow updateGrid() use with no arguments.
    QRectF                  _viewRectCache;
    //! Cache for container targetted by this grid, updated in updateGrid(), allow updateGrid() use with no arguments.
    QPointer<QQuickItem>    _containerCache;
    //! Cache for navigable where this grid is used, updated in updateGrid(), allow updateGrid() use with no arguments.
    QPointer<QQuickItem>    _navigableCache;
    //@}
    //-------------------------------------------------------------------------
};


/*! \brief Draw an (orthogonal) adaptative grid of point on a qan::Navigable \c overlay or \c underlay.
 *
 * A valid QML Component must be provided by the user to visualize points, for example:
 * \code
 *  Qan.Navigable {
 *    navigable: true
 *    Qan.PointGrid { id: pointGrid }
 *    grid: pointGrid
 *  }
 * \endcode
 *
 * \nosubgrouping
 */
class PointGrid : public OrthoGrid
{
    /*! \name PointGrid Object Management *///---------------------------------
    //@{
    Q_OBJECT
public:
    explicit PointGrid(QQuickItem* parent = nullptr);
    virtual ~PointGrid() override;
    PointGrid(const PointGrid&) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Grid Management *///---------------------------------------------
    //@{
public:
    virtual bool    updateGrid(const QRectF& viewRect,
                               const QQuickItem& container,
                               const QQuickItem& navigable ) noexcept override;
private:
    //! Grid points cache.
    std::vector<QQuickItem*>    _points;
    //@}
    //-------------------------------------------------------------------------
};

/*! * \brief Draw an orthogonal grid with lines.
 *
 * \code
 *  Qan.Navigable {
 *    navigable: true
 *    Qan.LineGrid { id: lineGrid }
 *    grid: lineGrid
 *  }
 * \endcode
 *
 * \nosubgrouping
 */
class LineGrid : public OrthoGrid
{
    /*! \name LineGrid Object Management *///----------------------------------
    //@{
    Q_OBJECT
public:
    explicit LineGrid( QQuickItem* parent = nullptr );
    virtual ~LineGrid() override;
    LineGrid( const LineGrid& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Grid Management *///---------------------------------------------
    //@{
public:
    //! Concrete grid Qt Quick Shape visual view.
    Q_PROPERTY( QObject* gridShape READ getGridShape WRITE setGridShape NOTIFY gridShapeChanged FINAL )
    //! \copydoc gridShape
    void                setGridShape( QObject* gridShape ) noexcept;
    //! \copydoc gridShape
    inline QObject*     getGridShape() noexcept { return _gridShape.data(); }
signals:
    //! \copydoc gridShape
    void                gridShapeChanged();
private:
    //! \copydoc gridShape
    QPointer< QObject > _gridShape{nullptr};
signals:
    //! Emitted when a new line should be inserted in the grid internall Shape (Qt Quick Shapes) view.
    void                addLine(QObject* line);

public:
    virtual bool        updateGrid(const QRectF& viewRect,
                                   const QQuickItem& container,
                                   const QQuickItem& navigable ) noexcept override;
private:
    //! Grid line cache (contains QuickShapePath* objects).
    std::vector<QObject*>   _lines;
    //@}
    //-------------------------------------------------------------------------
};

}  // ::qan

QML_DECLARE_TYPE( qan::Grid );
QML_DECLARE_TYPE( qan::OrthoGrid );
QML_DECLARE_TYPE( qan::PointGrid );
QML_DECLARE_TYPE( qan::LineGrid );
