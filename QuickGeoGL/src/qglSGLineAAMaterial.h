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
// \file	qglSGLineAAMaterial.h
// \author	benoit@destrat.io
// \date	2016 10 01
//-----------------------------------------------------------------------------

#ifndef qglSGLineAAMaterial_h
#define qglSGLineAAMaterial_h

// Qt headers
#include <QColor>
#include <QSGMaterial>
#include <QSGMaterialShader>

// QuickGeoGL headers
// Nil

namespace qgl { // ::qgl

/*! \brief Draw an AA "plain line stroke" with optimized OGL vertex/geometry and fragment shaders code.
 *
 *
 * \note See https://www.mapbox.com/blog/drawing-antialiased-lines/, http://mattdesl.svbtle.com/drawing-lines-is-hard and
 *       https://forum.libcinder.org/topic/smooth-thick-lines-using-geometry-shader for algorithm details.
 */
class SGLineAAShader : public QSGMaterialShader
{
public:
    SGLineAAShader( );
    virtual ~SGLineAAShader( ) { /* Nil */ }
    SGLineAAShader( const SGLineAAShader& ) = delete;
    SGLineAAShader& operator=(const SGLineAAShader& ) = delete;

public:
    char const *const *attributeNames() const override
    {
      static char const *const names[] = { "vertex", 0 };
      return names;
    }
    virtual void	updateState(const RenderState & state, QSGMaterial* newMaterial, QSGMaterial* oldMaterial ) override;
    virtual void    initialize() override;

private:
    static QOpenGLShader*  _gsh;
    int             _combMatrixId{-1};
    int             _mvMatrixId{-1};
    int             _opacityId{-1};
    int             _colorId{-1};
    int             _widthId{-1};
};

class SGLineAAMaterial : public QSGMaterial
{
public:
    SGLineAAMaterial() : QSGMaterial{}
    {
        setFlag( QSGMaterial::Blending );
        setFlag( QSGMaterial::RequiresFullMatrix );
        //setFlag( QSGMaterial::CustomCompileStep );  // Note 20151111: For GSH, try without this flag
    }
    virtual ~SGLineAAMaterial() { /* Nil*/ }
    SGLineAAMaterial( const SGLineAAMaterial& ) = delete;
    SGLineAAMaterial& operator=(const SGLineAAMaterial& ) = delete;

    virtual QSGMaterialType*    type( ) const override { return &_type; }
    virtual QSGMaterialShader*  createShader( ) const override { return new SGLineAAShader{}; }
    virtual int	compare( const QSGMaterial* other ) const override { Q_UNUSED( other ); return 42; }

    //! Get the actual material color.
    inline QColor   getColor( ) const noexcept { return _color; }
    //! Set the actual material color, use SGCanvasNode::modifyStrokeMaterial() to change the color.
    inline void     setColor( QColor color ) noexcept { _color = color; }
    //! Get the material actual stroke width (in pixels).
    inline qreal    getWidth( ) const noexcept { return _width; }
    //! Set the actual material stroke width, use SGCanvasNode::modifyStrokeMaterial() to change the width.
    inline void     setWidth( qreal width ) noexcept { _width = width; }
private:
    static QSGMaterialType  _type;
    QColor                  _color{0, 0, 0, 255};
    qreal                   _width{1.0};
};

} // ::qgl

#endif  // qglSGLineAAMaterial.h

