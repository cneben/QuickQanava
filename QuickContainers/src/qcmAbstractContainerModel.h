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
// \file    qcmAbstractContainerModel.h
// \author  benoit@destrat.io
// \date    2015 06 20
//-----------------------------------------------------------------------------

#ifndef qcmAbstractContainerModel_h
#define qcmAbstractContainerModel_h

// Std headers
#include <memory>               // std::unique_ptr

// Qt headers
#include <QAbstractListModel>
#include <QPointer>

// QuickContainers headers
#include "./qcmContainerModelListReference.h"

namespace qcm { // ::qcm

class ContainerModelComposerWatcher;
class ContainerModelComposerListReference;

//! Utility class used to watch a specific property notify signal in a container.
class AbstractContainerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    friend qcm::ContainerModelComposerWatcher;
    friend qcm::ContainerModelListReference;
    friend qcm::ContainerModelComposerListReference;

    explicit AbstractContainerModel( QObject* parent = nullptr ) : QAbstractListModel{ parent } { }
    virtual ~AbstractContainerModel( ) { }
    AbstractContainerModel( const AbstractContainerModel& displayRoleWatcher ) = delete;

public:
    virtual int         rowCount( const QModelIndex& parent = QModelIndex{} ) const override { Q_UNUSED(parent); return 0; }
    virtual QVariant    data( const QModelIndex& index, int role = Qt::DisplayRole ) const override { Q_UNUSED(index); Q_UNUSED(role); return QVariant{}; }

    /*! \name QML Container Interface *///-------------------------------------
    //@{
public:
    Q_PROPERTY( int itemCount READ getItemCount NOTIFY itemCountChanged FINAL )
    //! Return the number of element in this container (shortcut to rowCount( QModelIndex())).
    Q_INVOKABLE int getItemCount( ) const { return rowCount( QModelIndex{} ); }
protected:
    //! Shortcut to emit itemCountChanged() signal.
    inline void     emitItemCountChanged() { emit itemCountChanged(); }
signals:
    //! \sa itemCount
    void            itemCountChanged();

public:
    Q_PROPERTY( ContainerModelListReference*    listReference READ getListReference CONSTANT FINAL )
    //! Return a list reference to modify the underlining model from QML (listReference is created on demande, expect a quite slow first call).
    inline ContainerModelListReference*         getListReference( ) noexcept {
        if ( !_listReference )
            _listReference = createListReference();
        return _listReference.get();
    }
protected:
    //! Create a concrete container model list reference for this abstract interface (called once).
    virtual     std::unique_ptr<qcm::ContainerModelListReference>     createListReference() { return std::unique_ptr<qcm::ContainerModelListReference>{nullptr}; }
private:
    std::unique_ptr<ContainerModelListReference>    _listReference;
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
     * qcm::ContainerModel< QVector, QObject* > myContainer;
     * myContainer->setItemDisplayRole( QStringLiteral( "label" ) );    // Expose that as myContainer context property in QML
     *
     * // Then from QML, a ComboBox (for example) might be configured as:
     * ComboBox {
     *   model: myContainer
     *   textRole: "itemLabel"
     * }
     * \endcode
     */
    void        setItemDisplayRole( QString displayRoleProperty ) { _displayRoleProperty = displayRoleProperty; }
protected:
    QString     getItemDisplayRole( ) const { return _displayRoleProperty; }
private:
    QString     _displayRoleProperty{QStringLiteral("label")};
protected slots:
    void    itemDisplayPropertyChanged() {
        QObject* qItem = sender();
        if ( qItem == nullptr )
            return;
        if ( getListReference() != nullptr ) {
            int qItemIndex = getListReference()->itemIndex( qItem );
            if ( qItemIndex >= 0 ) {
                QModelIndex itemIndex{ index( qItemIndex ) };
                if ( itemIndex.isValid( ) )
                    emit dataChanged( itemIndex, itemIndex );
            } else
                disconnect( qItem, 0, this, 0 );    // Do not disconnect if getListReference() or qItem is nullptr !
        }
    }
    //-------------------------------------------------------------------------
};

} // ::qcm

QML_DECLARE_TYPE(qcm::AbstractContainerModel);

#endif // qcmAbstractContainerModel_h

