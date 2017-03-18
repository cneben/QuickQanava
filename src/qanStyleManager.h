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
// \file    qanStyleManager.h
// \author  benoit@destrat.io
// \date    2015 06 05
//-----------------------------------------------------------------------------

#ifndef qanStyleManager_h
#define qanStyleManager_h

// Qt headers
#include <QSortFilterProxyModel>
#include <QQuickImageProvider>

// QuickContainers headers
#include "../QuickContainers/src/qcmContainerModel.h"

// QuickQanava headers
#include "./qanStyle.h"

namespace qan { // ::qan

class Graph;

/*! \brief Manage node/edge/group styles in a qan::Graph.
 *
 * \nosubgrouping
 */
class StyleManager : public QObject
{
    Q_OBJECT
    /*! \name Style Object Management *///-------------------------------------
    //@{
public:
    //! Style manager must be initialized with a valid graph.
    explicit StyleManager( QObject* parent = nullptr );
    virtual ~StyleManager( );
    StyleManager( const StyleManager& ) = delete;
public:
    //! Clear this manager from all its content (default node and edge styles are cleared too).
    Q_INVOKABLE virtual void    clear();
    //@}
    //-------------------------------------------------------------------------

    /*! \name Style Management *///--------------------------------------------
    //@{
public:
    /*! \brief Create a copy of an existing style, return it and register it in this manager.
     *  \return a pointer on the new style owned by this manager, or \c nullptr if creation fails (either the requested style
     *          does not exists, or a style with the same name already exists.
     */
    Q_INVOKABLE qan::Style* duplicateStyle( QString styleName, QString duplicatedStyleName );

public:
    using StyleComponentMap = QMap< qan::Style*, QPointer<QQmlComponent> >;

    Q_INVOKABLE void            setStyleComponent(qan::Style* style, QQmlComponent* component) noexcept;
    Q_INVOKABLE QQmlComponent*  getStyleComponent(qan::Style* style)noexcept;
private:
    StyleComponentMap           _styleComponentMap;

public:
    //! Set style \c defaultNodeStyle a the default style for a specific class of nodes \c delegate.
    void                    setNodeStyle( QQmlComponent* delegate, qan::NodeStyle* nodeStyle );

    //! Get the style for a specific node \c delegate, if no such style exist, return default node style.
    qan::NodeStyle*         getNodeStyle( QQmlComponent* delegate );

    using DelegateNodeStyleMap = QMap< QQmlComponent*, qan::NodeStyle* >;
    const DelegateNodeStyleMap&     getNodeStyles() const noexcept { return _nodeStyles; }
    qan::NodeStyle&                 getDefaultNodeStyle() noexcept { return  _defaultNodeStyle; }
private:
    DelegateNodeStyleMap            _nodeStyles;
    qan::NodeStyle                  _defaultNodeStyle;

public:
    //! Set style \c defaultEdgeStyle a the default style for a specific class of edge \c delegate.
    void                    setEdgeStyle( QQmlComponent* delegate, qan::EdgeStyle* edgeStyle );

    //! Get the default style for a specific edge \c delegate, if no such style exist, return default node style.
    qan::EdgeStyle*         getEdgeStyle( QQmlComponent* delegate );

    using DelegateEdgeStyleMap = QMap< QQmlComponent*, qan::EdgeStyle* >;
    const DelegateEdgeStyleMap&     getEdgeStyles() const noexcept { return  _edgeStyles; }
    qan::EdgeStyle&                 getDefaultEdgeStyle() noexcept { return  _defaultEdgeStyle; }
private:
    DelegateEdgeStyleMap            _edgeStyles;
    qan::EdgeStyle                  _defaultEdgeStyle;

public:
    using ObjectVectorModel = qcm::ContainerModel<QVector, QObject*>;

    Q_PROPERTY( QAbstractItemModel* styles READ qmlGetStyles CONSTANT FINAL )
    inline QAbstractItemModel*      qmlGetStyles() noexcept { return &_styles; }
    inline const ObjectVectorModel& getStyles() const noexcept { return _styles; }
private:
    //! Styles containers.
    ObjectVectorModel               _styles;
public:
    Q_INVOKABLE qan::Style*         getStyleAt( int s );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::StyleManager )

#endif // qanStyleManager_h

