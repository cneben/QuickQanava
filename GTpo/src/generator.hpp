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
// \file	generator.hpp
// \author	benoit@destrat.io
// \date	2018 07 12
//-----------------------------------------------------------------------------

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared

// GTpo headers
#include "./utils.h"
#include "./config.h"
#include "./edge.h"
#include "./node.h"
#include "./group.h"

namespace gtpo { // ::gtpo

/* Tree Generation Algorithms *///---------------------------------------------
namespace impl { // ::gtpo::impl

template <class graph_t, class node_t>
auto complete_tree_impl_rec(graph_t& tree, node_t& node, const int depth, const int degree) -> void
{
    for ( int d = 0; d < degree; ++d ) {
        auto sub_tree = tree.create_node();
        tree.create_edge(node, sub_tree);

        if ( depth - 1 > 0 )   // Stop recursion condition
           complete_tree_impl_rec(tree, sub_tree, depth - 1, degree);
    }
}

} // ::gtpo::impl

template <class graph_t>
auto complete_tree_rec(graph_t& graph, const int depth, const int degree) -> void
{
    // PRECONDITIONS:
        // graph must be empty.
        // depth must be >= 0.
        // degree must be >= 1.
    if (!graph.get_node_count() == 0) {
        std::cerr << "gtpo::complete_tree_rec<>(): graph must be empty." << std::endl;
        return;
    }
    if (depth < 0 ) {
        std::cerr << "gtpo::complete_tree_rec<>(): depth should be positive." << std::endl;
        return;
    }
    if (degree < 1 ) {
        std::cerr << "gtpo::complete_tree_rec<>(): degree must be >= 1." << std::endl;
        return;
    }

    auto root = graph.create_node();   // Create root node
    if ( depth == 0 )
        return;                     // A tree with only one root node has depth 0...
    for ( int d = 0; d < degree; ++d) {
        auto sub_tree = graph.create_node();
        graph.create_edge(root, sub_tree);
        if ( depth - 1 > 0 )
            impl::complete_tree_impl_rec(graph, sub_tree, depth - 1, degree);
    }
}
//-----------------------------------------------------------------------------


/* Graph Generation Algorithms *///--------------------------------------------
template <class graph_t>
auto    complete_graph(const int n) noexcept -> graph_t
{
    static_cast<void>(n);
}

template <class graph_t>
auto    gnp_random_graph(const int n, const double p) noexcept -> graph_t
{
    static_cast<void>(n);
    static_cast<void>(p);
}
//-----------------------------------------------------------------------------

} // ::gtpo


