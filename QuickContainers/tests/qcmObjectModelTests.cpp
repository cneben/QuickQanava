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
// This file is a part of QuickContainers library.
//
// \file	qcmObjectModelTests.cpp
// \author	benoit@qanava.org
// \date	2016 02 08
//-----------------------------------------------------------------------------

// QuickContainers headers
#include "./qcmTests.h"

//-----------------------------------------------------------------------------
// qcm::ObjectModel
//-----------------------------------------------------------------------------

TEST(qcmObjectModel, empty)
{
    qcm::ObjectModel m;
    EXPECT_TRUE( ( m.getTarget() == nullptr ) ) ;
}

TEST(qcmObjectModel, target)
{
    const auto o = new QObject{};
    qcm::ObjectModel m;
    EXPECT_TRUE( ( m.getTarget() == nullptr ) ) ;
    m.setTarget( o );
    EXPECT_TRUE( ( m.getTarget() == o ) )  ;
    delete o;
    EXPECT_TRUE( ( m.getTarget() == nullptr ) ) ;
}

TEST(qcmObjectModel, staticPropertyCount)
{
    ObjectStaticProps* o = new ObjectStaticProps();
    qcm::ObjectModel m;
    m.setTarget( o );
    EXPECT_TRUE( m.hasProperty( "dummyReal" ) );
    EXPECT_EQ( m.count(), 2 ); // o: objectName + dummyReal
    delete o;
    EXPECT_EQ( m.count(), 0 ); // target is nullptr
}

TEST(qcmObjectModel, dynamicPropertyCount)
{
    ObjectStaticProps* o = new ObjectStaticProps();
    qcm::ObjectModel m;
    m.setTarget( o );
    o->setProperty( "test", QVariant( 42 ) );
    EXPECT_EQ( m.count(), 3 ); // o: static(objectName + dummyReal) + dynamic( test )
    EXPECT_TRUE( m.hasProperty( "test" ) );
    delete o;
    EXPECT_EQ( m.count(), 0 ); // target is nullptr
}

TEST(qcmObjectModel, resetProperties)
{
    Props p;

    // TEST: Explicitely set a static property, after a reset, expecting its value to be left unchanged
    p.setProperty( "staticReal", QVariant( 42.0 ) );
    EXPECT_TRUE( p.hasProperty( "staticReal" ) );
    EXPECT_EQ( p.getProperty( "staticReal" ), QVariant( 42.0 ) );
    p.resetProperties();
    EXPECT_TRUE( p.hasProperty( "staticReal" ) );
    EXPECT_EQ( p.getProperty( "staticReal" ), QVariant( 42.0 ) );

    // TEST: Explicitely set a dynamic property, after a reset, property should no longer exists
    p.setProperty( "unexisting", QVariant( true ) );
    EXPECT_TRUE( p.hasProperty( "unexisting" ) );
    p.resetProperties();
    EXPECT_FALSE( p.hasProperty( "unexisting" ) );
}

TEST(qcmObjectModel, duplicateTo)
{
    Props p;
    p.setStaticBool( true );
    p.setProperty( "unexisting", QVariant( true ) );

    Props p2;
    p2.setStaticBool( false );

    p.duplicateTo( &p2 );
    EXPECT_TRUE( p2.getStaticBool() );
    EXPECT_TRUE( p2.getProperty( "unexisting" ).toBool() );
    //EXPECT_TRUE( p == p2 );
}
