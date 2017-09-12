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

