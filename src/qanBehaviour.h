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
// \file	qanBehaviour.h
// \author	benoit@destrat.io
// \date	2016 04 04
//-----------------------------------------------------------------------------

#ifndef qanBehaviour_h
#define qanBehaviour_h

// Qt headers
#include <QObject>
#include <QQuickItem>

// GTpo headers
#include "../GTpo/src/gtpoNodeBehaviour.h"

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanEdge.h"

namespace qan { // ::qan

/*! \brief Define node behaviour interface to react to node topology events (QuickQanava adapter for gtpo::NodeBehaviour<>).
 *
 *  \nosubgrouping
 */
class NodeBehaviour : public QObject,
                      public gtpo::NodeBehaviour< qan::GraphConfig >
{
    Q_OBJECT
public:
    explicit NodeBehaviour( const std::string& name, QObject* parent = nullptr );
    virtual ~NodeBehaviour() { }
    NodeBehaviour( const NodeBehaviour& ) = delete;

    using WeakNode  = gtpo::NodeBehaviour< qan::GraphConfig >::WeakNode;
    using WeakEdge  = gtpo::NodeBehaviour< qan::GraphConfig >::WeakEdge;

    //! \copydoc gtpo::NodeBehaviour::inNodeInserted()
    virtual void    inNodeInserted( WeakNode& weakInNode, const WeakEdge& edge ) noexcept override;
    //! \copydoc gtpo::NodeBehaviour::inNodeRemoved()
    virtual void    inNodeRemoved( WeakNode& weakInNode, const WeakEdge& edge ) noexcept override;
    //! \copydoc gtpo::NodeBehaviour::inNodeRemoved()
    virtual void    inNodeRemoved() noexcept override { }

    //! \copydoc gtpo::NodeBehaviour::outNodeInserted()
    virtual void    outNodeInserted( WeakNode& weakOutNode, const WeakEdge& edge  ) noexcept override;
    //! \copydoc gtpo::NodeBehaviour::outNodeRemoved()
    virtual void    outNodeRemoved( WeakNode& weakOutNode, const WeakEdge& edge  ) noexcept override;
    //! \copydoc gtpo::NodeBehaviour::outNodeRemoved()
    virtual void    outNodeRemoved() noexcept override { }

protected:
    virtual void    inNodeInserted( qan::Node& inNode, qan::Edge& edge ) noexcept { Q_UNUSED( inNode ); Q_UNUSED(edge); }
    virtual void    inNodeRemoved( qan::Node& inNode, qan::Edge& edge  ) noexcept { Q_UNUSED( inNode ); Q_UNUSED(edge); }

    virtual void    outNodeInserted( qan::Node& outNode, qan::Edge& edge  ) noexcept { Q_UNUSED( outNode ); Q_UNUSED(edge); }
    virtual void    outNodeRemoved( qan::Node& outNode, qan::Edge& edge  ) noexcept { Q_UNUSED( outNode ); Q_UNUSED(edge); }
};

} // ::qan

QML_DECLARE_TYPE( qan::NodeBehaviour )

#endif // qanBehaviour_h
