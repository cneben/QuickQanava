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
// \file    qcmContainerModelComposer.h
// \author  benoit@destrat.io
// \date    2016 11 25
//-----------------------------------------------------------------------------

#ifndef qcmContainerModelComposer_h
#define qcmContainerModelComposer_h

// Qt headers
#include <QAbstractListModel>
#include <QPointer>

// QuickContainers headers
#include "./qcmContainerModel.h"
#include "./qcmContainerModelComposerWatcher.h"
#include "./qcmContainerModelComposerListReference.h"

namespace qcm { // ::qcm

/*! \brief Compose two abstracts item list models to a single one (
 *
 * \warning A composed model is necessarilly 'read-only', listReference property
 * will return a valid reference, but calling modification methods will have no
 * effect.
 *
 * \note Composing pre-existing models is fully static and fast, while modification of
 * sourcs models after there registration in a composer is very slow since it rely on Qt abstract item
 * model notification via signal and virtualized interface: when possible, delay the creation of
 * your composer until source models are fully initialized, then create the composer.
 *
 * \note Composition of heterogenous container model is possible if their concrete types inherits from a
 * common base and implicit constructors are available to convert types (Common base type must be used used a
 * template parameter \c T).
 * \note Source models must be qcm::ContainerModel objects.
 */
template < template<typename...CArgs> class Container, class T,
           template<typename...CArgs> class Container1 = Container, class T1 = T,
           template<typename...CArgs> class Container2 = Container, class T2 = T >
class ContainerModelComposer : public qcm::ContainerModel<Container, T>
{
    /*! \name ContainerModelComposer Object Management *///--------------------
    //@{
public:
    using Base      = qcm::ContainerModel<Container, T>;
    using M1        = qcm::ContainerModel<Container1, T1>;
    using M2        = qcm::ContainerModel<Container2, T2>;
    using This      = qcm::ContainerModelComposer<Container, T, Container1, T1, Container2, T2>;
    using WatcherImpl = qcm::ContainerModelComposerWatcherImpl<This>;

    /* Design node 20161128: ContainerComposerWatcher is in fact the "QObject" part of this composer
     * model. Composer model can't inherit from QObject since it is a template class and that
     * abstract container model virtual base class can't be specialized for composer.
     * Every code specific to composer model is located in watcher class (mainly catching source
     * model notifcations), and in ContainerModelComposerListReference for the QML interface.
     */
    friend qcm::ContainerModelComposerWatcher;
    friend qcm::ContainerModelComposerListReference;        // List reference need access to watcher.
    friend qcm::ContainerModelComposerListReferenceImpl< This >;  // List reference impl need access to This::createIndex() shortcut in qcm::ContainerModel
    friend qcm::ContainerModelListReference;                // List reference impl need access to This::createIndex() shortcut in qcm::ContainerModel
    friend qcm::ContainerModelListReferenceImpl< This >;
    friend qcm::ContainerModelListReferenceImpl< Base >;

    explicit ContainerModelComposer(QObject* parent = nullptr) :
        Base(parent),
        _watcher{ std::make_unique<WatcherImpl>(*this, nullptr) }
    { }
    virtual ~ContainerModelComposer() { }
    ContainerModelComposer(const This&) = delete;

protected:
    virtual     std::unique_ptr<qcm::ContainerModelListReference>     createListReference() override {
        return std::move( std::make_unique<qcm::ContainerModelComposerListReferenceImpl< This > >(*this) );
    }
public:
    // Shortcut to allow access from friend classes to QAbstractListModel::createIndex().
    QModelIndex createIndex(int row, int column, void *ptr = nullptr) const {
        return QAbstractListModel::createIndex(row, column, ptr);
    }
protected:
    ContainerModelComposerWatcher*                  getWatcher() { return _watcher.get(); }
    std::unique_ptr<ContainerModelComposerWatcher>  _watcher{};
    //@}
    //-------------------------------------------------------------------------

    /*! \name Source Models Management *///------------------------------------
    //@{
public:
    M1*     getM1( ) { return static_cast<M1*>( _watcher->getM1() ); }
    void    setM1( M1& m1 ) { _watcher->setM1(&m1); }
    M2*     getM2( ) { return static_cast<M2*>( _watcher->getM2() ); }
    void    setM2( M2& m2 ) { _watcher->setM2(&m2); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Item Model Interface *///----------------------------------------
    //@{
public:
    //! Map a model index from a composer source to the composed model.
    QModelIndex mapFromSource( QAbstractListModel* source, const QModelIndex& index ) {
        return QModelIndex{};
    }

    //! Map a row in from a composer source to the composed model.
    int         mapRowFromSource( QAbstractListModel* source, int row ) {
        int composerRow = row;
        if ( source == nullptr )
            return composerRow;
        if ( source == getM1() )
            composerRow = row;
        else if ( source == getM2() )
            composerRow = ( getM1() != nullptr ? getM1()->rowCount() + row :
                                                 row );
        return composerRow;
    }
    //@}
    //-------------------------------------------------------------------------
};

} // ::qcm

#endif // qcmContainerModelComposer_h

