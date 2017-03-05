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
// \file	qpsContainerModelTests.cpp
// \author	benoit@qanava.org
// \date	2016 11 25
//-----------------------------------------------------------------------------

// QuickContainers headers
#include "./qcmTests.h"
#include "../src/qcmContainerModelListReference.h"
#include "../src/qcmContainerModelComposer.h"
#include "./qcmContainerModelTests.h"

//-----------------------------------------------------------------------------
// Static tag dispatching test
//-----------------------------------------------------------------------------

TEST(qpsContainerModel, staticDispatch)
{
    std::cerr << "qcm::ItemDispatcher<int>::type=" << qcm::ItemDispatcherBase::debug_type< qcm::ItemDispatcher<int>::type >() << std::endl;
    bool isNonPtr{ std::is_same< qcm::ItemDispatcher<int>::type, qcm::ItemDispatcherBase::non_ptr_type>::value };
    EXPECT_TRUE( isNonPtr );

    std::cerr << "qcm::ItemDispatcher<int*>::type=" << qcm::ItemDispatcherBase::debug_type< qcm::ItemDispatcher<int*>::type >() << std::endl;
    bool isPtr{ std::is_same< qcm::ItemDispatcher<int*>::type, qcm::ItemDispatcherBase::ptr_type>::value };
    EXPECT_TRUE( isPtr );

    std::cerr << "qcm::ItemDispatcher<QObject>::type=" << qcm::ItemDispatcherBase::debug_type< qcm::ItemDispatcher<QObject>::type >() << std::endl;
    bool isUnsupported{ std::is_same< qcm::ItemDispatcher<QObject>::type, qcm::ItemDispatcherBase::unsupported_type>::value };
    EXPECT_TRUE( isUnsupported );

    std::cerr << "qcm::ItemDispatcher<QObject*>::type=" << qcm::ItemDispatcherBase::debug_type< qcm::ItemDispatcher<QObject*>::type >() << std::endl;
    bool isQObjectPtr{ std::is_same< qcm::ItemDispatcher<QObject*>::type, qcm::ItemDispatcherBase::ptr_qobject_type>::value };
    EXPECT_TRUE( isQObjectPtr );

    std::cerr << "qcm::ItemDispatcher<QPointer<QObject>>::type=" << qcm::ItemDispatcherBase::debug_type< qcm::ItemDispatcher<QPointer<QObject>>::type >() << std::endl;
    bool isQPointer{ std::is_same< qcm::ItemDispatcher<QPointer<QObject>>::type, qcm::ItemDispatcherBase::q_ptr_type>::value };
    EXPECT_TRUE( isQPointer );

    std::cerr << "qcm::ItemDispatcher<std::shared_ptr<int>>::type=" << qcm::ItemDispatcherBase::debug_type< qcm::ItemDispatcher<std::shared_ptr<int>>::type >() << std::endl;
    bool isSharedPtr{ std::is_same< qcm::ItemDispatcher<std::shared_ptr<int>>::type, qcm::ItemDispatcherBase::shared_ptr_type>::value };
    EXPECT_TRUE( isSharedPtr );

    std::cerr << "qcm::ItemDispatcher<std::shared_ptr<QObject>>::type=" << qcm::ItemDispatcherBase::debug_type< qcm::ItemDispatcher<std::shared_ptr<QObject>>::type >() << std::endl;
    bool isSharedQObjectPtr{ std::is_same< qcm::ItemDispatcher<std::shared_ptr<QObject>>::type, qcm::ItemDispatcherBase::shared_ptr_qobject_type>::value };
    EXPECT_TRUE( isSharedQObjectPtr );

    std::cerr << "qcm::ItemDispatcher<std::weak_ptr<int>>::type=" << qcm::ItemDispatcherBase::debug_type< qcm::ItemDispatcher<std::weak_ptr<int>>::type >() << std::endl;
    bool isWeakPtr{ std::is_same< qcm::ItemDispatcher<std::weak_ptr<int>>::type, qcm::ItemDispatcherBase::weak_ptr_type>::value };
    EXPECT_TRUE( isWeakPtr );

    std::cerr << "qcm::ItemDispatcher<std::weak_ptr<QObject>>::type=" << qcm::ItemDispatcherBase::debug_type< qcm::ItemDispatcher<std::weak_ptr<QObject>>::type >() << std::endl;
    bool isWeakQObjectPtr{ std::is_same< qcm::ItemDispatcher<std::weak_ptr<QObject>>::type, qcm::ItemDispatcherBase::weak_ptr_qobject_type>::value };
    EXPECT_TRUE( isWeakQObjectPtr );
}

//-----------------------------------------------------------------------------
// qcm::ContainerModel
//-----------------------------------------------------------------------------

TEST(qpsContainerModel, vectorPodEmpty)
{
    using Ints = qcm::ContainerModel< QVector, int >;
    Ints ints;
    EXPECT_EQ( ints.rowCount(), 0 );
    EXPECT_EQ( ints.getItemCount(), 0 );
}

TEST(qpsContainerModel, vectorQObjectPtrEmpty)
{
    using QObjects = qcm::ContainerModel< QVector, QObject* >;
    QObjects qobjects;
    EXPECT_EQ( qobjects.rowCount(), 0 );
    EXPECT_EQ( qobjects.getItemCount(), 0 );
}

TEST(qpsContainerModel, vectorPodItemCount)
{
    using Ints = qcm::ContainerModel< QVector, int >;
    Ints ints;
    EXPECT_EQ( ints.rowCount(), 0 );
    EXPECT_EQ( ints.getItemCount(), 0 );
    ints.append( 42 );
    ints.append( 43 );
    EXPECT_EQ( ints.rowCount(), 2 );
    EXPECT_EQ( ints.getItemCount(), 2 );
}

TEST(qpsContainerModel, vectorQObjectPtrItemCount)
{
    using QObjects = qcm::ContainerModel< QVector, QObject* >;
    QObjects qobjects;
    EXPECT_EQ( qobjects.rowCount(), 0 );
    EXPECT_EQ( qobjects.getItemCount(), 0 );
    qobjects.append( new QObject{nullptr} );
    qobjects.append( new QObject{nullptr} );
    EXPECT_EQ( qobjects.rowCount(), 2 );
    EXPECT_EQ( qobjects.getItemCount(), 2 );
}

TEST(qpsContainerModel, vectorQObjectPtr)
{
    using QObjects = qcm::ContainerModel< QVector, QObject* >;
    QObjects objects;
    QObject* o = new QObject();
    o->setObjectName( "Hola" );

    // ContainerModel::append()
    objects.append( o );
    objects.append( new QObject() );
    objects.append( new QObject() );
    objects.append( nullptr );  // Shouldn't throw
    EXPECT_EQ( objects.rowCount(), 3 );
    EXPECT_EQ( objects.getItemCount(), 3 );
    EXPECT_EQ( objects.at(0)->objectName(), "Hola" );

    // ContainerModel::remove()
    objects.remove( o );
    objects.remove( nullptr );
    EXPECT_EQ( objects.rowCount(), 2 );

    // ContainerModel::insert()
    objects.insert( new QObject(), 0 );
    EXPECT_EQ( objects.rowCount(), 3 );

    // ContainerModel::indexOf()
    QObject* o2 = new QObject();
    o2->setObjectName( "42" );
    objects.insert( o2, 1 );
    EXPECT_EQ( objects.indexOf( o2 ), 1 );
}

TEST(qpsContainerModel, vectorPtrData)
{
    using Ints = qcm::ContainerModel< QVector, int >;
    Ints ints;
    ints.append( 42 );
    ints.append( 43 );
    ints.append( 44 );
    QVariant v = ints.data( ints.index( 0 ) );
    EXPECT_EQ( v.toString(), "42" );

    using QObjects = qcm::ContainerModel< QVector, QObject* >;
    QObjects objects;
    QObject* o = new QObject();
    objects.append( o );
    EXPECT_EQ( objects.data( objects.index( 0 ), QObjects::ItemDataRole ), QVariant::fromValue<QObject*>( o ) );
    //EXPECT_EQ( objects.data( objects.index( 0 ) ), QVariant( "Item #0" ) );
}

//-----------------------------------------------------------------------------
// Container model list reference tests
// FIXME 20161127: Add strong test support for qcm::\ContainerModelListInterface insert()/remove() methods (for qobject ptr/shared_ptr/weak_ptr).
//-----------------------------------------------------------------------------

TEST(qpsContainerModel, qObjectPtrListReferenceItemAtEmpty)
{
    // Expect invalid return for invalid input arguments
    using QObjects = qcm::ContainerModel< QVector, QObject* >;
    QObjects objects;
    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );
    ASSERT_TRUE( ( listRef->itemAt(-1) == QVariant{} ) );   // EXPECT -1, argument is invalid
    ASSERT_TRUE( ( listRef->itemAt(0) == QVariant{} ) );    // EXPECT invalid return QVariant, index is invalid

    ASSERT_TRUE( ( listRef->at(-1) == nullptr ) );   // EXPECT nullptr, argument is invalid
    ASSERT_TRUE( ( listRef->at(0) == nullptr ) );    // EXPECT nullptr, index is invalid
}

TEST(qpsContainerModel, qObjectPtrListReferenceItemAt)
{
    using QObjects = qcm::ContainerModel< QVector, QObject* >;
    QObjects objects;

    QObject* o1{new QObject()};
    QObject* o2{new QObject()};

    objects.append( o1 );
    objects.append( new QObject() );
    objects.append( o2 );

    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );
    ASSERT_TRUE( ( listRef->itemAt(0) != QVariant{} ) );
    EXPECT_TRUE( ( qvariant_cast<QObject*>( listRef->itemAt(0) ) == o1 ) );
    EXPECT_TRUE( ( qvariant_cast<QObject*>( listRef->itemAt(2) ) == o2 ) );
    ASSERT_TRUE( ( listRef->itemAt(3) == QVariant{} ) );

    ASSERT_TRUE( ( listRef->at(0) != nullptr ) );
    EXPECT_TRUE( ( listRef->at(0) == o1 ) );
    EXPECT_TRUE( ( listRef->at(2) == o2 ) );
    ASSERT_TRUE( ( listRef->at(3) == nullptr ) );
}

TEST(qpsContainerModel, qObjectPtrListReferenceItemIndexEmpty)
{
    // Expect invalid return for invalid input arguments
    using QObjects = qcm::ContainerModel< QVector, QObject* >;
    QObjects objects;
    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );
    ASSERT_TRUE( ( listRef->itemIndex(nullptr) == -1 ) );   // EXPECT -1, argument is invalid
    QObject* o1{new QObject()};
    ASSERT_TRUE( ( listRef->itemIndex(o1) == -1 ) );        // EXPECT -1 return: o1 is not in the container model
}

TEST(qpsContainerModel, qObjectPtrListReferenceItemIndex)
{
    using QObjects = qcm::ContainerModel< QVector, QObject* >;
    QObjects objects;
    QObject* o1{new QObject()};
    QObject* o2{new QObject()};

    objects.append( o1 );
    objects.append( new QObject() );
    objects.append( o2 );

    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );

    ASSERT_TRUE( listRef->itemIndex(nullptr) == -1 );
    ASSERT_TRUE( listRef->itemIndex(o1) == 0 );
    ASSERT_TRUE( listRef->itemIndex(o2) == 2 );
    QObject* o3{new QObject()};
    ASSERT_TRUE( ( listRef->itemIndex(o3) == -1 ) );        // EXPECT -1 return: o3 is not in the container model
}

// std::shared_ptr<QObject> container model
TEST(qpsContainerModel, sharedQObjectListReferenceItemAtEmpty)
{
    // Expect invalid return for invalid input arguments
    using SharedQObjects = qcm::ContainerModel< QVector, std::shared_ptr<QObject> >;
    SharedQObjects objects;
    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );
    ASSERT_TRUE( ( listRef->itemAt(-1) == QVariant{} ) );   // EXPECT -1, argument is invalid
    ASSERT_TRUE( ( listRef->itemAt(0) == QVariant{} ) );    // EXPECT invalid return QVariant, index is invalid

    ASSERT_TRUE( ( listRef->at(-1) == nullptr ) );   // EXPECT nullptr, argument is invalid
    ASSERT_TRUE( ( listRef->at(0) == nullptr ) );    // EXPECT nullptr, index is invalid
}

TEST(qpsContainerModel, sharedQObjectPtrListReferenceItemAt)
{
    using SharedQObjects = qcm::ContainerModel< QVector, std::shared_ptr<QObject> >;
    SharedQObjects objects;
    auto o1{std::make_shared<QObject>()};
    auto o2{std::make_shared<QObject>()};
    objects.append( o1 );
    objects.append( std::make_shared<QObject>() );
    objects.append( o2 );

    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );
    ASSERT_TRUE( ( listRef->itemAt(0) != QVariant{} ) );
    EXPECT_TRUE( ( qvariant_cast<QObject*>( listRef->itemAt(0) ) == o1.get() ) );
    EXPECT_TRUE( ( qvariant_cast<QObject*>( listRef->itemAt(2) ) == o2.get() ) );
    ASSERT_TRUE( ( listRef->itemAt(3) == QVariant{} ) );

    ASSERT_TRUE( ( listRef->at(0) != nullptr ) );
    EXPECT_TRUE( ( listRef->at(0) == o1.get() ) );
    EXPECT_TRUE( ( listRef->at(2) == o2.get() ) );
    ASSERT_TRUE( ( listRef->at(3) == nullptr ) );
}

TEST(qpsContainerModel, sharedQObjectPtrListReferenceItemIndexEmpty)
{
    // Expect invalid return for invalid input arguments
    using SharedQObjects = qcm::ContainerModel< QVector, std::shared_ptr<QObject> >;
    SharedQObjects objects;
    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );
    ASSERT_TRUE( ( listRef->itemIndex(nullptr) == -1 ) );   // EXPECT -1, argument is invalid
    QObject* o1{new QObject()};
    ASSERT_TRUE( ( listRef->itemIndex(o1) == -1 ) );        // EXPECT -1 return: o1 is not in the container model
}

TEST(qpsContainerModel, sharedQObjectPtrListReferenceItemIndex)
{
    using SharedQObjects = qcm::ContainerModel< QVector, std::shared_ptr<QObject> >;
    SharedQObjects objects;
    auto o1{std::make_shared<QObject>()};
    auto o2{std::make_shared<QObject>()};
    objects.append( o1 );
    objects.append( std::make_shared<QObject>() );
    objects.append( o2 );

    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );

    ASSERT_TRUE( listRef->itemIndex(nullptr) == -1 );
    ASSERT_TRUE( listRef->itemIndex(o1.get()) == 0 );
    ASSERT_TRUE( listRef->itemIndex(o2.get()) == 2 );
    QObject* o3{new QObject()};
    ASSERT_TRUE( ( listRef->itemIndex(o3) == -1 ) );        // EXPECT -1 return: o3 is not in the container model
}

// std::weak_ptr<QObject> container model
TEST(qpsContainerModel, weakQObjectListReferenceItemAtEmpty)
{
    // Expect invalid return for invalid input arguments
    using WeakQObjects = qcm::ContainerModel< QVector, std::weak_ptr<QObject> >;
    WeakQObjects objects;
    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );
    ASSERT_TRUE( ( listRef->itemAt(-1) == QVariant{} ) );   // EXPECT -1, argument is invalid
    ASSERT_TRUE( ( listRef->itemAt(0) == QVariant{} ) );    // EXPECT invalid return QVariant, index is invalid

    ASSERT_TRUE( ( listRef->at(-1) == nullptr ) );   // EXPECT nullptr, argument is invalid
    ASSERT_TRUE( ( listRef->at(0) == nullptr ) );    // EXPECT nullptr, index is invalid
}

using WeakQObject = std::weak_ptr<QObject>;
Q_DECLARE_METATYPE( WeakQObject );

TEST(qpsContainerModel, weakQObjectPtrListReferenceItemAt)
{
    using WeakQObject = std::weak_ptr<QObject>;
    using WeakQObjects = qcm::ContainerModel< QVector, std::weak_ptr<QObject> >;
    WeakQObjects objects;
    auto o1Ptr{std::make_shared<QObject>()};
    auto o1{WeakQObject{o1Ptr}};
    auto o2Ptr{std::make_shared<QObject>()};
    auto o2{WeakQObject{o2Ptr}};
    objects.append( o1 );
    auto o3Ptr{std::make_shared<QObject>()};
    objects.append( WeakQObject{o3Ptr} );
    objects.append( o2 );

    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );
    ASSERT_TRUE( ( listRef->itemAt(0) != QVariant{} ) );
    EXPECT_TRUE( ( qvariant_cast<QObject*>( listRef->itemAt(0) ) == o1.lock().get() ) );
    EXPECT_TRUE( ( qvariant_cast<QObject*>( listRef->itemAt(2) ) == o2.lock().get() ) );
    ASSERT_TRUE( ( listRef->itemAt(3) == QVariant{} ) );

    ASSERT_TRUE( ( listRef->at(0) != nullptr ) );
    EXPECT_TRUE( ( listRef->at(0) == o1.lock().get() ) );
    EXPECT_TRUE( ( listRef->at(2) == o2.lock().get() ) );
    ASSERT_TRUE( ( listRef->at(3) == nullptr ) );
}

TEST(qpsContainerModel, weakQObjectPtrListReferenceItemIndexEmpty)
{
    // Expect invalid return for invalid input arguments
    using WeakQObjects = qcm::ContainerModel< QVector, std::weak_ptr<QObject> >;
    WeakQObjects objects;
    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );
    ASSERT_TRUE( ( listRef->itemIndex(nullptr) == -1 ) );   // EXPECT -1, argument is invalid
    QObject* o1{new QObject()};
    ASSERT_TRUE( ( listRef->itemIndex(o1) == -1 ) );        // EXPECT -1 return: o1 is not in the container model
}

TEST(qpsContainerModel, weakQObjectPtrListReferenceItemIndex)
{
    using WeakQObject = std::weak_ptr<QObject>;
    using WeakQObjects = qcm::ContainerModel< QVector, std::weak_ptr<QObject> >;
    WeakQObjects objects;
    auto o1Ptr{std::make_shared<QObject>()};
    auto o1{WeakQObject{o1Ptr}};
    auto o2Ptr{std::make_shared<QObject>()};
    auto o2{WeakQObject{o2Ptr}};
    objects.append( o1 );
    auto o3Ptr{std::make_shared<QObject>()};
    objects.append( WeakQObject{o3Ptr} );
    objects.append( o2 );

    auto listRef = objects.getListReference();
    ASSERT_TRUE( listRef != nullptr );

    ASSERT_TRUE( listRef->itemIndex(nullptr) == -1 );
    ASSERT_TRUE( listRef->itemIndex(o1.lock().get()) == 0 );
    ASSERT_TRUE( listRef->itemIndex(o2.lock().get()) == 2 );
    QObject* o3{new QObject()};
    ASSERT_TRUE( ( listRef->itemIndex(o3) == -1 ) );        // EXPECT -1 return: o3 is not in the container model
}

//-----------------------------------------------------------------------------
// Container model composer tests
//-----------------------------------------------------------------------------

TEST(qpsContainerModelComposer, m1VectorQObject)
{
    using QObjectsVector = qcm::ContainerModel< QVector, QObject* >;
    QObjectsVector m1;
    auto m1o1{new QObject()}, m1o2{new QObject()}, m1o3{new QObject()};
    m1.append( m1o1 ); m1.append( m1o2 ); m1.append( m1o3 );

    qcm::ContainerModelComposer<QVector, QObject*> composer;
    composer.setM1(m1);

    ASSERT_TRUE( composer.size() == 3 );
    ASSERT_TRUE( composer.getItemCount() == 3 );
    ASSERT_TRUE( composer.rowCount() == 3 );
    ASSERT_TRUE( ( composer.getListReference()->itemIndex(m1o3) == 2 ) );        // EXPECT -1 return: o3 is not in the container model
    ASSERT_TRUE( ( composer.getListReference()->itemIndex(nullptr) == -1 ) );        // EXPECT -1 return: o3 is not in the container model
}

TEST(qpsContainerModelComposer, m2VectorQObject)
{
    using QObjectsVector = qcm::ContainerModel< QVector, QObject* >;
    QObjectsVector m2;
    auto m2o1{new QObject()}; auto m2o2{new QObject()}; auto m2o3{new QObject()};
    m2.append( m2o1 ); m2.append( m2o2 ); m2.append( m2o3 );

    qcm::ContainerModelComposer<QVector, QObject*> composer;
    composer.setM2(m2);

    ASSERT_TRUE( composer.size() == 3 );
    ASSERT_TRUE( composer.getItemCount() == 3 );
    ASSERT_TRUE( composer.rowCount() == 3 );
    ASSERT_TRUE( ( composer.getListReference()->itemIndex(m2o3) == 2 ) );        // EXPECT -1 return: o3 is not in the container model
    ASSERT_TRUE( ( composer.getListReference()->itemIndex(nullptr) == -1 ) );        // EXPECT -1 return: o3 is not in the container model
}

TEST(qpsContainerModelComposer, m1m2VectorQObject)
{
    using QObjectsVector = qcm::ContainerModel< QVector, QObject* >;
    QObjectsVector m1;
    auto m1o1{new QObject()}, m1o2{new QObject()}, m1o3{new QObject()};
    m1.append( m1o1 ); m1.append( m1o2 ); m1.append( m1o3 );
    QObjectsVector m2;
    auto m2o1{new QObject()}; auto m2o2{new QObject()}; auto m2o3{new QObject()};
    m2.append( m2o1 ); m2.append( m2o2 ); m2.append( m2o3 );

    // Test creating a model and setting m1, then m2
    qcm::ContainerModelComposer<QVector, QObject*> composer;
    composer.setM1(m1);
    composer.setM2(m2);

    ASSERT_TRUE( composer.size() == 6 );
    ASSERT_TRUE( composer.getItemCount() == 6 );
    ASSERT_TRUE( composer.rowCount() == 6 );
    ASSERT_TRUE( ( composer.getListReference()->itemIndex(m1o3) == 2 ) );
    ASSERT_TRUE( ( composer.getListReference()->itemIndex(nullptr) == -1 ) );

    // Test creating a model and setting m2 then m1
    qcm::ContainerModelComposer<QVector, QObject*> composer2;
    composer2.setM2(m2);
    composer2.setM1(m1);

    ASSERT_TRUE( composer2.size() == 6 );
    ASSERT_TRUE( composer2.getItemCount() == 6 );
    ASSERT_TRUE( composer2.rowCount() == 6 );
    ASSERT_TRUE( ( composer2.getListReference()->itemIndex(m2o3) == 5 ) );
    ASSERT_TRUE( ( composer2.getListReference()->itemIndex(nullptr) == -1 ) );
}


// Composition of Heterogeneous containers of weak_ptr on QObject
using WeakQA = std::weak_ptr<QA>;
Q_DECLARE_METATYPE( WeakQA );

using WeakQB = std::weak_ptr<QB>;
Q_DECLARE_METATYPE( WeakQB );

TEST(qpsContainerModel, heterogeneousWeakQObjects)
{
    using WeakQAs = qcm::ContainerModel< QVector, WeakQA >;
    WeakQAs m1;
    auto m1o1Ptr{std::make_shared<QA>()}; auto m1o1{WeakQA{m1o1Ptr}};
    auto m1o2Ptr{std::make_shared<QA>()}; auto m1o2{WeakQA{m1o2Ptr}};
    auto m1o3Ptr{std::make_shared<QA>()}; auto m1o3{WeakQA{m1o3Ptr}};
    m1.append( m1o1 ); m1.append( m1o3 ); m1.append( m1o2 );

    using WeakQBs = qcm::ContainerModel< QVector, WeakQB >;
    WeakQBs m2;
    auto m2o1Ptr{std::make_shared<QB>()}; auto m2o1{WeakQB{m2o1Ptr}};
    auto m2o2Ptr{std::make_shared<QB>()}; auto m2o2{WeakQB{m2o2Ptr}};
    auto m2o3Ptr{std::make_shared<QB>()}; auto m2o3{WeakQB{m2o3Ptr}};
    m2.append( m2o1 ); m2.append( m2o3 ); m2.append( m2o2 );

    qcm::ContainerModelComposer< QVector, WeakQObject,
                                 QVector, WeakQA,
                                 QVector, WeakQB > m1m2;
    m1m2.setM1(m1);
    m1m2.setM2(m2);
    EXPECT_TRUE( m1m2.at(0).expired() == false);

    EXPECT_TRUE( m1m2.at(3).expired() == false );
    EXPECT_TRUE( m1m2.at(3).lock().get() != nullptr );
    EXPECT_EQ( m1m2.at(3).lock().get(), m2o1.lock().get() );
    EXPECT_EQ( m1m2.at(0).lock().get()->objectName(), QString( "::QA") ); // We have a pointer on QObject wich is a QA
    EXPECT_EQ( m1m2.at(3).lock().get()->objectName(), QString( "::QB") ); // We have a pointer on QObject wich is a QB
}

