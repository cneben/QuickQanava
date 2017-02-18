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

// GTpo headers
#include <memory>   // FIXME 20160404: remove that later, memory was missing from gtpoBehaviour.h
#include "../GTpo/src/gtpoBehaviour.h"

// QuickQanava headers
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
    explicit NodeBehaviour( const std::string& name, QObject* parent = nullptr );
    virtual ~NodeBehaviour() { }
    NodeBehaviour( const NodeBehaviour& ) = delete;

    using WeakNode  = std::weak_ptr< qan::Config::Node >;

    virtual void    inNodeInserted( WeakNode& weakInNode ) noexcept override;
    virtual void    inNodeAboutToBeRemoved( WeakNode& weakInNode ) noexcept override;
    virtual void    inNodeRemoved() noexcept override { }

    virtual void    outNodeInserted( WeakNode& weakOutNode ) noexcept override;
    virtual void    outNodeRemoved( WeakNode& weakOutNode ) noexcept override;
    virtual void    outNodeRemoved() noexcept override { }

    virtual void    nodeModified( WeakNode& weakNode ) noexcept override;

protected:
    virtual void    inNodeInserted( qan::Node& inNode ) noexcept { Q_UNUSED( inNode ); }
    virtual void    inNodeAboutToBeRemoved( qan::Node& inNode ) noexcept { Q_UNUSED( inNode ); }

    virtual void    outNodeInserted( qan::Node& outNode ) noexcept { Q_UNUSED( outNode ); }
    virtual void    outNodeRemoved( qan::Node& outNode ) noexcept { Q_UNUSED( outNode ); }

    virtual void    nodeModified( qan::Node& node ) noexcept { Q_UNUSED( node ); }
};

} // ::qan

QML_DECLARE_TYPE( qan::NodeBehaviour )

#endif // qanBehaviour_h
