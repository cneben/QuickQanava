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
// \file	qglSGCanvasNode.h
// \author	benoit@qanava.org
// \date	2015 11 03
//-----------------------------------------------------------------------------

#ifndef qglSGLineNode_h
#define qglSGLineNode_h

// Qt headers
#include <QSGGeometryNode>
#include <QSGMaterialShader>

// QuickGeoGL headers
#include "./qglSGPolyLineNode.h"

namespace qgl { // ::qgl

struct LineVertex {
    float x, y;
};

struct LineGadget {
    // Public
    LineVertex p1;
    // Public
    LineVertex p2;
};

class SGLineNode : public QSGGeometryNode
{
    /*! \name SGLineNode Scene Graph Interface *///----------------------------
    //@{
public:
    /*! \brief Construct a SGLineNode with a given \c strokeMaterial material.
     *
     *  \param strokeMaterial material used to "stroke" the line, line node does not get the material ownership.
     */
    SGLineNode() noexcept;
    virtual ~SGLineNode( ) {}
    SGLineNode(const SGLineNode& ) = delete;
    SGLineNode& operator=(const SGLineNode& ) = delete;

public:
    auto            updateGeometry( const QPointF& p1, const QPointF& p2 ) noexcept -> void;
    inline auto     getGadget( ) noexcept -> LineGadget* { return _gadget; }
    inline auto     getGadget( ) const noexcept -> const LineGadget* { return _gadget; }
private:
    static const QSGGeometry::AttributeSet& geometryAttributes();

protected:
    QSGGeometry*    _geometry{nullptr};
    QSGMaterial*    _material{nullptr};
    LineGadget*     _gadget{nullptr};
    //@}
    //-------------------------------------------------------------------------
};


} // ::qgl

#endif  // qglSGCanvas.h

