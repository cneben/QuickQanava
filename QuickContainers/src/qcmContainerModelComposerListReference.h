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

