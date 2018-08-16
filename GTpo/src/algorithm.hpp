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
// \file	algorithm.hpp
// \author	benoit@destrat.io
// \date	2018 07 04
//-----------------------------------------------------------------------------

// STD headers
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <stack>

namespace gtpo { // ::gtpo

/* Iterative Graph Traversal Algorithms *///-----------------------------------
namespace impl { // ::gtpo::impl

template <class weak_node_t, class marks_t>
auto    linearize_dfs_impl(weak_node_t n, marks_t& marks, std::vector<weak_node_t>& r) noexcept
{
    // Algorithm:
    // Iterative DFS (Wikipedia): https://en.wikipedia.org/wiki/Depth-first_search
    // 1  procedure DFS-iterative(G,v):
    // 2      let S be a stack
    // 3      S.push(v)
    // 4      while S is not empty
    // 5          v = S.pop()
    // 6          if v is not labeled as discovered:
    // 7              label v as discovered
    // 8              for all edges from v to w in G.adjacentEdges(v) do
    // 9                  S.push(w)

    std::stack<weak_node_t> s;
    s.push(n);
    while ( !s.empty() ) {                                      // 4.
        auto n = s.top();
        s.pop();
        if (marks.find(n) == marks.end() ) {
            marks.insert(n);                                    // 7.
            r.push_back(n);
            // Push all n out nodes
            auto n_ptr = n.lock();
            for ( auto& out_node : n_ptr->get_out_nodes() ) {   // 8.
                if ( marks.count(out_node) == 0 )
                    s.push(out_node);                           // 9.
            }
        }
    }
}

} // ::gtpo::impl

template <class graph_t>
auto    linearize_dfs(const graph_t& graph) noexcept -> std::vector<typename graph_t::weak_node_t>
{
    using weak_node_t = typename graph_t::weak_node_t;
    using shared_node_t = typename graph_t::shared_node_t;
    using mark_t = std::unordered_set<typename graph_t::weak_node_t>;
    mark_t marks;
    std::vector<weak_node_t> r;
    std::vector<shared_node_t> candidate_unmarked;

    // We apply a "weak" heuristic for DFS: node with 0 indegree (potential root or mother nodes) are
    // handled first.
        // All unmarked or non probable root or mother nodes are traversed in a second pass.
    for ( auto& n : graph.get_nodes())
        if ( n &&
             n->get_in_degree() == 0 &&
             marks.count(n) == 0 )
            impl::linearize_dfs_impl( weak_node_t{n}, marks, r);
        else
            candidate_unmarked.push_back(n);

    for ( auto& n : candidate_unmarked )    // Second pass for non marker (potentially unconnected?) or non DAG connex component
        if ( n  &&
             marks.count(n) == 0 )
            impl::linearize_dfs_impl( weak_node_t{n}, marks, r);

    return r;
}
//-----------------------------------------------------------------------------


/* Recursive Graph Traversal Algorithms *///-----------------------------------

namespace impl { // ::gtpo::impl

template <class mark_t, class node_t>
auto    is_dag_impl_rec(mark_t& marks, const node_t& node) noexcept -> bool
{
    // PRECONDITIONS:
        // node must be lockable
    const auto& node_ptr = node.lock();
    if ( node_ptr ) {
        if (marks.find(node) != marks.end())
            return false;
        marks.insert(node);
        for ( const auto& out_node : node_ptr->get_out_nodes())
            if (!is_dag_impl_rec(marks, out_node))
                return false;
    }
    return true;
}

} // ::gtpo::impl

template <class graph_t>
auto    is_dag_rec(const graph_t& graph) noexcept -> bool
{
    // PRECONDITIONS: none
    using mark_t = std::unordered_set<typename graph_t::weak_node_t>;
    mark_t marks;
    for ( const auto& node : graph.get_nodes()) {
        if ( marks.find(node) == marks.end() &&
             !impl::is_dag_impl_rec(marks, typename graph_t::weak_node_t{node}))
            return false;
    }
    return true;
}

namespace impl { // ::gtpo::impl

template <class mark_t, class node_t>
auto    is_tree_impl_rec(mark_t& marks, const node_t& node) noexcept -> bool
{
    // PRECONDITIONS:
        // node must be lockable
    const auto& node_ptr = node.lock();
    if ( node_ptr ) {
        if (node_ptr->get_in_nodes().size() > 1)
            return false;
        if (marks.find(node) != marks.end())
            return false;
        marks.insert(node);
        for ( const auto& out_node : node_ptr->get_out_nodes())
            if (!is_tree_impl_rec(marks, out_node))
                return false;
    }
    return true;
}

} // ::gtpo::impl

template <class graph_t>
auto    is_tree_rec(const graph_t& graph) noexcept -> bool
{
    // PRECONDITIONS: none
    using mark_t = std::unordered_set<typename graph_t::weak_node_t>;
    mark_t marks;
    for ( const auto& node : graph.get_nodes()) {
        if ( marks.find(node) == marks.end() &&
             !impl::is_tree_impl_rec(marks, typename graph_t::weak_node_t{node}))
            return false;
    }
    return true;
}


namespace impl { // ::gtpo::impl

template <class node_t>
auto    tree_depth_impl_rec(const node_t& node, int d = 0) noexcept -> int
{
    // PRECONDITIONS:
        // node must be lockable
    const auto& node_ptr = node.lock();
    if ( node_ptr ) {
        d += 1;
        for ( const auto& out_node : node_ptr->get_out_nodes())
            d = std::max(d, tree_depth_impl_rec(out_node, d));
    }
    return d;
}

} // ::gtpo::impl

template <class graph_t>
auto    tree_depth_rec(const graph_t& graph) noexcept -> int
{
    // PRECONDITIONS: none
    int r = 0;
    for ( const auto& root_node : graph.get_root_nodes())
        r = std::max(r, impl::tree_depth_impl_rec(root_node));
    return r;
}


namespace impl { // ::gtpo::impl

template <class nodes_t, class node_t>
auto    linearize_tree_dfs_impl_rec(nodes_t& nodes, const node_t& node) noexcept -> void
{
    // PRECONDITIONS:
        // node must be lockable
    const auto& node_ptr = node.lock();
    if ( !node_ptr )
        return;

    nodes.push_back(node);
    for ( const auto& out_node : node_ptr->get_out_nodes())
        linearize_tree_dfs_impl_rec(nodes, out_node);
}

} // ::gtpo::impl

template <class graph_t>
auto    linearize_tree_dfs_rec(const graph_t& graph) noexcept -> std::vector<typename graph_t::weak_node_t>
{
    // PRECONDITIONS: none
    std::vector<typename graph_t::weak_node_t> r;
    r.reserve(graph.get_node_count());
    for ( const auto& root_node : graph.get_root_nodes())
        impl::linearize_tree_dfs_impl_rec(r, root_node);
    return r;   // RVO
}


namespace impl { // ::gtpo::impl

template <class levels_t, class node_t>
auto    levelize_tree_dfs_impl_rec(levels_t& r, const node_t& node, int l) noexcept -> void
{
    // PRECONDITIONS:
        // node must be lockable
    const auto& node_ptr = node.lock();
    if ( !node_ptr )
        return;
    if ( static_cast<int>(r.size()) < l + 1 )   // Eventually, add a new vector of node for level l
        r.emplace_back(std::vector<node_t >{});
    r[l].push_back(node);   // Note: node is const, non moveable
    for ( const auto& out_node : node_ptr->get_out_nodes())
        levelize_tree_dfs_impl_rec(r, out_node, l + 1);
}

} // ::gtpo::impl

template <class graph_t>
auto    levelize_tree_dfs_rec(const graph_t& graph) noexcept -> std::vector<std::vector<typename graph_t::weak_node_t > >
{
    // PRECONDITIONS: none
    std::vector<std::vector<typename graph_t::weak_node_t > > r;
    for ( const auto& root_node : graph.get_root_nodes() ) {
        impl::levelize_tree_dfs_impl_rec(r, root_node, 0);
    }
    return r; // RVO
}
//-----------------------------------------------------------------------------


/* BFS Graph Iterator *///-----------------------------------------------------
/*template <class graph_t>
auto    begin_bfs(graph_t& graph) noexcept -> bfs_iterator<typename graph_t::weak_node_t>
{
    return bfs_iterator<typename graph_t::weak_node_t>{};
}

*/
//-----------------------------------------------------------------------------


} // ::gtpo

