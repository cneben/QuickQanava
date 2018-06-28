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
// \file	gtpoTopology.cpp
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
    EXPECT_EQ( g.getNodeCount(), 0 );
    EXPECT_EQ( g.getRootNodeCount(), 0 );
}

TEST(GTpoTopology, nodeInsertCount)
{
    // Inserting a node should increase node count and root node count
    gtpo::graph<> g;
    auto nc = g.getNodeCount();
    auto rnc = g.getRootNodeCount();
    EXPECT_EQ( nc, 0);
    EXPECT_EQ( rnc, 0);
    g.createNode();
    EXPECT_EQ( g.getNodeCount(), nc + 1 );
    EXPECT_EQ( g.getRootNodeCount(), rnc + 1 );
    g.clear();
}

TEST(GTpoTopology, removeNodeCount)
{
    gtpo::graph<> g;
    gtpo::graph<>::weak_node n = g.createNode();
    auto nc = g.getNodeCount();
    auto rnc = g.getRootNodeCount();
    g.installRootNode( n ); // Just a test, it should not be called by the user directly
    EXPECT_TRUE( g.isRootNode( n ) );
    EXPECT_TRUE( g.contains( n ) );
    g.removeNode( n );
    EXPECT_FALSE( g.contains( n ) );
    EXPECT_EQ( g.getNodeCount(), nc - 1 );
    EXPECT_EQ( g.getRootNodeCount(), rnc - 1 ) ;
    g.createNode();
    g.clear();
}

//-----------------------------------------------------------------------------
// Graph clear tests
//-----------------------------------------------------------------------------

TEST(GTpoTopology, graphClear)
{
    // TEST: clearing an empty graph, expecting no node after clearing an empty graph
    gtpo::graph<> g;
    EXPECT_EQ( g.getNodeCount(), 0 );
    g.clear();
    EXPECT_EQ( g.getNodeCount(), 0 );
}

TEST(GTpoTopo, graphClearNode)
{
    // TEST: clearing a graph with one node should lead to an empty graph
    gtpo::graph<> g;
    auto n = g.createNode();
    EXPECT_TRUE( g.isRootNode( n ) );
    EXPECT_TRUE( g.contains( n ) );
    g.clear();
    EXPECT_FALSE( g.contains( n ) );
    EXPECT_EQ( g.getNodeCount(), 0 );
    EXPECT_TRUE( n.expired() );
}

TEST(GTpoTopology, graphClearNodeEdge)
{
    // TEST: clearing a graph with two nodes linked by an edge should lead to an empty graph
    gtpo::graph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto e1 = g.createEdge<gtpo::edge<>>(n1, n2);
    EXPECT_EQ( g.getNodeCount(), 2 );
    EXPECT_TRUE( g.contains(e1) );
    g.clear();
    EXPECT_EQ( g.getNodeCount(), 0 );
    EXPECT_FALSE( g.contains(e1) );
    EXPECT_TRUE( n1.expired() );
    EXPECT_TRUE( n2.expired() );
    EXPECT_TRUE( e1.expired() );
    EXPECT_EQ( g.getEdgeCount(),0 );
}

//-----------------------------------------------------------------------------
// Graph edge tests
//-----------------------------------------------------------------------------

TEST(GTpoTopology, edgeBasic)
{
    // A default empty graph should have no edges
    gtpo::graph<> g;
    EXPECT_EQ( g.getEdgeCount(), 0 );
}

TEST(GTpoTopology, edgeCreateBadTopology)
{
    // TEST: Creating an edge with no source and/or destination should throw a bad_topology_error
    gtpo::graph<> g;
    auto n1 = g.createNode();
    EXPECT_THROW( g.createEdge(n1, gtpo::graph<>::weak_node{}), gtpo::bad_topology_error );
    EXPECT_THROW( g.createEdge(gtpo::graph<>::weak_node{}, gtpo::graph<>::weak_node{}), gtpo::bad_topology_error );
    EXPECT_THROW( g.createEdge(gtpo::graph<>::weak_node{}, n1), gtpo::bad_topology_error );

    auto n2 = g.createNode();
    EXPECT_NO_THROW( g.createEdge(n1, n2) );
}

TEST(GTpoTopology, edgeCreate)
{
    // TEST: Creating an edge should remove dst from the root node set
    gtpo::graph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    EXPECT_TRUE( ( g.getRootNodeCount() == 2 ) );
    auto e = g.createEdge(n1, n2);
    EXPECT_TRUE( ( g.getRootNodeCount() == 1 ) );   // n2 is no longer a root node
}

TEST(GTpoTopology, edgeCreateBasicCircuit)
{
    // TEST: Creating an edge that is a circuit to a root node does not remove destination
    // from root nodes
    {   // Test with createEdge()
        gtpo::graph<> g;
        EXPECT_TRUE( g.getRootNodeCount() == 0 );
        auto n1 = g.createNode();
        EXPECT_TRUE( g.getRootNodeCount() == 1 );
        g.createEdge( n1, n1 );
        EXPECT_TRUE( g.getRootNodeCount() == 1 );
    }
    {   // Test with insertEdge()
        gtpo::graph<> g;
        EXPECT_TRUE( g.getRootNodeCount() == 0 );
        auto n1 = g.createNode();
        EXPECT_TRUE( g.getRootNodeCount() == 1 );
        auto e1 = std::make_shared<gtpo::edge<>>(n1, n1);
        g.insertEdge( e1 );
        EXPECT_TRUE( g.getRootNodeCount() == 1 );
    }
}

TEST(GTpoTopology, edgeInsertRootNode)
{
    // TEST: inserting an existing edge must remove dst from the root node set
    gtpo::graph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto e1 = std::make_shared<gtpo::edge<>>(n1, n2);
    EXPECT_EQ( g.getRootNodeCount(), 2 );
    g.insertEdge(e1);
    EXPECT_EQ( g.getRootNodeCount(), 1 );   // n2 is no longer a root node
}

TEST(GTpoTopology, edgeCreateRootNode)
{
    // TEST: inserting an existing edge must remove dst from the root node set
    gtpo::graph<> g;
    EXPECT_EQ( g.getRootNodeCount(), 0 );
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    EXPECT_EQ( g.getRootNodeCount(), 2 );
    g.createEdge(n1, n2);
    EXPECT_EQ( g.getRootNodeCount(), 1 );   // n2 is no longer a root node
}

TEST(GTpoTopology, edgeInsertBadTopology)
{
    // TEST: Inserting an edge with no source and/or destination should throw a bad_topology_error
    gtpo::graph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    EXPECT_THROW( g.insertEdge(gtpo::graph<>::shared_edge{}), gtpo::bad_topology_error );
    auto e1 = std::make_shared<gtpo::edge<>>(n1, gtpo::graph<>::weak_node{});
    auto e2 = std::make_shared<gtpo::edge<>>(gtpo::graph<>::weak_node{}, gtpo::graph<>::weak_node{});
    auto e3 = std::make_shared<gtpo::edge<>>(gtpo::graph<>::weak_node{}, n1);

    // Test error with insertEdge()
    EXPECT_THROW( g.insertEdge(e1), gtpo::bad_topology_error );
    EXPECT_THROW( g.insertEdge(e2), gtpo::bad_topology_error );
    EXPECT_THROW( g.insertEdge(e3), gtpo::bad_topology_error );

    // Test error with createEdge()
    EXPECT_THROW( g.createEdge(n1, gtpo::graph<>::weak_node{}), gtpo::bad_topology_error );
    EXPECT_THROW( g.createEdge(gtpo::graph<>::weak_node{}, n1), gtpo::bad_topology_error );
    EXPECT_THROW( g.createEdge(gtpo::graph<>::weak_node{}, gtpo::graph<>::weak_node{}), gtpo::bad_topology_error );

    // Test success with insertEdge()
    auto e4 = std::make_shared<gtpo::edge<>>(n1, n2);
    EXPECT_NO_THROW( g.insertEdge(e4) );

    // Test success with createEdge()
    EXPECT_NO_THROW( g.createEdge(n1, n2) );
}

TEST(GTpoTopology, edgeInsertparallel)
{
    { // Test if createEdge() successfully generate parallel edges
        gtpo::graph<> g;
        auto n1 = g.createNode();
        auto n2 = g.createNode();
        auto n3 = g.createNode();
        auto n4 = g.createNode();
        g.createEdge(n1, n2);
        EXPECT_EQ( g.getEdgeCount(n1, n2), 1 );
        g.createEdge(n2, n3);
        g.createEdge(n2, n3);
        g.createEdge(n2, n3);
        EXPECT_EQ( g.getEdgeCount(n2, n3), 3 );   // Do not require, it depend on selected graph data structure
        g.removeEdge(n2, n3);
        EXPECT_EQ( g.getEdgeCount(n2, n3), 2 ) ;
        g.removeAllEdges(n2, n3);
        EXPECT_EQ( g.getEdgeCount(n2, n3), 0 ) ;
        g.clear();
    }

    { // Test if insertEdge() successfully generate parallel edges
        gtpo::graph<> g;
        auto n1 = g.createNode();
        auto n2 = g.createNode();
        auto n3 = g.createNode();
        auto n4 = g.createNode();

        auto e1 = std::make_shared<gtpo::edge<>>(n1, n2);
        g.insertEdge(e1);
        EXPECT_EQ( g.getEdgeCount(n1, n2), 1 );

        auto e2 = std::make_shared<gtpo::edge<>>(n2, n3);
        auto e3 = std::make_shared<gtpo::edge<>>(n2, n3);
        auto e4 = std::make_shared<gtpo::edge<>>(n2, n3);
        g.insertEdge(e2);
        g.insertEdge(e3);
        g.insertEdge(e3);
        EXPECT_EQ( g.getEdgeCount(n2, n3), 3 );   // Do not require, it depend on selected graph data structure
        g.removeEdge(n2, n3);
        EXPECT_EQ( g.getEdgeCount(n2, n3), 2 ) ;
        g.removeAllEdges(n2, n3);
        EXPECT_EQ( g.getEdgeCount(n2, n3), 0 ) ;
        g.clear();
    }
}

TEST(GTpoTopology, edgeRemoveContains)
{
    // Graph must no longer contains() an edge that has been removed
    gtpo::graph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    EXPECT_EQ( g.getEdgeCount(), 0 );
    auto e1 = g.createEdge(n1, n2);
    EXPECT_EQ( g.getEdgeCount(), 1 );
    EXPECT_TRUE( g.contains(e1) );
    g.removeEdge(n1, n2);
    EXPECT_FALSE( g.contains(e1) );
    EXPECT_FALSE( g.hasEdge(n1, n2) );
}

TEST(GTpoTopology, edgeNodeInOutDegree)
{
    gtpo::graph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto n4 = g.createNode();
    EXPECT_EQ( g.getRootNodeCount(), 4 );
    g.createEdge(n1, n2);
    EXPECT_EQ( g.getRootNodeCount(), 3 );
    g.createEdge(n2, n3);
    g.createEdge(n2, n4);
    EXPECT_EQ( g.getRootNodeCount(), 1 );

    // Check in/out degree after multiple edge insertion
    gtpo::graph<>::shared_node node2 = n2.lock();
    EXPECT_TRUE( node2 );
    EXPECT_EQ( node2->getInDegree(), 1 );
    EXPECT_EQ( node2->getOutDegree(), 2 );
    EXPECT_EQ( node2->getInEdges().size(), 1 );
    EXPECT_EQ( node2->getOutEdges().size(), 2 );
    EXPECT_EQ( node2->getInNodes().size(), 1 );
    EXPECT_EQ( node2->getOutNodes().size(), 2 );

    // Check in/out degree after multiple edge removal
    ASSERT_THROW(g.removeEdge(n2, gtpo::graph<>::weak_node()), gtpo::bad_topology_error );
    //INFO( "Incorrect removeEdge() calls successfully throw a gtpo::bad_topology_error" );
    EXPECT_TRUE( g.hasEdge( n2, n4 ) );
    g.removeEdge(n2, n4);
    EXPECT_FALSE( g.hasEdge( n2, n4 ) );
    g.removeEdge(n2, n3);
    EXPECT_EQ( node2->getOutDegree(), 0 );
    EXPECT_EQ( node2->getOutEdges().size(), 0 );
    EXPECT_EQ( node2->getOutNodes().size(), 0 );
    EXPECT_EQ( g.getRootNodeCount(), 3 );
    EXPECT_TRUE( g.isRootNode( n3 ) );
    EXPECT_TRUE( g.isRootNode( n4 ) );

    g.clear();
}

TEST(GTpoTopology, removeNodeInOutDegree)
{
    // Note: Test GTpo removeNode() method:
    //    - Removing a node must also invalidate (ie remove) all its out/in orphants edges.
    gtpo::graph<> g;
    auto wn1 = g.createNode();
    auto wn2 = g.createNode();
    auto wn3 = g.createNode();
    g.createEdge(wn1, wn2);
    g.createEdge(wn2, wn3);
    // n1 now has 1 in node and 1 out node
    auto n2 = wn2.lock();
    EXPECT_EQ( n2->getOutDegree(), 1 );
    EXPECT_EQ( n2->getInDegree(), 1 );

    // Removing node n2 should also remove all edges
    g.removeNode( wn2 );
    EXPECT_EQ( g.getNodeCount(), 2 );
    EXPECT_EQ( g.getEdgeCount(), 0 );
    auto n1 = wn1.lock();
    auto n3 = wn3.lock();
    EXPECT_EQ( n1->getOutDegree(), 0 );
    EXPECT_EQ( n3->getInDegree(), 0 );
}
