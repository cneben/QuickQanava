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
// \file	qpsProperties.cpp
// \author	benoit@qanava.org
// \date	2015 05 01
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "./qpsProperties.h"

// QT headers
#include <QtGlobal>
#include <QVariant>
#include <QFont>

namespace qps { // ::qps

/* Properties Object Management *///-------------------------------------------
Properties::Properties( QObject* parent, QObject* target ) :
    QAbstractListModel( parent ),
    _target( nullptr ),
    _hiddenStaticPropertiesCount( 0 )
{
    setTarget( target );
    monitorStaticProperties( );
}

Properties::~Properties( )
{
    // _limits destroyed via QObject (hum ? check that someday)
}

qps::Properties*    Properties::duplicate( QObject* copyParent )
{
    Q_UNUSED( copyParent );

/*    qps::Properties* destination = new qps::Properties( copyParent );
    Q_ASSERT( destination != nullptr );
    return ( duplicateTo( destination ) ? destination : nullptr );*/
    return nullptr;
}

bool    Properties::duplicateTo( qps::Properties* destination )
{
    if ( getTarget() == nullptr )
        return false;

    // Duplicate static properties
    int staticPropertyCount = getTarget()->metaObject( )->propertyCount( );
    for ( int s = 0; s < staticPropertyCount; s++ )
    {
        QMetaProperty metaProperty = getTarget()->metaObject( )->property( s );
        const char* propertyName = metaProperty.name( );
        QVariant srcValue = getTarget()->property( propertyName );
        if ( hasProperty( propertyName ) && metaProperty.isWritable( ) )
            destination->setProperty( propertyName, srcValue );
        else
            destination->addDynamicProperty( propertyName, srcValue );
    }

    // Duplicate dynamic properties
    QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
    for ( int d = 0; d < dynamicProperties.size( ); d++ )
    {
        QString propertyName = dynamicProperties.at( d );
        QVariant srcValue = getTarget()->property( propertyName.toLatin1( ) );
        //if ( isEnum( propertyName ) )
        //    destination->addEnumProperty( propertyName, srcValue, getEnumValueLabels( propertyName ) );
        //else
        {
            if ( hasProperty( propertyName ) )
                destination->setProperty( propertyName, srcValue );
            else
                destination->addDynamicProperty( propertyName, srcValue );
        }
    }
    return true;
}

void    Properties::setTarget( QObject* target )
{
    if ( target != nullptr &&
         target == _target )    // Don't set the same target multiple times
        return;
    if ( target != nullptr &&
         target != this )
        disconnect( target, 0, this, 0 ); // Disconnect everything from target

    // Use given target or this if no target is specified
    _target = ( target == nullptr ? this : target );
    emit targetChanged();

    if ( _target != nullptr &&
         target != this     )   // Monitor target destruction
        connect( _target, &QObject::destroyed, this, &qps::Properties::targetDestroyed );

    // FIXME v2
    // Force model update
}

void    Properties::targetDestroyed( QObject* target )
{
    if ( target == _target &&
         target != this )
        setTarget( nullptr );
}
//-----------------------------------------------------------------------------


/* Property Creation Management *///-------------------------------------------
bool    Properties::hasProperty( QString propertyName )
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

bool    Properties::addDynamicProperty( QString name, QVariant value )
{
    if ( name.isNull( ) || name.isEmpty( ) )
        return false;
    if ( getTarget() == nullptr )
        return false;

    if ( getTarget()->property( name.toLatin1( ) ).isValid( ) )  // Can't create a property if it already exists
        return false;

    beginResetModel( );
    getTarget()->setProperty( name.toLatin1( ), value );
    int pi = getTarget()->metaObject( )->indexOfProperty( name.toLatin1( ) );
    if ( pi != -1 )
    {
        endResetModel( );
        return true;
    }

    // Note: property changes will be catch in object event() method when
    //       a dynamic property change event is sent.
    endResetModel( );

    return false;   // Creation fails
}

QVariant    Properties::getProperty( int propertyIndex )
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

int     Properties::getPropertyIndex( QString propertyName, bool hideStaticProperties )
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

bool    Properties::addEnumProperty( QString name, QVariant value, QStringList valueLabels )
{
    if ( getTarget() == nullptr )
        return false;
    // FIXME 20150620: Test if the property isn't already existing (both dynamic and enum...)
    getTarget()->setProperty( name.toLatin1( ), value );
    _enumValueLabels.insert( name, valueLabels );
    return true;
}

bool    Properties::isEnum( QString propertyName ) const
{
    return _enumValueLabels.contains( propertyName );
}

QStringList Properties::getEnumValueLabels( QString propertyName )
{
    return _enumValueLabels.value( propertyName );
}

/*!
 * \note Be carefull, that method is actually overriden in qps::Properties, but is not
 * declared virtual in the Qt framework.
 */
bool    Properties::setProperty( const char* name, QVariant value )
{
    if ( getTarget() == nullptr )
        return false;
    Q_ASSERT( name != nullptr );
    getTarget()->setProperty( name, value );  // Not: Do not test return value, it could be false even when set value is successfull
    emit propertiesModified( QString( name ) );
    return true;
}
//-----------------------------------------------------------------------------


/* Interview Interface *///----------------------------------------------------
int Properties::rowCount( const QModelIndex & parent) const
{
    if ( getTarget() == nullptr )
        return false;
    Q_UNUSED( parent );
    return ( getTarget()->metaObject( )->propertyCount( )
             - _hiddenStaticPropertiesCount
             + getTarget()->dynamicPropertyNames( ).size( ) );
}

QVariant Properties::data( const QModelIndex & index, int role ) const
{
    if ( getTarget() == nullptr )
        return QVariant();

    QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
    int staticPropertyCount = getTarget()->metaObject( )->propertyCount( )
                                            - _hiddenStaticPropertiesCount;
    int propertyCount = getTarget()->metaObject( )->propertyCount( ) + dynamicProperties.size( );

    QString propertyName( "" );
    QString propertyType( "" );
    bool    propertyIsEnum( false );

    if ( index.row( ) >= 0 && index.row( ) < staticPropertyCount )   // Dealing with static properties
    {
        QMetaProperty staticProperty = getTarget()->metaObject( )->property( _hiddenStaticPropertiesCount + index.row( ) );
        propertyName = ( staticProperty.name( ) != nullptr ? QString( staticProperty.name( ) ) : "" );
        propertyType = ( staticProperty.typeName( ) != nullptr ? QString( staticProperty.typeName( ) ) : "" );
    }
    else if ( index.row( ) >= staticPropertyCount &&    // Dealing with dynamic properties
         index.row( ) < propertyCount )
    {
        int dynamicPropertyIndex = index.row( ) - staticPropertyCount;
        propertyName = dynamicProperties.at( dynamicPropertyIndex );
        propertyType = getTarget()->property( propertyName.toLatin1( ) ).typeName( );
        propertyIsEnum = isEnum( propertyName );
    }

    if ( !propertyName.isEmpty( ) && !propertyType.isEmpty( ) )
    {
        if ( role == PropertyNameRole )
            return propertyName;
        else if ( role == PropertyTypeRole )
            return propertyType;
        else if ( role == PropertyIsEnumRole )
            return propertyIsEnum;
        else if ( role == PropertyDataRole )
            return property( propertyName.toLatin1( ) );
        else if ( role == PropertyLimitRole )
            return QVariant::fromValue< qps::Limit* >( const_cast< qps::Properties* >( this )->getPropertyLimit( propertyName ) ); // Return the corresponding limit object for the requested property
    }

    return data( index, role );
}

void    Properties::updateProperty( QString property )
{
    if ( getTarget() == nullptr )
        return;
    if ( property.isEmpty() )
        return;
    int propertyIndex = getTarget()->metaObject( )->indexOfProperty( property.toLatin1( ) );
    QModelIndex propertyModelIndex;
    if ( propertyIndex != -1 )  // Update a static property
        propertyModelIndex = index( propertyIndex - _hiddenStaticPropertiesCount );
    else
    {       // Updating a dynamic property
        QList< QByteArray > dynamicProperties = getTarget()->dynamicPropertyNames( );
        propertyIndex = dynamicProperties.indexOf( property.toLatin1( ) );
        if ( propertyIndex != -1 )
            propertyModelIndex = index( metaObject( )->propertyCount( ) - _hiddenStaticPropertiesCount + propertyIndex );
    }
    if ( propertyModelIndex.isValid( ) )
        emit dataChanged( propertyModelIndex, propertyModelIndex );
}

QHash< int, QByteArray > Properties::roleNames( ) const
{
    QHash< int, QByteArray > roles;
    roles[ PropertyNameRole ] = "propertyName";
    roles[ PropertyTypeRole ] = "propertyType";
    roles[ PropertyIsEnumRole ] = "propertyIsEnum";
    roles[ PropertyDataRole ] = "propertyData";
    roles[ PropertyLimitRole ] = "propertyLimit";
    return roles;
}

void    Properties::hideStaticProperties( int count )
{
    if ( getTarget() == nullptr )
        return;
    if ( count <= metaObject( )->propertyCount( ) )
        _hiddenStaticPropertiesCount = count;
}

qps::Limit* Properties::getPropertyLimit( QString propertyName )
{
    if ( getTarget() == nullptr )
        return nullptr;
    if ( propertyName.isEmpty() )
        return nullptr;

    if ( _propertiesLimits.contains( propertyName ) )       // Look for a cached result
        return _propertiesLimits.value( propertyName );

    // Otherwise, look in 'limits' default property for childs object of this QpsProperties object
    foreach ( qps::Limit* l, _limits )
        if ( l->getTarget( ) == propertyName ) {
            _propertiesLimits.insert( propertyName, l );    // Cache the lookup
            return l;
        }

            // Otherwise, look in properties children for a qps::Limit children with a target corresponding to propertyName
    foreach ( QObject* c, children( ) )
        if ( c->inherits( "qps::Limit" ) ) {
            qps::Limit* l = qobject_cast< qps::Limit* >( c );
            if ( l->getTarget( ) == propertyName ) {
                _propertiesLimits.insert( propertyName, l );
                return l;
            }
        }
    return nullptr;
}
//-----------------------------------------------------------------------------


/* Property Modification Monitoring *///---------------------------------------
void    Properties::monitorStaticProperties( )
{
    if ( getTarget() == nullptr )
        return;
    int staticPropertyCount = getTarget()->metaObject( )->propertyCount( );
    QMetaMethod propertiesModifiedSlot = getTarget()->metaObject( )->method( metaObject( )->indexOfSlot( "propertyChanged()" ) );
    for ( int p = 0; p < staticPropertyCount; p++ )
    {
        QMetaProperty staticProperty = getTarget()->metaObject( )->property( p );
        if ( staticProperty.hasNotifySignal( ) )
        {
            QMetaMethod propertyNotifySignal = staticProperty.notifySignal( );
            connect( this, propertyNotifySignal, this, propertiesModifiedSlot, Qt::UniqueConnection );
        }
    }
}

void    Properties::propertyChanged( )
{
    if ( getTarget() == nullptr )
        return;

    // Note 20151029: Here is the magic, get the sender signal name an extract the 'property name'
    int senderSignalId = senderSignalIndex( );

    // FIXME: 20160208 BROKEN v2

    if ( sender( ) == this && senderSignalId != -1 )
    {
        QMetaMethod senderSignal = metaObject( )->method( senderSignalId );
        if ( senderSignal.isValid( ) )
        {
            QString senderSignalName( senderSignal.name( ) );
            // FIXME 20151029 Document that behaviour: QuickProperties only works with xxxChanged() NOTIFY signals (this is the standard Qt "coding style").
            QStringList propertyNames = senderSignalName.split( QRegExp( "Changed" ) );
            if ( propertyNames.size( ) > 0 )
            {
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

bool    Properties::event( QEvent* e )
{
    // FIXME v2: Install an event filter on target instead of looking this object events...
    /*if ( e->type( ) == QEvent::DynamicPropertyChange )
    {
        QDynamicPropertyChangeEvent* propertyChangeEvent = static_cast< QDynamicPropertyChangeEvent* >( e );
        updateProperty( propertyChangeEvent->propertyName( ) );
        emit propertiesModified( propertyChangeEvent->propertyName( ) );
    }*/
    return QObject::event( e );
}
//-----------------------------------------------------------------------------

QDebug  operator<<( QDebug dbg, const Properties& p )
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

} // ::qps


