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
// \file    qcmContainerModelFilter.h
// \author  benoit@destrat.io
// \date    2016 11 25
//-----------------------------------------------------------------------------

#ifndef qcmContainerModelFilter_h
#define qcmContainerModelFilter_h

// Qt headers
#include <QSortFilterProxyModel>

// QuickContainers headers
#include "./qcmContainerModel.h"

namespace qcm { // ::qcm

class AbstractContainerModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    AbstractContainerModelFilter(QObject* parent = nullptr);
    virtual ~AbstractContainerModelFilter() { /* Nil */ }
    AbstractContainerModelFilter(const AbstractContainerModelFilter&) = delete;
    AbstractContainerModelFilter(AbstractContainerModelFilter&&) = delete;

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

template < class ContainerModel, typename FilterFunctor >
class ContainerModelFilter : AbstractContainerModelFilter
{
public:
    explicit ContainerModelFilter(QObject* parent = nullptr) : AbstractContainerModelFilter(parent) { }
    virtual ~ContainerModelFilter() { /* Nil */ }
    ContainerModelFilter(const ContainerModelFilter&) = delete;
    ContainerModelFilter(ContainerModelFilter&&) = delete;

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        return FilterFunctor(sourceRow, sourceParent);
    }
};

} // ::qcm

#endif // qcmContainerModelFilter_h

