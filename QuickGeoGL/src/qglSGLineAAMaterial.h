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

