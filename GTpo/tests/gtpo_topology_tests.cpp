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
// \file	gtpo_topology_tests.cpp
// \author	benoit@qanava.org
// \date	2016 01 26
//-----------------------------------------------------------------------------

// STD headers
#include <list>
#include <memory>
#include <iostream>

// GTpo headers
#include <GTpo>

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//-----------------------------------------------------------------------------
// Graph node tests
//-----------------------------------------------------------------------------

TEST(GTpoTopology, nodeBasic)
{
    // A default empty graph should have no nodes nor root nodes
    gtpo::graph<> g;
    EXPECT_EQ( g.get_node_count(), 0 );
    EXPECT_EQ( g.get_root_node_count(), 0 );
}

TEST(GTpoTopology, nodeInsertCount)
{
    // Inserting a node should increase node count and root node count
    gtpo::graph<> g;
    auto nc = g.get_node_count();
    auto rnc = g.get_root_node_count();
    EXPECT_EQ( nc, 0);
    EXPECT_EQ( rnc, 0);
    g.create_node();
    EXPECT_EQ( g.get_node_count(), nc + 1 );
    EXPECT_EQ( g.get_root_node_count(), rnc + 1 );
    g.clear();
}

TEST(GTpoTopology, remove_nodeCount)
{
    gtpo::graph<> g;
    gtpo::graph<>::weak_node_t n = g.create_node();
    auto nc = g.get_node_count();
    auto rnc = g.get_root_node_count();
    g.install_root_node( n ); // Just a test, it should not be called by the user directly
    EXPECT_TRUE( g.is_root_node( n ) );
    EXPECT_TRUE( g.contains( n ) );
    g.remove_node( n );
    EXPECT_FALSE( g.contains( n ) );
    EXPECT_EQ( g.get_node_count(), nc - 1 );
    EXPECT_EQ( g.get_root_node_count(), rnc - 1 ) ;
    g.create_node();
    g.clear();
}

//-----------------------------------------------------------------------------
// Graph clear tests
//-----------------------------------------------------------------------------

TEST(GTpoTopology, graphClear)
{
    // TEST: clearing an empty graph, expecting no node after clearing an empty graph
    gtpo::graph<> g;
    EXPECT_EQ( g.get_node_count(), 0 );
    g.clear();
    EXPECT_EQ( g.get_node_count(), 0 );
}

TEST(GTpoTopo, graphClearNode)
{
    // TEST: clearing a graph with one node should lead to an empty graph
    gtpo::graph<> g;
    auto n = g.create_node();
    EXPECT_TRUE( g.is_root_node( n ) );
    EXPECT_TRUE( g.contains( n ) );
    g.clear();
    EXPECT_FALSE( g.contains( n ) );
    EXPECT_EQ( g.get_node_count(), 0 );
    EXPECT_TRUE( n.expired() );
}

TEST(GTpoTopology, graphClearNodeEdge)
{
    // TEST: clearing a graph with two nodes linked by an edge should lead to an empty graph
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    auto e1 = g.create_edge<gtpo::edge<>>(n1, n2);
    EXPECT_EQ( g.get_node_count(), 2 );
    EXPECT_TRUE( g.contains(e1) );
    g.clear();
    EXPECT_EQ( g.get_node_count(), 0 );
    EXPECT_FALSE( g.contains(e1) );
    EXPECT_TRUE( n1.expired() );
    EXPECT_TRUE( n2.expired() );
    EXPECT_TRUE( e1.expired() );
    EXPECT_EQ( g.get_edge_count(),0 );
}

//-----------------------------------------------------------------------------
// Graph edge tests
//-----------------------------------------------------------------------------

TEST(GTpoTopology, edgeBasic)
{
    // A default empty graph should have no edges
    gtpo::graph<> g;
    EXPECT_EQ( g.get_edge_count(), 0 );
}

TEST(GTpoTopology, edgeCreateBadTopology)
{
    // TEST: Creating an edge with no source and/or destination should throw a bad_topology_error
    gtpo::graph<> g;
    auto n1 = g.create_node();
    EXPECT_THROW( g.create_edge(n1, gtpo::graph<>::weak_node_t{}), gtpo::bad_topology_error );
    EXPECT_THROW( g.create_edge(gtpo::graph<>::weak_node_t{}, gtpo::graph<>::weak_node_t{}), gtpo::bad_topology_error );
    EXPECT_THROW( g.create_edge(gtpo::graph<>::weak_node_t{}, n1), gtpo::bad_topology_error );

    auto n2 = g.create_node();
    EXPECT_NO_THROW( g.create_edge(n1, n2) );
}

TEST(GTpoTopology, edgeCreate)
{
    // TEST: Creating an edge should remove dst from the root node set
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    EXPECT_TRUE( ( g.get_root_node_count() == 2 ) );
    auto e = g.create_edge(n1, n2);
    EXPECT_TRUE( ( g.get_root_node_count() == 1 ) );   // n2 is no longer a root node
}

TEST(GTpoTopology, edgeCreateBasicCircuit)
{
    // TEST: Creating an edge that is a circuit to a root node does not remove destination
    // from root nodes
    {   // Test with create_edge()
        gtpo::graph<> g;
        EXPECT_TRUE( g.get_root_node_count() == 0 );
        auto n1 = g.create_node();
        EXPECT_TRUE( g.get_root_node_count() == 1 );
        g.create_edge( n1, n1 );
        EXPECT_TRUE( g.get_root_node_count() == 1 );
    }
    {   // Test with insert_edge()
        gtpo::graph<> g;
        EXPECT_TRUE( g.get_root_node_count() == 0 );
        auto n1 = g.create_node();
        EXPECT_TRUE( g.get_root_node_count() == 1 );
        auto e1 = std::make_shared<gtpo::edge<>>(n1, n1);
        g.insert_edge( e1 );
        EXPECT_TRUE( g.get_root_node_count() == 1 );
    }
}

TEST(GTpoTopology, edgeInsertRootNode)
{
    // TEST: inserting an existing edge must remove dst from the root node set
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    auto e1 = std::make_shared<gtpo::edge<>>(n1, n2);
    EXPECT_EQ( g.get_root_node_count(), 2 );
    g.insert_edge(e1);
    EXPECT_EQ( g.get_root_node_count(), 1 );   // n2 is no longer a root node
}

TEST(GTpoTopology, edgeCreateRootNode)
{
    // TEST: inserting an existing edge must remove dst from the root node set
    gtpo::graph<> g;
    EXPECT_EQ( g.get_root_node_count(), 0 );
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    EXPECT_EQ( g.get_root_node_count(), 2 );
    g.create_edge(n1, n2);
    EXPECT_EQ( g.get_root_node_count(), 1 );   // n2 is no longer a root node
}

TEST(GTpoTopology, edgeInsertBadTopology)
{
    // TEST: Inserting an edge with no source and/or destination should throw a bad_topology_error
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    EXPECT_THROW( g.insert_edge(gtpo::graph<>::shared_edge_t{}), gtpo::bad_topology_error );
    auto e1 = std::make_shared<gtpo::edge<>>(n1, gtpo::graph<>::weak_node_t{});
    auto e2 = std::make_shared<gtpo::edge<>>(gtpo::graph<>::weak_node_t{}, gtpo::graph<>::weak_node_t{});
    auto e3 = std::make_shared<gtpo::edge<>>(gtpo::graph<>::weak_node_t{}, n1);

    // Test error with insert_edge()
    EXPECT_THROW( g.insert_edge(e1), gtpo::bad_topology_error );
    EXPECT_THROW( g.insert_edge(e2), gtpo::bad_topology_error );
    EXPECT_THROW( g.insert_edge(e3), gtpo::bad_topology_error );

    // Test error with create_edge()
    EXPECT_THROW( g.create_edge(n1, gtpo::graph<>::weak_node_t{}), gtpo::bad_topology_error );
    EXPECT_THROW( g.create_edge(gtpo::graph<>::weak_node_t{}, n1), gtpo::bad_topology_error );
    EXPECT_THROW( g.create_edge(gtpo::graph<>::weak_node_t{}, gtpo::graph<>::weak_node_t{}), gtpo::bad_topology_error );

    // Test success with insert_edge()
    auto e4 = std::make_shared<gtpo::edge<>>(n1, n2);
    EXPECT_NO_THROW( g.insert_edge(e4) );

    // Test success with create_edge()
    EXPECT_NO_THROW( g.create_edge(n1, n2) );
}

TEST(GTpoTopology, edgeInsertparallel)
{
    { // Test if create_edge() successfully generate parallel edges
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto n4 = g.create_node();
        g.create_edge(n1, n2);
        EXPECT_EQ( g.get_edge_count(n1, n2), 1 );
        g.create_edge(n2, n3);
        g.create_edge(n2, n3);
        g.create_edge(n2, n3);
        EXPECT_EQ( g.get_edge_count(n2, n3), 3 );   // Do not require, it depend on selected graph data structure
        g.remove_edge(n2, n3);
        EXPECT_EQ( g.get_edge_count(n2, n3), 2 ) ;
        g.remove_all_edges(n2, n3);
        EXPECT_EQ( g.get_edge_count(n2, n3), 0 ) ;
        g.clear();
    }

    { // Test if insert_edge() successfully generate parallel edges
        gtpo::graph<> g;
        auto n1 = g.create_node();
        auto n2 = g.create_node();
        auto n3 = g.create_node();
        auto n4 = g.create_node();

        auto e1 = std::make_shared<gtpo::edge<>>(n1, n2);
        g.insert_edge(e1);
        EXPECT_EQ( g.get_edge_count(n1, n2), 1 );

        auto e2 = std::make_shared<gtpo::edge<>>(n2, n3);
        auto e3 = std::make_shared<gtpo::edge<>>(n2, n3);
        auto e4 = std::make_shared<gtpo::edge<>>(n2, n3);
        g.insert_edge(e2);
        g.insert_edge(e3);
        g.insert_edge(e3);
        EXPECT_EQ( g.get_edge_count(n2, n3), 3 );   // Do not require, it depend on selected graph data structure
        g.remove_edge(n2, n3);
        EXPECT_EQ( g.get_edge_count(n2, n3), 2 ) ;
        g.remove_all_edges(n2, n3);
        EXPECT_EQ( g.get_edge_count(n2, n3), 0 ) ;
        g.clear();
    }
}

TEST(GTpoTopology, edgeRemoveContains)
{
    // Graph must no longer contains() an edge that has been removed
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    EXPECT_EQ( g.get_edge_count(), 0 );
    auto e1 = g.create_edge(n1, n2);
    EXPECT_EQ( g.get_edge_count(), 1 );
    EXPECT_TRUE( g.contains(e1) );
    g.remove_edge(n1, n2);
    EXPECT_FALSE( g.contains(e1) );
    EXPECT_FALSE( g.has_edge(n1, n2) );
}

TEST(GTpoTopology, edgeNodeInOutDegree)
{
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    auto n3 = g.create_node();
    auto n4 = g.create_node();
    EXPECT_EQ( g.get_root_node_count(), 4 );
    g.create_edge(n1, n2);
    EXPECT_EQ( g.get_root_node_count(), 3 );
    g.create_edge(n2, n3);
    g.create_edge(n2, n4);
    EXPECT_EQ( g.get_root_node_count(), 1 );

    // Check in/out degree after multiple edge insertion
    gtpo::graph<>::shared_node_t node2 = n2.lock();
    EXPECT_TRUE( node2 );
    EXPECT_EQ( node2->get_in_degree(), 1 );
    EXPECT_EQ( node2->get_out_degree(), 2 );
    EXPECT_EQ( node2->get_in_edges().size(), 1 );
    EXPECT_EQ( node2->get_out_edges().size(), 2 );
    EXPECT_EQ( node2->get_in_nodes().size(), 1 );
    EXPECT_EQ( node2->get_out_nodes().size(), 2 );

    // Check in/out degree after multiple edge removal
    ASSERT_THROW(g.remove_edge(n2, gtpo::graph<>::weak_node_t()), gtpo::bad_topology_error );
    //INFO( "Incorrect remove_edge() calls successfully throw a gtpo::bad_topology_error" );
    EXPECT_TRUE( g.has_edge( n2, n4 ) );
    g.remove_edge(n2, n4);
    EXPECT_FALSE( g.has_edge( n2, n4 ) );
    g.remove_edge(n2, n3);
    EXPECT_EQ( node2->get_out_degree(), 0 );
    EXPECT_EQ( node2->get_out_edges().size(), 0 );
    EXPECT_EQ( node2->get_out_nodes().size(), 0 );
    EXPECT_EQ( g.get_root_node_count(), 3 );
    EXPECT_TRUE( g.is_root_node( n3 ) );
    EXPECT_TRUE( g.is_root_node( n4 ) );

    g.clear();
}

TEST(GTpoTopology, remove_nodeInOutDegree)
{
    // Note: Test GTpo remove_node() method:
    //    - Removing a node must also invalidate (ie remove) all its out/in orphants edges.
    gtpo::graph<> g;
    auto wn1 = g.create_node();
    auto wn2 = g.create_node();
    auto wn3 = g.create_node();
    g.create_edge(wn1, wn2);
    g.create_edge(wn2, wn3);
    // n1 now has 1 in node and 1 out node
    auto n2 = wn2.lock();
    EXPECT_EQ( n2->get_out_degree(), 1 );
    EXPECT_EQ( n2->get_in_degree(), 1 );

    // Removing node n2 should also remove all edges
    g.remove_node( wn2 );
    EXPECT_EQ( g.get_node_count(), 2 );
    EXPECT_EQ( g.get_edge_count(), 0 );
    auto n1 = wn1.lock();
    auto n3 = wn3.lock();
    EXPECT_EQ( n1->get_out_degree(), 0 );
    EXPECT_EQ( n3->get_in_degree(), 0 );
}
