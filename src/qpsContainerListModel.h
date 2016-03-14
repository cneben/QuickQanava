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
// \file    qpsContainerListModel.h
// \author  benoit@qanava.org
// \date    2015 06 20
//-----------------------------------------------------------------------------

#ifndef qpsContainerListModel_h
#define qpsContainerListModel_h

// Qt headers
#include <QAbstractListModel>

// QuickProperties headers
#include "./qpsProperties.h"

// STD headers
#include <memory>       // shared_ptr, weak_ptr
#include <type_traits>  // integral_constant

namespace qps { // ::qps

//#define QPS_CONTAINER_DEBUG

//! Utility class used to watch a specific property notify signal in a container.
class DisplayRoleWatcher : public QAbstractListModel
{
    Q_OBJECT
public:
    DisplayRoleWatcher( QObject* parent = nullptr );

    DisplayRoleWatcher( const DisplayRoleWatcher& displayRoleWatcher ) :
        _displayRoleProperty( displayRoleWatcher._displayRoleProperty ) { }

    /*! \name QML Container Interface *///-------------------------------------
    //@{
public:
    Q_PROPERTY( int itemCount READ getItemCount NOTIFY itemCountChanged )
    //! Return the number of element in this container (shortcut to rowCount( QModelIndex())).
    Q_INVOKABLE int getItemCount( ) const { return rowCount( QModelIndex( ) ); }
protected:
    //! Shortcut to emit itemCountChanged().
    void            emitItemCountChanged() { emit itemCountChanged(); }
signals:
    //! \sa itemCount
    void            itemCountChanged();
public:
    //! Return the element at the requested container \c index position.
    Q_INVOKABLE QVariant    itemAt( int index ) {
        return data( createIndex( index, 0 ), Qt::UserRole + 1 ); // Note 20160220: Qt::UserRole + 1 is in fact ContainerListModel::LabelRole
    }
    //! Return index of specific object (works if the underlining ContainerListModel use smart pointers.
    Q_INVOKABLE int         itemIndex( QObject* item ) const { return getIndexOfItem( item ); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Display Role Monitoring *///-------------------------------------
    //@{
public:
    /*! \brief When the container is a container of QObject*, the data() method will return the content of property
     *  \c displayRoleProperty for both Qt::DisplayRole and "label" dispay role.
     *
     * When you are using a container of QObject* with a QString "label" property, then use:
     * \code
     * qps::ContainerListModel< QVector, QObject* > myContainer;
     * myContainer->setItemDisplayRole( QStringLiteral( "label" ) );    // Expose that as myContainer context property in QML
     *
     * // Then from QML, a ComboBox (for example) might be configured as:
     * ComboBox {
     *   model: myContainer
     *   textRole: "label"
     * }
     * \endcode
     */
    void        setItemDisplayRole( QString displayRoleProperty ) { _displayRoleProperty = displayRoleProperty; }
protected:
    QString     getItemDisplayRole( ) const { return _displayRoleProperty; }
private:
    QString     _displayRoleProperty = "label";
private slots:
    void    itemDisplayPropertyChanged() {
        QObject* qItem = sender();
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "itemDisplayPropertyChanged() sender=" << qItem;
#endif
        int qItemIndex = getIndexOfItem( qItem );
        if ( qItemIndex >= 0 ) {
            QModelIndex itemIndex = index( qItemIndex );
            if ( itemIndex.isValid( ) )
                emit dataChanged( itemIndex, itemIndex );
        }
        else
            disconnect( qItem, 0, this, 0 );
    }

    virtual int getIndexOfItem( QObject* item ) const = 0;
    //-------------------------------------------------------------------------
};

/*! \brief Expose a standard container such as QList or QVector to an abstract list model.
 *
 * Defining a item model for a QVector container of int or pointers on QObject:
 * \code
 *   using Ints = qps::ContainerListModel< QVector, int >;
 *   Ints ints;
 *   using QObjects = qps::ContainerListModel< QVector, QObject* >;
 *   QObjects qobjects;
 * \endcode
 *
 * \c ints or \c qobjects could then be used in any item model view such as ListView.
 *
 * ContainerListModel containers could be iterated using Qt for_each or standard C++11 for(:) keyword.
 *
 * Do not forget to declare the type to QML:
 * \code
 * using MyContainer = qps::ContainerListModel<Qlist, QObject>;
 * QML_DECLARE_TYPE( MyContainer )
 * \endcode
 *
 * If you intend to use the container type with standard library smart pointers, the resulting types
 * must be declared with Q_DECLARE_METATYPE:
 * \code
 * #include <memory>
 *
 * Q_DECLARE_METATYPE( std::shared_ptr<QObject> )
 * using MyContainer = qps::ContainerListModel< QVector, std::shared_ptr<QObject> >;
 *
 * Q_DECLARE_METATYPE( std::weak_ptr<QObject> )
 * using MyContainer = qps::ContainerListModel< QVector, std::weak_ptr<QObject> >;
 * \endcode
 *
 * FIXME 20160218: Document use of std::shared_ptr<T> and std::weak_ptr<T> an QVariant( (QObject*)T* ) return cast
 * for QAbstractItemModel data() method.
 * FIXME 20160218: Document the bound checking done for pointer arguments for most insert/remove methods.
 * FIXME 20160218: Add expiration and nullptr checking for std::weak_ptr and std::shared_ptr.
 *
 *
 * \warning data() method will return a QVariant with a QObject* value when container is shared_ptr_qobject_type
 * or weak_ptr_qobject_type. In ptr_qobject_type, the returned QVariant will not be QObject but T*. Sometime QML
 * might have trouble making a distinction between a QVariant encoding a QObject* while it should be a concrete
 * QObject* subclass.
 */
template < template<typename> class Container, class T >
class ContainerListModel : public DisplayRoleWatcher
{
    /*! \name ContainerListModel Object Management *///------------------------
    //@{
public:
    //! Parent must be the qps::Graph managing nodes exposed in this model.
    ContainerListModel( QObject* parent = nullptr ) :
        DisplayRoleWatcher( parent ) { }

    virtual ~ContainerListModel( ) { }

    //! Defined for QList<> foreach iteration, do not use.
    ContainerListModel( const ContainerListModel<Container, T>& container ) :
        DisplayRoleWatcher( container ),
        _list( container._list ) { }
    //@}
    //-------------------------------------------------------------------------

private:
    using undef_type                = std::integral_constant<int, 0>;

    using non_ptr_type              = std::integral_constant<int, 1>;
    using non_ptr_qobject_type      = std::integral_constant<int, 2>;

    using ptr_type                  = std::integral_constant<int, 3>;
    using ptr_qobject_type          = std::integral_constant<int, 4>;

    using shared_ptr_type           = std::integral_constant<int, 5>;
    using shared_ptr_qobject_type   = std::integral_constant<int, 6>;

    using weak_ptr_type             = std::integral_constant<int, 7>;
    using weak_ptr_qobject_type     = std::integral_constant<int, 8>;

    template < typename T >
    struct item_type : std::conditional< std::is_pointer<T>::value,
                                                typename std::conditional< std::is_base_of< QObject, typename std::remove_pointer< T >::type >::value,
                                                                                    ptr_qobject_type,
                                                                                    ptr_type            >::type,
                                                typename std::conditional< std::is_base_of< QObject, T >::value,
                                                                                    non_ptr_qobject_type,
                                                                                    non_ptr_type        >::type
                                                                        >::type {};

    template < typename T >
    struct item_type< std::shared_ptr<T> > : std::conditional< std::is_base_of< QObject, T >::value,
                                                                                    shared_ptr_qobject_type,
                                                                                    shared_ptr_type >::type {};

    template < typename T >
    struct item_type< std::weak_ptr<T> > : std::conditional< std::is_base_of< QObject, T >::value,
                                                                                    weak_ptr_qobject_type,
                                                                                    weak_ptr_type >::type {};
private:
    inline bool    isNullPtr( T item, non_ptr_type )            const { (void)item; return false; }
    inline bool    isNullPtr( T item, non_ptr_qobject_type )    const { return isNullPtr(item, non_ptr_type{} ); }
    inline bool    isNullPtr( T item, ptr_type )                const { return item == nullptr; }
    inline bool    isNullPtr( T item, ptr_qobject_type )        const { return isNullPtr( item, ptr_type{} ); }
    inline bool    isNullPtr( T item, shared_ptr_type )         const { return !item; }
    inline bool    isNullPtr( T item, shared_ptr_qobject_type ) const { return isNullPtr(item, shared_ptr_type{} ); }
    inline bool    isNullPtr( T item, weak_ptr_type )           const { return item.expired(); }
    inline bool    isNullPtr( T item, weak_ptr_qobject_type )   const { return isNullPtr( item, weak_ptr_type{} ); }

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
    auto    begin( ) const -> const_iterator { return _list.begin( ); }
    auto    end( ) const -> const_iterator { return _list.end( ); }

    auto    begin( ) -> iterator { return _list.begin( ); }
    auto    end( ) -> iterator { return _list.end( ); }

    auto    cbegin( ) const -> const_iterator { return _list.cbegin( ); }
    auto    cend( ) const -> const_iterator { return _list.cend( ); }

    //! Define a cast operator to qps::Node::List, to iterate over qps::NodeList with the foreach keyword.
    operator Container< T >&() { return _list; }

public:
    auto    push_back( const T& value ) -> void { append( value ); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name QList Interface *///---------------------------------------------
    //@{
public:
    /*! \brief Shortcut to Container<T>::at() with bounding checking.
     *
     * FIXME: Dual throw/nullptr behaviour is not perfect, fix that.
     * \param i index of the element to access, if i < 0 std::out_of_range is thrown, if i >= container.size(), a nullptr or empty smart pointer is returned.
     */
    auto at( int i ) const -> T {
        if ( i < 0 )
            throw std::out_of_range( "qps::ContainerList::at(): Error: Index out of range < 0." );
        return atImpl( i, item_type<T>{} );
    }
private:
    inline auto atImpl( int i, non_ptr_type )            const -> T {
        return _list.at( i );
    }
    inline auto atImpl( int i, non_ptr_qobject_type )    const -> T { return atImpl( i, non_ptr_type{} ); }

    inline auto atImpl( int i, ptr_type )                const -> T {
        return ( i < _list.size() ? _list.at( i ) : nullptr );
    }
    inline auto atImpl( int i, ptr_qobject_type )        const -> T { return atImpl( i, ptr_type{} ); }

    inline auto atImpl( int i, shared_ptr_type )         const -> T {
        return ( i < _list.size() ? _list.at( i ) : T{} );
    }
    inline auto atImpl( int i, shared_ptr_qobject_type ) const -> T { return atImpl( i, shared_ptr_type{} ); }

    inline auto atImpl( int i, weak_ptr_type )           const -> T {
        return ( i < _list.size() ? _list.at( i ) : T{} );
    }
    inline auto atImpl( int i, weak_ptr_qobject_type )   const -> T { return atImpl( i, weak_ptr_type{} ); }

public:
    //! Shortcut to Container<T>::size().
    inline int  size( ) const { return _list.size( ); }

    //! Shortcut to Container<T>::append().
    void        append( T item ) {
        if ( isNullPtr( item, item_type<T>() ) )
            return;
        beginInsertRows( QModelIndex( ), _list.size( ), _list.size( ) );
        _list.append( item );
        endInsertRows( );
        emitItemCountChanged();
        //itemInserted( item );  // FIXME 20150219
    }

public:
    //! Shortcut to Container<T>::insert().
    void        insert( int i, T item ) {
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "insert() i=" << i;
        qDebug() << "\t_list.size()=" << _list.size();
#endif
        if ( i < 0 || isNullPtr( item, item_type<T>() ) )
            return;
        beginInsertRows( QModelIndex( ), i, i );
        _list.insert( i, item );
        endInsertRows( );
        emitItemCountChanged();
        itemInserted( item );
    }

    //! Shortcut to Container<T>::remove().
    void        remove( T item ) {
        if ( isNullPtr( item, item_type<T>() ) )
            return;
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "remove(): _list.size()=" << _list.size() << "  _qObjectItemMap.size()=" << _qObjectItemMap.size();
#endif
        int itemIndex = _list.indexOf( item );
        if ( itemIndex < 0 )
            return;
        beginRemoveRows( QModelIndex(), itemIndex, itemIndex );
        removeImpl( item, item_type<T>{} );
        _list.removeAll( item );
        endRemoveRows( );
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "end remove(): _list.size()=" << _list.size() << "  _qObjectItemMap.size()=" << _qObjectItemMap.size();
#endif
        emitItemCountChanged();
        itemRemoved( item );
    }
private:
    inline auto removeImpl( T item, non_ptr_type )            -> void { }
    inline auto removeImpl( T item, non_ptr_qobject_type )    -> void { _qObjectItemMap.remove( item ); }

    inline auto removeImpl( T item, ptr_type )                -> void { }
    inline auto removeImpl( T item, ptr_qobject_type )        -> void { _qObjectItemMap.remove( item ); }

    inline auto removeImpl( T item, shared_ptr_type )         -> void { }
    inline auto removeImpl( T item, shared_ptr_qobject_type ) -> void { _qObjectItemMap.remove( item.get() ); }

    inline auto removeImpl( T item, weak_ptr_type )           -> void { }
    inline auto removeImpl( T item, weak_ptr_qobject_type )   -> void {
        if ( item.lock() )
            _qObjectItemMap.remove( item.lock().get() );
        _list.removeAll( item );
    }

public:
    /*! Remove a given node from the internal node list.
     *
     *  Similar to remove(), defined for QList compatibility.
     */
    void        removeAll( T item ) {
        if ( isNullPtr( item, item_type<T>() ) )
            return;
        remove( item );
    }

    virtual void clear() {
        beginResetModel();
        clearImpl( item_type<T>{} );
        _list.clear( );
        endResetModel();
        emitItemCountChanged();
    }
private:
    inline auto clearImpl( non_ptr_type )            -> void { /* empty */ }
    inline auto clearImpl( non_ptr_qobject_type )    -> void { _qObjectItemMap.clear(); }

    inline auto clearImpl( ptr_type )                -> void { /* empty */ }
    inline auto clearImpl( ptr_qobject_type )        -> void { _qObjectItemMap.clear(); }

    inline auto clearImpl( shared_ptr_type )         -> void { /* empty */ }
    inline auto clearImpl( shared_ptr_qobject_type ) -> void { _qObjectItemMap.clear(); }

    inline auto clearImpl( weak_ptr_type )           -> void { /* empty */ }
    inline auto clearImpl( weak_ptr_qobject_type )   -> void { _qObjectItemMap.clear(); }

public:
    /*! Clear the container and optionally call delete on contained objects.
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
        clearImpl( deleteContent, item_type<T>{} );
        _list.clear();
        endResetModel();
        emitItemCountChanged();
    }
private:
    inline auto clearImpl( bool deleteContent, non_ptr_type )            -> void { /* empty */ Q_UNUSED( deleteContent ); }
    inline auto clearImpl( bool deleteContent, non_ptr_qobject_type )    -> void { _qObjectItemMap.clear(); }

    inline auto clearImpl( bool deleteContent, ptr_type )                -> void {
        if ( deleteContent ) {
            for ( const auto& p: _list )
                delete p;
        }
    }
    inline auto clearImpl( bool deleteContent, ptr_qobject_type )        -> void {
        clearImpl( deleteContent, ptr_type{} ); _qObjectItemMap.clear();
    }

    inline auto clearImpl( bool deleteContent, shared_ptr_type )         -> void { /* empty */ Q_UNUSED( deleteContent ); }
    inline auto clearImpl( bool deleteContent, shared_ptr_qobject_type ) -> void { _qObjectItemMap.clear(); }

    inline auto clearImpl( bool deleteContent, weak_ptr_type )           -> void { /* empty */ Q_UNUSED( deleteContent ); }
    inline auto clearImpl( bool deleteContent, weak_ptr_qobject_type )   -> void { _qObjectItemMap.clear(); Q_UNUSED( deleteContent ); }

public:
    /*! Shortcut to Container<T>::contains().
     *
     * \note argument item could be nullptr, no exception will be thrown, it will just check if the container store nullptrs...
     */
    auto    contains( T item ) const -> bool { return _list.contains( item ); }

public:
    //! Shortcut to Container<T>::move().
    inline void move( int from, int to ) { _list.move( from, to ); }

    /*! \brief Shortcut to Container<T>::indexOf(), return index of a given \c item element in this model container.
     *
     * \note argument item could be nullptr, no exception will be thrown...
     */
    auto    indexOf( T item ) const -> int { return _list.indexOf( item ); }

protected:
    // FIXME: keep that ? See if there is a need for properties list...
    virtual void    itemInserted( T ) { }
    virtual void    itemRemoved( T ) { }

protected:
    Container< typename T >&    getList( ) { return _list; }
    const Container< T >&       getList( ) const { return _list; }
private:
    Container< typename T >     _list;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Qt Item Model Interface *///-------------------------------------
    //@{
public:
    enum PropertiesRoles {
        ItemRole = Qt::UserRole + 1,
        LabelRole // Note 20160220: Warning, this value is hardencoded in DisplayRoleWatcher::itemAt(), modify with care
    };
public:
    virtual int         rowCount( const QModelIndex& parent = QModelIndex( ) ) const override {
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "rowCount(): parent=" << parent << "  _list.size()=" << _list.size();
#endif
        return ( parent.isValid() ? 0 : _list.size( ) );
    }

public:
    virtual QVariant    data( const QModelIndex& index, int role = Qt::DisplayRole ) const override {
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "data() index=" << index << "  role=" << role;
#endif
        if ( index.row( ) >= 0 && index.row( ) < _list.size( ) ) {
            if ( role == Qt::DisplayRole || role == LabelRole ) {
                //qDebug() << "Data: querying DisplayRole for index.row=" << index.row();
                return dataDisplayRole( index.row(), item_type<T>{} );
            }
            else if ( role == ItemRole ) {
                //qDebug() << "Data: querying ItemRole for index.row=" << index.row();
                return dataItemRole( index.row(), item_type<T>{} );
            }
        }
        return QVariant( "Data error" );
    }

protected:
    // FIXME: Document interaction with base watcher class
    virtual int getIndexOfItem( QObject* item ) const override
    {
        return indexOfItemImpl( item, item_type<T>() );
    }
private:
    inline auto indexOfItemImpl( QObject* qItem, non_ptr_type )            const -> int {
        Q_ASSERT( false && "indexOfItemImpl(QObject*) unsupported for non_ptr_type" );
        return -1;
    }
    inline auto indexOfItemImpl( QObject* qItem, non_ptr_qobject_type )    const -> int {
        Q_ASSERT( false && "indexOfItemImpl(QObject*) unsupported for non_ptr_qobject_type" );
        return -1;
    }
    inline auto indexOfItemImpl( QObject* qItem, ptr_type )                const -> int {
        Q_ASSERT( false && "indexOfItemImpl(QObject*) unsupported for ptr_type" );
        return -1;
    }
    inline auto indexOfItemImpl( QObject* qItem, ptr_qobject_type )        const -> int { return indexOf( qobject_cast< T >( qItem ) ); }
    inline auto indexOfItemImpl( QObject* qItem, shared_ptr_type )         const -> int { return -1; }
    inline auto indexOfItemImpl( QObject* qItem, shared_ptr_qobject_type ) const -> int {
        if ( _qObjectItemMap.contains( qItem ) ) {
            T item = _qObjectItemMap.value( qItem );
            return indexOf( item );
        }
        return -1;
    }
    inline auto indexOfItemImpl( QObject* qItem, weak_ptr_type )           const -> int { return -1; }
    inline auto indexOfItemImpl( QObject* qItem, weak_ptr_qobject_type )   const -> int {
        if ( _qObjectItemMap.contains( qItem ) ) {
            T item = _qObjectItemMap.value( qItem );
            return indexOf( item );
        }
        return -1;
    }

private:
    //! Maintain a mapping between smart<T> and T* when T is a QObject.
    mutable QMap< const QObject*, T >  _qObjectItemMap;

    inline auto dataDisplayRole( int row, non_ptr_type )            const -> QVariant {
        return QVariant( at( row ) );
    }
    inline auto dataDisplayRole( int row, non_ptr_qobject_type )    const -> QVariant {
        return QVariant( at( row ) );
    }
    inline auto dataDisplayRole( int row, ptr_type )                const -> QVariant {
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "dataDisplayRole(ptr_type) row=" << row;
#endif
        T ptrItem = at( row );
        return ( ptrItem != nullptr ? QVariant::from_value<std::remove_ptr<T>::type>( *ptrItem ) :
                                      QVariant() );
    }
    inline auto dataDisplayRole( int row, ptr_qobject_type )        const -> QVariant {
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "dataDisplayRole(ptr_qobject_type) row=" << row;
#endif
        T ptrItem = at( row );
        return ( ptrItem != nullptr ? ptrItem->property( getItemDisplayRole( ).toLatin1() ) :
                                      QVariant() );
    }
    inline auto dataDisplayRole( int row, shared_ptr_type )         const -> QVariant {
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "dataDisplayRole(shared_ptr_type) row=" << row;
#endif
        T item = at( index.row() );
        return ( item ? QVariant::from_value<std::remove_ptr<T::element_type>::type>( *item.get() ) :
                        QVariant() );
    }
    inline auto dataDisplayRole( int row, shared_ptr_qobject_type ) const -> QVariant {
#ifdef QPS_CONTAINER_DEBUG
        qDebug() << "dataDisplayRole(shared_ptr_qobject_type) row=" << row;
#endif
        T item = at( row );
        QObject* qItem = item.get();
        if ( qItem != nullptr ) {
            QMetaProperty displayProperty = qItem->metaObject()->property(
                                                    qItem->metaObject()->indexOfProperty( getItemDisplayRole().toLatin1() ) );
            if ( displayProperty.isValid() && displayProperty.hasNotifySignal() ) {
                QMetaMethod displayPropertyChanged = displayProperty.notifySignal( );
                _qObjectItemMap.insert( qItem, item );
                QMetaMethod itemDisplayPropertyChangedSlot = metaObject( )->method( metaObject( )->indexOfSlot( "itemDisplayPropertyChanged()" ) );
                connect( qItem, displayPropertyChanged,
                         const_cast< ContainerListModel< Container, T>* >( this ), itemDisplayPropertyChangedSlot );
            }
        }
        return ( qItem != nullptr ? qItem->property( getItemDisplayRole().toLatin1() ) :
                                    QVariant() );
    }
    inline auto dataDisplayRole( int row, weak_ptr_type )           const -> QVariant {
        return QVariant();
    }
    inline auto dataDisplayRole( int row, weak_ptr_qobject_type )   const -> QVariant {
        T item = at( row ); // A default weak_ptr<>() will be expired...
        return ( item.expired() || item.lock() == nullptr ? QVariant() :
                                                            item.lock().get()->property( getItemDisplayRole().toLatin1() ) );
    }

private:
    inline auto dataItemRole( int row, non_ptr_type )            const -> QVariant {
        T item = at( row );
        return QVariant{ item };
    }
    inline auto dataItemRole( int row, non_ptr_qobject_type )    const -> QVariant { /* empty */ Q_UNUSED( row ); return QVariant();  }

    inline auto dataItemRole( int row, ptr_type )                const -> QVariant { /* empty */ Q_UNUSED( row ); return QVariant();  }
    inline auto dataItemRole( int row, ptr_qobject_type )        const -> QVariant {
        T item = at( row );
        return QVariant::fromValue< T >( item );
    }
    inline auto dataItemRole( int row, shared_ptr_type )         const -> QVariant { /* empty */ Q_UNUSED( row ); return QVariant();  }
    inline auto dataItemRole( int row, shared_ptr_qobject_type ) const -> QVariant {
        T item = at( row );
        return QVariant::fromValue< QObject* >( ( QObject* )item.get() );
    }

    inline auto dataItemRole( int row, weak_ptr_type )           const -> QVariant { /* empty */ Q_UNUSED( row ); return QVariant(); }
    inline auto dataItemRole( int row, weak_ptr_qobject_type )   const -> QVariant {
        T item = at( row );
        return ( item.expired() || item.lock() == nullptr ? QVariant() :
                                                            QVariant::fromValue< QObject* >( ( QObject* )item.lock().get() ) );
    }

protected: 
    virtual QHash< int, QByteArray >    roleNames( ) const override {
        QHash< int, QByteArray > roles;
        roles[ ItemRole ] = "itemData";
        roles[ LabelRole ] = "itemLabel";
        return roles;
    }
    //@}
    //-------------------------------------------------------------------------
};

} // ::qps

#endif // qpsContainerListModel_h

