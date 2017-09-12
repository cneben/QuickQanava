/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickContainers library.
//
// \file    qcmContainerModelListReference.h
// \author  benoit@destrat.io
// \date    2016 11 25
//-----------------------------------------------------------------------------

#ifndef qcmContainerModelListReference_h
#define qcmContainerModelListReference_h

// Std headers
#include <memory>

// Qt headers
#include <QObject>
#include <QDebug>
#include <QQmlEngine>   // Q_QML_DECLARE_TYPE, qmlEngine()

namespace qcm { // ::qcm

struct ItemDispatcherBase {
    using unsupported_type          = std::integral_constant<int, 0>;

    using non_ptr_type              = std::integral_constant<int, 1>;
    using ptr_type                  = std::integral_constant<int, 2>;
    using ptr_qobject_type          = std::integral_constant<int, 3>;
    using q_ptr_type                = std::integral_constant<int, 4>;

    using shared_ptr_type           = std::integral_constant<int, 5>;
    using shared_ptr_qobject_type   = std::integral_constant<int, 6>;

    using weak_ptr_type             = std::integral_constant<int, 7>;
    using weak_ptr_qobject_type     = std::integral_constant<int, 8>;

    template <typename T>
    inline static constexpr auto debug_type() -> const char* { return "unsupported"; }
};

template <>
inline auto ItemDispatcherBase::debug_type<ItemDispatcherBase::non_ptr_type>() -> const char* { return "non_ptr_type"; }

template <>
inline auto ItemDispatcherBase::debug_type<ItemDispatcherBase::ptr_type>() -> const char* { return "ptr_type"; }

template <>
inline auto ItemDispatcherBase::debug_type<ItemDispatcherBase::ptr_qobject_type>() -> const char* { return "ptr_qobject_type"; }

template <>
inline auto ItemDispatcherBase::debug_type<ItemDispatcherBase::q_ptr_type>() -> const char* { return "q_ptr_type"; }

template <>
inline auto ItemDispatcherBase::debug_type<ItemDispatcherBase::shared_ptr_type>() -> const char* { return "shared_ptr_type"; }

template <>
inline auto ItemDispatcherBase::debug_type<ItemDispatcherBase::shared_ptr_qobject_type>() -> const char* { return "shared_ptr_qobject_type"; }

template <>
inline auto ItemDispatcherBase::debug_type<ItemDispatcherBase::weak_ptr_type>() -> const char* { return "weak_ptr_type"; }

template <>
inline auto ItemDispatcherBase::debug_type<ItemDispatcherBase::weak_ptr_qobject_type>() -> const char* { return "weak_ptr_qobject_type"; }

template <typename ItemT>
struct ItemDispatcher : public ItemDispatcherBase {
    // QObject or non copy constructible POD are unsupported
    using type = typename std::conditional< std::is_base_of< QObject, ItemT >::value ||
                                            !std::is_copy_constructible<ItemT>::value,
                                                                                ItemDispatcherBase::unsupported_type,
                                                                                ItemDispatcherBase::non_ptr_type
                                          >::type;
};


template <typename ItemT>
struct ItemDispatcher<ItemT*>  : public ItemDispatcherBase {
    using type = typename std::conditional< std::is_base_of< QObject, typename std::remove_pointer< ItemT >::type >::value,
                                                                                ItemDispatcherBase::ptr_qobject_type,
                                                                                ItemDispatcherBase::ptr_type
                                            >::type;
};

template < typename ItemT >
struct ItemDispatcher<QPointer<ItemT>>  : public ItemDispatcherBase {
    using type = ItemDispatcherBase::q_ptr_type;
};

template < typename ItemT >
struct ItemDispatcher<std::shared_ptr<ItemT>>  : public ItemDispatcherBase {
    using type = typename std::conditional< std::is_base_of< QObject, ItemT >::value,
                                                                                ItemDispatcherBase::shared_ptr_qobject_type,
                                                                                ItemDispatcherBase::shared_ptr_type
                                            >::type;
};

template < typename ItemT >
struct ItemDispatcher<std::weak_ptr<ItemT>>  : public ItemDispatcherBase {
    using type = typename std::conditional< std::is_base_of< QObject, ItemT >::value,
                                                                        ItemDispatcherBase::weak_ptr_qobject_type,
                                                                        ItemDispatcherBase::weak_ptr_type
                                            >::type;
};

/*! \brief Reference on an abstrat container model allowing to access a qcm::ContainerModel directly from QML.
 *
 * \code
 * Button {
 *   text: "remove"
 *   onClicked: { testModel.listReference.insert(object) }
 * }
 * ListView {
 *   id: testList
 *   model: testModel
 * }
 * \endcode
 *
 * \note ContainerModelListReference is very simlar in meaning and interface to a read/write QQmlListReference,
 * please refer to Qt documentation.
 */
class ContainerModelListReference : public QObject
{
    Q_OBJECT
public:
    ContainerModelListReference() : QObject{nullptr} {}
    virtual ~ContainerModelListReference() {}
    ContainerModelListReference(const ContainerModelListReference&) = delete;

public:
    Q_INVOKABLE virtual bool        append(QObject *object) const { Q_UNUSED(object); return false; }
    Q_INVOKABLE virtual QObject*    at(int index) const { Q_UNUSED(index); return nullptr; }
    Q_INVOKABLE virtual bool        clear() const { return false; }
    Q_INVOKABLE virtual int         count() const { return 0; }
    Q_INVOKABLE virtual void        remove(QObject *object) const { Q_UNUSED(object); }
    Q_INVOKABLE virtual bool        contains(QObject *object) const { return itemIndex(object) >= 0; }
public:
    //! Return the element at the requested container \c index position.
    Q_INVOKABLE virtual QVariant    itemAt( int index ) { Q_UNUSED(index); return QVariant{}; }
    //! Return index of specific object (works if the underlining ContainerModel use smart pointers).
    Q_INVOKABLE virtual int         itemIndex( QObject* item ) const { Q_UNUSED(item); return 0; }
};

template < class ContainerModel, class Base = qcm::ContainerModelListReference >
class ContainerModelListReferenceImpl : public Base
{
public:
    explicit ContainerModelListReferenceImpl( ContainerModel& containerModel ) :
        Base{},
        _containerModel( &containerModel ) { }
    ContainerModelListReferenceImpl(const ContainerModelListReferenceImpl<ContainerModel, Base>&) = delete;
private:
    QPointer<ContainerModel>    _containerModel{nullptr};

public:
    virtual bool        append(QObject* object) const { return appendImpl( object, typename ItemDispatcher<typename ContainerModel::ItemT>::type{} ); }
    virtual QObject*    at(int index) const override {
        auto item = atImpl( index, typename ItemDispatcher<typename ContainerModel::ItemT>::type{} );
        QQmlEngine::setObjectOwnership(item, QQmlEngine::CppOwnership);
        return item;
    }
    virtual bool        clear() const override { if ( _containerModel ) _containerModel->clear(); return true; }
    virtual int         count() const override { return ( _containerModel ? _containerModel->size() : 0 ); }

    virtual void        remove(QObject *object) const override {
        return removeImpl( object, typename ItemDispatcher<typename ContainerModel::ItemT>::type{} );
    }

    virtual QVariant    itemAt( int index ) override {
        return ( _containerModel ? _containerModel->data( _containerModel->createIndex( index, 0 ), Qt::UserRole + 1 ) : QVariant{} ); // Qt::UserRole + 1 is in fact ContainerModel::ItemRole (ie itemData property exposed to QML).
    }
    virtual int         itemIndex( QObject* item ) const {
        return itemIndexImpl( item, typename ItemDispatcher<typename ContainerModel::ItemT>::type{} );
    }

private:
    inline auto appendImpl( QObject*, ItemDispatcherBase::non_ptr_type )                    const -> bool { return false; }
    inline auto appendImpl( QObject*, ItemDispatcherBase::unsupported_type )                const -> bool { return false; }
    inline auto appendImpl( QObject*, ItemDispatcherBase::ptr_type )                        const -> bool { return false; }
    inline auto appendImpl( QObject* object, ItemDispatcherBase::ptr_qobject_type )         const -> bool {
        if ( _containerModel )
            _containerModel->append(reinterpret_cast<typename ContainerModel::ItemT>(object));
        return true;
    }
    inline auto appendImpl( QObject*, ItemDispatcherBase::shared_ptr_type )                 const ->bool { return false; }
    inline auto appendImpl( QObject* object, ItemDispatcherBase::shared_ptr_qobject_type )  const -> bool {
        (void)object;
        /*if ( _containerModel ) {
            auto itemConcretePtr = qobject_cast<typename ContainerModel::ItemT::element_type*>(object);
            if ( itemConcretePtr ) {    // Do not try to build a new shared_ptr if conversion to concrete type fails
                auto itemPtr = std::make_shared<typename ContainerModel::ItemT::element_type>(itemConcretePtr);
                _containerModel->append(itemPtr); // Calling std::shared_ptr{} ctor with target qobject
            }
        }
        return true;*/
        return false;
    }
    inline auto appendImpl( QObject*, ItemDispatcherBase::weak_ptr_type )                   const -> bool { return false; }
    inline auto appendImpl( QObject*, ItemDispatcherBase::weak_ptr_qobject_type )           const -> bool {
        // Note 20161127: It is conceptually impossible to append a weak_ptr without adding also to master
        // container of shared_ptr (otherwise, it is expired before beeing added!)
        return false;
    }

private:
    inline auto atImpl( int, ItemDispatcherBase::non_ptr_type )             const -> QObject* { return nullptr; }
    inline auto atImpl( int, ItemDispatcherBase::unsupported_type )         const -> QObject* { return nullptr; }
    inline auto atImpl( int, ItemDispatcherBase::ptr_type )                 const -> QObject* { return nullptr; }
    inline auto atImpl( int index, ItemDispatcherBase::ptr_qobject_type )   const -> QObject* {
        return ( _containerModel ? _containerModel->at(index) : nullptr );
    }
    inline auto atImpl( int, ItemDispatcherBase::shared_ptr_type )                  const -> QObject* { return nullptr; }
    inline auto atImpl( int index, ItemDispatcherBase::shared_ptr_qobject_type )    const -> QObject* {
        auto itemPtr = _containerModel ? _containerModel->at(index) : typename ContainerModel::ItemT{};
        return ( itemPtr ? itemPtr.get() : nullptr );
    }
    inline auto atImpl( int, ItemDispatcherBase::weak_ptr_type )                    const -> QObject* { return nullptr; }
    inline auto atImpl( int index, ItemDispatcherBase::weak_ptr_qobject_type )      const -> QObject* {
        auto itemPtr = _containerModel ? _containerModel->at(index) : typename ContainerModel::ItemT{};
        return ( !itemPtr.expired() ? itemPtr.lock().get() : nullptr );
    }

private:
    inline auto removeImpl( QObject*, ItemDispatcherBase::non_ptr_type )                const -> void {}
    inline auto removeImpl( QObject*, ItemDispatcherBase::unsupported_type )            const -> void {}
    inline auto removeImpl( QObject*, ItemDispatcherBase::ptr_type )                    const -> void {}
    inline auto removeImpl( QObject* object, ItemDispatcherBase::ptr_qobject_type )     const -> void {
        if ( _containerModel )
            _containerModel->remove(reinterpret_cast<typename ContainerModel::ItemT>(object));
    }
    inline auto removeImpl( QObject*, ItemDispatcherBase::shared_ptr_type )                 const -> void {}
    inline auto removeImpl( QObject* object, ItemDispatcherBase::shared_ptr_qobject_type )  const -> void {
        if ( _containerModel != nullptr &&
             object != nullptr &&
             _containerModel->getqObjectItemMap().contains( object ) )
                _containerModel->remove( _containerModel->getqObjectItemMap().value( object ) );
    }
    inline auto removeImpl( QObject*, ItemDispatcherBase::weak_ptr_type )                   const -> void {}
    inline auto removeImpl( QObject* object, ItemDispatcherBase::weak_ptr_qobject_type )    const -> void {
        if ( _containerModel != nullptr &&
             object != nullptr &&
             _containerModel->getqObjectItemMap().contains( object ) ) {
            auto objectItem = _containerModel->getqObjectItemMap().value( object );
            _containerModel->remove( _containerModel->getqObjectItemMap().value( object ) );
        }
    }

private:
    inline auto itemIndexImpl( QObject*, ItemDispatcherBase::non_ptr_type )             const -> int { return -1; }
    inline auto itemIndexImpl( QObject*, ItemDispatcherBase::unsupported_type )         const -> int { return -1;  }
    inline auto itemIndexImpl( QObject*, ItemDispatcherBase::ptr_type )                 const -> int { return -1;  }
    inline auto itemIndexImpl( QObject* item, ItemDispatcherBase::ptr_qobject_type )    const -> int {
        return ( item != nullptr &&
                _containerModel != nullptr ? const_cast<const ContainerModel*>(_containerModel.data())->getContainer().indexOf( reinterpret_cast<typename ContainerModel::ItemT>( item ) ) :
                                             -1 );
    }
    inline auto itemIndexImpl( QObject*, ItemDispatcherBase::shared_ptr_type )              const -> int { return -1;  }
    inline auto itemIndexImpl( QObject* item, ItemDispatcherBase::shared_ptr_qobject_type ) const -> int {
        if ( _containerModel == nullptr ||
             item == nullptr )
            return -1;
        if ( _containerModel->getqObjectItemMap().contains( item ) )
            return _containerModel->indexOf( _containerModel->getqObjectItemMap().value( item ) );
        return -1;
    }
    inline auto itemIndexImpl( QObject*, ItemDispatcherBase::weak_ptr_type )                const -> int { return -1; }
    inline auto itemIndexImpl( QObject* item, ItemDispatcherBase::weak_ptr_qobject_type )   const -> int {
        if ( _containerModel == nullptr ||
             item == nullptr )
            return -1;
        if ( _containerModel->getqObjectItemMap().contains( item ) )
            return _containerModel->indexOf( _containerModel->getqObjectItemMap().value( item ) );
        return -1;
    }
};

} // ::qcm

QML_DECLARE_TYPE(qcm::ContainerModelListReference)

#endif // qcmContainerModelListModel_h

