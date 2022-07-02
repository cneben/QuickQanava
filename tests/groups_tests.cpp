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
// \file	groups_tests.cpp
// \author	benoit@qanava.org
// \date	2017 03 08
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
// GTpo groups tests
//-----------------------------------------------------------------------------

TEST(gtpo_group, basic)
{
    qan::Graph g;
    EXPECT_EQ(g.get_group_count(), 0);
}

TEST(gtpo_group, create)
{
    qan::Graph g;
    EXPECT_EQ( g.get_group_count(), 0 );
    auto g1 = g.insert_group(new qan::Group{});
    ASSERT_TRUE(g1);
    EXPECT_EQ(g.get_group_count(), 1);
    auto g2 = g.insert_group(new qan::Group{});
    ASSERT_TRUE(g2);
    EXPECT_EQ(g.get_group_count(), 2);
}

TEST(gtpo_group, remove)
{
    qan::Graph g;
    auto g1 = new qan::Group{};
    ASSERT_TRUE(g.insert_group(g1));
    EXPECT_EQ(g.get_group_count(), 1);
    g.remove_group(g1);
    EXPECT_EQ(g.get_group_count(), 0);
}

TEST(gtpo_group, removeGroupNodes)
{
    qan::Graph g;
    auto g1 = new qan::Group{};
    g.insert_group(g1);
    EXPECT_EQ(g.get_group_count(), 1);
    auto n1 = g.create_node();
    g.insert_node(n1);
    EXPECT_EQ(g.get_root_node_count(), 2);
    g.group_node(n1, g1);

    // Warning 20220702: that one is subject to change, actually nodes in
    // groups are still handled as root nodes.
    EXPECT_EQ(g.get_root_node_count(), 2);

    // g1
    //  +- n1
    g.remove_group(g1);
    EXPECT_EQ(g.get_group_count(), 0);

    // EXPECT n1.get_group() to return nullptr
    EXPECT_EQ(n1->getGroup(), nullptr);
    EXPECT_EQ(g.get_root_node_count(), 1);
}

TEST(gtpo_group, group_nodeExcept)
{
    qan::Graph g;
    auto group = new qan::Group{};
    ASSERT_TRUE(g.insert_group(group));
    EXPECT_FALSE(g.group_node(nullptr, group));
    auto n = g.create_node();
    g.insert_node(n);
    EXPECT_FALSE(g.group_node(n, nullptr));
    ASSERT_TRUE(g.group_node(n, group));
}


TEST(gtpo_group, group_node)
{
    qan::Graph g;
    EXPECT_EQ(g.get_group_count(), 0);
    auto group = new qan::Group{};
    g.insert_group(group);
    EXPECT_EQ(g.get_group_count(), 1);
    EXPECT_EQ(group->get_node_count(), 0);
    auto n = g.create_node();
    g.insert_node(n);
    g.group_node(n, group);
    EXPECT_EQ(group->get_node_count(), 1);
}

TEST(gtpo_group, ungroup_node_error)
{
    qan::Graph g;
    auto group = new qan::Group{};
    g.insert_group(group);
    EXPECT_FALSE(g.ungroup_node(nullptr, group));
    auto n = g.create_node();
    g.insert_node(n);
    EXPECT_FALSE(g.ungroup_node(n, nullptr));
}

TEST(gtpo_group, ungroup_node)
{
    qan::Graph g;
    auto group = new qan::Group{};
    g.insert_group(group);
    auto n = g.create_node();
    g.insert_node(n);
    g.group_node(n, group);
    EXPECT_EQ(group->get_node_count(), 1);
    EXPECT_EQ(g.get_group_count(), 1);
    g.ungroup_node(n, group);
    EXPECT_EQ(group->get_node_count(), 0);
}

TEST(gtpo_group, ungroup_ungrouped_node)
{
    qan::Graph g;
    auto group = new qan::Group{};
    g.insert_group(group);
    auto n = g.create_node();
    g.insert_node(n);
    // n is not part of group, it can't be ungrouped, a bad topo exception should be thrown.
    EXPECT_FALSE(g.ungroup_node(n, group));
}

TEST(gtpo_group, group_root_node)
{
    // Ensure that graph root node count is not modified when a node is grouped
    // CONTRACT: A node with zero in-degree is a root node even when it is grouped.
    qan::Graph g;
    auto group = new qan::Group{};
    g.insert_group(group);
    EXPECT_EQ(g.get_root_node_count(), 1);
    auto n = g.create_node();
    g.insert_node(n);
    EXPECT_EQ(g.get_root_node_count(), 2);

    g.group_node(n, group);
    EXPECT_EQ(group->get_node_count(), 1);
    EXPECT_EQ(g.get_group_count(), 1);
    EXPECT_EQ(g.get_root_node_count(), 2);

    g.ungroup_node(n, group);
    EXPECT_EQ(group->get_node_count(), 0);
    EXPECT_EQ(g.get_root_node_count(), 2);
}

TEST(gtpo_group, group_group)
{
    qan::Graph g;
    auto group1 = new qan::Group{};
    g.insert_group(group1);
    auto group2 = new qan::Group{};
    g.insert_group(group2);
    EXPECT_EQ(group1->get_node_count(), 0);
    g.group_node(group2, group1);
    EXPECT_EQ(group1->get_node_count(), 1);
}

TEST(gtpo_group, ungroup_group)
{
    qan::Graph g;
    auto group1 = new qan::Group{};
    g.insert_group(group1);
    auto group2 = new qan::Group{};
    g.insert_group(group2);
    g.group_node(group2, group1);
    EXPECT_EQ(group1->get_node_count(), 1);
    EXPECT_EQ(g.get_group_count(), 2);
    g.ungroup_node(group2, group1);
    EXPECT_EQ(group1->get_node_count(), 0);
}

TEST(gtpo_group, clearTopology)
{
    // TEST: clearing a graph with a group with content should remove group and group
    // topology
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);
    auto n2 = g.create_node();
    g.insert_node(n2);
    g.insert_edge(n1, n2);
    auto g1 = new qan::Group{};
    g.insert_group(g1);
    EXPECT_EQ(g.get_node_count(), 3);
    EXPECT_EQ(g.get_group_count(), 1);
    EXPECT_EQ(g.get_edge_count(), 1);
    g.group_node(n1, g1);
    g.group_node(n2, g1);
    g.clear();
    EXPECT_EQ(g.get_node_count(), 0);
    EXPECT_EQ(g.get_group_count(), 0);
    EXPECT_EQ(g.get_edge_count(), 0);
}

TEST(gtpo_group, remove_group_topology)
{
    // TEST: removing a group with content should preserve group topology
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);
    auto n2 = g.create_node();
    g.insert_node(n2);
    g.insert_edge(n1, n2);
    EXPECT_EQ(g.get_node_count(), 2);
    EXPECT_EQ(g.get_edge_count(), 1);

    auto g1 = new qan::Group{};
    g.insert_group(g1);
    EXPECT_EQ(g.get_group_count(), 1);
    g.group_node(n1, g1);
    g.group_node(n2, g1);

    g.remove_group(g1);
    EXPECT_EQ(g.get_group_count(), 0);

    // Group topology should remain in the graph
    EXPECT_EQ(g.get_node_count(), 2);
    EXPECT_EQ(g.get_edge_count(), 1);
    g.clear();
}

//-----------------------------------------------------------------------------
// GTpo Groups adjacent node/edge
//-----------------------------------------------------------------------------

TEST(gtpo_group, adjacent_edges)
{
    qan::Graph g;
    auto n1 = g.create_node();
    g.insert_node(n1);
    auto n2 = g.create_node();
    g.insert_node(n2);

    auto group1 = new qan::Group{};
    g.insert_group(group1);
    g.group_node(n1, group1);
    g.group_node(n2, group1);

    // Expecting 0 adjacent edges since no edge has been added
    auto g1AdjacentEdgeSet = group1->collectAdjacentEdges();
    EXPECT_EQ(g1AdjacentEdgeSet.size(), 0);

    // Adding an edge at graph level, expecting edge to be inserted in group adjacent edge set
    g.insert_edge(n1, n2);
    g1AdjacentEdgeSet = group1->collectAdjacentEdges();
    EXPECT_EQ(g1AdjacentEdgeSet.size(), 1);

    // Removing the edge at graph level, expecting group empty adjacent edge
    g.remove_edge(n1, n2);
    g1AdjacentEdgeSet = group1->collectAdjacentEdges();
    EXPECT_EQ(g1AdjacentEdgeSet.size(), 0);

    g.insert_edge(n1, n2);
    g1AdjacentEdgeSet = group1->collectAdjacentEdges();
    EXPECT_EQ(g1AdjacentEdgeSet.size(), 1);

    // Removing node 1 from group, edge is still adjacent
    g.ungroup_node(n1, group1);
    g1AdjacentEdgeSet = group1->collectAdjacentEdges();
    EXPECT_EQ(g1AdjacentEdgeSet.size(), 1);

    // Removing node 2 from group, edge is no longer adjacent to group
    g.ungroup_node(n2, group1);
    g1AdjacentEdgeSet = group1->collectAdjacentEdges();
    EXPECT_EQ(g1AdjacentEdgeSet.size(), 0);
}

TEST(gtpo_group, adjacent_edges_nodes)
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
    auto n5 = g.create_node();
    g.insert_node(n5);

    auto e1 = g.insert_edge(n1, n2);
    auto e2 = g.insert_edge(n1, n3);
    auto e3 = g.insert_edge(n2, n4);
    auto e4 = g.insert_edge(n3, n5);
    auto e5 = g.insert_edge(n4, n3);

    auto g1 = new qan::Group{};
    g.insert_group(g1);
    g.group_node(n3, g1);
    g.group_node(n4, g1);
    g.group_node(n5, g1);

    // See manual topology section, it is the same topology
    // Check that group 1 adjacent edge set is {e2, e3, e4, e5}
    auto g1AdjacentEdgeSet = g1->collectAdjacentEdges();
    EXPECT_TRUE(g1AdjacentEdgeSet.find(e2) != g1AdjacentEdgeSet.end());
    EXPECT_TRUE(g1AdjacentEdgeSet.find(e3) != g1AdjacentEdgeSet.end());
    EXPECT_TRUE(g1AdjacentEdgeSet.find(e4) != g1AdjacentEdgeSet.end());
    EXPECT_TRUE(g1AdjacentEdgeSet.find(e5) != g1AdjacentEdgeSet.end());
    EXPECT_EQ(g1AdjacentEdgeSet.size(), 4);

    // Removing a node from the group should remove its in/out edges from group adjacent edge set
    // Here, removing n4 should remove e3 and e5
    g.ungroup_node(n4, g1);
    g1AdjacentEdgeSet = g1->collectAdjacentEdges();
    EXPECT_EQ(g1AdjacentEdgeSet.size(), 3);

    g.clear();
}
