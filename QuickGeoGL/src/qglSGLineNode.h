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
// \file	qglSGCanvasNode.h
// \author	benoit@destrat.io
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

