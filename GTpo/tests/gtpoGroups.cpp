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
// \file	gtpoGroups.cpp
// \author	benoit@qanava.org
// \date	2017 03 08
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
// GTpo groups tests
//-----------------------------------------------------------------------------

TEST(GTpoGroups, basic)
{
    gtpo::GenGraph<> g;
    EXPECT_EQ( g.getGroupCount(), 0 );
}

TEST(GTpoGroups, createGroup)
{
    gtpo::GenGraph<> g;
    EXPECT_EQ( g.getGroupCount(), 0 );
    auto group1 = g.createGroup().lock();
    ASSERT_TRUE(group1);
    EXPECT_EQ( g.getGroupCount(), 1 );
    auto group2 = g.createGroup().lock();
    ASSERT_TRUE(group2);
    EXPECT_EQ( g.getGroupCount(), 2 );
}

TEST(GTpoGroups, removeGroup)
{
    gtpo::GenGraph<> g;
    auto group = g.createGroup().lock();
    ASSERT_TRUE(group);
    EXPECT_EQ( g.getGroupCount(), 1 );
    g.removeGroup(group);
    EXPECT_EQ( g.getGroupCount(), 0 );
}

TEST(GTpoGroups, groupNodeExcept)
{
    gtpo::GenGraph<> g;
    auto group = g.createGroup().lock();
    using WeakGroup = gtpo::GenGroup<>::WeakGroup;
    using WeakNode = gtpo::GenGroup<>::WeakNode;
    EXPECT_THROW( g.groupNode(group, WeakNode{}), gtpo::bad_topology_error );
    auto n = g.createNode();
    EXPECT_THROW( g.groupNode(WeakGroup{}, n), gtpo::bad_topology_error );
    ASSERT_NO_THROW( g.groupNode(group, n) );
}

TEST(GTpoGroups, groupNode)
{
    gtpo::GenGraph<> g;
    auto group = g.createGroup().lock();
    ASSERT_TRUE(group);
    EXPECT_EQ( g.getGroupCount(), 1 );
    EXPECT_EQ( group->getNodeCount(), 0 );
    auto n = g.createNode();
    g.groupNode(group, n);
    EXPECT_EQ( group->getNodeCount(), 1 );
}

TEST(GTpoGroups, ungroupNodeExcept)
{
    gtpo::GenGraph<> g;
    auto group = g.createGroup().lock();
    using WeakGroup = gtpo::GenGroup<>::WeakGroup;
    using WeakNode = gtpo::GenGroup<>::WeakNode;
    EXPECT_THROW( g.ungroupNode(group, WeakNode{}), gtpo::bad_topology_error );
    auto n = g.createNode();
    EXPECT_THROW( g.ungroupNode(WeakGroup{}, n), gtpo::bad_topology_error );
}

TEST(GTpoGroups, ungroupNode)
{
    gtpo::GenGraph<> g;
    auto group = g.createGroup().lock();
    ASSERT_TRUE(group);
    auto n = g.createNode();
    g.groupNode(group, n);
    EXPECT_EQ( group->getNodeCount(), 1 );
    EXPECT_EQ( g.getGroupCount(), 1 );
    g.ungroupNode(group, n);
    EXPECT_EQ( group->getNodeCount(), 0 );
}

TEST(GTpoGroups, ungroupUngroupedNode)
{
    gtpo::GenGraph<> g;
    auto group = g.createGroup().lock();
    ASSERT_TRUE(group);
    auto n = g.createNode();
    // n is not part of group, it can't be ungrouped, a bad topo exception should be thrown.
    EXPECT_THROW( g.ungroupNode(group, n), gtpo::bad_topology_error );
}

TEST(GTpoGroups, groupGroup)
{
    gtpo::GenGraph<> g;
    auto group1 = g.createGroup().lock();
    auto group2 = g.createGroup().lock();
    using WeakGroup = gtpo::GenGroup<>::WeakGroup;
    EXPECT_EQ( group1->getNodeCount(), 0 );
    g.groupNode(group1, WeakGroup{group2});
    EXPECT_EQ( group1->getNodeCount(), 1 );
}

TEST(GTpoGroups, ungroupGroup)
{
    gtpo::GenGraph<> g;
    auto group1 = g.createGroup().lock();
    auto group2 = g.createGroup().lock();
    using WeakGroup = gtpo::GenGroup<>::WeakGroup;
    g.groupNode(group1, WeakGroup{group2});
    EXPECT_EQ( group1->getNodeCount(), 1 );
    EXPECT_EQ( g.getGroupCount(), 2 );
    g.ungroupNode(group1, WeakGroup{group2});
    EXPECT_EQ( group1->getNodeCount(), 0 );
}

TEST(GTpoGroups, clear)
{
    // TEST: clearing a graph with two nodes linked by an edge in a group should lead to an empty graph
/*    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto e1 = g.createEdge(n1, n2);
    auto g1 = g.createGroup();
    {
        auto g1Ptr = g1.lock();     // g1Ptr is released at the end of block to ensure all groups are cleared
        if ( g1Ptr ) {
            g1Ptr->insertNode( n1 );
            g1Ptr->insertNode( n2 );
        }
    }
    g.clear();
    EXPECT_TRUE( n1.expired() );
    EXPECT_EQ( g.getNodeCount(), 0 );
    EXPECT_EQ( g.getGroupCount(), 0 );
    EXPECT_TRUE( g1.expired() );
 */
}


//-----------------------------------------------------------------------------
// GTpo Groups adjacent node/edge
//-----------------------------------------------------------------------------

TEST(GTpoGroups, adjacentEdgesSimple)
{
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();

    auto group1 = g.createGroup().lock();
    g.groupNode(group1, n1);
    g.groupNode(group1, n2);

    // Expecting 0 adjacent edges since no edge has been added
    const auto& g1AdjacentEdgeSet = group1->getAdjacentEdges();
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 0 );

    // Adding an edge at graph level, expecting edge to be inserted in group adjacent edge set
    g.createEdge( n1, n2 );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 1 );

    // Removing the edge at graph level, expecting group empty adjacent edge
    g.removeEdge( n1, n2 );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 0 );

    auto e2 = g.createEdge( n1, n2 );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 1 );

    // Removing node 1 from group, edge is still adjacent
    g.ungroupNode(group1, n1);
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 1 );

    // Removing node 2 from group, edge is no longer adjacent to group
    g.ungroupNode(group1, n2);
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 0 );
}

TEST(GTpoGroups, adjacentEdges)
{
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto n3 = g.createNode();
    auto n4 = g.createNode();
    auto n5 = g.createNode();

    auto e1 = g.createEdge( n1, n2 );
    auto e2 = g.createEdge( n1, n3 );
    auto e3 = g.createEdge( n2, n4 );
    auto e4 = g.createEdge( n3, n5 );
    auto e5 = g.createEdge( n4, n3 );

    auto g1 = g.createGroup().lock();
    g.groupNode(g1, n3);
    g.groupNode(g1, n4);
    g.groupNode(g1, n5);

    // See manual topology section, it is the same topology
    // Check that group 1 adjacent edge set is {e2, e3, e4, e5}
    auto& g1AdjacentEdgeSet = g1->getAdjacentEdges();
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e2 ) );
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e3 ) );
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e4 ) );
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e5 ) );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 4 );

    // Removing a node from the group should remove its in/out edges from group adjacent edge set
    // Here, removing n4 should remove e3 and e5
    g.ungroupNode(g1, n4);
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 3 );

    g.clear();
}

