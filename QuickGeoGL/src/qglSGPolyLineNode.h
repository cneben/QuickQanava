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
// \file	qglSGPolyCanvasNode.h
// \author	benoit@destrat.io
// \date	2015 11 11
//-----------------------------------------------------------------------------

#ifndef qglSGPolyLineNode_h
#define qglSGPolyLineNode_h

// Std headers
#include <memory>

// Qt headers
#include <QSGGeometryNode>
#include <QSGMaterialShader>

// QuickGeoGL headers
// Nil

namespace qgl { // ::qgl

struct PolyLinePoint {
    float x, y;
    float ppX, ppY;
    float npX, npY;
};

struct PolyLineGadget {
    int             pointCount{0};
    PolyLinePoint*  points{nullptr};
};

class SGPolyLineNode : public QSGGeometryNode
{
    /*! \name SGPolyLineNode Scene Graph Interface *///------------------------
    //@{
public:
    /*! \brief Construct a SGPolyLineNode polyline with \c pointCount points an a given \c strokeMaterial material.
     *
     *  \note The internal PolLineGadget gadget points are left uninitialized.
     *  \param strokeMaterial material used to "stroke" the line, polyline node does not get the material ownership.
     */
    explicit SGPolyLineNode( const QVector<QPointF>& points ) noexcept;
    virtual ~SGPolyLineNode( ) {}
    SGPolyLineNode( const SGPolyLineNode& ) = delete;
    SGPolyLineNode& operator=(const SGPolyLineNode& ) = delete;

public:
    void    createGeometry( const QVector<QPointF>& points );
    //! Update internal geometry with given \c points, is poly line path should be closed, call updateClosedGeometry(true).
    void    updateGeometry( const QVector<QPointF>& points );

    /*! FIXME.
     */
    inline auto getGadget() noexcept -> PolyLineGadget& { return _gadget; }
    inline auto getGadget() const noexcept -> const PolyLineGadget& { return _gadget; }
private:
    static const QSGGeometry::AttributeSet& geometryAttributes();
protected:
    QSGGeometry*    _geometry{nullptr};
    QSGMaterial*    _material{nullptr};
    PolyLineGadget  _gadget;
    //@}
    //-------------------------------------------------------------------------
};


} // ::qgl

#endif  // qglSGPolyLineNode.h

