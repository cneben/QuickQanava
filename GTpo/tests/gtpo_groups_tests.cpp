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
// \file	gtpo_groups_tests.cpp
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
    gtpo::graph<> g;
    EXPECT_EQ( g.get_group_count(), 0 );
}

TEST(GTpoGroups, createGroup)
{
    gtpo::graph<> g;
    EXPECT_EQ( g.get_group_count(), 0 );
    auto group1 = g.create_group().lock();
    ASSERT_TRUE(group1);
    EXPECT_EQ( g.get_group_count(), 1 );
    auto group2 = g.create_group().lock();
    ASSERT_TRUE(group2);
    EXPECT_EQ( g.get_group_count(), 2 );
}

TEST(GTpoGroups, remove_group)
{
    gtpo::graph<> g;
    auto group = g.create_group().lock();
    ASSERT_TRUE(group);
    EXPECT_EQ( g.get_group_count(), 1 );
    g.remove_group(group);
    EXPECT_EQ( g.get_group_count(), 0 );
}

TEST(GTpoGroups, group_nodeExcept)
{
    gtpo::graph<> g;
    auto group = g.create_group().lock();
    using weak_group_t = gtpo::group<>::weak_group_t;
    using weak_node_t = gtpo::group<>::weak_node_t;
    EXPECT_THROW( g.group_node(weak_node_t{}, group), gtpo::bad_topology_error );
    auto n = g.create_node();
    EXPECT_THROW( g.group_node(n, weak_group_t{}), gtpo::bad_topology_error );
    ASSERT_NO_THROW( g.group_node(n, group) );
}

TEST(GTpoGroups, group_node)
{
    gtpo::graph<> g;
    EXPECT_EQ( g.get_group_count(), 0 );
    auto group = g.create_group().lock();
    ASSERT_TRUE(group);
    EXPECT_EQ( g.get_group_count(), 1 );
    EXPECT_EQ( group->get_node_count(), 0 );
    auto n = g.create_node();
    g.group_node(n, group);
    EXPECT_EQ( group->get_node_count(), 1 );
}

TEST(GTpoGroups, ungroup_nodeExcept)
{
    gtpo::graph<> g;
    auto group = g.create_group().lock();
    using weak_group_t = gtpo::group<>::weak_group_t;
    using weak_node_t = gtpo::group<>::weak_node_t;
    EXPECT_THROW( g.ungroup_node(weak_node_t{}, group), gtpo::bad_topology_error );
    auto n = g.create_node();
    EXPECT_THROW( g.ungroup_node(n, weak_group_t{}), gtpo::bad_topology_error );
}

TEST(GTpoGroups, ungroup_node)
{
    gtpo::graph<> g;
    auto group = g.create_group().lock();
    ASSERT_TRUE(group);
    auto n = g.create_node();
    g.group_node(n, group);
    EXPECT_EQ( group->get_node_count(), 1 );
    EXPECT_EQ( g.get_group_count(), 1 );
    g.ungroup_node(n, group);
    EXPECT_EQ( group->get_node_count(), 0 );
}

TEST(GTpoGroups, ungroupUngroupedNode)
{
    gtpo::graph<> g;
    auto group = g.create_group().lock();
    ASSERT_TRUE(group);
    auto n = g.create_node();
    // n is not part of group, it can't be ungrouped, a bad topo exception should be thrown.
    EXPECT_THROW( g.ungroup_node(n, group), gtpo::bad_topology_error );
}

TEST(GTpoGroups, groupRootNodeContract)
{
    // Ensure that graph root node count is not modified when a node is grouped
    // CONTRACT: A node with zero in-degree is a root node even when it is grouped.
    gtpo::graph<> g;
    auto group = g.create_group().lock();
    ASSERT_TRUE(group);
    EXPECT_EQ(g.get_root_node_count(), 0);
    auto n = g.create_node();
    EXPECT_EQ(g.get_root_node_count(), 1);

    g.group_node(n, group);
    EXPECT_EQ( group->get_node_count(), 1 );
    EXPECT_EQ( g.get_group_count(), 1 );
    EXPECT_EQ(g.get_root_node_count(), 1);

    g.ungroup_node(n, group);
    EXPECT_EQ( group->get_node_count(), 0 );
    EXPECT_EQ(g.get_root_node_count(), 1);
}

TEST(GTpoGroups, groupGroup)
{
    gtpo::graph<> g;
    auto group1 = g.create_group().lock();
    auto group2 = g.create_group().lock();
    using weak_group_t = gtpo::group<>::weak_group_t;
    EXPECT_EQ( group1->get_node_count(), 0 );
    g.group_node(weak_group_t{group2}, group1);
    EXPECT_EQ( group1->get_node_count(), 1 );
}

TEST(GTpoGroups, ungroupGroup)
{
    gtpo::graph<> g;
    auto group1 = g.create_group().lock();
    auto group2 = g.create_group().lock();
    using weak_group_t = gtpo::group<>::weak_group_t;
    g.group_node(weak_group_t{group2}, group1);
    EXPECT_EQ( group1->get_node_count(), 1 );
    EXPECT_EQ( g.get_group_count(), 2 );
    g.ungroup_node(weak_group_t{group2}, group1);
    EXPECT_EQ( group1->get_node_count(), 0 );
}

TEST(GTpoGroups, clearTopology)
{
    // TEST: clearing a graph with a group with content should remove group and group
    // topology
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    auto e1 = g.create_edge(n1, n2);
    auto g1 = g.create_group();
    EXPECT_EQ( g.get_node_count(), 2 );
    EXPECT_EQ( g.get_group_count(), 1 );
    EXPECT_EQ( g.get_edge_count(), 1 );
    g.group_node( n1, g1 );
    g.group_node( n2, g1 );
    g.clear();
    EXPECT_TRUE( n1.expired() );
    EXPECT_EQ( g.get_node_count(), 0 );
    EXPECT_EQ( g.get_group_count(), 0 );
    EXPECT_EQ( g.get_edge_count(), 0 );
    EXPECT_TRUE( g1.expired() );
}

TEST(GTpoGroups, remove_groupTopology)
{
    // TEST: removing a group with content should preserve group topology
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    auto e1 = g.create_edge(n1, n2);
    EXPECT_EQ( g.get_node_count(), 2 );
    EXPECT_EQ( g.get_edge_count(), 1 );

    auto g1 = g.create_group();
    EXPECT_EQ( g.get_group_count(), 1 );
    g.group_node( n1, g1 );
    g.group_node( n2, g1 );

    g.remove_group(g1);
    EXPECT_EQ( g.get_group_count(), 0 );

    // Group topology should remain in the graph
    EXPECT_EQ( g.get_node_count(), 2 );
    EXPECT_EQ( g.get_edge_count(), 1 );
    g.clear();
}

//-----------------------------------------------------------------------------
// GTpo Groups adjacent node/edge
//-----------------------------------------------------------------------------

TEST(GTpoGroups, adjacentEdgesSimple)
{
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();

    auto group1 = g.create_group().lock();
    g.group_node(n1, group1);
    g.group_node(n2, group1);

    // Expecting 0 adjacent edges since no edge has been added
    const auto& g1AdjacentEdgeSet = group1->get_adjacent_edges();
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 0 );

    // Adding an edge at graph level, expecting edge to be inserted in group adjacent edge set
    g.create_edge( n1, n2 );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 1 );

    // Removing the edge at graph level, expecting group empty adjacent edge
    g.remove_edge( n1, n2 );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 0 );

    auto e2 = g.create_edge( n1, n2 );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 1 );

    // Removing node 1 from group, edge is still adjacent
    g.ungroup_node(n1, group1);
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 1 );

    // Removing node 2 from group, edge is no longer adjacent to group
    g.ungroup_node(n2, group1);
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 0 );
}

TEST(GTpoGroups, adjacentEdges)
{
    gtpo::graph<> g;
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    auto n3 = g.create_node();
    auto n4 = g.create_node();
    auto n5 = g.create_node();

    auto e1 = g.create_edge( n1, n2 );
    auto e2 = g.create_edge( n1, n3 );
    auto e3 = g.create_edge( n2, n4 );
    auto e4 = g.create_edge( n3, n5 );
    auto e5 = g.create_edge( n4, n3 );

    auto g1 = g.create_group().lock();
    g.group_node(n3, g1);
    g.group_node(n4, g1);
    g.group_node(n5, g1);

    // See manual topology section, it is the same topology
    // Check that group 1 adjacent edge set is {e2, e3, e4, e5}
    auto& g1AdjacentEdgeSet = g1->get_adjacent_edges();
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e2 ) );
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e3 ) );
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e4 ) );
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e5 ) );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 4 );

    // Removing a node from the group should remove its in/out edges from group adjacent edge set
    // Here, removing n4 should remove e3 and e5
    g.ungroup_node(n4, g1);
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 3 );

    g.clear();
}

