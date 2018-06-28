/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
// \file	edge.h
// \author	benoit@destrat.io
// \date	2017 03 04
//-----------------------------------------------------------------------------

#ifndef gtpo_edge_h
#define gtpo_edge_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <functional>       // std::hash
#include <cassert>
#include <iterator>         // std::back_inserter

// GTpo headers
#include "./utils.h"
#include "./behaviour.h"
#include "./config.h"

/*! \brief Main GTpo namespace (\#include \<GTpo\>).
 */
namespace gtpo { // ::gtpo

template <class config>
class graph;

template <class config>
class node;

template <class config>
class group;


/*! Directed edge linking two nodes in a graph.
 *
 * \nosubgrouping
 */
template <class config_t = gtpo::config>
class edge : public config_t::edge_base,
                public std::enable_shared_from_this<typename config_t::final_edge_t>
{
    /*! \name Edge Construction *///-------------------------------------------
    //@{
    friend graph<config_t>;   // graph need access to setGraph()
public:
    using graph_t       = graph<config_t>;

    using weak_node     = typename gtpo::node<config_t>::weak;
    using shared_node   = typename gtpo::node<config_t>::shared;

    using weak          = std::weak_ptr<typename config_t::final_edge_t>;
    using shared        = std::shared_ptr<typename config_t::final_edge_t>;
    using weak_edge     = weak;
    using shared_edge   = shared;

    edge() noexcept : config_t::edge_base{} {}
    explicit edge( const weak_node& src, const weak_node& dst ) :
        config_t::edge_base{}, _src{ src }, _dst{ dst } { }
    explicit edge( const weak_node& src, const weak_edge& hDst ) :
        config_t::edge_base{}, _src{ src }, _hDst{ hDst } { }
    ~edge() {
        if ( _graph != nullptr )
            std::cerr << "gtpo::edge<>::~edge(): Warning: an edge has been deleted before beeing " <<
                         "removed from the graph." << std::endl;
        _graph = nullptr;
    }
    edge(const edge& ) = delete;

public:
    inline graph_t*         getGraph() noexcept { return _graph; }
    inline const graph_t*   getGraph() const noexcept { return _graph; }
private:
    void                    setGraph( graph_t* graph ) { _graph = graph; }
public:
    graph_t*                _graph{ nullptr };
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
    inline auto setSrc( weak_node src ) noexcept -> void { _src = src; }
    inline auto setDst( weak_node dst ) noexcept -> void { _dst = dst; }
    inline auto getSrc( ) noexcept -> weak_node& { return _src; }
    inline auto getSrc( ) const noexcept -> const weak_node& { return _src; }
    inline auto getDst( ) noexcept -> weak_node& { return _dst; }
    inline auto getDst( ) const noexcept -> const weak_node& { return _dst; }
private:
    weak_node    _src;
    weak_node    _dst;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Restricted Hyper Edge Management *///----------------------------
    //@{
public:
    using weak_edges     = typename config_t::template edge_container_t< weak_edge >;
    using weak_nodes     = typename config_t::template node_container_t< weak_node >;

    inline auto setHDst( weak_edge hDst ) noexcept -> void { _hDst = hDst; }
    inline auto getHDst() const noexcept -> const weak_edge& { return _hDst; }
    inline auto getInHEdges() const noexcept -> const weak_edges& { return _inHEdges; }
    inline auto addInHEdge( weak_edge inHEdge ) -> void;
    inline auto removeInHEdge( weak_edge inHEdge ) -> void;
    inline auto getInHDegree() const noexcept -> int { return static_cast<int>( _inHEdges.size() ); }

    inline auto getInHNodes() const noexcept -> const weak_nodes& { return _inHNodes; }

protected:
    inline auto getInHEdges() noexcept -> weak_edges& { return _inHEdges; }
private:
    //! Restricted hyper edge destination (ie this edge target another edge as destination).
    weak_edge    _hDst;
    //! Restricted in hyper edges (ie an in hyper edge with this edge as a destination).
    weak_edges   _inHEdges;
    //! Restricted hyper in nodes (ie all source node for in restricted hyper edges).
    weak_nodes   _inHNodes;
    //@}
    //-------------------------------------------------------------------------
};

template <class config_t = gtpo::config>
class group_edge : public gtpo::edge<config_t>
{

};

} // ::gtpo

#endif // gtpo_edge_h

