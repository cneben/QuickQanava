/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

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
// \file	qanBehaviour.h
// \author	benoit@qanava.org
// \date	2016 04 04
//-----------------------------------------------------------------------------

#ifndef qanBehaviour_h
#define qanBehaviour_h

// QT headers
#include <QObject>

// GTpo headers
#include <memory>   // FIXME 20160404: remove that later, memory was missing from gtpoBehaviour.h
#include "../GTpo/src/gtpoBehaviour.h"

// Qanava headers
#include "./qanConfig.h"

namespace qan { // ::qan

/*! \brief Define node behaviours reacting to change in node (QuickQanava adapter for gtpo::NodeBehaviour<>).
 *
 *  \nosubgrouping
 */
class NodeBehaviour : public QObject,
                      public gtpo::NodeBehaviour< qan::Config >
{
    Q_OBJECT
public:
    NodeBehaviour( QObject* parent = nullptr );
    virtual ~NodeBehaviour() { }

    using WeakNode  = std::weak_ptr< qan::Config::Node >;

    virtual auto    inNodeInserted( WeakNode& weakInNode ) -> void;
    virtual auto    inNodeRemoved( WeakNode& weakInNode ) -> void;

    virtual auto    outNodeInserted( WeakNode& weakOutNode ) -> void;
    virtual auto    outNodeRemoved( WeakNode& weakOutNode ) -> void;

    virtual auto    nodeModified( WeakNode& weakNode ) -> void;

protected:
    virtual auto    inNodeInserted( qan::Node& inNode ) -> void { Q_UNUSED( inNode ); }
    virtual auto    inNodeRemoved( qan::Node& inNode ) -> void { Q_UNUSED( inNode ); }

    virtual auto    outNodeInserted( qan::Node& outNode ) -> void { Q_UNUSED( outNode ); }
    virtual auto    outNodeRemoved( qan::Node& outNode ) -> void { Q_UNUSED( outNode ); }

    virtual auto    nodeModified( qan::Node& node ) -> void { Q_UNUSED( node ); }
};

} // ::qan

QML_DECLARE_TYPE( qan::NodeBehaviour )

#endif // qanBehaviour_h
