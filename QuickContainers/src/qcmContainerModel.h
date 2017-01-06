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
// \file    qcmContainerModel.h
// \author  benoit@destrat.io
// \date    2015 06 20
//-----------------------------------------------------------------------------

#ifndef qcmContainerModel_h
#define qcmContainerModel_h

// Qt headers
#include <QQmlEngine>           // QQmlEngine::setObjectOwnership()
#include <QAbstractListModel>

// QuickContainers headers
#include "./qcmObjectmodel.h"
#include "./qcmAbstractContainerModel.h"
#include "./qcmContainerConcept.h"
#include "./qcmContainerModelComposerListReference.h"
#include "./qcmContainerModelListReference.h"

// STD headers
#include <memory>       // shared_ptr, weak_ptr
#include <type_traits>  // integral_constant

QT_BEGIN_NAMESPACE

#ifndef COMPARE_WEAK_PTR
#define COMPARE_WEAK_PTR

//! Compare two std::weak_ptr that must have been checked for expired() and nullptr content (ie use_count()==0).
template < class T >
auto    qcm_compare_weak_ptr( const std::weak_ptr<T>& left, const std::weak_ptr<T>& right ) noexcept -> bool {
    return !left.owner_before( right ) && !right.owner_before( left );
}

template< typename T >
inline bool operator==(const std::weak_ptr<T>& e1, const std::weak_ptr<T>& e2)
{
    return qcm_compare_weak_ptr( e1, e2 );
}

template<typename T>
inline uint qHash(const std::weak_ptr<T>& t, uint seed ) /*noexcept( qHash(t) ) )*/
{
    return qHash( static_cast<T*>( t.lock().get() ), seed );
}
#endif

QT_END_NAMESPACE


namespace qcm { // ::qcm

/*! \brief Expose a standard container such as QList or QVector to a Qt QAbstractItemModel abstract list model easy to use in a QML application.
 *
 * Defining a item model for a QVector container of int or pointers on QObject:
 * \code
 *   using Ints = qcm::ContainerModel< QVector, int >;
 *   Ints ints;
 *   using QObjects = qcm::ContainerModel< QVector, QObject* >;
 *   QObjects qobjects;
 * \endcode
 *
 * \c ints or \c qobjects could then be used in any QML item model view such as ListView, ComboBox (with displayRole set
 * to "itemLabel") or GridView.
 *
 * \note ContainerModel containers could be iterated standard C++11 for(:) keyword. Qt \c for_each "keyword" usually
 * generate compilations errors for non const iterator because container model (volontarily)
 * does not define a copy constructor.
 *
 * Do not forget to declare the type to QML:
 * \code
 * using MyContainer = qcm::ContainerModel<Qlist, QObject>;
 * QML_DECLARE_TYPE( MyContainer )
 * \endcode
 *
 * When a container is defined with standard library smart pointers (currently std::weak_ptr or std::shared_ptr) or QPointer<>, the resulting types
 * must be declared with Q_DECLARE_METATYPE:
 * \code
 * #include <memory>
 *
 * Q_DECLARE_METATYPE( std::shared_ptr<QObject> )
 * using MyContainer = qcm::ContainerModel< QVector, std::shared_ptr<QObject> >;
 *
 * Q_DECLARE_METATYPE( std::weak_ptr<QObject> )
 * using MyContainer = qcm::ContainerModel< QVector, std::weak_ptr<QObject> >;
 * \endcode
 *
 * \note std::unique_ptr and QScoppedPointer are not supported since they are not commonly used in containers. Support
 * for QSharedPointer and QWeakPointer is not planned (but easy to implement) since they are conceptually equivalent
 * to existing standard smart pointers: std::shared_ptr and std::weak_ptr.
 *
 * \note In data intensive application, delay calls to getListReference() (and access to \c listReference property from
 * QML) until your model is fully initialized: \c listReference interface is created on the fly at first
 * getListReference() call and until this object is created, there is no modifications notifications sent trought signals
 * or virtualized calls (still some signals are sent from QAbstractListModel internally).
 *
 * \warning Depending of your build configuration: method indexOf() specialization is O(n) for containers of std::weak_ptr (with
 * n the number of items in the underlining container, even if it is a sorted container defining fast lookup methods). Note
 * 20161125: There is actually a qobjectfriend == operator defined for weak_ptr in ... Qt namespace, be aware of it since it could
 * collide with user code (behaviour for expired weak_ptr is undefined...).
 *
 * \warning data() method for containers of objects inheriting QObject stored in std::shared_ptr<> or
 * std::weak_ptr<> return QVariant variants that could be converted back to QObject* with a qvariant_cast() (this
 * behaviour is "transparent" for the user on the QML side).
 *
 * \warning data() method will return a QVariant with a QObject* value when container is shared_ptr_qobject_type
 * or weak_ptr_qobject_type. In ptr_qobject_type, the returned QVariant will not be QObject but T*. Sometime QML
 * might have trouble making a distinction between a QVariant encoding a QObject* while it should be a concrete
 * QObject* subclass.
 */
template < template<typename...CArgs> class Container, class T >
class ContainerModel : public AbstractContainerModel
{
    /*! \name ContainerModel Object Management *///------------------------
    //@{
public:
    //! Parent must be the qcm::Graph managing nodes exposed in this model.
    ContainerModel( QObject* parent = nullptr ) :
        AbstractContainerModel( parent ) { }
    virtual ~ContainerModel( ) { }
    ContainerModel( const ContainerModel<Container, T>& container ) = delete;

    // Container model list reference need access to createIndex().
    friend qcm::ContainerModelListReference;
    friend qcm::ContainerModelListReferenceImpl< ContainerModel<Container, T>, qcm::ContainerModelListReference >;
    friend qcm::ContainerModelListReferenceImpl< ContainerModel<Container, T>, qcm::ContainerModelComposerListReference >;
    friend qcm::ContainerModelListReferenceImpl< ContainerModel<Container, T> >;
    friend qcm::ContainerModelComposerListReferenceImpl< ContainerModel<Container, T> >;
protected:
    virtual     std::unique_ptr<qcm::ContainerModelListReference>     createListReference() override {
        return std::move( std::make_unique< qcm::ContainerModelListReferenceImpl< ContainerModel<Container, T> > >(*this) );
    }
public:
    using   ItemT = T;
    using   Item_type = T;
    //@}
    //-------------------------------------------------------------------------

    /*! \name STL Interface *///-----------------------------------------------
    //@{
public:
    using   const_pointer   = typename Container< T >::const_pointer;
    using   const_reference = typename Container< T >::const_reference;
    using   size_type       = typename Container< T >::size_type;
    using   value_type      = typename Container< T >::value_type;
    using   const_iterator  = typename Container< T >::const_iterator;
    using   iterator        = typename Container< T >::iterator;

public:
    inline auto    begin( ) const noexcept-> const_iterator { return _container.begin( ); }
    inline auto    end( ) const noexcept-> const_iterator { return _container.end( ); }

    inline auto    begin( ) noexcept -> iterator { return _container.begin( ); }
    inline auto    end( ) noexcept -> iterator { return _container.end( ); }

    inline auto    cbegin( ) const noexcept -> const_iterator { return _container.cbegin( ); }
    inline auto    cend( ) const noexcept -> const_iterator { return _container.cend( ); }

    //! Define a cast operator to const Container<T>&.
    inline operator Container< T >&() noexcept { return _container; }

    //! Define a cast operator to const Container<T>&.
    inline operator const Container< T >&() const noexcept { return _container; }

public:
    inline auto     push_back( const T& value ) -> void { append( value ); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Generic Interface *///-------------------------------------------
    //@{
private:
    inline auto     isNullPtr( T item, ItemDispatcherBase::unsupported_type )        const noexcept -> bool { return isNullPtr(item, ItemDispatcherBase::non_ptr_type{} ); }
    inline auto     isNullPtr( T item, ItemDispatcherBase::non_ptr_type )            const noexcept -> bool { Q_UNUSED(item); return false; }
    inline auto     isNullPtr( T item, ItemDispatcherBase::ptr_type )                const noexcept -> bool { return item == nullptr; }
    inline auto     isNullPtr( T item, ItemDispatcherBase::ptr_qobject_type )        const noexcept -> bool { return isNullPtr( item, ItemDispatcherBase::ptr_type{} ); }
    inline auto     isNullPtr( T item, ItemDispatcherBase::q_ptr_type )              const noexcept -> bool { return isNullPtr( item, ItemDispatcherBase::ptr_type{} ); }
    inline auto     isNullPtr( T item, ItemDispatcherBase::shared_ptr_type )         const noexcept -> bool { return !item; }
    inline auto     isNullPtr( T item, ItemDispatcherBase::shared_ptr_qobject_type ) const noexcept -> bool { return isNullPtr(item, ItemDispatcherBase::shared_ptr_type{} ); }
    inline auto     isNullPtr( T item, ItemDispatcherBase::weak_ptr_type )           const noexcept -> bool { return item.expired(); }
    inline auto     isNullPtr( T item, ItemDispatcherBase::weak_ptr_qobject_type )   const noexcept -> bool { return isNullPtr( item, ItemDispatcherBase::weak_ptr_type{} ); }
private:
    inline auto     getNullT( ItemDispatcherBase::unsupported_type )        const -> T { return T{}; }
    inline auto     getNullT( ItemDispatcherBase::non_ptr_type )            const -> T  { return T{}; }
    inline auto     getNullT( ItemDispatcherBase::ptr_type )                const -> T  { return nullptr; }
    inline auto     getNullT( ItemDispatcherBase::ptr_qobject_type )        const -> T  { return nullptr; }
    inline auto     getNullT( ItemDispatcherBase::q_ptr_type )              const -> T  { return T{}; }
    inline auto     getNullT( ItemDispatcherBase::shared_ptr_type )         const -> T  { return T{}; }
    inline auto     getNullT( ItemDispatcherBase::shared_ptr_qobject_type ) const -> T  { return T{}; }
    inline auto     getNullT( ItemDispatcherBase::weak_ptr_type )           const -> T  { return T{}; }
    inline auto     getNullT( ItemDispatcherBase::weak_ptr_qobject_type )   const -> T  { return T{}; }

public:
    /*! \brief Shortcut to Container<T>::at() with bounding checking.
     *
     * \param i index of the element to access, if i < 0 std::out_of_range is thrown, if i >= container.size(), a nullptr or empty smart pointer is returned.
     */
    auto at( int i ) const noexcept -> T {
        if ( i < 0 )
            return getNullT( typename ItemDispatcher<T>::type{} );
        return atImpl( i, typename ItemDispatcher<T>::type{} );
    }
private:
    inline auto atImpl( int i, ItemDispatcherBase::non_ptr_type )           const -> T {
        return _container.at( i );
    }
    inline auto atImpl( int i, ItemDispatcherBase::unsupported_type )       const -> T { return atImpl( i, ItemDispatcherBase::non_ptr_type{} ); }

    inline auto atImpl( int i, ItemDispatcherBase::ptr_type )               const -> T {
        return ( i < _container.size() ? _container.at( i ) : nullptr );
    }
    inline auto atImpl( int i, ItemDispatcherBase::ptr_qobject_type )       const -> T { return atImpl( i, ItemDispatcherBase::ptr_type{} ); }

    inline auto atImpl( int i, ItemDispatcherBase::shared_ptr_type )        const -> T {
        return ( i < _container.size() ? _container.at( i ) : T{} );
    }
    inline auto atImpl( int i, ItemDispatcherBase::shared_ptr_qobject_type ) const -> T { return atImpl( i, ItemDispatcherBase::shared_ptr_type{} ); }

    inline auto atImpl( int i, ItemDispatcherBase::weak_ptr_type )          const -> T {
        return ( i < _container.size() ? _container.at( i ) : T{} );
    }
    inline auto atImpl( int i, ItemDispatcherBase::weak_ptr_qobject_type )  const -> T { return atImpl( i, ItemDispatcherBase::weak_ptr_type{} ); }

public:
    //! Shortcut to Container<T>::size().
    inline int  size( ) const { return _container.size( ); }

    //! Shortcut to Container<T>::append().
    void        append( T item ) {
        if ( isNullPtr( item, typename ItemDispatcher<T>::type{} ) )
            return;
        beginInsertRows( QModelIndex{}, _container.size( ), _container.size( ) );
        _container.append( item );
        appendImpl( item, typename ItemDispatcher<T>::type{} );
        endInsertRows( );
        emitItemCountChanged();
    }

    //! Shortcut to Container<T>::insert().
    void        insert( T item, int i ) {
        if ( i < 0 ||           // i == 0      === prepend
             i > size() ||      // i == size() === append
             isNullPtr( item, typename ItemDispatcher<T>::type{} ) )
            return;
        beginInsertRows( QModelIndex{}, i, i );
        qcm::container<Container, T>::insert( item, _container, i );
        appendImpl( item, typename ItemDispatcher<T>::type{} );
        endInsertRows( );
        emitItemCountChanged();
    }

private:
    inline auto appendImpl( T, ItemDispatcherBase::unsupported_type ) noexcept  -> void {}
    inline auto appendImpl( T, ItemDispatcherBase::non_ptr_type ) noexcept  -> void {}
    inline auto appendImpl( T item, ItemDispatcherBase::ptr_qobject_type ) noexcept  -> void {
        if ( item != nullptr )
            _qObjectItemMap.insert( item, item );
    }
    inline auto appendImpl( T item, ItemDispatcherBase::q_ptr_type )   noexcept -> void {
        if ( item != nullptr )
            _qObjectItemMap.insert( item.data(), item );
    }
    inline auto appendImpl( T, ItemDispatcherBase::shared_ptr_type ) noexcept  -> void {}
    inline auto appendImpl( T item, ItemDispatcherBase::shared_ptr_qobject_type ) noexcept -> void {
        _qObjectItemMap.insert( item.get(), item );
    }
    inline auto appendImpl( T, ItemDispatcherBase::weak_ptr_type ) noexcept  -> void {}
    inline auto appendImpl( T item, ItemDispatcherBase::weak_ptr_qobject_type )   noexcept -> void {
        if ( !item.expired() )
            _qObjectItemMap.insert( item.lock().get(), item );
    }

public:
    //! Shortcut to Container<T>::remove().
    void        remove( T item ) {
        if ( isNullPtr( item, typename ItemDispatcher<T>::type{} ) )
            return;
        int itemIndex = _container.indexOf( item );
        if ( itemIndex < 0 )
            return;
        beginRemoveRows( QModelIndex{}, itemIndex, itemIndex );
        removeImpl( item, typename ItemDispatcher<T>::type{} );
        _container.removeAll( item );
        endRemoveRows( );
        emitItemCountChanged();
    }
private:
    inline auto removeImpl( T, ItemDispatcherBase::unsupported_type )               -> void {}
    inline auto removeImpl( T, ItemDispatcherBase::non_ptr_type )                   -> void {}
    inline auto removeImpl( T, ItemDispatcherBase::ptr_type )                       -> void {}
    inline auto removeImpl( T item, ItemDispatcherBase::ptr_qobject_type )          -> void {
        if ( item != nullptr ) {
            _qObjectItemMap.remove( item );
            item->disconnect( 0, this, 0 );
        }
    }
    inline auto removeImpl( T item, ItemDispatcherBase::q_ptr_type )                -> void {
        if ( item != nullptr ) {
            _qObjectItemMap.remove( item.data() );
            item->disconnect( 0, this, 0 );
        }
    }
    inline auto removeImpl( T, ItemDispatcherBase::shared_ptr_type )                -> void {}
    inline auto removeImpl( T item, ItemDispatcherBase::shared_ptr_qobject_type )   -> void {
        QObject* qobject = item.get();
        if ( qobject != nullptr ) {
            _qObjectItemMap.remove( item.get() );
            qobject->disconnect( 0, this, 0 );
        }
    }
    inline auto removeImpl( T, ItemDispatcherBase::weak_ptr_type )                  -> void {}
    inline auto removeImpl( T item, ItemDispatcherBase::weak_ptr_qobject_type )     -> void {
        if ( !item.expired() ) {
            QObject* qobject = item.lock().get();
            if ( qobject ) {
                _qObjectItemMap.remove( item.lock().get() );
                qobject->disconnect( 0, this, 0 );
            }
        }
    }

public:
    /*! \brief Remove a given node from the internal node list.
     *
     *  Similar to remove(), defined for QList/QVector compatibility.
     */
    inline  void    removeAll( T item ) noexcept {
        if ( !isNullPtr( item, typename ItemDispatcher<T>::type{} ) )
            remove( item );
    }

    inline  void    clear() noexcept {
        beginResetModel();
        _qObjectItemMap.clear();
        _container.clear( );
        endResetModel();
        emitItemCountChanged();
    }

public:
    /*! \brief Clear the container and optionally call delete on contained objects.
     *
     * Argument \c deleteContent is taken into account only in the following situations:
     * \li Delete will be called if \c deleteContent is true if \c T is a pointer type, or a pointer on QObject.
     * \li If \c T is a smart pointer (either a std::shared_ptr or std::weak_ptr), or a non pointer type (either QObject or POD), delete
     *      won't be called even if \c deleteContent is true.
     *
     * \arg deleteContent if true, delete will eventually be called on each container item before the container is cleared.
     */
    void    clear( bool deleteContent ) {
        beginResetModel();
        clearImpl( deleteContent, typename ItemDispatcher<T>::type{} );
        _qObjectItemMap.clear();
        _container.clear();
        endResetModel();
        emitItemCountChanged();
    }
private:
    inline auto clearImpl( bool deleteContent, ItemDispatcherBase::ptr_type ) -> void {
        if ( deleteContent ) {
            for ( const auto& p: qAsConst(_container) )
                delete p;
        }
    }
    inline auto clearImpl( bool deleteContent, ItemDispatcherBase::ptr_qobject_type ) -> void {
        clearImpl( deleteContent, ItemDispatcherBase::ptr_type{} );
    }

public:
    /*! \brief Shortcut to Container<T>::contains().
     *
     * \note argument \c item could be nullptr.
     */
    auto    contains( T item ) const noexcept -> bool { return _container.contains( item ); }

public:
    /*! \brief Shortcut to Container<T>::indexOf(), return index of a given \c item element in this model container.
     *
     * \note argument \c item could be nullptr.
     */
    auto    indexOf( T item ) const noexcept -> int {
        return _container.indexOf( item );
    }

private:
    Container< T >          _container;
public:
    //! The preffered way of accessing internal list is to use cast operator with static_cast<const Container<T>>( containerModel ).
    inline const Container< T >&    getContainer( ) const noexcept { return _container; }
protected:
    inline Container< T >&          getContainer( ) noexcept { return _container; }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Qt Item Model Interface *///-------------------------------------
    //@{
public:
    enum PropertiesRoles {
        ItemDataRole = Qt::UserRole + 1,
        LabelRole
    };
public:
    virtual int         rowCount( const QModelIndex& parent = QModelIndex{} ) const override {
        return ( parent.isValid() ? 0 : _container.size( ) );
    }

public:
    virtual QVariant    data( const QModelIndex& index, int role = Qt::DisplayRole ) const override {
        if ( index.row( ) >= 0 &&
             index.row( ) < _container.size( ) ) {
            if ( role == Qt::DisplayRole ||
                 role == LabelRole )
                return dataDisplayRole( index.row(), typename ItemDispatcher<T>::type{} );
            else if ( role == ItemDataRole )
                return dataItemRole( index.row(), typename ItemDispatcher<T>::type{} );
        }
        return QVariant{};
    }

public:
    //! Defined for MSVC2015U2 which has trouble with friendship of classe inheriting from on of it's template parameter, do not use, should be considered protected.
    const QMap< const QObject*, T >&    getqObjectItemMap() const { return _qObjectItemMap; }
protected:
    //! Maintain a mapping between smart<T> and T* when T is a QObject.
    mutable QMap< const QObject*, T >  _qObjectItemMap;

protected:
    inline auto dataDisplayRole( int row, ItemDispatcherBase::non_ptr_type )        const -> QVariant {
        return QVariant{ at( row ) };
    }
    inline auto dataDisplayRole( int, ItemDispatcherBase::unsupported_type )    const -> QVariant {
        return QVariant{};
    }
    inline auto dataDisplayRole( int row, ItemDispatcherBase::ptr_type )            const -> QVariant {
        T ptrItem = at( row );
        return ( ptrItem != nullptr ? QVariant::fromValue<std::remove_pointer<T>::type>( *ptrItem ) :
                                      QVariant{} );
    }
    inline auto dataDisplayRole( int row, ItemDispatcherBase::ptr_qobject_type )    const -> QVariant {
        T item = at( row );
        monitorItem( item );
        return ( item != nullptr ? item->property( getItemDisplayRole( ).toLatin1() ) :
                                      QVariant{} );
    }
    inline auto dataDisplayRole( int row, ItemDispatcherBase::shared_ptr_type )     const -> QVariant {
        T item = at( row );
        return ( item ? QVariant::fromValue<std::remove_pointer<typename T::element_type>::type>( *item.get() ) :
                        QVariant{} );
    }
    inline auto dataDisplayRole( int row, ItemDispatcherBase::shared_ptr_qobject_type ) const -> QVariant {
        T item = at( row );
        QObject* qItem = item.get();
        monitorItem( qItem );
        return ( qItem != nullptr ? qItem->property( getItemDisplayRole().toLatin1() ) :
                                    QVariant{} );
    }
    inline auto dataDisplayRole( int row, ItemDispatcherBase::weak_ptr_type )           const -> QVariant { return QVariant{}; }
    inline auto dataDisplayRole( int row, ItemDispatcherBase::weak_ptr_qobject_type )   const -> QVariant {
        T item = at( row );
        monitorItem( item.lock().get() );
        return ( item.expired() ||
                 item.lock() == nullptr ? QVariant{} :
                                          item.lock()->property( getItemDisplayRole().toLatin1() ) );
    }

private:
    //! Catch an item label property change notify signal and force model update for it's index.
    void    monitorItem( QObject* item ) const {
        if ( item != nullptr ) {
            QMetaProperty displayProperty = item->metaObject()->property(
                        item->metaObject()->indexOfProperty( getItemDisplayRole().toLatin1() ) );
            if ( displayProperty.isValid() &&
                 displayProperty.hasNotifySignal() ) {
                QMetaMethod displayPropertyChanged = displayProperty.notifySignal( );
                // Note 20161125: Direct connection (without method(indexOfSlot()) call is impossible, there is no existing QObject::connect
                // overload taking (QObject*, QMetaMethod, QObject*, pointer on method).
                QMetaMethod itemDisplayPropertyChangedSlot = metaObject( )->method( metaObject( )->indexOfSlot( "itemDisplayPropertyChanged()" ) );
                auto c = connect( item, displayPropertyChanged,
                                  const_cast< ContainerModel< Container, T>* >( this ), itemDisplayPropertyChangedSlot );
            }
        }
    }

private:
    inline auto dataItemRole( int row, ItemDispatcherBase::non_ptr_type )           const -> QVariant {
        T item = at( row );
        return QVariant{ item };
    }
    inline auto dataItemRole( int, ItemDispatcherBase::unsupported_type )           const -> QVariant { /* empty */ return QVariant{};  }

    inline auto dataItemRole( int, ItemDispatcherBase::ptr_type )                   const -> QVariant { /* empty */ return QVariant{};  }
    inline auto dataItemRole( int row, ItemDispatcherBase::ptr_qobject_type )       const -> QVariant {
        T item = at( row );
        if ( item != nullptr )
            QQmlEngine::setObjectOwnership( item, QQmlEngine::CppOwnership );
        return QVariant::fromValue< T >( item );
    }
    inline auto dataItemRole( int, ItemDispatcherBase::shared_ptr_type )            const -> QVariant { /* empty */ return QVariant{};  }
    inline auto dataItemRole( int row, ItemDispatcherBase::shared_ptr_qobject_type ) const -> QVariant {
        T item = at( row );
        auto qItem = qobject_cast<QObject*>( item.get() );
        if ( qItem != nullptr )
            QQmlEngine::setObjectOwnership( qItem, QQmlEngine::CppOwnership );
        return QVariant::fromValue<QObject*>( qItem );
    }

    inline auto dataItemRole( int, ItemDispatcherBase::weak_ptr_type )              const -> QVariant { /* empty */ return QVariant{}; }
    inline auto dataItemRole( int row, ItemDispatcherBase::weak_ptr_qobject_type )  const -> QVariant {
        T item = at( row );
        auto qItem = ( item.expired() ? nullptr : qobject_cast<QObject*>( item.lock().get() ) );
        if ( qItem )
            QQmlEngine::setObjectOwnership( qItem, QQmlEngine::CppOwnership );
        return ( qItem != nullptr ? QVariant::fromValue<QObject*>(qItem) :
                                    QVariant{} );
    }

protected: 
    virtual QHash< int, QByteArray >    roleNames( ) const override {
        QHash< int, QByteArray > roles;
        roles[ ItemDataRole ] = "itemData";
        roles[ LabelRole ] = "itemLabel";
        return roles;
    }
    //@}
    //-------------------------------------------------------------------------
};

} // ::qcm

#endif // qcmContainerModel_h

