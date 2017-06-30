/*
    This file is part of QuickContainers library.

    Copyright (C) 2016  Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickContainers library.
//
// \file    qcmObjectModel.h
// \author	benoit@destrat.io
// \date	2015 05 01
//-----------------------------------------------------------------------------

#ifndef qcmObjectModel_h
#define qcmObjectModel_h

// Qt headers
#include <QDebug>
#include <QAbstractListModel>
#include <QPointer>
#include <QQmlEngine>   // QML_DECLARE_TYPE

//! QuickContainers main namespace.
namespace qcm { // ::qcm

/*! \brief Interface for accessing Qt QObject static and dynamic properties with support for XML serialization and Interview abstract list model.
 *
 * See the QuickContainers documentation for more information: \ref qpsintro
 *
 * Static properties are defined at compile time via meta object compiler Q_PROPERTY macro. Static properties are the
 * prefered way to access QObject properties from QML.
 * Dynamic properties are defined at run time via QObject::setProperty() method.
 *
 * Static properties are displayed first in the properties model list, they are followed
 * by dynamic properties in the order returned by QObject::dynamicPropertyNames() method.
 *
 * Dynamic enumerations are supported with the addEnumProperty(), isEnum() and getEnumValueLabels(). To add
 * a dynamic property enum based on Qt Qt::PenStyle, use the following code:
 * \code
 * qcm::ObjectModel properties = new qcm::ObjectModel();
 * QStringList lineStyles; lineStyles << "No line" << "Solid line" << "Dash line" << "Dot line" << "Dash Dot line" << "Dash Dot Dot line";
 * properties->addEnumProperty( "Pen Style property", QVariant::fromValue< Qt::PenStyle >( Qt::SolidLine ), lineStyles );
 * \endcode
 *
 * \note When defining "property elements" statically with Q_PROPERTY macro in a qcm::ObjectModel subclass, call the monitorStaticProperties() method in your
 *       constructor, otherwise, changes will not be reflected in the model (should be used if you use your properties in qcm::ObjectModelList).
 *
 * \note Copy constructor is disabled for consistency with QObject one, use the duplicate() method for deep copying a qcm::ObjectModel object.
 *
 * Actual limitations (2015/06/20):
 *   - Dynamic properties name can't be changed after creation.
 *   - Static Enum properties are not supported (use the addEnumProperty() method instead).
 *
 * Plus the actual QObject static properties limitations (2015/06/20):
 *  - Static properties defined with the Q_PROPERTY macro can't be suppressed.
 *  - Static properties defined with the Q_PROPERTY name can't be dynamically changed.
 *
 * \note 20151029 internals: when using a Q_PROPERTY with a notify signal, its notify signal name must finish with "Changed", or QuickContainers will not be able
 *                           to detect "property elements" changes.
 * \nosubgrouping
 */
class ObjectModel : public QAbstractListModel
{
    /*! \name ObjectModel Object Management *///-------------------------------
    //@{
    Q_OBJECT
public:
    explicit ObjectModel( QObject* parent = nullptr, QObject* target = nullptr );
    virtual ~ObjectModel( );
    ObjectModel( const ObjectModel& ) = delete;
public:
    /*! \brief Deep copy this qcm::ObjectModel object to \c destination.
     *
     *  \note if destination does not define the same static properties than this qcm::ObjectModel properties, corresponding property in \c destination will be created as a dynamic property.
     *  \warning \c destination has to be "clear", ie either a new empty qcm::ObjectModel or an existance instance where clearProperties() has been previously called.
     *  \return false if copy fails.
     */
    Q_INVOKABLE bool    duplicateTo( qcm::ObjectModel* destination ) const;
public:
    /*! \brief Test equality between two qcm::ObjectModel object (static and dynamic properties are considered equal when they have the same type).
     *
     * \note If this properties has no target or \c right has no target, false is immediatly returned.
     * \note Comparing two properties with the same target return a fast true.
     */
    bool    operator==( const qcm::ObjectModel& right ) const;

public:
    //! Remove all dynamic properties.
    virtual void    resetProperties();

public:
    //! Target QObject (ie exposed properties will be target properties).
    Q_PROPERTY( QObject* target READ getTarget WRITE setTarget NOTIFY targetChanged FINAL )
    inline QObject*     getTarget( ) const noexcept { return _target.data(); }
    /*!
     * \warning setHiddenStaticPropertyCount() is automatically set to true when target is this to avoid visualizing internal qcm::ObjectModel properties.
     * \sa target
     */
    virtual void        setTarget( QObject* target );
protected:
    //! \copydoc qcm::ObjectModel::target
    QPointer<QObject>   _target;
signals:
    //! \copydoc qcm::ObjectModel::target
    void                targetChanged( );
protected slots:
    void                targetDestroyed( QObject* target );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Property Creation Management *///--------------------------------
    //@{
public:
    /*! \brief Test if a property with the given name exist in this QObject static or dynamic properties.
     * \return true if a property exists, false otherwise.
     */
    Q_INVOKABLE bool    hasProperty( QString propertyName ) const;

    //! Return true if a static property with name \c propertyName exists in this object or in targeted object.
    bool                hasStaticProperty( QString propertyName ) const;

    //! Return true if a given static property is writable (return false if the property is read-only or the property does not exists).
    bool                staticPropertyIsWritable( QString propertyName ) const;

    //! Add a QObject dynamic property, property changes will be monitored and reflected in this properties models (PropertyGraphModel and PropertiesModel).
    /*!
     * \param name  property desired name.
     * \param value property type will be the same as the one returned by value QVariant type() method, value can be invalid.
     * \return true if property creation succeed, false otherwise (eg a property with the given name already exists).
     */
    Q_INVOKABLE bool    addDynamicProperty( QString name, QVariant value );
    //! Shortcut for QObject::setProperty() method with a QString instead of a char* for property name (and protection against binding loop)s.
    Q_INVOKABLE void    setProperty( QString propertyName, QVariant value );
    //! Return property with name 'propertyName' current value.
    Q_INVOKABLE virtual QVariant    getProperty( QString propertyName ) const { return property( propertyName.toLatin1( ) ); }
    /*! \brief Return property number 'propertyIndex' current value.
     *
     * \note Be carefull, hidden static properties count is taken into account, if 2 static "property elements" are hidden, you should use 0 to
     * access the third property.
     */
    Q_INVOKABLE virtual QVariant    getProperty( int propertyIndex );
    /*! \brief Get abstract model index of property \c propertyName.
     *
     * \param  hideStaticProperties when true, current hidden static properties count will be substract to the returned index (default behaviour, to
     * access index of visible properties).
     */
    Q_INVOKABLE virtual int         getPropertyIndex( QString propertyName, bool hideStaticProperties = true );

    /*! \brief Equivalent to QObject::setProperty(), but ensure faster model update.
     *
     * \note Works for both synamic and static properties.
     */
    bool            setProperty( const char* propertyName, QVariant propertyValue );

public:
    /*! \brief Set the number of static properties that will be kept hidden in this model (for example set to 1 in order to hide Qt only objectName property).
     *
     * This value default to 2 when target is this, in order to hide Qt 'objectName' property attached to every qobjects and internal qcm::ObjectModel Q_PROPERTY.
     *
     * \param count number of the first count static property that will be hidden by this model.
     */
    auto            hideStaticProperties( int count ) -> void;
    inline auto     getHiddenStaticPropertiesCount() const noexcept -> int { return _hiddenStaticPropertiesCount; }
protected:
    int     _hiddenStaticPropertiesCount = 0;
    //@}
    //-------------------------------------------------------------------------


    /*! \name Interview Interface *///-----------------------------------------
    //@{
public:
    enum PropertyRoles {
        PropertyNameRole = Qt::UserRole + 1,
        PropertyDataRole,
        PropertyTypeRole
    };
public:
    //! Return the number of "property elements" in this properties (be carefull, hidden static properties count is substracted from the returned results).
    Q_INVOKABLE int     count( ) { return rowCount( QModelIndex( ) ); }
    Q_INVOKABLE void    resetPropertiesModel( ) { beginResetModel( ); endResetModel( ); }

    virtual int         rowCount( const QModelIndex & parent = QModelIndex( ) ) const override;
    virtual QVariant    data( const QModelIndex & index, int role = Qt::DisplayRole ) const override;
    void                updateProperty( QString propertyName );
protected:
    virtual QHash< int, QByteArray >    roleNames( ) const override;
    //@}
    //-------------------------------------------------------------------------


    /*! \name Property Modification Monitoring *///----------------------------
    //@{
public:
    //! Should be called in your custom c++ properties class constructor to allow change detection for properties defined with Q_PROPERTY macros calls.
    Q_INVOKABLE void    monitorStaticProperties( );
signals:
    //! Cast whenever a property (either dynamic or static) is modified.
    void                propertiesModified( QString propertyName = "" );
protected slots:
    void                propertyChanged( );
protected:
    virtual bool        eventFilter( QObject *obj, QEvent* e ) override;
    //@}
    //-------------------------------------------------------------------------
};

//! Properties qDebug() stream operator.
QDebug      operator<<( QDebug dbg, const ObjectModel& p );

} // ::qcm

QML_DECLARE_TYPE( qcm::ObjectModel )

#endif // qcmObjectModel_h

