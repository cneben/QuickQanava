/*
 Copyright (c) 2008-2022, Benoit AUTHEMAN All rights reserved.

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
// \file	topology_tests.cpp
// \author	benoit@qanava.org
// \date	2016 01 26
//-----------------------------------------------------------------------------

// STD headers
#include <list>
#include <memory>
#include <iostream>

// GTpo headers
#include <QuickQanava>

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//-----------------------------------------------------------------------------
// Graph node tests
//-----------------------------------------------------------------------------

TEST(qan_Graph, empty)
{
    // A default empty graph should have no nodes nor root nodes
    qan::Graph g;
    EXPECT_EQ(g.get_node_count(), 0);
    EXPECT_EQ(g.get_root_node_count(), 0);
}

TEST(qan_Graph, insert)
{
    // Inserting a node should increase node count and root node count
    qan::Graph g;
    EXPECT_EQ(g.get_node_count(), 0);
    EXPECT_EQ(g.get_root_node_count(), 0);
    g.insertNonVisualNode(new qan::Node{});
    EXPECT_EQ(g.get_node_count(), 1);
    EXPECT_EQ(g.get_root_node_count(), 1);
    g.clear();
}

TEST(qan_Graph, root_node)
{
    qan::Graph g;
    auto n = g.create_node();
    EXPECT_TRUE(g.insert_node(n));
    EXPECT_EQ(g.get_node_count(), 1);
    EXPECT_EQ(g.get_root_node_count(), 1);
    EXPECT_TRUE(g.is_root_node(n));
    EXPECT_TRUE(g.contains(n));
    g.remove_node(n);
    EXPECT_FALSE(g.contains(n));
    EXPECT_EQ(g.get_node_count(), 0);
    EXPECT_EQ(g.get_root_node_count(), 0);
}

//-----------------------------------------------------------------------------
// Graph clear tests
//-----------------------------------------------------------------------------

TEST(qan_Graph, clear)
{
    // TEST: clearing an empty graph, expecting no node after clearing an empty graph
    qan::Graph g;
    EXPECT_EQ(g.get_node_count(), 0);
    g.clear();
    EXPECT_EQ(g.get_node_count(), 0);
}


TEST(qan_Graph, clear_nodes)
{
    // TEST: clearing a graph with one node should lead to an empty graph
    qan::Graph g;
    auto n = g.create_node();
    EXPECT_TRUE(g.insert_node(n));
    EXPECT_TRUE(g.is_root_node(n));
    EXPECT_TRUE(g.contains(n));
    g.clear();
    EXPECT_FALSE(g.contains(n));
    EXPECT_EQ(g.get_node_count(), 0);
}

TEST(qan_Graph, clear_edges)
{
    // TEST: clearing a graph with two nodes linked by an edge should lead to an empty graph
    qan::Graph g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    g.insert_node(n1);
    g.insert_node(n2);
    auto e1 = g.insert_edge(n1, n2);
    //g.insert_edge(e1);
    EXPECT_EQ(g.get_node_count(), 2);
    EXPECT_TRUE(g.contains(e1));
    g.clear();
    EXPECT_EQ(g.get_node_count(), 0);
    EXPECT_FALSE(g.contains(e1));
    EXPECT_EQ(g.get_edge_count(), 0);
}


//-----------------------------------------------------------------------------
// Graph edge tests
//-----------------------------------------------------------------------------

TEST(qan_Graph, edge_empty)
{
    // A default empty graph should have no edges
    qan::Graph g;
    EXPECT_EQ(g.get_edge_count(), 0);
}

TEST(qan_Graph, insert_edge_error)
{
    // TEST: Creating an edge with no source and/or destination should return nullptr
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);

    EXPECT_EQ(g.insert_edge(n1, nullptr), nullptr);
    EXPECT_EQ(g.insert_edge(nullptr, nullptr), nullptr);
    EXPECT_EQ(g.insert_edge(nullptr, n1), nullptr);

    // FIXME reactivate this, in v2, there is no check that n1/n2 is
    // actually part of the graph...
    /*EXPECT_EQ(g.insert_edge(n1, new qan::Node{}), nullptr);
    EXPECT_EQ(g.insert_edge(new qan::Node{}, new qan::Node{}), nullptr);
    EXPECT_EQ(g.insert_edge(new qan::Node{}, n1), nullptr);
    */

    auto n2 = g.create_node();
    g.insert_node(n2);
    auto e1 = g.insert_edge(n1, n2);
    EXPECT_NE(e1, nullptr);

    // Parallel edges are valid
    auto e2 = g.insert_edge(n1, n2);
    EXPECT_NE(e2, nullptr);

    // FIXME: should be false ;)
    //EXPECT_TRUE(g.insert_edge(e1));
}

TEST(qan_Graph, edge_insert)
{
    // TEST: Creating an edge should remove dst from the root node set
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);
    auto n2 = g.create_node();
    g.insert_node(n2);
    EXPECT_EQ(g.get_root_node_count(), 2);
    auto e = g.insert_edge(n1, n2);
    EXPECT_NE(e, nullptr);
    EXPECT_EQ(g.get_root_node_count(), 1);   // n2 is no longer a root node
}


TEST(qan_Graph, edge_circuits)
{
    // TEST: Creating an edge that is a circuit to a root node does not remove destination
    // from root nodes
    {   // Test with insert_edge()
        qan::Graph g;
        EXPECT_TRUE(g.get_root_node_count() == 0);
        auto n1 = g.create_node();
        g.insert_node(n1);
        EXPECT_TRUE(g.get_root_node_count() == 1);
        g.insert_edge(n1, n1);
        EXPECT_TRUE(g.get_root_node_count() == 1);
    }
    {   // Test with insert_edge()
        qan::Graph g;
        EXPECT_TRUE(g.get_root_node_count() == 0);
        auto n1 = g.create_node();
        g.insert_node(n1);
        EXPECT_TRUE(g.get_root_node_count() == 1);
        auto e1 = new qan::Edge();
        e1->set_src(n1);
        e1->set_dst(n1);
        g.insert_edge(e1);
        EXPECT_TRUE(g.get_root_node_count() == 1);
    }
}

TEST(qan_Graph, edge_insert_root_node)
{
    // TEST: inserting an existing edge must remove dst from the root node set
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);
    auto n2 = g.create_node();
    g.insert_node(n2);
    EXPECT_EQ(g.get_root_node_count(), 2);
    g.insert_edge(n1, n2);
    EXPECT_EQ(g.get_root_node_count(), 1);   // n2 is no longer a root node
}


TEST(qan_Graph, edgeInsertBadTopology)
{
    // TEST: Inserting an edge with no source and/or destination should throw a bad_topology_error
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);
    auto n2 = g.create_node();
    g.insert_node(n2);
    EXPECT_FALSE(g.insert_edge(nullptr));

    auto e1 = new qan::Edge();
    e1->set_src(n1);
    auto e2 = new qan::Edge();
    auto e3 = new qan::Edge();
    e3->set_dst(n1);

    // Test error with insert_edge()
    EXPECT_FALSE(g.insert_edge(e1));
    EXPECT_FALSE(g.insert_edge(e2));
    EXPECT_FALSE(g.insert_edge(e3));

    // Test success with insert_edge()
    auto e4 = new qan::Edge();
    e4->set_src(n1);
    e4->set_dst(n2);
    EXPECT_TRUE(g.insert_edge(e4));

    // Test success with insert_edge()
    EXPECT_NE(g.insert_edge(n1, n2), nullptr);
}


TEST(qan_Graph, edge_insert_parallel)
{
    { // Test if insert_edge() successfully generate parallel edges
        qan::Graph g;
        auto n1 = g.create_node();
        g.insert_node(n1);
        auto n2 = g.create_node();
        g.insert_node(n2);
        auto n3 = g.create_node();
        g.insert_node(n3);
        g.insert_edge(n1, n2);
        EXPECT_EQ(g.get_edge_count(n1, n2), 1);
        g.insert_edge(n2, n3);
        g.insert_edge(n2, n3);
        g.insert_edge(n2, n3);
        EXPECT_EQ(g.get_edge_count(n2, n3), 3);   // Do not require, it depend on selected graph data structure
        g.remove_edge(n2, n3);
        EXPECT_EQ(g.get_edge_count(n2, n3), 2) ;
        g.remove_all_edges(n2, n3);
        EXPECT_EQ(g.get_edge_count(n2, n3), 0) ;
        g.clear();
    }

    { // Test if insert_edge() successfully generate parallel edges
        qan::Graph g;
        auto n1 = g.create_node();
        g.insert_node(n1);
        auto n2 = g.create_node();
        g.insert_node(n2);
        auto n3 = g.create_node();
        g.insert_node(n3);
        auto n4 = g.create_node();
        g.insert_node(n4);

        g.insert_edge(n1, n2);
        EXPECT_EQ(g.get_edge_count(n1, n2), 1);
        g.insert_edge(n2, n3);
        g.insert_edge(n2, n3);
        g.insert_edge(n2, n3);
        EXPECT_EQ(g.get_edge_count(n2, n3), 3);   // Do not require, it depend on selected graph data structure
        g.remove_edge(n2, n3);
        EXPECT_EQ(g.get_edge_count(n2, n3), 2) ;
        g.remove_all_edges(n2, n3);
        EXPECT_EQ(g.get_edge_count(n2, n3), 0) ;
        g.clear();
    }
}

TEST(qan_Graph, edge_remove_contains)
{
    // Graph must no longer contains() an edge that has been removed
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);
    auto n2 = g.create_node();
    g.insert_node(n2);
    EXPECT_EQ(g.get_edge_count(), 0);
    auto e1 = g.insert_edge(n1, n2);
    EXPECT_EQ(g.get_edge_count(), 1);
    EXPECT_TRUE(g.contains(e1));
    g.remove_edge(n1, n2);
    EXPECT_FALSE(g.contains(e1));
    EXPECT_FALSE(g.has_edge(n1, n2));
}

TEST(qan_Graph, edge_node_degree)
{
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);
    auto n2 = g.create_node();
    g.insert_node(n2);
    auto n3 = g.create_node();
    g.insert_node(n3);
    auto n4 = g.create_node();
    g.insert_node(n4);
    EXPECT_EQ(g.get_root_node_count(), 4);
    g.insert_edge(n1, n2);
    EXPECT_EQ(g.get_root_node_count(), 3);
    g.insert_edge(n2, n3);
    g.insert_edge(n2, n4);
    EXPECT_EQ(g.get_root_node_count(), 1);

    // Check in/out degree after multiple edge insertion
    EXPECT_EQ(n2->get_in_degree(), 1);
    EXPECT_EQ(n2->get_out_degree(), 2);
    EXPECT_EQ(n2->get_in_edges().size(), 1);
    EXPECT_EQ(n2->get_out_edges().size(), 2);
    EXPECT_EQ(n2->get_in_nodes().size(), 1);
    EXPECT_EQ(n2->get_out_nodes().size(), 2);

    // Check in/out degree after multiple edge removal
    EXPECT_FALSE(g.remove_edge(n2, nullptr));

    EXPECT_TRUE(g.has_edge(n2, n4));
    g.remove_edge(n2, n4);
    EXPECT_FALSE(g.has_edge(n2, n4));
    g.remove_edge(n2, n3);
    EXPECT_EQ(n2->get_out_degree(), 0);
    EXPECT_EQ(n2->get_out_edges().size(), 0);
    EXPECT_EQ(n2->get_out_nodes().size(), 0);
    EXPECT_EQ(g.get_root_node_count(), 3);
    EXPECT_TRUE(g.is_root_node(n3));
    EXPECT_TRUE(g.is_root_node(n4));

    g.clear();
}

TEST(qan_Graph, remove_node_degree)
{
    // Note: Test GTpo remove_node() method:
    //    - Removing a node must also invalidate (ie remove) all its out/in orphants edges.
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);
    auto n2 = g.create_node();
    g.insert_node(n2);
    auto n3 = g.create_node();
    g.insert_node(n3);
    g.insert_edge(n1, n2);
    g.insert_edge(n2, n3);

    // n1 now has 1 in node and 1 out node
    EXPECT_EQ(n2->get_out_degree(), 1);
    EXPECT_EQ(n2->get_in_degree(), 1);

    // Removing node n2 should also remove all edges
    g.remove_node(n2);
    EXPECT_EQ(g.get_node_count(), 2);
    EXPECT_EQ(g.get_edge_count(), 0);
    EXPECT_EQ(n1->get_out_degree(), 0);
    EXPECT_EQ(n3->get_in_degree(), 0);
}


//-----------------------------------------------------------------------------
// Graph topology tests
//-----------------------------------------------------------------------------

TEST(qan_Graph, collectAncestorsDfs_basic)
{
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);

    // g = n1
    const auto r1 = g.collectAncestorsDfs(*n1);
    ASSERT_EQ(r1.size(), 1);    // n1

    //     +-----------+
    //     v           |
    // g = n3 -> n2 -> n1 -> n4    n5

    auto n2 = g.create_node();  // Ancestor level 1
    g.insert_node(n2);
    g.insert_edge(n2, n1);
    auto n3 = g.create_node();  // Ancestor level 2
    g.insert_node(n3);
    g.insert_edge(n3, n2);
    g.insert_edge(n1, n3);      // Circuit from n1 to n3

    auto n4 = g.create_node();  // Child node
    g.insert_node(n4);
    g.insert_edge(n1, n4);

    auto n5 = g.create_node();  // Orphant node
    g.insert_node(n5);

    const auto r2 = g.collectAncestorsDfs(*n1);
    ASSERT_EQ(r2.size(), 3);   // n3, n2 and n1
    EXPECT_TRUE(std::find(r2.cbegin(), r2.cend(), n1) != r2.cend());
    EXPECT_TRUE(std::find(r2.cbegin(), r2.cend(), n2) != r2.cend());
    EXPECT_TRUE(std::find(r2.cbegin(), r2.cend(), n3) != r2.cend());
}


// FIXME isAncestor test with circuit !
TEST(qan_Graph, isAncestor_basic)
{
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);

    // g = n1
    const auto r1 = g.collectAncestorsDfs(*n1);
    ASSERT_FALSE(g.isAncestor(n1, nullptr));
    ASSERT_FALSE(g.isAncestor(nullptr, n1));
    ASSERT_FALSE(g.isAncestor(nullptr, nullptr));
    ASSERT_FALSE(g.isAncestor(n1, n1));

    //     +-+
    //     v |
    // g = n1
    g.insert_edge(n1, n1);  // Add circuit
    ASSERT_FALSE(g.isAncestor(n1, n1));
}

TEST(qan_Graph, isAncestor)
{
    qan::Graph g;
    //     +-----------+
    //     v           |
    // g = n3 -> n2 -> n1 -> n4    n5

    auto n1 = g.create_node();
    g.insert_node(n1);

    auto n2 = g.create_node();  // Ancestor level 1
    g.insert_node(n2);
    g.insert_edge(n2, n1);
    auto n3 = g.create_node();  // Ancestor level 2
    g.insert_node(n3);
    g.insert_edge(n3, n2);
    g.insert_edge(n1, n3);      // Circuit from n1 to n3

    auto n4 = g.create_node();  // Child node
    g.insert_node(n4);
    g.insert_edge(n1, n4);

    auto n5 = g.create_node();  // Orphant node
    g.insert_node(n5);

    const auto r2 = g.collectAncestorsDfs(*n1);
    ASSERT_EQ(r2.size(), 3);   // n3, n2 and n1
    EXPECT_TRUE(std::find(r2.cbegin(), r2.cend(), n1) != r2.cend());
    EXPECT_TRUE(std::find(r2.cbegin(), r2.cend(), n2) != r2.cend());
    EXPECT_TRUE(std::find(r2.cbegin(), r2.cend(), n3) != r2.cend());
}


