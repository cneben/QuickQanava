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
// \file	qcmProperties.cpp
// \author	benoit@destrat.io
// \date	2015 05 01
//-----------------------------------------------------------------------------

// Std headers
#include <cstring>  // std::strcmp

// Qt headers
#include <QtGlobal>
#include <QMetaProperty>
#include <QVariant>
#include <QDynamicPropertyChangeEvent>

// QuickContainers headers
#include "./qcmObjectModel.h"

namespace qcm { // ::qcm

/* Properties Object Management *///-------------------------------------------
ObjectModel::ObjectModel( QObject* parent, QObject* target ) :
    QAbstractListModel{ parent }
{
    setTarget( target );
    monitorStaticProperties( );
}

ObjectModel::~ObjectModel( )
{
    // _limits destroyed via QObject (hum ? check that someday)
}

bool    ObjectModel::duplicateTo( qcm::ObjectModel* destination ) const
{
    if ( destination == nullptr )
        return false;
    if ( getTarget() == nullptr )
        return false;

    // Duplicate static properties
    int staticPropertyCount = getTarget()->metaObject( )->propertyCount( );
    for ( int s = 0; s < staticPropertyCount; s++ ) {
        QMetaProperty metaProperty = getTarget()->metaObject( )->property( s );
        const char* propertyName = metaProperty.name( );
        if ( propertyName == nullptr )
            continue;
        if ( std::strcmp( propertyName, "target" ) == 0 )    // Do not modify destination target
            continue;
        QVariant srcValue = getTarget()->property( propertyName );
        if ( hasProperty( propertyName ) && metaProperty.isWritable( ) )
            destination->setProperty( propertyName, srcValue );
        else
            destination->addDynamicProperty( propertyName, srcValue );
    }

    // Duplicate dynamic properties
    QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
    for ( int d = 0; d < dynamicProperties.size( ); d++ ) {
        QString propertyName = dynamicProperties.at( d );
        QVariant srcValue = getTarget()->property( propertyName.toLatin1( ) );
        if ( hasProperty( propertyName ) )
            destination->setProperty( propertyName, srcValue );
        else
            destination->addDynamicProperty( propertyName, srcValue );
    }
    return true;
}

bool    ObjectModel::operator==( const qcm::ObjectModel& right ) const
{
    if ( getTarget() == nullptr ||
         right.getTarget() == nullptr )
        return false;
    if ( getTarget() == right.getTarget() )
        return true;

    if ( getHiddenStaticPropertiesCount() != right.getHiddenStaticPropertiesCount() )
        return false;   // Fast exit

    int pCount = getTarget()->metaObject( )->propertyCount( );
    for ( int i = getHiddenStaticPropertiesCount(); i < pCount; ++i ) {
        QMetaProperty metaProperty = getTarget()->metaObject( )->property( i );
        QString propertyName( metaProperty.name( ) );
        if ( !right.hasProperty( propertyName ) )
            return false;
        QVariant propertyValue = getTarget()->property( propertyName.toLatin1() );
        QVariant rightPropertyValue = right.getProperty( propertyName );
        if ( propertyValue != rightPropertyValue )
            return false;
    }
    QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
    for ( int d = 0; d < dynamicProperties.size( ); d++ ) {
        QString propertyName = dynamicProperties.at( d );
        if ( !right.hasProperty( propertyName ) )
            return false;
        QVariant propertyValue = getTarget()->property( propertyName.toLatin1( ) );
        if ( propertyValue != right.getProperty( propertyName ) )
            return false;
    }
    return true;
}

void    ObjectModel::resetProperties()
{
    QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
    for ( int d = 0; d < dynamicProperties.size( ); d++ ) {
        QString propertyName = dynamicProperties.at( d );
        if ( getTarget() )
            getTarget()->setProperty( propertyName.toLatin1(), QVariant{} );
        else
            QObject::setProperty( propertyName.toLatin1(), QVariant{} );
    }
}

void    ObjectModel::setTarget( QObject* target )
{
    if ( target == this ) {
        qWarning() << "qcm::ObjectModel::setTarget(): Can't set target to this object model.";
        return;
    }
    if ( target != nullptr &&
         target == _target )    // Don't set the same target multiple times
        return;

    if ( _target != nullptr ) {
        disconnect( target, 0, this, 0 ); // Disconnect everything from target
        removeEventFilter( target );
    }

    _target = target;
    emit targetChanged();

    if ( _target != nullptr ) {  // Monitor target destruction
        connect( _target, &QObject::destroyed, this, &qcm::ObjectModel::targetDestroyed );
        installEventFilter( _target );
    }
    resetPropertiesModel();
}

void    ObjectModel::targetDestroyed( QObject* target )
{
    if ( target == _target )
        setTarget( nullptr );
}
//-----------------------------------------------------------------------------


/* Property Creation Management *///-------------------------------------------
bool    ObjectModel::hasProperty( QString propertyName ) const
{
    if ( getTarget() == nullptr )
        return false;
    int pi = getTarget()->metaObject( )->indexOfProperty( propertyName.toLatin1( ) );
    if ( pi != -1 )
        return true;    // A static property with the searched name exists

    QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
    foreach ( QByteArray dynamicProperty, dynamicProperties )
        if ( QString( dynamicProperty ) == propertyName )
            return true;
    return false;
}

bool    ObjectModel::hasStaticProperty( QString propertyName ) const
{
    if ( getTarget() == nullptr )
        return false;
    return ( getTarget()->metaObject( )->indexOfProperty( propertyName.toLatin1( ) ) != -1 );
}

bool    ObjectModel::staticPropertyIsWritable( QString propertyName ) const
{
    if ( getTarget() == nullptr )
        return false;
    int pi = getTarget()->metaObject( )->indexOfProperty( propertyName.toLatin1( ) );
    if ( pi == -1 )
        return false;
    QMetaProperty mp = getTarget()->metaObject()->property( pi );
    return mp.isWritable();
}

bool    ObjectModel::addDynamicProperty( QString name, QVariant value )
{
    if ( name.isNull( ) || name.isEmpty( ) )
        return false;
    if ( getTarget() == nullptr )
        return false;

    if ( getTarget()->property( name.toLatin1( ) ).isValid( ) )  // Can't create a property if it already exists
        return false;

    beginResetModel( );
    getTarget()->setProperty( name.toLatin1( ), value );
    // Note: property changes will be catch in object event() method when
    //       a dynamic property change event is sent.
    endResetModel( );

    return true;
}

void        ObjectModel::setProperty( QString propertyName, QVariant value )
{
    if ( getTarget() == nullptr )
        return;
    QVariant oldValue = getTarget()->property( propertyName.toLatin1() );
    if ( value != oldValue )
        static_cast<QObject*>( getTarget() )->setProperty( propertyName.toLatin1( ), value );
}

QVariant    ObjectModel::getProperty( int propertyIndex )
{
    if ( getTarget() == nullptr )
        return QVariant();
    QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
    int staticPropertyCount = getTarget()->metaObject( )->propertyCount( )
                                - _hiddenStaticPropertiesCount;
    int propertyCount = getTarget()->metaObject( )->propertyCount( ) + dynamicProperties.size( );
    QString propertyName = QString( );
    if ( propertyIndex >= 0 && propertyIndex < staticPropertyCount )   // Dealing with static properties
    {
        QMetaProperty staticProperty = getTarget()->metaObject( )->property( _hiddenStaticPropertiesCount + propertyIndex );
        propertyName = ( staticProperty.name( ) != nullptr ? QString( staticProperty.name( ) ) : "" );
    }
    else if ( propertyIndex >= staticPropertyCount &&    // Dealing with dynamic properties
         propertyIndex < propertyCount )
    {
        int dynamicPropertyIndex = propertyIndex - staticPropertyCount;
        propertyName = dynamicProperties.at( dynamicPropertyIndex );
    }
    return ( ( !propertyName.isEmpty( ) && !propertyName.isNull( ) ) ?  getTarget()->property( propertyName.toLatin1( ) ) :
                                                                        QVariant( ) );
}

int     ObjectModel::getPropertyIndex( QString propertyName, bool hideStaticProperties )
{
    if ( getTarget() == nullptr )
        return -1;
    int staticIndex = getTarget()->metaObject( )->indexOfProperty( propertyName.toLatin1( ) );
    if ( staticIndex != -1 ) {
        if ( hideStaticProperties )
            return staticIndex - _hiddenStaticPropertiesCount;
        return staticIndex;
    } else {
        QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
        int dynamicPropertyIndex = dynamicProperties.indexOf( propertyName.toLatin1( ) );
        if ( hideStaticProperties )
            return dynamicPropertyIndex + staticIndex - _hiddenStaticPropertiesCount;
        return dynamicPropertyIndex + staticIndex;
    }
    return -1;  // Should never happen
}

/*!
 * \note Be carefull, that method is actually overriden in qcm::ObjectModel, but is not
 * declared virtual in the Qt framework.
 */
bool    ObjectModel::setProperty( const char* name, QVariant value )
{
    if ( getTarget() == nullptr )
        return false;
    if ( name == nullptr )
        return false;
    getTarget()->setProperty( name, value );  // Not: Do not test return value, it could be false even when set value is successfull
    emit propertiesModified( QString( name ) );
    return true;
}
//-----------------------------------------------------------------------------

/* Interview Interface *///----------------------------------------------------
int     ObjectModel::rowCount( const QModelIndex & parent) const
{
    if ( getTarget() == nullptr )
        return false;
    Q_UNUSED( parent );
    return ( getTarget()->metaObject( )->propertyCount( )
             - _hiddenStaticPropertiesCount
             + getTarget()->dynamicPropertyNames( ).size( ) );
}

QVariant ObjectModel::data( const QModelIndex & index, int role ) const
{
    if ( getTarget() == nullptr )
        return QVariant();

    QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
    int staticPropertyCount = getTarget()->metaObject( )->propertyCount( )
                                            - _hiddenStaticPropertiesCount;
    int propertyCount = getTarget()->metaObject( )->propertyCount( ) + dynamicProperties.size( );

    QString propertyName( "" );
    QString propertyType( "" );

    if ( index.row( ) >= 0 && index.row( ) < staticPropertyCount ) {   // Dealing with static properties
        QMetaProperty staticProperty = getTarget()->metaObject( )->property( _hiddenStaticPropertiesCount + index.row( ) );
        propertyName = ( staticProperty.name( ) != nullptr ? QString( staticProperty.name() ) : "" );
        propertyType = ( staticProperty.typeName( ) != nullptr ? QString( staticProperty.typeName() ) : "" );
    }
    else if ( index.row( ) >= staticPropertyCount &&    // Dealing with dynamic properties
         index.row( ) < propertyCount ) {
        int dynamicPropertyIndex = index.row( ) - staticPropertyCount;
        propertyName = dynamicProperties.at( dynamicPropertyIndex );
        propertyType = getTarget()->property( propertyName.toLatin1() ).typeName( );
    }

    if ( role == PropertyTypeRole &&
         !propertyType.isEmpty() )
        return propertyType;
    else if ( role == PropertyNameRole &&
              !propertyName.isEmpty() )
        return propertyName;
    else if ( role == PropertyDataRole )
        return getTarget() != nullptr ? getTarget()->property( propertyName.toLatin1( ) ) : QVariant{};

    return data( index, role );
}

void    ObjectModel::updateProperty( QString property )
{
    if ( getTarget() == nullptr )
        return;
    if ( property.isEmpty() )
        return;
    int propertyIndex = getTarget()->metaObject( )->indexOfProperty( property.toLatin1( ) );
    QModelIndex propertyModelIndex;
    if ( propertyIndex != -1 )  // Update a static property
        propertyModelIndex = index( propertyIndex - _hiddenStaticPropertiesCount );
    else { // Update a dynamic property
        QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
        propertyIndex = dynamicProperties.indexOf( property.toLatin1( ) );
        if ( propertyIndex != -1 )
            propertyModelIndex = index( getTarget()->metaObject( )->propertyCount( ) - _hiddenStaticPropertiesCount + propertyIndex );
    }
    if ( propertyModelIndex.isValid( ) )
        emit dataChanged( propertyModelIndex, propertyModelIndex );
}

QHash< int, QByteArray > ObjectModel::roleNames( ) const
{
    QHash< int, QByteArray > roles;
    roles[ PropertyNameRole ] = "propertyName";
    roles[ PropertyDataRole ] = "propertyData";
    roles[ PropertyTypeRole ] = "propertyType";
    return roles;
}

void    ObjectModel::hideStaticProperties( int count )
{
    if ( getTarget() == nullptr )
        return;
    if ( count <= metaObject( )->propertyCount( ) )
        _hiddenStaticPropertiesCount = count;
}
//-----------------------------------------------------------------------------


/* Property Modification Monitoring *///---------------------------------------
void    ObjectModel::monitorStaticProperties( )
{
    if ( getTarget() == nullptr )
        return;
    int staticPropertyCount = getTarget()->metaObject( )->propertyCount( );
    QMetaMethod propertiesModifiedSlot = getTarget()->metaObject( )->method( metaObject( )->indexOfSlot( "propertyChanged()" ) );
    for ( int p = 0; p < staticPropertyCount; p++ ) {
        QMetaProperty staticProperty = getTarget()->metaObject( )->property( p );
        if ( staticProperty.hasNotifySignal( ) ) {
            QMetaMethod propertyNotifySignal = staticProperty.notifySignal( );
            connect( getTarget(),   propertyNotifySignal,
                     this,          propertiesModifiedSlot, Qt::UniqueConnection );
        }
    }
}

void    ObjectModel::propertyChanged( )
{
    if ( getTarget() == nullptr )
        return;

    // Note 20151029: Here is the magic, get the sender signal name an extract the 'property name'
    int senderSignalId = senderSignalIndex( );
    if ( sender() == getTarget()&&
         senderSignalId != -1 ) {
        QMetaMethod senderSignal = getTarget()->metaObject( )->method( senderSignalId );
        if ( senderSignal.isValid( ) ) {
            QString senderSignalName( senderSignal.name( ) );
            // FIXME 20151029 Document that behaviour: QuickContainers only works with xxxChanged() NOTIFY signals (this is the standard Qt "coding style").
            QStringList propertyNames = senderSignalName.split( QRegExp( "Changed" ) );
            if ( propertyNames.size( ) > 0 ) {
                QString propertyName = propertyNames.at( 0 );
                int propertyIndex = getPropertyIndex( propertyName );
                QModelIndex propertyModelIndex = index( propertyIndex );
                if ( propertyModelIndex.isValid( ) ) {
                    emit dataChanged( propertyModelIndex, propertyModelIndex );
                    emit propertiesModified( propertyName );
                }
            }
        }
    }
}

bool    ObjectModel::eventFilter( QObject *obj, QEvent* e )
{
    if ( getTarget() != nullptr &&
         getTarget() == obj &&
         e->type( ) == QEvent::DynamicPropertyChange ) {
        QDynamicPropertyChangeEvent* propertyChangeEvent = static_cast< QDynamicPropertyChangeEvent* >( e );
        updateProperty( propertyChangeEvent->propertyName( ) );
        emit propertiesModified( propertyChangeEvent->propertyName( ) );
        return false;
    }
    return QObject::eventFilter( obj, e );
}
//-----------------------------------------------------------------------------

QDebug  operator<<( QDebug dbg, const ObjectModel& p )
{
    QDebugStateSaver saver( dbg ); Q_UNUSED( saver );  // Automatically save dbg settings, and restore them at destruction
    if ( p.getTarget() == nullptr )
        return dbg;
    int pCount = p.getTarget()->metaObject( )->propertyCount( );
    for ( int i = 0; i < pCount; ++i ) {
        QMetaProperty metaProperty = p.getTarget()->metaObject( )->property( i );
        const char* propertyName = metaProperty.name( );
        QVariant propertyValue = p.getTarget()->property( propertyName );
        dbg.nospace( ) << "( static " << propertyName << "=" << propertyValue.toString( ) << ") ";
    }
    QList< QByteArray > dynamicProperties = p.getTarget()->dynamicPropertyNames( );
    for ( int d = 0; d < dynamicProperties.size( ); d++ ) {
        QString propertyName = dynamicProperties.at( d );
        QVariant propertyValue = p.getTarget()->property( propertyName.toLatin1( ) );
        dbg.nospace( ) << "( dynamic " << propertyName << "=" << propertyValue.toString( ) << ")";
    }
    dbg.nospace() << "\n";
    return dbg;
}

} // ::qcm


