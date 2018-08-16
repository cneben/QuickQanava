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
// \file	algorithm.h
// \author	benoit@destrat.io
// \date	2018 07 05
//-----------------------------------------------------------------------------

#ifndef gtpo_algorithm_h
#define gtpo_algorithm_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <iterator>         // std::forward_iterator_tag
#include <unordered_map>

#include <stack>        // FIXME: move to .hpp...

// GTpo headers
#include "./utils.h"
#include "./config.h"
#include "./edge.h"
#include "./node.h"
#include "./group.h"

namespace gtpo { // ::gtpo

template <class config_t>
class graph;

/* Iterative Graph Traversal Algorithms *///-----------------------------------
/*! \brief Return a linearized DFS ordered version of a given tree.
 *
 */
template <class graph_t>
auto    linearize_dfs(const graph_t& graph) noexcept -> std::vector<typename graph_t::weak_node_t>;
//-----------------------------------------------------------------------------


/* Recursive Graph Traversal Algorithms *///-----------------------------------

/*! \brief Return true if a graph is an acyclic graph (DAG), ie it does not contains circuits.
 *
 *  \note is_dag_rec() will return true for an empty graph.
 *  \note is_dag_rec() will return true for a graph with only one vertice.
 *  \note complexity is at most O(n) with n beeing number of vertices in input \c graph.
 *  \note Mandatory static behaviours: none
 *  \note Recursive algorithm with no overflow protection.
 */
template <class graph_t>
auto    is_dag_rec(const graph_t& graph) noexcept -> bool;

/*! \brief Return true if a graph is actually a tree (ie it does not not contains circuits and nodes maximum indegree <= 1).
 *
 * Test if a given graph \c graph is a tree, ie if it is an acyclic directed graph where node in degree is at
 * most one. There is no complete connectivity test, so a forest is considered a tree (ie any sub graph of
 * graph "root nodes" is a tree).
 *
 *  \note is_tree_rec() will return true for an empty graph.
 *  \note is_tree_rec() will return true for a graph with only one vertice.
 *  \note complexity is at most O(2n) with n beeing number of vertices in input \c graph.
 *  \note Mandatory static behaviours: none
 *  \note Recursive algorithm with no overflow protection.
 */
template <class graph_t>
auto    is_tree_rec(const graph_t& graph) noexcept -> bool;

/*! \brief Compute and return graph depth.
 *
 *  FIXME: Warning, depend on graph root nodes (graph should bot contains circuits...)
 *
 *  \note recursive algorithm with no overflow protection.
 */
template <class graph_t>
auto    tree_depth_rec(const graph_t& graph) noexcept -> int;

/*! \brief Return a linearized DFS ordered version of a given tree.
 *
 *  FIXME: Warning, depend on graph root nodes (graph should bot contains circuits...)
 *
 *  \note recursive algorithm with no overflow protection.
 */
template <class graph_t>
auto    linearize_tree_dfs_rec(const graph_t& graph) noexcept -> std::vector<typename graph_t::weak_node_t>;

/*! \brief Return nodes ordered by their level in DFS order.
 *
 *  FIXME: Warning, depend on graph root nodes (graph should bot contains circuits...)
 *
 *  \note recursive algorithm with no overflow protection.
 */
template <class graph_t>
auto    levelize_tree_dfs_rec(const graph_t& graph) noexcept -> std::vector<std::vector<typename graph_t::weak_node_t > >;
//-----------------------------------------------------------------------------



/* BFS Graph Iterator *///-----------------------------------------------------

/*
 foraward_iterator_tag, standard:  http://www.cplusplus.com/reference/iterator/ForwardIterator/
    Is default-constructible, copy-constructible, copy-assignable and destructible	X a;  X b(a);  b = a;
    Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence).	a == b a != b
    Can be dereferenced as an rvalue (if in a dereferenceable state).	*a  a->m
    For mutable iterators (non-constant iterators):
    Can be dereferenced as an lvalue (if in a dereferenceable state).	*a = t
    Can be incremented (if in a dereferenceable state).
    The result is either also dereferenceable or a past-the-end iterator.
    Two iterators that compare equal, keep comparing equal when both are increased. ++a  a++  *a++
    Lvalues are swappable.	swap(a,b)
*/

template <class graph_t>
class bfs_iterator
{
public:
    using weak_node_t = typename graph_t::weak_node_t;

    using iterator_category = ::std::forward_iterator_tag;
    using value_type = weak_node_t;
    using difference_type = int;
    using pointer = weak_node_t*;
    using reference = int&;

    //! Create an invalid bfs_iterator.
    explicit bfs_iterator() { }

    enum class pos_tag_t { BEGIN = 0, END = 1 };

    explicit bfs_iterator(graph_t& graph, pos_tag_t pos_tag = pos_tag_t::BEGIN) :
        _nodes{&graph.get_nodes()}
    {
        if ( pos_tag == pos_tag_t::END ) {
            // Do nothing, _nodes is initialized, let all internal iterators invalid
        } else {
            _node_iter = graph.get_nodes().begin();
            // Note: reserve() must be called on _candidate_unmarked, otherwise, _candidate_unmarked_iter
            // is invalid at startup even after the call to begin()...
            _candidate_unmarked.reserve(std::max(1, static_cast<int>(graph.get_nodes().size() / 3)));
            _candidate_unmarked_iter = _candidate_unmarked.begin();
            operator++();
        }
    }

    bfs_iterator& operator++()
    {
        if ( _nodes == nullptr )
            return *this;

        if ( _stack.empty() )
            step();

        // stack is empty, _nodes and _candidate_unmarked have all been
        // consummed, set this iterator to end.
        if ( _stack.empty() &&
             ( _node_iter >= _nodes->end() &&
               _candidate_unmarked_iter >= _candidate_unmarked.end() ) ) {
            _node = weak_node_t{};
            return *this;
        }

        // FIXME FIXME FIXME !!!!!!
            // N'enregistrer que lorsque on a mark.insert, il peut y avoir plusieurs
            // node identiques dans la stack....
        bool node_found = false;
        while ( !node_found &&
             !_stack.empty() ) {
            auto node = _stack.top();
            _stack.pop();
            if ( _marks.find(node) == _marks.end() ) {
                _node = node;
                node_found = true;
                _marks.insert(node);
                push_sub_nodes_to_stack(node); // Push all current node out nodes on stack
            }
        }
        return *this;
    }

    void    push_sub_nodes_to_stack(weak_node_t& weak_node)
    {
        auto node = weak_node.lock();
        if ( !node )
            return;
        for ( auto& out_node : node->get_out_nodes() ) {
            if ( _marks.find(out_node) == _marks.end() )
                _stack.push(out_node);
        }
    }

    // Advance from step node element our iterative DFS: multiple
    // nodes may be enstacked during one step.
    void    step() {
        if ( _nodes == nullptr )
            return;

        const auto push_to_stack = [this](const weak_node_t& weak_node) -> void {
            auto node = weak_node.lock();
            if ( !node )
                return;
            _stack.push(weak_node);
        };

        if ( _node_iter < _nodes->end() ) {
            while ( _node_iter < _nodes->end() ) {      // Eat all marked nodes until we find an unmarked potential root/mother node
                if ( !*_node_iter )
                    continue;
                if ( _marks.find(*_node_iter) == _marks.end() ) {   // Skip already marked nodes
                    if ( (*_node_iter)->get_in_degree() == 0 ) {    // Feed root/mother nodes first
                        push_to_stack(*_node_iter);             // Push current node and it's out nodes to stack
                        _node_iter++;                           // Switch to next node for next call to feed_stack()
                        break;
                    } else
                        _candidate_unmarked.push_back(*_node_iter);
                }
                _node_iter++;                               // Switch to next node for next call to feed_stack()
            }
        } else if ( _candidate_unmarked_iter < _candidate_unmarked.end() ) { // all graph nodes have been consumed in a
                                                                            // first pass, consume unmarked nodes with non
                                                                            // zero indegree
            while ( _candidate_unmarked_iter < _candidate_unmarked.end() ) {
                if ( *_candidate_unmarked_iter &&
                     _marks.count(*_candidate_unmarked_iter) == 0 ) {
                    push_to_stack(*_candidate_unmarked_iter);         // Push current node and it's out nodes to stack
                    _candidate_unmarked_iter++;             // Switch to next node for next call to feed_stack()
                    break;
                }
                _candidate_unmarked_iter++;
            }
        }
    }

    /*! Dereference DFS iterator at current position
     *
     * \warning There is no out of range protection, but dereferencing an out of bound iterator does NOT lead
     * to undefined behaviour: an expired weak_ptr is returned.
     * \throw noexcept
     */
    value_type operator* () const
    {
        return _node;
    }

    bool operator==(const bfs_iterator<graph_t>& rhs) const
    {
        if ( _nodes != rhs._nodes )
            return false;   // Fast exit

        // 2 expired current node are considered equals
        if ( _node.expired() &&
             rhs._node.expired() )
            return true;
        if ( _node.expired() ||     // If one ptr is expired, they can't be equal (since they are not both nullptr, see previous check...)
             rhs._node.expired() )
            return false;
        return _nodes != rhs._nodes &&
               _node.lock().get() == rhs._node.lock().get();
    }

    bool operator!=(const bfs_iterator<graph_t>& rhs) const { return !(*this == rhs); }

private:
    typename graph_t::weak_node_t                      _node;
    typename graph_t::shared_nodes_t::const_iterator   _node_iter;

    const typename graph_t::shared_nodes_t*    _nodes = nullptr;
    std::vector<typename graph_t::shared_node_t>       _candidate_unmarked;
    typename std::vector<typename graph_t::shared_node_t>::const_iterator    _candidate_unmarked_iter;

    using mark_t = ::std::unordered_set<weak_node_t>;
    mark_t  _marks;

    ::std::stack<weak_node_t> _stack;
};

template <class graph_t>
auto    begin_dfs(graph_t& graph) noexcept -> bfs_iterator<graph_t>
{
    return bfs_iterator<graph_t>{graph, bfs_iterator<graph_t>::pos_tag_t::BEGIN};  // RVO
}

template <class graph_t>
auto    end_dfs(graph_t& graph) noexcept -> bfs_iterator<graph_t>
{
    static_cast<void>(graph);
    return bfs_iterator<graph_t>{graph, bfs_iterator<graph_t>::pos_tag_t::END};  // RVO
}
//-----------------------------------------------------------------------------

} // ::gtpo

#include "./algorithm.hpp"

#endif // gtpo_algorithm_h

