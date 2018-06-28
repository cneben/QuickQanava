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
// \file	gtpo_behaviours_tests.cpp
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

using testing::AtLeast;

//-----------------------------------------------------------------------------
// GTpo Graph behaviour tests
//-----------------------------------------------------------------------------

template < class config_t = gtpo::default_config >
class GraphBehaviourMock : public gtpo::dynamic_graph_behaviour<config_t>
{
public:
    GraphBehaviourMock() { }
    virtual ~GraphBehaviourMock() override { std::cerr << "Deleting mock graph behaviour..."; }

    using weak_node_t      = std::weak_ptr< typename config_t::final_node_t >;
    using weak_edge_t      = std::weak_ptr< typename config_t::final_edge_t >;
    using weak_group_t     = std::weak_ptr< typename config_t::final_group_t >;

protected:
    virtual void    on_node_inserted( weak_node_t& ) noexcept override { std::cerr << "GraphBehaviourMock::on_node_inserted()" << std::endl; mockNodeInserted(); }
    virtual void    on_node_removed( weak_node_t& ) noexcept override { std::cerr << "GraphBehaviourMock::on_node_removed()" << std::endl; mockNodeRemoved(); }
    virtual void    on_edge_inserted( weak_edge_t& ) noexcept override { std::cerr << "GraphBehaviourMock::on_edge_inserted()" << std::endl; mockEdgeInserted(); }
    virtual void    on_edge_removed( weak_edge_t& ) noexcept override { std::cerr << "GraphBehaviourMock::on_edge_removed()" << std::endl; mockEdgeRemoved(); }
    virtual void    on_group_inserted( weak_group_t& ) noexcept override { std::cerr << "GraphBehaviourMock::on_group_inserted()" << std::endl; mockGroupInserted(); }
    virtual void    on_group_removed( weak_group_t& ) noexcept override { std::cerr << "GraphBehaviourMock::on_group_removed()" << std::endl; mockGroupRemoved(); }

public:
    MOCK_METHOD0(mockNodeInserted, void(void));
    MOCK_METHOD0(mockNodeRemoved, void(void));
    MOCK_METHOD0(mockEdgeInserted, void(void));
    MOCK_METHOD0(mockEdgeRemoved, void(void));
    MOCK_METHOD0(mockGroupInserted, void(void));
    MOCK_METHOD0(mockGroupRemoved, void(void));
};

TEST(GTpoBehaviour, graphEnabledDisabled)
{
    gtpo::graph<> g;

    using MockGraphBehaviour = GraphBehaviourMock<>;
    auto mockBehaviour = new MockGraphBehaviour();

    // Test basic enable/disable behaviour property
    EXPECT_TRUE( mockBehaviour->isEnabled() );
    mockBehaviour->disable();
    EXPECT_TRUE( !mockBehaviour->isEnabled() );

    // FIXME: checker move et copy et == ...
}

TEST(GTpoBehaviour, graphBehaviour)
{
    gtpo::graph<> g;

    using MockGraphBehaviour = GraphBehaviourMock<>;
    auto mockBehaviour = new MockGraphBehaviour();  // Do not use unique_ptr here because of gmock

    // behaviour notify virtual methods should be called when graph topology changes
    g.add_dynamic_graph_behaviour( std::unique_ptr<MockGraphBehaviour>(mockBehaviour) );

    // node_inserted() notification
    EXPECT_CALL(*mockBehaviour, mockNodeInserted()).Times(1);
    auto n = g.create_node();
    EXPECT_CALL(*mockBehaviour, mockNodeInserted()).Times(AtLeast(1));  // Reset counter...

    // node_inserted() notification
    EXPECT_CALL(*mockBehaviour, mockNodeRemoved()).Times(1);
    g.remove_node( n );

    // edge_inserted() notification
    auto s = g.create_node();
    auto d = g.create_node();
    EXPECT_CALL(*mockBehaviour, mockEdgeInserted()).Times(1);
    auto e = g.create_edge(s, d);

    // edge_removed() notification
    EXPECT_CALL(*mockBehaviour, mockEdgeRemoved()).Times(1);
    g.remove_edge(e);

    // group_inserted() notification
    EXPECT_CALL(*mockBehaviour, mockGroupInserted()).Times(1);
    auto gg = g.create_group();

    // group_removed() notification
    EXPECT_CALL(*mockBehaviour, mockGroupRemoved()).Times(1);
    g.remove_group( gg );

    //delete mockBehaviour;
}

//-----------------------------------------------------------------------------
// GTpo Group behaviour tests
//-----------------------------------------------------------------------------

template < class config_t = gtpo::default_config >
class GroupBehaviourMock : public gtpo::dynamic_group_behaviour< config_t >
{
public:
    GroupBehaviourMock() { }
    virtual ~GroupBehaviourMock() { }

    using weak_node_t      = std::weak_ptr< typename config_t::final_node_t >;
    using weak_edge_t      = std::weak_ptr< typename config_t::final_edge_t >;
    using weak_group_t     = std::weak_ptr< typename config_t::final_group_t >;

protected:
    virtual void    on_node_inserted( weak_node_t& ) noexcept override { mockNodeInserted(); }
    virtual void    on_node_removed( weak_node_t& ) noexcept override { mockNodeRemoved(); }
    virtual void    on_group_inserted( weak_group_t& ) noexcept override { mockGroupInserted(); }
    virtual void    on_group_removed( weak_group_t& ) noexcept override { mockGroupRemoved(); }

public:
    MOCK_METHOD0(mockNodeInserted, void(void));
    MOCK_METHOD0(mockNodeRemoved, void(void));
    MOCK_METHOD0(mockGroupInserted, void(void));
    MOCK_METHOD0(mockGroupRemoved, void(void));
};

// FIXME groups:
    // A contract should enforce the behaviour when a grouped node or edge is actually
    // removed from topology, is a group edge_removed() or node_removed() signal emitted ?

TEST(GTpoBehaviour, groupBehaviourNodeInserted)
{
    gtpo::graph<> g;
    using MockGroupBehaviour = GroupBehaviourMock<>;
    auto group = g.create_group();
    auto n = g.create_node();

    ASSERT_TRUE(group.lock());
    auto groupMockBehaviour = new MockGroupBehaviour(); // Can't use unique_ptr here because of gmock
    group.lock()->add_dynamic_group_behaviour( std::unique_ptr<MockGroupBehaviour>(groupMockBehaviour) );

    // node_inserted() notification
    EXPECT_CALL(*groupMockBehaviour, mockNodeInserted()).Times(1);
    g.group_node(n, group);
    //EXPECT_CALL(*groupMockBehaviour, mockNodeInserted()).Times(0);

    // node_removed() notification
    EXPECT_CALL(*groupMockBehaviour, mockNodeRemoved()).Times(1);
    g.ungroup_node(n, group);
    //EXPECT_CALL(*groupMockBehaviour, mockNodeRemoved()).Times(0);

    // When a node is removed from graph, it is automatically ungrouped,
    // we expect the group behaviour to be notified that the node is removed...
    EXPECT_CALL(*groupMockBehaviour, mockNodeInserted()).Times(1);
    g.group_node(n, group);
    EXPECT_CALL(*groupMockBehaviour, mockNodeRemoved()).Times(1);
    g.remove_node(n);
}

TEST(GTpoBehaviour, groupBehaviourGroupInserted)
{
    gtpo::graph<> g;
    using MockGroupBehaviour = GroupBehaviourMock< gtpo::graph<>::configuration >;
    auto group = g.create_group();
    auto n = g.create_node();

    ASSERT_TRUE(group.lock());
    auto groupMockBehaviour = new MockGroupBehaviour(); // Can't use unique_ptr here because of gmock
    group.lock()->add_dynamic_group_behaviour( std::unique_ptr<MockGroupBehaviour>(groupMockBehaviour) );

    // node_inserted() notification
    using weak_group_t = gtpo::group<>::weak_group_t;
    auto group2 = g.create_group();
    EXPECT_CALL(*groupMockBehaviour, mockGroupInserted()).Times(1);
    g.group_node(weak_group_t{group2}, group);

    // group_removed() notification
    EXPECT_CALL(*groupMockBehaviour, mockGroupRemoved()).Times(1);
    g.ungroup_node(weak_group_t{group2}, group);
}


//-----------------------------------------------------------------------------
// GTpo Node behaviour tests
//-----------------------------------------------------------------------------

template < class config_t >
class NodeBehaviourMock : public gtpo::dynamic_node_behaviour< config_t >
{
public:
    NodeBehaviourMock() { }
    virtual ~NodeBehaviourMock() override { }

    using weak_node_t      = std::weak_ptr< typename config_t::final_node_t >;
    using weak_edge_t      = std::weak_ptr< typename config_t::final_edge_t >;
    using weak_group_t     = std::weak_ptr< typename config_t::final_group_t >;

protected:
    virtual void    on_in_node_inserted( weak_node_t&, weak_node_t&, const weak_edge_t& ) noexcept override { mockInNodeInserted(); }
    virtual void    on_in_node_removed( weak_node_t&, weak_node_t&, const weak_edge_t& ) noexcept override { mockInNodeRemoved(); }
    virtual void    on_in_node_removed( weak_node_t& ) noexcept override { mockInNodeRemoved(); }

    virtual void    on_out_node_inserted( weak_node_t&, weak_node_t&, const weak_edge_t& ) noexcept override { mockOutNodeInserted(); }
    virtual void    on_out_node_removed( weak_node_t&, weak_node_t&, const weak_edge_t& ) noexcept override { mockOutNodeRemoved(); }
    virtual void    on_out_node_removed( weak_node_t& ) noexcept override { mockOutNodeRemoved(); }

public:
    MOCK_METHOD0(mockInNodeInserted, void(void));
    MOCK_METHOD0(mockInNodeRemoved, void(void));
    MOCK_METHOD0(mockOutNodeInserted, void(void));
    MOCK_METHOD0(mockOutNodeRemoved, void(void));
};

TEST(GTpoBehaviour, nodeBehaviour)
{
    gtpo::graph<> g;
    using MockNodeBehaviour = NodeBehaviourMock< gtpo::graph<>::configuration >;
    auto nodeMockBehaviour = new MockNodeBehaviour{}; // Can't use unique_ptr here because of gmock
    auto n = g.create_node().lock();
    ASSERT_TRUE(n);
    n->add_dynamic_node_behaviour( std::unique_ptr<MockNodeBehaviour>{nodeMockBehaviour} );

    // in_node_inserted() notification
    EXPECT_CALL(*nodeMockBehaviour, mockInNodeInserted()).Times(1);
    auto n2 = g.create_node();
    auto e1 = g.create_edge(n2, n);
    EXPECT_CALL(*nodeMockBehaviour, mockInNodeInserted()).Times(0);

    // in_node_removed() notification
    EXPECT_CALL(*nodeMockBehaviour, mockInNodeRemoved()).Times(2); // Expecting two call for in_node_removed(arg1, arg2) just before removal an in_node_removed(void) after removal
    g.remove_edge(e1);
    EXPECT_CALL(*nodeMockBehaviour, mockInNodeRemoved()).Times(0);

    // out_node_inserted() notification
    EXPECT_CALL(*nodeMockBehaviour, mockOutNodeInserted()).Times(1);
    auto n3 = g.create_node();
    auto e2 = g.create_edge(n, n3);
    EXPECT_CALL(*nodeMockBehaviour, mockOutNodeInserted()).Times(0);

    // out_node_removed() notification
    EXPECT_CALL(*nodeMockBehaviour, mockOutNodeRemoved()).Times(2);  // Expecting two call for out_node_removed(arg1, arg2) just before removal an out_node_removed(void) after removal
    g.remove_edge(e2);
    EXPECT_CALL(*nodeMockBehaviour, mockOutNodeRemoved()).Times(0);
}

