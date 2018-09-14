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
// \file	node.h
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

#ifndef gtpo_node_h
#define gtpo_node_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <functional>       // std::hash
#include <cassert>
#include <iterator>         // std::back_inserter

// GTpo headers
#include "./utils.h"
#include "./config.h"
#include "./graph_property.h"
#include "./node_behaviour.h"

namespace gtpo { // ::gtpo

template <class config_t>
class graph;

template <class config_t>
class group;

template <class config_t>
class edge;

/*! \brief Base class for modelling nodes with an in/out edges list in a gtpo::graph graph.
 *
 * \nosubgrouping
 */
template <class config_t = gtpo::config<>>
class node : public config_t::node_base,
             public graph_property_impl<gtpo::graph<config_t>>,
             public gtpo::behaviourable_node< config_t >,
             public std::enable_shared_from_this<node<config_t>>
             //Note: following don't work : public std::enable_shared_from_this<typename config_t::final_node_t>
                // It prevent node and group from having a common ancestor...
{
    friend gtpo::graph<config_t>;   // graph need access to graph_property_impl<>::set_graph()

    /*! \name Node Management *///---------------------------------------------
    //@{
public:
    using graph_t       = gtpo::graph<config_t>;
    using weak_node_t     = std::weak_ptr<typename config_t::final_node_t>;
    using shared_node_t   = std::shared_ptr<typename config_t::final_node_t>;
    using weak_nodes_t    = typename config_t::template node_container_t< weak_node_t >;

    //! User friendly shortcut type to this concrete node Behaviourable base type.
    using behaviourable_base = gtpo::behaviourable_node< config_t >;

    node() noexcept : config_t::node_base{} { }
    ~node() noexcept {
        _inEdges.clear(); _outEdges.clear();
        _inNodes.clear(); _outNodes.clear();
        if ( this->_graph != nullptr ) {
            std::cerr << "gtpo::node<>::~node(): Warning: Node has been destroyed before beeing removed from the graph." << std::endl;
        }
        this->_graph = nullptr;
    }
    /*! \brief Default copy constructor does _nothing_.
     *
     * \note Node topology is not copied since a node identity from a topology point of
     * view is it's pointer adress. Copy constructor is defined to allow copying node
     * properties, topology (in/out edges and group membership is not copied), see documentation
     * for a more detailled description of restrictions on move and copy.
     */
    node(const node& node ) {
        static_cast<void>(node);
    }
    node& operator=( node const& ) = delete;

    inline auto     add_dynamic_node_behaviour( std::unique_ptr<gtpo::dynamic_node_behaviour<config_t>> behaviour ) -> void {
        if (behaviour)
            behaviour->set_target(this->shared_from_this());
        behaviourable_base::add_behaviour(std::move(behaviour));
    }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node Edges Management *///---------------------------------------
    //@{
public:
    using weak_edge_t     = std::weak_ptr<typename config_t::final_edge_t>;
    using shared_edge_t   = std::shared_ptr<typename config_t::final_edge_t>;
    using weak_edges_t    = typename config_t::template edge_container_t< weak_edge_t >;

    /*! \brief Insert edge \c outEdge as an out edge for this node.
     *
     * \note if \c outEdge source node is different from this node, it is set to this node.
     */
    auto    add_out_edge( weak_edge_t sharedOutEdge ) noexcept( false ) -> void;
    /*! \brief Insert edge \c inEdge as an in edge for \c node.
     *
     * \note if \c inEdge destination node is different from \c node, it is automatically set to \c node.
     */
    auto    add_in_edge( weak_edge_t sharedInEdge ) noexcept( false ) -> void;
    /*! \brief Remove edge \c outEdge from this node out edges.
     *
     * \throw gtpo::bad_topology_error
     */
    auto    remove_out_edge( const weak_edge_t outEdge ) noexcept( false ) -> void;
    /*! \brief Remove edge \c inEdge from this node in edges.
     *
     * \throw gtpo::bad_topology_error
     */
    auto    remove_in_edge( const weak_edge_t inEdge ) noexcept( false ) -> void;

    inline auto     get_in_edges() const noexcept -> const weak_edges_t& { return _inEdges; }
    inline auto     get_out_edges() const noexcept -> const weak_edges_t& { return _outEdges; }

    inline auto     get_in_nodes() const noexcept -> const weak_nodes_t& { return _inNodes; }
    inline auto     get_out_nodes() const noexcept -> const weak_nodes_t& { return _outNodes; }

    inline auto     get_in_degree() const noexcept -> unsigned int { return static_cast<int>( _inEdges.size() ); }
    inline auto     get_out_degree() const noexcept -> unsigned int { return static_cast<int>( _outEdges.size() ); }
private:
    weak_edges_t       _inEdges;
    weak_edges_t       _outEdges;
    weak_nodes_t       _inNodes;
    weak_nodes_t       _outNodes;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node Edges Management *///---------------------------------------
    //@{
public:
    inline auto set_group( const std::weak_ptr<typename config_t::final_group_t>& group ) noexcept -> void { _group = group; }
    inline auto get_group( ) noexcept -> std::weak_ptr<typename config_t::final_group_t>& { return _group; }
    inline auto get_group( ) const noexcept -> const std::weak_ptr<typename config_t::final_group_t>& { return _group; }
private:
    std::weak_ptr<typename config_t::final_group_t> _group;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#endif // gtpo_node_h

