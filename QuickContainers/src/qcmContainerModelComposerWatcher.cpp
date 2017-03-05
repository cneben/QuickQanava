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

