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
// \file    qcmContainerModelComposerWatcher.cpp
// \author	benoit@destrat.io
// \date	2016 11 28
//-----------------------------------------------------------------------------

// Qt headers
#include <QDebug>

// QuickContainers headers
#include "./qcmContainerModelComposerWatcher.h"

namespace qcm { // ::qcm

/* ContainerModelComposerWatcher Management *///-------------------------------
void    ContainerModelComposerWatcher::onSourceModelAboutToBeReset() { /* empty */ }

void    ContainerModelComposerWatcher::onSourceModelReset() { /* empty */ }

void    ContainerModelComposerWatcher::onSourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                                           const QVector<int> &roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);
}

void    ContainerModelComposerWatcher::onSourceRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
}

void    ContainerModelComposerWatcher::onSourceRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
}

void    ContainerModelComposerWatcher::connectSource(QAbstractListModel* source) {
    if ( source == nullptr )
        return;
    connect( source,    &QAbstractListModel::modelAboutToBeReset,
             this,      &ContainerModelComposerWatcher::onSourceModelAboutToBeReset );
    connect( source,    &QAbstractListModel::modelReset,
             this,      &ContainerModelComposerWatcher::onSourceModelReset );

    connect( source,    &QAbstractListModel::dataChanged,
             this,      &ContainerModelComposerWatcher::onSourceDataChanged );
    connect( source,    &QAbstractListModel::modelReset,
             this,      &ContainerModelComposerWatcher::onSourceModelReset );
    connect( source,    &QAbstractListModel::rowsInserted,
             this,      &ContainerModelComposerWatcher::onSourceRowsInserted );
    connect( source,    &QAbstractListModel::rowsAboutToBeRemoved,
             this,      &ContainerModelComposerWatcher::onSourceRowsAboutToBeRemoved );
}

//! Disconnect a source from this composed model.
void    ContainerModelComposerWatcher::disconnectSource(QAbstractListModel* source) {
    if ( source != nullptr )
        disconnect( source, 0, this, 0 );
}
//-----------------------------------------------------------------------------

} // ::qcm

