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
// This file is a part of the QuickQanava software library.
//
// \file	qanStyleManager.cpp
// \author	benoit@destrat.io
// \date	2015 06 05
//-----------------------------------------------------------------------------

// Qt headers
#include <QFont>
#include <QQuickItemGrabResult>
#include <QQmlEngine>

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanStyleManager.h"

namespace qan { // ::qan

/* Style Object Management *///------------------------------------------------
StyleManager::StyleManager( QObject* parent ) :
    QObject{ parent }
{
    _styles.setItemDisplayRole( QStringLiteral("name") ); // Use 'name' property for abstract list model display role
    _styles.append(&_defaultNodeStyle);
    setStyleComponent(&_defaultNodeStyle, qan::Node::delegate(this) );
    _styles.append(&_defaultEdgeStyle);
    setStyleComponent(&_defaultEdgeStyle, qan::Edge::delegate(this) );
}

StyleManager::~StyleManager( )
{
    clear();
}

void    StyleManager::clear()
{
    _styles.clear();
    for ( const auto nodeStyle : _nodeStyles )
        if ( nodeStyle != nullptr )
            nodeStyle->deleteLater();
    _nodeStyles.clear();
    _edgeStyles.clear();
}
//-----------------------------------------------------------------------------

/* Style Management *///-------------------------------------------------------
Style*  StyleManager::duplicateStyle( QString styleName, QString duplicatedStyleName )
{
/*    if ( styleName.isEmpty() )
        return nullptr;

    // Find the existing style
    qan::Style* style = findStyleByName( styleName );
    qan::Style* duplicatedStyle = nullptr;

    if ( style != nullptr ) {
        if ( duplicatedStyleName.isEmpty() )
            duplicatedStyleName = styleName + QString( " 2" );
        duplicatedStyle = style->duplicate( duplicatedStyleName, this );
    }
    if ( duplicatedStyle != nullptr ) {
        _styles.append( duplicatedStyle );
        QQmlEngine::setObjectOwnership( style, QQmlEngine::CppOwnership );
    }
    return duplicatedStyle;*/
    return nullptr; // FIXME QAN3 styles
}

void    StyleManager::setStyleComponent(qan::Style* style, QQmlComponent* component) noexcept
{
    qDebug() << "qan::StyleManager::setStylecomponent(): style=" << style << "\tcomponent=" << component;
    if ( style != nullptr &&
         component != nullptr ) {
        _styleComponentMap.insert( style, component );
        if ( !_styles.contains(style) )
            _styles.append(style);
    }
}

QQmlComponent*  StyleManager::getStyleComponent(qan::Style* style) noexcept
{
    if ( style == nullptr )
        return nullptr;
    auto component = _styleComponentMap.value(style, nullptr);
    if ( component != nullptr )
        QQmlEngine::setObjectOwnership(component, QQmlEngine::CppOwnership);
    return component;
}

void    StyleManager::setNodeStyle( QQmlComponent* delegate, qan::NodeStyle* nodeStyle )
{
    if ( delegate != nullptr &&
         nodeStyle != nullptr )
        _nodeStyles.insert( delegate, nodeStyle );
}

qan::NodeStyle* StyleManager::getNodeStyle( QQmlComponent* delegate )
{
    if ( delegate != nullptr &&
         _nodeStyles.contains( delegate ) )
        return _nodeStyles.value( delegate, nullptr );
    return &_defaultNodeStyle;
}

void    StyleManager::setEdgeStyle( QQmlComponent* delegate, qan::EdgeStyle* edgeStyle )
{
    if ( delegate != nullptr &&
         edgeStyle != nullptr )
        _edgeStyles.insert( delegate, edgeStyle );
}

qan::EdgeStyle* StyleManager::getEdgeStyle( QQmlComponent* delegate )
{
    if ( delegate != nullptr &&
         _edgeStyles.contains( delegate ) )
        return _edgeStyles.value( delegate, nullptr );
    return &_defaultEdgeStyle;
}

qan::Style*     StyleManager::getStyleAt( int s )
{
    const auto style = qobject_cast< qan::Style* >( _styles.at( s ) );
    if ( style != nullptr )
        QQmlEngine::setObjectOwnership(style, QQmlEngine::CppOwnership );
    return style;
}

//-----------------------------------------------------------------------------

} // ::qan

