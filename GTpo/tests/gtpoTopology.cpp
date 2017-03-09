/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include <gtpoProgressNotifier.h>

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//-----------------------------------------------------------------------------
// GTpo topology tests
//-----------------------------------------------------------------------------

TEST(GTpoTopology, insertNodeCount)
{
    gtpo::GenGraph<> g;
    auto nc = g.getNodeCount();
    auto rnc = g.getRootNodeCount();
    g.createNode();
    EXPECT_TRUE( ( g.getNodeCount() == nc + 1 ) );
    EXPECT_TRUE( ( g.getRootNodeCount() == rnc + 1 ) );
    g.clear();
}

TEST(GTpoTopology, removeNodeCount)
{
    gtpo::GenGraph<> g;
    gtpo::GenGraph<>::WeakNode n = g.createNode();
    auto nc = g.getNodeCount();
    auto rnc = g.getRootNodeCount();
    g.installRootNode( n ); // Just a test, it should not be called by the user directly
    EXPECT_TRUE( g.isRootNode( n ) );
    EXPECT_TRUE( g.contains( n ) );
    g.removeNode( n );
    EXPECT_FALSE( g.contains( n ) );
    EXPECT_TRUE( ( g.getNodeCount() == nc - 1 ) );
    EXPECT_TRUE( ( g.getRootNodeCount() == rnc - 1 ) ) ;
    g.createNode();
    g.clear();
}

TEST(GTpoTopology, graphClear)
{
    // TEST: clearing an empty graph, expecting no node after clearing an empty graph
    gtpo::GenGraph<> g;
    EXPECT_EQ( g.getNodeCount(), 0 );
    g.clear();
    EXPECT_EQ( g.getNodeCount(), 0 );
}

TEST(GTpoTopo, graphClearNode)
{
    // TEST: clearing a graph with one node should lead to an empty graph
    gtpo::GenGraph<> g;
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
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    EXPECT_EQ( g.getNodeCount(), 2 );
    EXPECT_TRUE( g.contains(e1) );
    g.clear();
    EXPECT_EQ( g.getNodeCount(), 0 );
    EXPECT_FALSE( g.contains(e1) );
    EXPECT_TRUE( n1.expired() );
    EXPECT_TRUE( n2.expired() );
    EXPECT_TRUE( e1.expired() );
}

TEST(GTpoTopology, createEdgeBadTopology)
{
    // TEST: Creating an edge with no source and/or destination should throw a bad_topology_error
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    EXPECT_THROW( g.createEdge(n1, gtpo::GenGraph<>::WeakNode{}), gtpo::bad_topology_error );
    EXPECT_THROW( g.createEdge(gtpo::GenGraph<>::WeakNode{}, gtpo::GenGraph<>::WeakNode{}), gtpo::bad_topology_error );
    EXPECT_THROW( g.createEdge(gtpo::GenGraph<>::WeakNode{}, n1), gtpo::bad_topology_error );

    auto n2 = g.createNode();
    EXPECT_NO_THROW( g.createEdge(n1, n2) );
}

TEST(GTpoTopology, createEdge)
{
    // TEST: Creating an edge should remove dst from the root node set
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    EXPECT_TRUE( ( g.getRootNodeCount() == 2 ) );
    auto e = g.createEdge(n1, n2);
    EXPECT_TRUE( ( g.getRootNodeCount() == 1 ) );   // n2 is no longer a root node
}

TEST(GTpoTopology, createEdgeBasicCircuit)
{
    // TEST: Creating an edge that is a circuit to a root node does not remove destination
    // from root nodes
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    EXPECT_TRUE( g.getRootNodeCount() == 1 );
    g.createEdge( n1, n1 );
    EXPECT_TRUE( g.getRootNodeCount() == 1 );
}

TEST(GTpoTopology, insertEdge)
{
    // TEST: inserting an existing edge must remove dst from the root node set
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto e1 = std::make_shared<gtpo::GenEdge<>>(n1, n2);
    EXPECT_TRUE( ( g.getRootNodeCount() == 2 ) );
    g.insertEdge(e1);
    EXPECT_TRUE( ( g.getRootNodeCount() == 1 ) );   // n2 is no longer a root node
}

TEST(GTpoTopology, insertEdgeBadTopology)
{
    // TEST: Inserting an edge with no source and/or destination should throw a bad_topology_error
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    EXPECT_THROW( g.insertEdge(gtpo::GenGraph<>::SharedEdge{}), gtpo::bad_topology_error );
    auto e1 = std::make_shared<gtpo::GenEdge<>>(n1, gtpo::GenGraph<>::WeakNode{});
    auto e2 = std::make_shared<gtpo::GenEdge<>>(gtpo::GenGraph<>::WeakNode{}, gtpo::GenGraph<>::WeakNode{});
    auto e3 = std::make_shared<gtpo::GenEdge<>>(gtpo::GenGraph<>::WeakNode{}, n1);
    EXPECT_THROW( g.insertEdge(e1), gtpo::bad_topology_error );
    EXPECT_THROW( g.insertEdge(e2), gtpo::bad_topology_error );
    EXPECT_THROW( g.insertEdge(e3), gtpo::bad_topology_error );

    auto e4 = std::make_shared<gtpo::GenEdge<>>(n1, n2);
    EXPECT_NO_THROW( g.insertEdge(e4) );
}

TEST(GTpoTopology, insertEdgeBasicCircuit)
{
    // TEST: Creating an edge that is a circuit to a root node does not remove destination
    // from root nodes
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto e1 = std::make_shared<gtpo::GenEdge<>>(n1, n1);
    EXPECT_TRUE( g.getRootNodeCount() == 1 );
    g.insertEdge(e1);
    EXPECT_TRUE( ( g.getRootNodeCount() == 1 ) );   // n2 is no longer a root node
}

TEST(GTpoTopology, removeEdgeContains)
{
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    EXPECT_TRUE( g.contains(e1) );
    g.removeEdge(n1, n2);
    EXPECT_FALSE( g.contains(e1) );
    EXPECT_FALSE( g.hasEdge(n1, n2) );
}

TEST(GTpoTopology, inOutDegree)
{
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto n4 = g.createNode();
    EXPECT_TRUE( g.getRootNodeCount() == 4 );
    g.createEdge(n1, n2);
    EXPECT_TRUE( g.getRootNodeCount() == 3 );
    g.createEdge(n2, n3);
    g.createEdge(n2, n4);
    EXPECT_TRUE( g.getRootNodeCount() == 1 );

    // Check in/out degree after multiple edge insertion
    gtpo::GenGraph<>::SharedNode node2 = n2.lock();
    EXPECT_TRUE( node2 );
    EXPECT_TRUE( node2->getInDegree() == 1 );
    EXPECT_TRUE( node2->getOutDegree() == 2 );
    EXPECT_TRUE( node2->getInEdges().size() == 1 );
    EXPECT_TRUE( node2->getOutEdges().size() == 2 );
    EXPECT_TRUE( node2->getInNodes().size() == 1 );
    EXPECT_TRUE( node2->getOutNodes().size() == 2 );

    // Check in/out degree after multiple edge removal
    ASSERT_THROW(g.removeEdge(n2, gtpo::GenGraph<>::WeakNode()), gtpo::bad_topology_error );
    //INFO( "Incorrect removeEdge() calls successfully throw a gtpo::bad_topology_error" );
    EXPECT_TRUE( g.hasEdge( n2, n4 ) == true );
    g.removeEdge(n2, n4);
    EXPECT_TRUE( g.hasEdge( n2, n4 ) == false );
    g.removeEdge(n2, n3);
    EXPECT_TRUE( node2->getOutDegree() == 0 );
    EXPECT_TRUE( node2->getOutEdges().size() == 0 );
    EXPECT_TRUE( node2->getOutNodes().size() == 0 );
    EXPECT_TRUE( g.getRootNodeCount() == 3 );
    EXPECT_TRUE( g.isRootNode( n3 ) );
    EXPECT_TRUE( g.isRootNode( n4 ) );

    g.clear();
}

TEST(GTpoTopology, removeNodeInOutDegree)
{
    // Note: Test GTpo removeNode() method:
    //    - Removing a node must also invalidate (ie remove) all its out/in orphants edges.
    gtpo::GenGraph<> g;
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

TEST(GTpoTopology, parallelEdges)
{
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto n4 = g.createNode();
    g.createEdge(n1, n2);
    EXPECT_TRUE( g.getEdgeCount(n1, n2) == 1 );
    g.createEdge(n2, n3);
    g.createEdge(n2, n3);
    g.createEdge(n2, n3);
    EXPECT_TRUE( g.getEdgeCount(n2, n3) == 3 );   // Do not require, it depend on selected graph data structure
    g.removeEdge(n2, n3);
    EXPECT_TRUE( g.getEdgeCount(n2, n3) == 2 ) ;
    g.removeAllEdges(n2, n3);
    EXPECT_TRUE( g.getEdgeCount(n2, n3) == 0 ) ;
    g.clear();
}

//-----------------------------------------------------------------------------
// GTpo topology restricted hyper edges tests
//-----------------------------------------------------------------------------

TEST(GTpoTopology, createHEdge)
{
    // TEST: create hyper edge and expect hyper edge in degree change
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    EXPECT_TRUE( g.getEdgeCount(n1, n2) == 1 );
    auto he1 = g.createEdge(n3, e1);
    {
        auto e1Ptr = e1.lock();
        ASSERT_TRUE( e1Ptr.get() != nullptr );
        EXPECT_EQ( e1Ptr->getInHDegree(), 1 );
    }
}

TEST(GTpoTopology, inHDegree)
{
    // TEST: create multiple hyper edge and expect an hyper edge in degree > 1
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    EXPECT_TRUE( g.getEdgeCount(n1, n2) == 1 );
    g.createEdge(n3, e1);
    g.createEdge(n3, e1);
    auto e1Ptr = e1.lock();
    ASSERT_TRUE( e1Ptr.get() != nullptr );
    EXPECT_EQ( e1Ptr->getInHDegree(), 2 );
}

TEST(GTpoTopology, containsHEdge)
{
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    auto he1 = g.createEdge(n3, e1);
    EXPECT_TRUE( g.contains(he1) );
}

TEST(GTpoTopology, hasHEdge)
{
    // TEST: expecting and edge when an hEdge has been created
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    EXPECT_FALSE( g.hasEdge(n3, e1) );
    auto he1 = g.createEdge(n3, e1);
    EXPECT_TRUE( g.hasEdge(n3, e1) );
}

TEST(GTpoTopology, removeHEdgeHasEdge)
{
    // TEST: Expecting hasEdge to return true after having removed an hyper edge
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    EXPECT_FALSE( g.hasEdge(n3, e1) );
    auto he1 = g.createEdge(n3, e1);
    EXPECT_TRUE( g.hasEdge(n3, e1) );
    g.removeEdge(he1);
    EXPECT_FALSE( g.hasEdge(n3, e1) );
    EXPECT_TRUE( he1.expired() );       // Hyper edge should have been released after destruction
}

TEST(GTpoTopology, removeHEdgeHInDegree)
{
    // TEST: Removing an hedge should left it's destination edge with no in hEdges
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    auto he1 = g.createEdge(n3, e1);

    auto e1Ptr = e1.lock();
    ASSERT_TRUE( e1Ptr.get() != nullptr );
    EXPECT_EQ( e1Ptr->getInHDegree(), 1 );

    g.removeEdge(he1);
    EXPECT_FALSE( g.hasEdge(n3, e1) );
    EXPECT_TRUE( he1.expired() );       // Hyper edge should have been released after destruction

    ASSERT_TRUE( e1Ptr.get() != nullptr );
    EXPECT_EQ( e1Ptr->getInHDegree(), 0 );
}
/*
TEST(GTpoTopology, removeEdgeWithHEdge)
{
    // TEST: Removing an edge with one or more in hEdge should remove edge and all in hEdges
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    auto he1 = g.createEdge(n3, e1);
    g.removeEdge(e1);
    EXPECT_TRUE( he1.expired() );  // In hEdge should have been destroyed
    EXPECT_FALSE( g.hasEdge(n3, e1) );
}

*/
