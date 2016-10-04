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
// \file	qglSGConvexPolygonNode.h
// \author	benoit@destrat.io
// \date	2016 09 24
//-----------------------------------------------------------------------------

#ifndef qglSGConvexPolygonNode_h
#define qglSGConvexPolygonNode_h

// Std headers
#include <memory>

// Qt headers
#include <QSGGeometryNode>
#include <QSGMaterialShader>

// QuickGeoGL headers
// Nil

namespace qgl { // ::qgl

struct PolygonPoint {
    float x, y;
};

struct PolygonGadget {
    int             pointCount;
    PolygonPoint*   points{nullptr};
};

class SGConvexPolygonNode : public QSGGeometryNode
{
    /*! \name SGConvexPolygonNode Scene Graph Interface *///-------------------
    //@{
public:
    /*! \brief Construct a SGConvexPolygonNode polyline with \c pointCount points an a given \c strokeMaterial material.
     *
     *  \note The internal PolLineGadget gadget points are left uninitialized.
     *  \param strokeMaterial material used to "stroke" the line, polyline node does not get the material ownership.
     */
    SGConvexPolygonNode() noexcept;
    virtual     ~SGConvexPolygonNode( ) { }
    SGConvexPolygonNode( const SGConvexPolygonNode& ) = delete;
    SGConvexPolygonNode& operator=(const SGConvexPolygonNode& ) = delete;

    auto        createGeometry( const QPolygonF& polygon ) noexcept -> void;
    auto        updateGeometry( const QPolygonF& polygon ) noexcept -> void;

    /*! Every time you access the underlining polyline gadget, this node consider that you have modified it, internal geometry may be updated.
     *
     * \note Automatically call setDirty().
     */
    inline  auto    getGadget( ) noexcept -> PolygonGadget& { return _gadget; }
    inline  auto    getGadget( ) const noexcept -> const PolygonGadget& { return _gadget; }
private:
    static const QSGGeometry::AttributeSet& geometryAttributes();
protected:
    std::unique_ptr<QSGGeometry>    _geometry{nullptr};
    std::unique_ptr<QSGMaterial>    _material{nullptr};
    PolygonGadget                   _gadget;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qgl

#endif  // qglSGConvexPolygonNode.h

