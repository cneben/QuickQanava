/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

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

// QT headers
#include <QSortFilterProxyModel>
#include <QQuickImageProvider>

// QuickProperties headers
#include "../QuickProperties/src/qpsPropertiesList.h"

// QuickQanava headers
#include "./qanStyle.h"

namespace qan { // ::qan

class IDInterface;
class Graph;

/*! Model a styles list model filtered by style target.
 *
 * This model is a proxy flter based on QSortFilterProxyModel that use a qan::PropertiesList
 * model as its source, and filter styles according to their target. It is usefull to select
 * styles applying only to nodes or edges (by setting the target property to "qan::Node" or
 * qan::Edge). See QanStyleSelector.qml for an example of use.
 *
 * \sa qan::StyleManager::getModelForTarget()
 * \sa QanStyleSelector.qml
 */
class StylesFilterModel : public QSortFilterProxyModel
{
    /*! \name StylesFilterModel Object Management *///-------------------------
    //@{
    Q_OBJECT
public:
    /*! Build a styles list model filtered by style target.             */
    StylesFilterModel( QString target, QObject* parent = 0 ) : QSortFilterProxyModel( parent ),
        _target( target ) {     Q_ASSERT( !_target.isEmpty( ) ); }
    virtual ~StylesFilterModel( ) { }
private:
    Q_DISABLE_COPY( StylesFilterModel )
    QString         _target;
    //@}
    //-------------------------------------------------------------------------


    /*! \name Interview Interface *///-----------------------------------------
    //@{
protected:
    virtual bool    filterAcceptsColumn( int sourceColumn, const QModelIndex& sourceParent) const;
    virtual bool    filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent) const;

public:
    //! \sa qps::PropertiesList::itemCount()
    Q_INVOKABLE int                 itemCount( ) { return rowCount( QModelIndex( ) ); }
    //! \sa qps::PropertiesList::itemAt()
    Q_INVOKABLE qps::Properties*    itemAt( int styleIndex ) { return getStyleAt( styleIndex ); }

    //! Return the style corresponding to this model index (it is different from PropertiesModel index since we are in a model proxy...).
    Q_INVOKABLE qan::Style* getStyleAt( int styleIndex );
    //! FIXME.
    Q_INVOKABLE int         getStyleIndex( qan::Style* style );
    //! Return true if a style with the requested name exists in this model (it will be filtered with the actual style target).
    Q_INVOKABLE bool        hasStyle( QString styleName );
    //@}
    //-------------------------------------------------------------------------
};

/*! Factory for Style objects in a Graph.
 */
class StyleManager : public qps::PropertiesList
{
    Q_OBJECT
    /*! \name Style Object Management *///-------------------------------------
    //@{
public:
    //! Style manager must be initialized with a valid graph.
    StyleManager( QObject* parent = nullptr );
    virtual ~StyleManager( );
private:
    Q_DISABLE_COPY( StyleManager )
public:
    //! Clear this manager from all its content.
    Q_INVOKABLE virtual void    clear() override;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Style Management *///--------------------------------------------
    //@{
public:
    /*! Generate default styles for node and edge.
     *
     * Generate a "default node" qan::NodeStyle and a "default edge" qan::EdgeStyle registered
     * using setNodeDefaultStyle() and setEdgeDefaultStyle().
     */
    Q_INVOKABLE void            generateDefaultStyles();

    /*! Create an empty style and register it in this manager.
     *  \return a pointer on the new style owned by this manager, or \c nullptr if creation fails.
     */
    Q_INVOKABLE qan::Style*     createStyle( QString styleName, QString targetName = QString( "" ), QString metaTarget = QString( "" ) );
    /*! Create an empty node style and register it in this manager.
     *  \return a pointer on the new style owned by this manager, or \c nullptr if creation fails.
     */
    Q_INVOKABLE qan::NodeStyle* createNodeStyle( QString styleName, QString targetName = QString( "" ) );
    /*! Create an empty edge style and register it in this manager.
     *  \return a pointer on the new style owned by this manager, or \c nullptr if creation fails.
     */
    Q_INVOKABLE qan::EdgeStyle* createEdgeStyle( QString styleName, QString targetName = QString( "" ) );

    /*! Create a copy of an existing style, return it and register it in this manager.
     *  \return a pointer on the new style owned by this manager, or \c nullptr if creation fails (either the requested style
     *          does not exists, or a style with the same name already exists.
     */
    Q_INVOKABLE qan::Style* duplicateStyle( QString styleName, QString duplicatedStyleName );
    /*! Remove an existing style from this manager and automatically delete it.
     *  \return true if the style was succesfully removed.
     */
    bool                    removeStyle( QString styleName );

    //! Return a style with a given name, return \c nullptr if no such style exists.
    Q_INVOKABLE qan::Style* findStyleByName( QString styleName );
    //! Return a style defined as default for a given class name.
    /*!
        \return the oldest registered style with the requested target or \c nullptr if no such style exists.
     */
    Q_INVOKABLE qan::Style* findStyleByTarget( QString targetName );
    //! Get a list of all styles targeted for a specific class name (might be empty()).
    QList< qan::Style* >    getStylesByTarget( QString targetName );

    //! Set style \c defaultNodeStyle a the default style for a specific class of nodes \c targetName.
    void                    setDefaultNodeStyle( QString targetName, qan::NodeStyle* defaultNodeStyle );

    //! Get the default style for a specific node \c targetName.
    Q_INVOKABLE qan::Style* getDefaultNodeStyle( QString targetName );

    //! Set style \c defaultEdgeStyle a the default style for a specific class of edge \c targetName.
    void                    setDefaultEdgeStyle( QString targetName, qan::EdgeStyle* defaultEdgeStyle );

    //! Get the default style for a specific edge \c targetName.
    Q_INVOKABLE qan::Style* getDefaultEdgeStyle( QString targetName );

    using TargetNodeStyleMap = QMap< QString, qan::NodeStyle* >;
    const TargetNodeStyleMap&   getDefaultNodeStyles( ) const { return _defaultNodeStyles; }
    using TargetEdgeStyleMap = QMap< QString, qan::EdgeStyle* >;
    const TargetEdgeStyleMap&   getDefaultEdgeStyles( ) const { return  _defaultEdgeStyles; }
private:
    TargetNodeStyleMap      _defaultNodeStyles;
    TargetEdgeStyleMap      _defaultEdgeStyles;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Styles Model Management *///-------------------------------------
    //@{
public:
    Q_INVOKABLE qan::Style*         getStyleAt( int s ) { return qobject_cast< qan::Style* >( at( s ) ); }
    /*! Get a style manager model for styles with a specific target.
     *
     *  Use this method to get an Interview model of styles registered in this manager
     *  with a specific target. For example, a model exposing styles applying only to
     *  nodes could be accessed by calling this method with the "qan::Node" parameter.
     *  Internally, QSortFilterProxyModel is created with the necessary glue code to
     *  connect on this qan::PropertiesListModel managed by this style manager root qan::PropertiesList class.
     *  Returned model is kept in an internal cache, returned models will be the same
     *  for multiple calls with a similar target.
     *
     *  \note qan::StyleManager keep ownership for the returned models.
     *  \sa qan::PropertiesList::getPropertiesListModel()
     */
    Q_INVOKABLE QAbstractItemModel*     getStylesModelForTarget( QString target );
private:
    /*! Map filtered proxy models of qan::PropertiesList getPropertiesListModel() for
     * a specific style target. */
    QMap< QString, QAbstractItemModel* > _targetModelMap;

public:
    Q_PROPERTY( QAbstractItemModel* nodeStylesModel READ getNodeStylesModel NOTIFY nodeStylesModelChanged FINAL )
    QAbstractItemModel* getNodeStylesModel( ) { return getStylesModelForTarget( "qan::Node" ); }
signals:
    //! Never used, defined to avoid a QML NOTIFY warning.
    void                nodeStylesModelChanged( );

public:
    Q_PROPERTY( QAbstractItemModel* edgeStylesModel READ getEdgeStylesModel NOTIFY edgeStylesModelChanged FINAL )
    QAbstractItemModel* getEdgeStylesModel( ) { return getStylesModelForTarget( "qan::Edge" ); }
signals:
    //! Never used, defined to avoid a QML NOTIFY warning.
    void                edgeStylesModelChanged( );

public:
    Q_PROPERTY( QAbstractItemModel* hEdgesStylesModel READ getHEdgeStylesModel NOTIFY hEdgeStylesModelChanged FINAL )
    QAbstractItemModel* getHEdgeStylesModel( ) { return getStylesModelForTarget( "qan::HEdge" ); }
signals:
    //! Never used, defined to avoid a QML NOTIFY warning.
    void                hEdgeStylesModelChanged( );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::StyleManager )

#endif // qanStyleManager_h

