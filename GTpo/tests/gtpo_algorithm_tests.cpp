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
// \file	gtpo_algorithm_tests.cpp
// \author	benoit@qanava.org
// \date	2018 06 19
//-----------------------------------------------------------------------------

// STD headers
#include <list>
#include <memory>
#include <iostream>

// GTpo headers
#include <GTpo>
#include <../src/algorithm.h>
#include <../src/functional.h>

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>


//-----------------------------------------------------------------------------
// Graph is_dag()
//-----------------------------------------------------------------------------

TEST(GTpoGraph, is_dag_rec)
{
    { // Basic positive tree test (empty graph)
        gtpo::graph<> g;
        EXPECT_TRUE(gtpo::is_dag_rec(g));

        auto n1 = g.create_node();
        EXPECT_TRUE(gtpo::is_dag_rec(g));

        auto n2 = g.create_node();
        g.create_edge(n1, n2);
        EXPECT_TRUE(gtpo::is_dag_rec(g));

        g.clear();
        EXPECT_TRUE(gtpo::is_dag_rec(g));
    }

    { // Basic positive test
        // g = { [n1, n2, n3], [(n1 -> n2), (n1 -> n3)] }
        // Expect: true
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        g.create_edge(n1, n2);
        g.create_edge(n1, n3);
        EXPECT_TRUE(gtpo::is_dag_rec(g));
    }

    {   // g = { [n1, n2, n3], [(n1 -> n3), (n2 -> n3)] }
        // Expect: true
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        g.create_edge(n1, n3);
        g.create_edge(n2, n3);
        EXPECT_TRUE(gtpo::is_dag_rec(g));
    }

    { // Circuit == non DAG
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        g.create_edge(n1, n2);
        g.create_edge(n2, n1);
        EXPECT_FALSE(gtpo::is_dag_rec(g));
    }
}


//-----------------------------------------------------------------------------
// Graph is_tree()
//-----------------------------------------------------------------------------

TEST(GTpoGraph, is_tree_rec)
{
    { // Basic positive tree test (empty graph)
        gtpo::graph<> g;
        EXPECT_TRUE(gtpo::is_tree_rec(g));

        auto n1 = g.create_node();
        EXPECT_TRUE(gtpo::is_tree_rec(g));

        auto n2 = g.create_node();
        g.create_edge(n1, n2);
        EXPECT_TRUE(gtpo::is_tree_rec(g));

        g.clear();
        EXPECT_TRUE(gtpo::is_tree_rec(g));
    }

    { // Basic positive test
        // g = { [n1, n2, n3], [(n1 -> n2), (n1 -> n3)] }
        // Expect: true
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        g.create_edge(n1, n2);
        g.create_edge(n1, n3);
        EXPECT_TRUE(gtpo::is_tree_rec(g));
    }

    { // Basic negative test (n
        // g = { [n1, n2, n3], [(n1 -> n3), (n2 -> n3)] }
        // Expect: false since n3 in degree is > 1
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        g.create_edge(n1, n3);
        g.create_edge(n2, n3);
        EXPECT_FALSE(gtpo::is_tree_rec(g));
    }

    { // Basic negative test (graph with degree 0 and more circuits)
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        g.create_edge(n1, n2);
        g.create_edge(n2, n1);
        EXPECT_FALSE(gtpo::is_tree_rec(g));
    }
}

//-----------------------------------------------------------------------------
// Graph recursive traversal algorithms
//-----------------------------------------------------------------------------

TEST(GTpoGraph, tree_depth_rec)
{
    gtpo::graph<> g;
    EXPECT_EQ(gtpo::tree_depth_rec(g), 0);

    auto n1 = g.create_node();
    EXPECT_EQ(gtpo::tree_depth_rec(g), 1);

    auto n2 = g.create_node();
    g.create_edge(n1, n2);
    EXPECT_EQ(gtpo::tree_depth_rec(g), 2);

    g.clear();
    EXPECT_EQ(gtpo::tree_depth_rec(g), 0);
}

TEST(GTpoGraph, linearize_tree_dfs_rec)
{
    {
        gtpo::graph<> g;
        auto r = linearize_tree_dfs_rec(g);
        EXPECT_EQ(r.size(), 0);
    }

    {
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto r = linearize_tree_dfs_rec(g);
        EXPECT_EQ(r.size(), 1);
    }

    {
        gtpo::graph<> g;
        // g = {n1, (n2 -> n3)}
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        g.create_edge(n2, n3);
        auto r = linearize_tree_dfs_rec(g);
        ASSERT_EQ(r.size(), 3);
        EXPECT_EQ(r[0].lock().get(), n1.lock().get());
        EXPECT_EQ(r[1].lock().get(), n2.lock().get());
        EXPECT_EQ(r[2].lock().get(), n3.lock().get());
    }

    {
        gtpo::graph<> g;
        // g = {(n1 -> n3), (n4 -> n2)}
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto n4 = g.create_node();
        g.create_edge(n1, n3);
        g.create_edge(n4, n2);
        auto r = linearize_tree_dfs_rec(g);
        ASSERT_EQ(r.size(), 4);
        EXPECT_EQ(r[0].lock().get(), n1.lock().get());
        EXPECT_EQ(r[1].lock().get(), n3.lock().get());
        EXPECT_EQ(r[2].lock().get(), n4.lock().get());
        EXPECT_EQ(r[3].lock().get(), n2.lock().get());
    }
}

TEST(GTpoGraph, levelize_tree_dfs_rec)
{
    {   // Test empty graph, expecting empty result vector
            // g = {[], []}
            // Expect: r = []
        gtpo::graph<> g;
        auto r = gtpo::levelize_tree_dfs_rec(g);
        EXPECT_EQ( r.size(), 0);
    }

    {   // Test with one node (level 1) graph
            // g = {[n1], []}
            // Expect: r = [ [n1] ]
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto r = gtpo::levelize_tree_dfs_rec(g);
        ASSERT_EQ( r.size(), 1);
        ASSERT_EQ( r[0].size(), 1);
        EXPECT_EQ( r[0][0].lock().get(), n1.lock().get());
    }

    {   // Test with level 1 graph with multiple nodes
            // g = {[n1, n2, n3], []}
            // Expect: r = [ [ n1, n2, n3] ]
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto r = gtpo::levelize_tree_dfs_rec(g);
        ASSERT_EQ( r.size(), 1);
        ASSERT_EQ( r[0].size(), 3);
        EXPECT_EQ( r[0][0].lock().get(), n1.lock().get());
        EXPECT_EQ( r[0][1].lock().get(), n2.lock().get());
        EXPECT_EQ( r[0][2].lock().get(), n3.lock().get());
    }

    {   // Test with level 2 graph with 2 nodes
            // g = {[n1, n2], [(n1 -> n2)]}
            // Expect: r = [ [n1], [n2] ]
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        g.create_edge(n1, n2);
        auto r = gtpo::levelize_tree_dfs_rec(g);
        ASSERT_EQ( r.size(), 2);
        ASSERT_EQ( r[0].size(), 1);
        ASSERT_EQ( r[1].size(), 1);
        EXPECT_EQ( r[0][0].lock().get(), n1.lock().get());
        EXPECT_EQ( r[1][0].lock().get(), n2.lock().get());
    }

    {   // Test with level 2 graph with multiple nodes
            // g = {[n1, n3, n4, n2], [(n3 -> n4)]}
            // Expect: r = [ [n1, n3, n2], [n4] ]
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n3 = g.create_node();
        auto n4 = g.create_node();
        auto n2 = g.create_node();
        g.create_edge(n3, n4);
        auto r = gtpo::levelize_tree_dfs_rec(g);
        ASSERT_EQ( r.size(), 2);
        ASSERT_EQ( r[0].size(), 3);
        ASSERT_EQ( r[1].size(), 1);
        EXPECT_EQ( r[0][1].lock().get(), n3.lock().get());
        EXPECT_EQ( r[1][0].lock().get(), n4.lock().get());
    }
}


//-----------------------------------------------------------------------------
// Graph (iterative) BFS iterator
//-----------------------------------------------------------------------------

TEST(GTpoGraph, linearize_dfs)
{
    {
        gtpo::graph<> g;
        auto r = linearize_dfs(g);
        EXPECT_EQ(r.size(), 0);
    }

    {
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto r = linearize_dfs(g);
        EXPECT_EQ(r.size(), 1);
    }

    {
        gtpo::graph<> g;
        // g = {[n1, n2, n3], [(n2 -> n3)] }
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        g.create_edge(n2, n3);
        auto r = linearize_dfs(g);
        ASSERT_EQ(r.size(), 3);
        EXPECT_EQ(r[0].lock().get(), n1.lock().get());
        EXPECT_EQ(r[1].lock().get(), n2.lock().get());
        EXPECT_EQ(r[2].lock().get(), n3.lock().get());
    }

    {
        gtpo::graph<> g;
        // g = {[n1, n2, n3, n4], [(n1 -> n3), (n4 -> n2)]}
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto n4 = g.create_node();
        g.create_edge(n1, n3);
        g.create_edge(n4, n2);
        auto r = linearize_dfs(g);
        ASSERT_EQ(r.size(), 4);
        EXPECT_EQ(r[0].lock().get(), n1.lock().get());
        EXPECT_EQ(r[1].lock().get(), n3.lock().get());
        EXPECT_EQ(r[2].lock().get(), n4.lock().get());
        EXPECT_EQ(r[3].lock().get(), n2.lock().get());
    }

    {
        // Depth first ...
        // g = {[n1, n2, n3, n4],
        //      [(n1 -> n2), (n2 -> n3), (n1, n4)]}
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto n4 = g.create_node();
        g.create_edge(n1, n4);
        g.create_edge(n1, n2);
        g.create_edge(n2, n3);
        auto r = linearize_dfs(g);
        ASSERT_TRUE(r.size() == 4);
        EXPECT_EQ(r[0].lock().get(), n1.lock().get());
        EXPECT_EQ(r[1].lock().get(), n2.lock().get());
        EXPECT_EQ(r[2].lock().get(), n3.lock().get());
        EXPECT_EQ(r[3].lock().get(), n4.lock().get());
    }
}

TEST(GTpoGraph, begin_end_dfs)
{
    {
        gtpo::graph<> g;
        auto it = gtpo::begin_dfs(g);
        auto it_end = gtpo::end_dfs(g);

        // begoin_dfs() has been called on an empty graph: it should be an end() iterator and equals gtpo::end_dfs()...
        EXPECT_TRUE(it == it_end);
        EXPECT_FALSE(it != it_end);
    }

    {
        gtpo::graph<> g;
        g.create_node();
        auto it = gtpo::begin_dfs(g);
        auto it_end = gtpo::end_dfs(g);

        // begin iterator should not equal end iterator since graph is not empty...
        EXPECT_FALSE(it == it_end);
        EXPECT_TRUE(it != it_end);
    }
}

TEST(GTpoGraph, dfs_iterator)
{
    {
        gtpo::graph<> g;
        auto n = g.create_node();
        auto it = gtpo::begin_dfs(g);

        auto ni = *it;
        EXPECT_EQ(ni.lock().get(), n.lock().get());

        auto it_end = gtpo::end_dfs(g);
        EXPECT_TRUE(it != it_end);
        ++it;
        EXPECT_TRUE(it == it_end);
    }

    using weak_nodes_t = std::vector<typename gtpo::graph<>::weak_node_t>;
    const auto iterate_dfs = [](auto& graph) -> weak_nodes_t {
        weak_nodes_t r;
        for ( auto dfs_iter = gtpo::begin_dfs(graph);
              dfs_iter != gtpo::end_dfs(graph);
              ++dfs_iter ) {
            r.push_back(*dfs_iter);
        }
        return r;
    };

    const auto compare_vector_of_weak = [](auto& a, auto& b) -> bool {
        if ( a.size() != b.size() )
            return false;
        auto bit = b.begin();
        for ( const auto& ait : a ) {
            const auto a_ptr = ait.lock();
            const auto b_ptr = (*bit).lock();
            if ( a_ptr.get() != b_ptr.get() )
                return false;
            ++bit;
        }
        return true;
    };

    {   // Linear graph
        // g = {[n1, n2, n3], []}
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto r = linearize_dfs(g);
        auto r2 = iterate_dfs(g);
        ASSERT_TRUE(compare_vector_of_weak(r, r2));
    }

    {   // Simple tree
        // g = {[n1, n2], [(n1 -> n2)] }
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        g.create_edge(n1, n2);
        auto r = linearize_dfs(g);
        auto r2 = iterate_dfs(g);
        ASSERT_TRUE(compare_vector_of_weak(r, r2));
    }

    {   // Simple forest
        // g = {[n1, n2, n3], [(n2 -> n3)] }
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        g.create_edge(n2, n3);
        auto r = linearize_dfs(g);
        auto r2 = iterate_dfs(g);
        ASSERT_TRUE(compare_vector_of_weak(r, r2));
    }

    {
        // "Complex" forest
        // g = {[n1, n2, n3, n4], [(n1 -> n3), (n4 -> n2)]}
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto n4 = g.create_node();
        g.create_edge(n1, n3);
        g.create_edge(n4, n2);
        auto r = linearize_dfs(g);
        auto r2 = iterate_dfs(g);
        EXPECT_TRUE(compare_vector_of_weak(r, r2));
    }

    {
        // Depth first ...
        // g = {[n1, n2, n3, n4],
        //      [(n1 -> n2), (n2 -> n3), (n1, n4)]}
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto n4 = g.create_node();
        g.create_edge(n1, n4);
        g.create_edge(n1, n2);
        g.create_edge(n2, n3);
        auto r = iterate_dfs(g);
        ASSERT_TRUE(r.size() == 4);
        EXPECT_EQ(r[0].lock().get(), n1.lock().get());
        EXPECT_EQ(r[1].lock().get(), n2.lock().get());
        EXPECT_EQ(r[2].lock().get(), n3.lock().get());
        EXPECT_EQ(r[3].lock().get(), n4.lock().get());
    }

    {
        // DAG
        // g = {[n1, n2, n3, n4],
        //      [(n1 -> n2), (n1 -> n3), (n2, n4), (n3, n4), (n1, n4)]}
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto n4 = g.create_node();
        g.create_edge(n1, n2);
        g.create_edge(n1, n4);
        g.create_edge(n1, n3);
        g.create_edge(n2, n4);
        g.create_edge(n3, n4);
        auto r = iterate_dfs(g);
        ASSERT_TRUE(r.size() == 4);
        EXPECT_EQ(r[0].lock().get(), n1.lock().get());
        EXPECT_EQ(r[1].lock().get(), n3.lock().get());
        EXPECT_EQ(r[2].lock().get(), n4.lock().get());
        EXPECT_EQ(r[3].lock().get(), n2.lock().get());
    }

    // FIXME: Test a non DAG...
}
