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
template <class config_t = gtpo::default_config>
class edge : public config_t::edge_base,
                public std::enable_shared_from_this<typename config_t::final_edge_t>
{
    /*! \name Edge Construction *///-------------------------------------------
    //@{
    friend graph<config_t>;   // graph need access to set_graph()
public:
    using graph_t       = graph<config_t>;

    using weak_node_t   = std::weak_ptr<typename config_t::final_node_t>;
    using shared_node_t = std::shared_ptr<typename config_t::final_node_t>;

    using weak_edge_t   = std::weak_ptr<typename config_t::final_edge_t>;
    using shared_edge_t = std::shared_ptr<typename config_t::final_edge_t>;

    edge() noexcept : config_t::edge_base{} {}
    explicit edge( const weak_node_t& src, const weak_node_t& dst ) :
        config_t::edge_base{}, _src{ src }, _dst{ dst } { }
    ~edge() {
        if ( _graph != nullptr )
            std::cerr << "gtpo::edge<>::~edge(): Warning: an edge has been deleted before beeing " <<
                         "removed from the graph." << std::endl;
        _graph = nullptr;
    }
    edge(const edge& ) = delete;

public:
    inline graph_t*         get_graph() noexcept { return _graph; }
    inline const graph_t*   get_graph() const noexcept { return _graph; }
private:
    void                    set_graph( graph_t* graph ) { _graph = graph; }
public:
    graph_t*                _graph{ nullptr };
    //@}
    //-------------------------------------------------------------------------

    /*! \name Edge Meta Properties *///----------------------------------------
    //@{
public:
    //! Get the edge current serializable property (false=not serializable, for example a control node).
    inline  auto    get_serializable( ) const -> bool { return _serializable; }
    //! Shortcut to get_serializable().
    inline  auto    is_serializable( ) const -> bool { return get_serializable(); }
    //! Change the edge serializable property (it will not trigger an edge changed call in graph behaviour).
    inline  auto    set_serializable( bool serializable ) -> void { _serializable = serializable; }
private:
    //! Edge serializable property (default to true ie serializable).
    bool            _serializable = true;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Source / Destination Management *///-----------------------------
    //@{
public:
    inline auto set_src( weak_node_t src ) noexcept -> void { _src = src; }
    inline auto set_dst( weak_node_t dst ) noexcept -> void { _dst = dst; }
    inline auto get_src( ) noexcept -> weak_node_t& { return _src; }
    inline auto get_src( ) const noexcept -> const weak_node_t& { return _src; }
    inline auto get_dst( ) noexcept -> weak_node_t& { return _dst; }
    inline auto get_dst( ) const noexcept -> const weak_node_t& { return _dst; }
private:
    weak_node_t _src;
    weak_node_t _dst;
    //@}
    //-------------------------------------------------------------------------
};

template <class config_t = gtpo::config<>>
class group_edge : public gtpo::edge<config_t>
{

};

} // ::gtpo

#endif // gtpo_edge_h

