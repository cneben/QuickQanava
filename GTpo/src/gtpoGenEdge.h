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
// This file is a part of the GTpo software library.
//
// \file	gtpoGenEdge.h
// \author	benoit@destrat.io
// \date	2017 03 04
//-----------------------------------------------------------------------------

#ifndef gtpoGenEdge_h
#define gtpoGenEdge_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <functional>       // std::hash
#include <cassert>
#include <iterator>         // std::back_inserter

// GTpo headers
#include "./gtpoUtils.h"
#include "./gtpoBehaviour.h"
#include "./gtpoGraphConfig.h"

/*! \brief Main GTpo namespace (\#include \<GTpo\>).
 */
namespace gtpo { // ::gtpo

template <class Config>
class GenGraph;

template <class Config>
class GenNode;

template <class Config>
class GenGroup;


/*! Directed edge linking two nodes in a graph.
 *
 * \nosubgrouping
 */
template <class Config = GraphConfig>
class GenEdge : public Config::EdgeBase,
                public std::enable_shared_from_this<typename Config::FinalEdge>
{
    /*! \name Edge Construction *///-------------------------------------------
    //@{
    friend GenGraph<Config>;   // GenGraph need access to setGraph()
public:
    using Graph         = GenGraph<Config>;

    using WeakNode      = typename GenNode<Config>::Weak;
    using SharedNode    = typename GenNode<Config>::Shared;

    using Weak          = std::weak_ptr<typename Config::FinalEdge>;
    using Shared        = std::shared_ptr<typename Config::FinalEdge>;
    using WeakEdge      = Weak;
    using SharedEdge    = Shared;

    GenEdge() noexcept : Config::EdgeBase{} {}
    explicit GenEdge( const WeakNode& src, const WeakNode& dst ) :
        Config::EdgeBase{}, _src{ src }, _dst{ dst } { }
    explicit GenEdge( const WeakNode& src, const WeakEdge& hDst ) :
        Config::EdgeBase{}, _src{ src }, _hDst{ hDst } { }
    ~GenEdge() {
        if ( _graph != nullptr )
            std::cerr << "gtpo::GenEdge<>::~GenEdge(): Warning: an edge has been deleted before beeing " <<
                         "removed from the graph." << std::endl;
        _graph = nullptr;
    }
    GenEdge(const GenEdge& ) = delete;

protected:
    inline Graph*       getGraph() noexcept { return _graph; }
    inline const Graph* getGraph() const noexcept { return _graph; }
private:
    void                setGraph( Graph* graph ) { _graph = graph; }
public:
    Graph*              _graph{ nullptr };
    //@}
    //-------------------------------------------------------------------------

    /*! \name Edge Meta Properties *///----------------------------------------
    //@{
public:
    //! Get the edge current serializable property (false=not serializable, for example a control node).
    inline  auto    getSerializable( ) const -> bool { return _serializable; }
    //! Shortcut to getSerializable().
    inline  auto    isSerializable( ) const -> bool { return getSerializable(); }
    //! Change the edge serializable property (it will not trigger an edge changed call in graph behaviour).
    inline  auto    setSerializable( bool serializable ) -> void { _serializable = serializable; }
private:
    //! Edge serializable property (default to true ie serializable).
    bool            _serializable = true;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Source / Destination Management *///-----------------------------
    //@{
public:
    inline auto setSrc( WeakNode src ) noexcept -> void { _src = src; }
    inline auto setDst( WeakNode dst ) noexcept -> void { _dst = dst; }
    inline auto getSrc( ) noexcept -> WeakNode& { return _src; }
    inline auto getSrc( ) const noexcept -> const WeakNode& { return _src; }
    inline auto getDst( ) noexcept -> WeakNode& { return _dst; }
    inline auto getDst( ) const noexcept -> const WeakNode& { return _dst; }
private:
    WeakNode    _src;
    WeakNode    _dst;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Restricted Hyper Edge Management *///----------------------------
    //@{
public:
    using WeakEdges     = typename Config::template EdgeContainer< WeakEdge >;
    using WeakNodes     = typename Config::template NodeContainer< WeakNode >;

    inline auto setHDst( WeakEdge hDst ) noexcept -> void { _hDst = hDst; }
    inline auto getHDst() const noexcept -> const WeakEdge& { return _hDst; }
    inline auto getInHEdges() const noexcept -> const WeakEdges& { return _inHEdges; }
    inline auto addInHEdge( WeakEdge inHEdge ) -> void;
    inline auto removeInHEdge( WeakEdge inHEdge ) -> void;
    inline auto getInHDegree() const noexcept -> int { return static_cast<int>( _inHEdges.size() ); }

    inline auto getInHNodes() const noexcept -> const WeakNodes& { return _inHNodes; }

protected:
    inline auto getInHEdges() noexcept -> WeakEdges& { return _inHEdges; }
private:
    //! Restricted hyper edge destination (ie this edge target another edge as destination).
    WeakEdge    _hDst;
    //! Restricted in hyper edges (ie an in hyper edge with this edge as a destination).
    WeakEdges   _inHEdges;
    //! Restricted hyper in nodes (ie all source node for in restricted hyper edges).
    WeakNodes   _inHNodes;
    //@}
    //-------------------------------------------------------------------------
};

template <class Config = GraphConfig>
class GenGroupEdge : public gtpo::GenEdge<Config>
{

};

} // ::gtpo

#endif // gtpoGenEdge_h

