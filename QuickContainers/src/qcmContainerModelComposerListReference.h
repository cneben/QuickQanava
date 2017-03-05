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
// \file    qcmContainerModelComposerListReference.h
// \author  benoit@destrat.io
// \date    2016 11 28
//-----------------------------------------------------------------------------

#ifndef qcmContainerModelComposerListReference_h
#define qcmContainerModelComposerListReference_h

// Std headers
#include <memory>

// Qt headers
#include <QObject>
#include <QDebug>
#include <QQmlEngine>   // Q_QML_DECLARE_TYPE

// QuickContainers headers
#include "./qcmAbstractContainerModel.h"
#include "./qcmContainerModelListReference.h"
#include "./qcmContainerModelComposerWatcher.h"

namespace qcm { // ::qcm

class ContainerModelComposerListReference : public ContainerModelListReference
{
    Q_OBJECT
public:
    ContainerModelComposerListReference() : ContainerModelListReference{} {}
    virtual ~ContainerModelComposerListReference() {}
    ContainerModelComposerListReference(const ContainerModelComposerListReference&) = delete;

public:
    Q_PROPERTY( qcm::AbstractContainerModel* m1 READ getM1 WRITE setM1 NOTIFY m1Changed FINAL )
    virtual qcm::AbstractContainerModel* getM1() { return nullptr; }
    virtual void                    setM1(qcm::AbstractContainerModel*) { }
signals:
    void                            m1Changed();

public:
    Q_PROPERTY( qcm::AbstractContainerModel* m2 READ getM2 WRITE setM2 NOTIFY m2Changed FINAL )
    virtual qcm::AbstractContainerModel* getM2() { return nullptr; }
    virtual void                    setM2(qcm::AbstractContainerModel*) { }
signals:
    void                            m2Changed();
};


template < class ContainerModelComposer >
class ContainerModelComposerListReferenceImpl :
        public ContainerModelListReferenceImpl< ContainerModelComposer, ContainerModelComposerListReference >
{
public:
    using Base = ContainerModelListReferenceImpl<ContainerModelComposer, ContainerModelComposerListReference >;
    using This = ContainerModelComposerListReferenceImpl<ContainerModelComposer >;
    explicit ContainerModelComposerListReferenceImpl( ContainerModelComposer& containerModelComposer ) :
        Base{containerModelComposer},
        _watcher{ *containerModelComposer.getWatcher() } { }
    ContainerModelComposerListReferenceImpl(const This&) = delete;

    ContainerModelComposerWatcher& _watcher;
    virtual AbstractContainerModel* getM1() override { return _watcher.getM1(); }
    virtual void                    setM1(AbstractContainerModel* m1) override {
        _watcher.setM1(m1); emit Base::m1Changed();
    }
    virtual AbstractContainerModel* getM2() override { return _watcher.getM2(); }
    virtual void                    setM2(AbstractContainerModel* m2) override {
        _watcher.setM2(m2); emit Base::m2Changed();
    }
};

} // ::qcm

QML_DECLARE_TYPE(qcm::ContainerModelComposerListReference)

#endif // qcmContainerModelComposerListModel_h

