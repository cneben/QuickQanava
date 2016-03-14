/*
    This file is part of QuickProperties2 library.

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
// This file is a part of the QuickProperties2 library.
//
// \file    qpsProperties.h
// \author	benoit@qanava.org
// \date	2015 05 01
//-----------------------------------------------------------------------------

#ifndef qpsProperties_h
#define qpsProperties_h

// Qt headers
#include <QtDebug>
#include <QDebug>
#include <QDateTime>
#include <QAbstractListModel>

// QuickProperties headers
#include "./qpsLimit.h"

//! QuickProperties main namespace.
namespace qps { // ::qps

/*! \brief Interface for accessing Qt QObject static and dynamic properties with support for XML serialization and Interview abstract list model.
 *
 * See the QuickProperties documentation for more information: \ref qpsintro
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
 * qps::Properties properties = new qps::Properties();
 * QStringList lineStyles; lineStyles << "No line" << "Solid line" << "Dash line" << "Dot line" << "Dash Dot line" << "Dash Dot Dot line";
 * properties->addEnumProperty( "Pen Style property", QVariant::fromValue< Qt::PenStyle >( Qt::SolidLine ), lineStyles );
 * \endcode
 *
 * \note When defining "property elements" statically with Q_PROPERTY macro in a qps::Properties subclass, call the monitorStaticProperties() method in your
 *       constructor, otherwise, changes will not be reflected in the model (should be used if you use your properties in qps::PropertiesList).
 *
 * \note Copy constructor is disabled for consistency with QObject one, use the duplicate() method for deep copying a qps::Properties object.
 *
 * Actual limitations (2015/06/20):
 *   - Dynamic properties name can't be changed after creation.
 *   - Static Enum properties are not supported (use the addEnumProperty() method instead).
 *
 * Plus the actual QObject static properties limitations (2015/06/20):
 *  - Static properties defined with the Q_PROPERTY macro can't be suppressed.
 *  - Static properties defined with the Q_PROPERTY name can't be dynamically changed.
 *
 * \note 20151029 internals: when using a Q_PROPERTY with a notify signal, its notify signal name must finish with "Changed", or QuickProperties will not be able
 *                           to detect "property elements" changes.
 * \nosubgrouping
 */
class Properties : public QAbstractListModel
{
    /*! \name Properties Object Management *///--------------------------------
    //@{
    Q_OBJECT
public:
    explicit Properties( QObject* parent = 0, QObject* target = 0 );
    virtual ~Properties( );
private:
    //! Copy constructor is disabled for consistency with QObject one, use the duplicate() method for deep copying a qps::Properties object.
    Q_DISABLE_COPY( Properties )
public:
    /*! \brief Create a deep copy of this qan::Properties object and return it.
     *
     *  \note 20151029 limitation: static property defined wit Q_PROPERTY macro will be duplicated as dynamic properties.
     *  \param  copyParent set the optional copy parent (if nullptr, this properties parent will not be used as the copy parent)->
     *  \return nullptr if copy fails.*/
    Q_INVOKABLE qps::Properties*    duplicate( QObject* copyParent = 0 );
    /*! \brief Deep copy this qan::Properties object to destination.
     *
     *  \note 20151029 limitation: static property defined wit Q_PROPERTY macro will be ducplicated as dynamic properties.
     *  \return false if copy fails.*/
    Q_INVOKABLE bool                duplicateTo( qps::Properties* destination );

public:
    //! FIXME.
    Q_PROPERTY( QObject* target READ getTarget WRITE setTarget NOTIFY targetChanged )
    QObject*    getTarget( ) const { return _target; }
    void        setTarget( QObject* target );
protected:
    //! \copydoc qps::Properties::target
    QObject*    _target = nullptr;
signals:
    //! \copydoc qps::Properties::target
    void        targetChanged( );
protected slots:
    // FIXME: handle target destroyed signal...
    void        targetDestroyed( QObject* target );
    //@}
    //-------------------------------------------------------------------------


    /*! \name Property Creation Management *///--------------------------------
    //@{
public:
    /*! Test if a property with the given name exist in this QObject static or dynamic properties.
     * \return true if a property exists, false otherwise.
     */
    Q_INVOKABLE bool    hasProperty( QString propertyName );
    //! Add a QObject dynamic property, property changes will be monitored and reflected in this properties models (PropertyGraphModel and PropertiesModel).
    /*!
     * \param name  property desired name.
     * \param value property type will be the same as the one returned by value QVariant type() method, value can be invalid.
     * \return true if property creation succeed, false otherwise (eg a property with the given name already exists).
     */
    Q_INVOKABLE bool    addDynamicProperty( QString name, QVariant value );
    //! Shortcut for QObject::setProperty() method with a QString instead of a char* for property name.
    Q_INVOKABLE void    setProperty( QString propertyName, QVariant value ) { QObject::setProperty( propertyName.toLatin1( ), value ); }
    //! Return property with name 'propertyName' current value.
    Q_INVOKABLE virtual QVariant    getProperty( QString propertyName ) { return property( propertyName.toLatin1( ) ); }
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

    //! Add a dynamic enumeration property.
    /*!
     * \param name  enum property desired name.
     * \param value property type will be the same as the one returned by value QVariant type() method, value can be invalid.
     * \param valueLabels list of string containging the possible enum ordered by value (for exemple for Qt::PenStyle enum,
     * it would be QStringList penStyles; penStyles << "No line" << "Solid line" << "Dash line" << "Dot line" << "Dash Dot line" << "Dash Dot Dot line";
     * \return true if property creation succeed, false otherwise (eg a property with the given name already exists).
     */
    bool            addEnumProperty( QString name, QVariant value, QStringList valueLabels );
    /*! \brief When using either PropertiesModel or PropertyGraphModel, ensure to set properties values trought this method instead of QObject::setProperty().
     *
     * Works for both synamic and static properties.
     */
    bool            setProperty( const char* propertyName, QVariant propertyValue );
    //! Return true if the property with given name is a dynamic enum property.
    bool            isEnum( QString propertyName ) const;
    //! Get a dynamic enumeration property possible labels.
    Q_INVOKABLE QStringList         getEnumValueLabels( QString propertyName );
    //! Map an enum property name to its possible value labels (works only for dynamic properties).
    QMap< QString, QStringList >    _enumValueLabels;

public:
    /*! Set the number of static properties that will be kept hidden in this model (for
     * exemple set to 1 in order to hide Qt objectName property).
     *
     * This value default to 1, in order to hide Qt 'objectName' property attached to every qobjects.
     *
     * \param count number of the first count static property that will be hidden by this model.
     */
    void    hideStaticProperties( int count );
protected:
    int     _hiddenStaticPropertiesCount;

public:
    //! Allow construction of child QpsLimit based object in QpsProperties directly in declarative QML code.
    Q_PROPERTY( QQmlListProperty< qps::Limit > limits READ getLimits )
    Q_CLASSINFO( "DefaultProperty", "limits" )
public:
    QQmlListProperty< qps::Limit > getLimits( ) { return QQmlListProperty< qps::Limit >( this, _limits ); }
private:
    QList< qps::Limit* > _limits;

    /*! \brief Return the actual qps::Limit object registered for a given property name.
     *
     *  Since limits are often wth pure declarative code, this method has to search in this
     *  Properties children for a limit with 'propertyName' target. The first lookup might be quite
     *  costly, second call will return a cached result.
     *
     * \return nullptr if no limit has been found. The returned qps::Limit must match the property type (ie for a QString
     *  property, you should expect a qps::StringLimit object is returned.
     */
    qps::Limit*                     getPropertyLimit( QString propertyName );
protected:
    //! Internal cache for qps::Limit* objectf lookup according to their 'propertyName' target attribute.
    QMap< QString, qps::Limit* >    _propertiesLimits;
    //@}
    //-------------------------------------------------------------------------


    /*! \name Interview Interface *///-----------------------------------------
    //@{
public:
    enum PropertyRoles {
        PropertyNameRole = Qt::UserRole + 1,
        PropertyTypeRole,
        PropertyIsEnumRole,
        PropertyDataRole,
        PropertyLimitRole
    };
public:
    //! Return the number of "property elements" in this properties (be carefull, hidden static properties count is substracted from the returned results).
    Q_INVOKABLE int     count( ) { return rowCount( QModelIndex( ) ); }
    Q_INVOKABLE void    resetPropertiesModel( ) { beginResetModel( ); endResetModel( ); }

    virtual int         rowCount( const QModelIndex & parent = QModelIndex( ) ) const;
    virtual QVariant    data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    void                updateProperty( QString propertyName );
protected:
    virtual QHash< int, QByteArray >    roleNames( ) const;
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
    //! Used internally, monitor \c QEvent::DynamicPropertyChange events to detect dynamic property modification.
    virtual bool        event( QEvent* e );
    //@}
    //-------------------------------------------------------------------------
};

//! Properties qDebug() stream operator.
QDebug      operator<<( QDebug dbg, const Properties& p );

} // ::qps

QML_DECLARE_TYPE( qps::Properties )

#endif // qpsProperties_h

