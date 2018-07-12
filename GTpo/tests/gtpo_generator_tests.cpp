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
// This file is a part of the GTpo software.
//
// \file	gtpo_generator_tests.cpp
// \author	benoit@qanava.org
// \date	2018 07 12
//-----------------------------------------------------------------------------

// STD headers
#include <list>
#include <memory>
#include <iostream>

// GTpo headers
#include <GTpo>
#include <../src/generator.h>

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//-----------------------------------------------------------------------------
// gtpo::complete_tree_rec
//-----------------------------------------------------------------------------

TEST(GTpoGenerator, complete_tree_rec)
{
    // Signature:
    // template <class graph_t>
    // auto complete_tree_rec(graph_t& graph, const int depth, const int degree = 2) -> void;

    // Definition:
    // Depth: The depth of a node is the number of edges from the tree's root node to the node.
    // See: https://en.wikipedia.org/wiki/Tree_(data_structure)

    {
        // depth=0
        // degree=0
        gtpo::graph<> t;
        // EXPECT: empty graph, degree == 0 is an invalid argument
        gtpo::complete_tree_rec<gtpo::graph<>>(t, 0, 0);
        EXPECT_TRUE(t.is_empty());
    }
    {
        // depth=0
        // degree=1
        gtpo::graph<> t;
        gtpo::complete_tree_rec<gtpo::graph<>>(t, 0, 1);
        EXPECT_EQ(t.get_node_count(), 1);
    }

    {
        // depth = 1
        // degree = 0
        gtpo::graph<> t;
        // EXPECT: empty graph, degree == 0 is an invalid argument
        gtpo::complete_tree_rec<gtpo::graph<>>(t, 1, 0);
        EXPECT_EQ(t.get_node_count(), 0);
    }

    {
        // depth = 1
        // degree = 1
        gtpo::graph<> t;
        // Expect: t = { [n0, n1], [(n0->n1)] }
        gtpo::complete_tree_rec<gtpo::graph<>>(t, 1, 1);
        ASSERT_EQ(t.get_node_count(), 2);
        ASSERT_EQ(t.get_edge_count(), 1);
    }

    {
        // depth = 1
        // degree = 2
        gtpo::graph<> t;
        // Expect: t = { [n0, n1, n2], [(n0->n1, (n0->n2))] }
        gtpo::complete_tree_rec<gtpo::graph<>>(t, 1, 2);
        ASSERT_EQ(t.get_node_count(), 3);
        ASSERT_EQ(t.get_edge_count(), 2);
        const auto& nodes = t.get_nodes();
        EXPECT_TRUE(t.has_edge(nodes[0], nodes[1]));
        EXPECT_TRUE(t.has_edge(nodes[0], nodes[2]));
    }

    {
        // depth = 2
        // degree = 2
        gtpo::graph<> t;
        gtpo::complete_tree_rec<gtpo::graph<>>(t, 2, 2);
        ASSERT_EQ(t.get_node_count(), 7);
        ASSERT_EQ(t.get_edge_count(), 6);
    }

    {
        // depth = 2
        // degree = 3
        gtpo::graph<> t;
        gtpo::complete_tree_rec<gtpo::graph<>>(t, 2, 3);
        ASSERT_EQ(t.get_node_count(), 13);
        ASSERT_EQ(t.get_edge_count(), 12);
    }
}
