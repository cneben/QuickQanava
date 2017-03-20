/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library.
//
// \file	qanGraphConfig.h
// \author	benoit@destrat.io
// \date	2016 02 04
//-----------------------------------------------------------------------------

#ifndef qanGraphConfig_h
#define qanGraphConfig_h

#ifdef _MSC_VER
#pragma warning(disable: 4100)  // Disable unreferenced formal parameter warning for Protocol Buffer generated code
#endif

// Qt headers
#include <QObject>
#include <QVector>
#include <QSet>
#include <QQuickItem>

// GTpo headers
#include <GTpo>

// QuickContainers headers
#include "../QuickContainers/src/qcmContainerModel.h"

// QuickQanava headers
#include "./qanNavigable.h"
#include "./qanContainerAdapter.h"

namespace qan { // ::qan

/*! \brief Exception thrown by QuickQanava to notify runtime error (nullptr assert, etc.).
 *
 * Use what() to have a detailled error description.
 */
class runtime_error : public std::runtime_error
{
public:
    explicit runtime_error (const std::string& what_arg) : std::runtime_error( what_arg ) { }
    explicit runtime_error (const char* what_arg) : std::runtime_error( what_arg ) { }
    runtime_error () : runtime_error( "QuickQanava runtime unrecoverable error." ) { }
};

class Node;
class Edge;
class Group;

class GraphConfig final : public gtpo::GraphConfig
{
public:
    using GraphBase = QQuickItem;
    using NodeBase  = QObject;
    using EdgeBase  = QObject;
    using GroupBase = QObject;

    using GraphBehaviours = std::tuple< gtpo::GraphGroupAjacentEdgesBehaviour< qan::GraphConfig > >;
    using GroupBehaviours = std::tuple< gtpo::GroupAdjacentEdgesBehaviour< qan::GraphConfig > >;

    template <typename T>
    using container_adapter = qan::ContainerAdapter<T>;

    using FinalNode     = qan::Node;
    using FinalEdge     = qan::Edge;
    using FinalGroup    = qan::Group;

    template <class ...Args>
    using NodeContainer = qcm::ContainerModel< QVector, Args... >;

    template <class ...Args>
    using EdgeContainer = qcm::ContainerModel< QVector, Args... >;

    template < class ...Args >
    using SearchContainer = QSet< Args... >;
};

} // ::qan

#endif // qanGraphConfig_h

