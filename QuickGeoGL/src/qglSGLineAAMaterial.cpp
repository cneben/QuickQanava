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
// \file	qglSGLineAAMaterial.cpp
// \author	benoit@qanava.org
// \date	2016 10 01
//-----------------------------------------------------------------------------

// Qt headers
#include <QOpenGLFunctions>
#include <QSGEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

// QuickGeoGL headers
#include "./qglSGLineAAMaterial.h"

namespace qgl { // ::qgl

SGLineAAShader::SGLineAAShader( ) :
    QSGMaterialShader{}
{
    setShaderSourceFile( QOpenGLShader::Vertex, ":/QuickGeoGL/qglLineAAVsh.glsl");

    _gsh = new QOpenGLShader( QOpenGLShader::Geometry );
    // First time I need to be plateform specific with Qt, but shader is no found on linux with qrc:// url
    if ( !_gsh->compileSourceFile( ":/QuickGeoGL/qglLineAAGsh.glsl" ) )
        qDebug( ) << "SGLineAAShader::SGLineAAShader(): geometry shader compilation fails: " << _gsh->log( );
    else if ( !program()->addShader( _gsh ) )
        qDebug( ) << "SGLineAAShader::SGLineAAShader(): shader could not be added to OGL program.";

    setShaderSourceFile( QOpenGLShader::Fragment, ":/QuickGeoGL/qglLineAAFsh.glsl");
}

void	SGLineAAShader::updateState(const RenderState & state, QSGMaterial* newMaterial, QSGMaterial* oldMaterial )
{
    QSGMaterialShader::updateState( state, newMaterial, oldMaterial );
    if ( program() == nullptr ||
         !program()->isLinked( ) ) {
        qDebug() << "qgl::SGLineAAShader::updateState(): Error: Material shader is not linked.";
        return;
    }
    if ( state.isMatrixDirty( ) ) {
        program()->setUniformValue( _combMatrixId, state.combinedMatrix( ) );
        program()->setUniformValue( _mvMatrixId, state.modelViewMatrix( ) );
    }
    if ( state.isOpacityDirty( ) )
        program()->setUniformValue( _opacityId, state.opacity( ) );

    auto m = static_cast< SGLineAAMaterial* >( newMaterial );
    if ( m != nullptr ) {
        program()->setUniformValue( _colorId, m->getColor( ) );
        program()->setUniformValue( _widthId, ( GLfloat )m->getWidth( ) );
    }
}

void SGLineAAShader::initialize( )
{
    if ( program() == nullptr ||
         !program()->isLinked( ) ) {
        qDebug() << "qgl::SGLineAAShader::initialize(): Error: Material shader is not linked.";
        return;
    }
    _combMatrixId = program()->uniformLocation( "combmatrix" );
    _mvMatrixId = program()->uniformLocation( "mvmatrix" );
    _opacityId = program()->uniformLocation( "opacity" );
    _colorId = program()->uniformLocation( "color" );
    _widthId = program()->uniformLocation( "width" );
}
//-----------------------------------------------------------------------------

} // ::qgl
