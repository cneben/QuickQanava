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
// This file is a part of QuickProperties2 library.
//
// \file	qps2Tests.cpp
// \author	benoit@qanava.org
// \date	2016 02 08
//-----------------------------------------------------------------------------

// Qt headers
#include <QScopedPointer>

// QuickProperties2 headers
#include "./qps2Tests.h"
#include "qpsSerializer.h"

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// STD headers
#include <fstream>

// Protobuf v3 headers
#include "quickproperties.pb.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

//-----------------------------------------------------------------------------
// qps::Properties
//-----------------------------------------------------------------------------

TEST(qpsProperties, target)
{
    QObject* o =new QObject();
    QScopedPointer< qps::Properties > p{ new qps::Properties() };
    EXPECT_TRUE( ( p->getTarget() == p.data() ) ) ;
    p->setTarget( o );
    EXPECT_TRUE( ( p->getTarget() == o ) )  ;
    delete o;
    EXPECT_TRUE( ( p->getTarget() == p.data() ) ) ;
}

TEST(qpsProperties, staticPropertyCount)
{
    ObjectStaticProps* o = new ObjectStaticProps();
    QScopedPointer< qps::Properties > p{ new qps::Properties() };
    p->setTarget( o );
    EXPECT_TRUE( p->hasProperty( "dummyReal" ) );
    EXPECT_EQ( p->count(), 2 ); // o: objectName + dummyReal
    delete o;
    EXPECT_EQ( p->count(), 3 ); // p: objectName + _target + _limits
}

TEST(qpsProperties, dynamicPropertyCount)
{
    ObjectStaticProps* o = new ObjectStaticProps();
    QScopedPointer< qps::Properties > p{ new qps::Properties() };
    p->setTarget( o );
    o->setProperty( "test", QVariant( 42 ) );
    EXPECT_EQ( p->count(), 3 ); // o: static(objectName + dummyReal) + dynamic( test )
    EXPECT_TRUE( p->hasProperty( "test" ) );
    delete o;
    EXPECT_EQ( p->count(), 3 ); // p: objectName + _target + _limits
    p->setProperty( "test", QVariant( 42 ) );
    EXPECT_EQ( p->count(), 4 ); // p: static(objectName + _target + _limits) + dynamic( test )
}

//-----------------------------------------------------------------------------
// qps::Properties Serialization
//-----------------------------------------------------------------------------

TEST(qpsProperties, pb3Serialization)
{
    QScopedPointer< Props > p1{ new Props() };
    p1->setStaticReal( 42.42 );
    p1->setStaticBool( true );
    p1->setStaticInt( 42 );
    p1->setStaticString( "42" );
    p1->setStaticColor( QColor( 42, 25, 255, 255 ) );
    p1->setStaticSizeF( QSizeF( 42.42, 42.42 ) );

    // Serialize out a simple object with both static and dynamic properties
    std::ofstream os( "test.qps", std::ios::out | std::ios::trunc | std::ios::binary );
    if ( os ) {
        qps::pb::Properties pbProperties;
        qps::ProtoSerializer::serializeOut( *p1, pbProperties );
        pbProperties.SerializeToOstream( &os );
        if ( os.is_open() )
            os.close();
    }

    // Serialize in what has been serialized out
    QScopedPointer< Props > p2{ new Props() };
    std::ifstream is( "test.qps", std::ios::in | std::ios::binary );
    if ( is ) {
        qps::pb::Properties pbProperties;
        pbProperties.ParseFromIstream( &is );
        qps::ProtoSerializer::serializeIn( pbProperties, *p2 );
        if ( is.is_open() )
            is.close();
    }

    // Compare the initial and serialized in objects
    EXPECT_EQ( p1->getStaticReal(), p2->getStaticReal() ) ;
    EXPECT_EQ( p1->getStaticBool(), p2->getStaticBool() ) ;
    EXPECT_EQ( p1->getStaticInt(), p2->getStaticInt() ) ;
    EXPECT_EQ( p1->getStaticString(), p2->getStaticString() ) ;
    EXPECT_EQ( p1->getStaticColor(), p2->getStaticColor() ) ;
    EXPECT_EQ( p1->getStaticSizeF(), p2->getStaticSizeF() ) ;
}

//-----------------------------------------------------------------------------
// qps::ContainerListModel
//-----------------------------------------------------------------------------

TEST(qpsContainerListModel, valueItem)
{
    using Ints = qps::ContainerListModel< QVector, int >;
    Ints ints;
    ints.append( 42 );
    ints.append( 43 );
    ints.append( 44 );
    EXPECT_EQ( ints.rowCount(), 3 );    
}

TEST(qpsContainerListModel, pointerItem)
{
    using QObjects = qps::ContainerListModel< QVector, QObject* >;
    QObjects objects;
    QObject* o = new QObject();
    o->setObjectName( "Hola" );

    // ContainerListModel::append()
    objects.append( o );
    objects.append( new QObject() );
    objects.append( new QObject() );
    objects.append( nullptr );  // Shouldn't throw
    EXPECT_EQ( objects.rowCount(), 3 );
    EXPECT_EQ( objects.getItemCount(), 3 );
    EXPECT_EQ( objects.at(0)->objectName(), "Hola" );

    // ContainerListModel::remove()
    objects.remove( o );
    objects.remove( nullptr );
    EXPECT_EQ( objects.rowCount(), 2 );

    // ContainerListModel::insert()
    objects.insert( 0, new QObject() );
    EXPECT_EQ( objects.rowCount(), 3 );

    // ContainerListModel::indexOf()
    QObject* o2 = new QObject();
    o2->setObjectName( "42" );
    objects.insert( 1, o2 );
    EXPECT_EQ( objects.indexOf( o2 ), 1 );
}

TEST(qpsContainerListModel, modelData)
{
    using Ints = qps::ContainerListModel< QVector, int >;
    Ints ints;
    ints.append( 42 );
    ints.append( 43 );
    ints.append( 44 );
    QVariant v = ints.data( ints.index( 0 ) );
    EXPECT_EQ( v.toString(), "42" );

    using QObjects = qps::ContainerListModel< QVector, QObject* >;
    QObjects objects;
    QObject* o = new QObject();
    objects.append( o );
    EXPECT_EQ( objects.data( objects.index( 0 ), QObjects::ItemRole ), QVariant::fromValue<QObject*>( o ) );
    //EXPECT_EQ( objects.data( objects.index( 0 ) ), QVariant( "Item #0" ) );
}

