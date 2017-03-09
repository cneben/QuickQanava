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
// \file	gtpoBehaviour.cpp
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

using testing::AtLeast;

//-----------------------------------------------------------------------------
// GTpo Graph Behaviour tests
//-----------------------------------------------------------------------------

template < class Config = gtpo::GraphConfig >
class GraphBehaviourMock : public gtpo::GraphBehaviour< Config >
{
public:
    GraphBehaviourMock() { }
    virtual ~GraphBehaviourMock() { }

    using WeakNode      = std::weak_ptr< typename Config::FinalNode >;
    using WeakEdge      = std::weak_ptr< typename Config::FinalEdge >;
    using WeakGroup     = std::weak_ptr< typename Config::FinalGroup >;

protected:
    virtual void    nodeInserted( WeakNode& ) noexcept override { mockNodeInserted(); }
    virtual void    nodeRemoved( WeakNode& ) noexcept override { mockNodeRemoved(); }
    virtual void    edgeInserted( WeakEdge& ) noexcept override { mockEdgeInserted(); }
    virtual void    edgeRemoved( WeakEdge& ) noexcept override { mockEdgeRemoved(); }
    virtual void    groupInserted( WeakGroup& ) noexcept override { mockGroupInserted(); }
    virtual void    groupRemoved( WeakGroup& ) noexcept override { mockGroupRemoved(); }

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
    gtpo::GenGraph<> g;

    using MockGraphBehaviour = GraphBehaviourMock<>;
    auto mockBehaviour = new MockGraphBehaviour();

    // Test basic enable/disable behaviour property
    EXPECT_TRUE( mockBehaviour->isEnabled() );
    mockBehaviour->disable();
    EXPECT_TRUE( !mockBehaviour->isEnabled() );
}

TEST(GTpoBehaviour, graphBehaviour)
{
    gtpo::GenGraph<> g;

    using MockGraphBehaviour = GraphBehaviourMock<>;
    auto mockBehaviour = new MockGraphBehaviour();  // Don't use unique_ptr here because of gmock

    // Behaviour notify virtual methods should be called when graph topology changes
    g.addGraphBehaviour( std::unique_ptr<MockGraphBehaviour>(mockBehaviour) );

    // nodeInserted() notification
    EXPECT_CALL(*mockBehaviour, mockNodeInserted()).Times(1);
    auto n = g.createNode();
    EXPECT_CALL(*mockBehaviour, mockNodeInserted()).Times(AtLeast(1));  // Reset counter...

    // nodeInserted() notification
    EXPECT_CALL(*mockBehaviour, mockNodeRemoved()).Times(1);
    g.removeNode( n );

    // edgeInserted() notification
    auto s = g.createNode();
    auto d = g.createNode();
    EXPECT_CALL(*mockBehaviour, mockEdgeInserted()).Times(1);
    auto e = g.createEdge(s, d);

    // edgeRemoved() notification
    EXPECT_CALL(*mockBehaviour, mockEdgeRemoved()).Times(1);
    g.removeEdge(e);

    // groupInserted() notification
    EXPECT_CALL(*mockBehaviour, mockGroupInserted()).Times(1);
    auto gg = g.createGroup();

    // groupRemoved() notification
    EXPECT_CALL(*mockBehaviour, mockGroupRemoved()).Times(1);
    g.removeGroup( gg );
}

//-----------------------------------------------------------------------------
// GTpo Group Behaviour tests
//-----------------------------------------------------------------------------

template < class Config >
class GroupBehaviourMock : public gtpo::GroupBehaviour< Config >
{
public:
    GroupBehaviourMock() { }
    virtual ~GroupBehaviourMock() { }

    using WeakNode      = std::weak_ptr< typename Config::FinalNode >;
    using WeakEdge      = std::weak_ptr< typename Config::FinalEdge >;
    using WeakGroup     = std::weak_ptr< typename Config::FinalGroup >;

protected:
    virtual void    nodeInserted( WeakNode& ) noexcept override { mockNodeInserted(); }
    virtual void    nodeRemoved( WeakNode& ) noexcept override { mockNodeRemoved(); }
    virtual void    groupInserted( WeakGroup& ) noexcept override { mockGroupInserted(); }
    virtual void    groupRemoved( WeakGroup& ) noexcept override { mockGroupRemoved(); }

public:
    MOCK_METHOD0(mockNodeInserted, void(void));
    MOCK_METHOD0(mockNodeRemoved, void(void));
    MOCK_METHOD0(mockGroupInserted, void(void));
    MOCK_METHOD0(mockGroupRemoved, void(void));
};

TEST(GTpoBehaviour, groupBehaviour)
{
    gtpo::GenGraph<> g;
    using MockGroupBehaviour = GroupBehaviourMock< gtpo::GenGraph<>::Configuration >;
    auto group = g.createGroup();
    auto n = g.createNode();

    ASSERT_TRUE(group.lock());
    auto groupMockBehaviour = new MockGroupBehaviour(); // Can't use unique_ptr here because of gmock
    group.lock()->addGroupBehaviour( std::unique_ptr<MockGroupBehaviour>(groupMockBehaviour) );

    // nodeInserted() notification
    EXPECT_CALL(*groupMockBehaviour, mockNodeInserted()).Times(1);
    g.groupNode(group, n);
    EXPECT_CALL(*groupMockBehaviour, mockNodeInserted()).Times(AtLeast(1));

    // nodeRemoved() notification
    EXPECT_CALL(*groupMockBehaviour, mockNodeRemoved()).Times(1);
    g.ungroupNode(group, n);
    EXPECT_CALL(*groupMockBehaviour, mockNodeRemoved()).Times(AtLeast(1));

    // FIXME
    //EXPECT_CALL(*groupMockBehaviour, mockNodeRemoved()).Times(1);
    //g.removeNode(n);

    // nodeInserted() notification
    using WeakGroup = gtpo::GenGroup<>::WeakGroup;
    auto group2 = g.createGroup();
    EXPECT_CALL(*groupMockBehaviour, mockGroupInserted()).Times(1);
    g.groupNode(group, WeakGroup{group2});

    // groupRemoved() notification
    EXPECT_CALL(*groupMockBehaviour, mockGroupRemoved()).Times(1);
    g.ungroupNode(group, WeakGroup{group2});
}


//-----------------------------------------------------------------------------
// GTpo Node Behaviour tests
//-----------------------------------------------------------------------------

template < class Config >
class NodeBehaviourMock : public gtpo::NodeBehaviour< Config >
{
public:
    NodeBehaviourMock() { }
    virtual ~NodeBehaviourMock() { }

    using WeakNode      = std::weak_ptr< typename Config::FinalNode >;
    using WeakEdge      = std::weak_ptr< typename Config::FinalEdge >;
    using WeakGroup     = std::weak_ptr< typename Config::FinalGroup >;

protected:
    virtual void    inNodeInserted( WeakNode&, const WeakEdge& ) noexcept override { mockInNodeInserted(); }
    virtual void    inNodeRemoved( WeakNode&, const WeakEdge& ) noexcept override { mockInNodeRemoved(); }
    virtual void    inNodeRemoved() noexcept override { mockInNodeRemoved(); }

    virtual void    outNodeInserted( WeakNode&, const WeakEdge& ) noexcept override { mockOutNodeInserted(); }
    virtual void    outNodeRemoved( WeakNode&, const WeakEdge& ) noexcept override { mockOutNodeRemoved(); }
    virtual void    outNodeRemoved() noexcept override { mockOutNodeRemoved(); }

public:
    MOCK_METHOD0(mockInNodeInserted, void(void));
    MOCK_METHOD0(mockInNodeRemoved, void(void));
    MOCK_METHOD0(mockOutNodeInserted, void(void));
    MOCK_METHOD0(mockOutNodeRemoved, void(void));
};

TEST(GTpoBehaviour, nodeBehaviour)
{
    gtpo::GenGraph<> g;
    using MockNodeBehaviour = NodeBehaviourMock< gtpo::GenGraph<>::Configuration >;
    auto nodeMockBehaviour = new MockNodeBehaviour{}; // Can't use unique_ptr here because of gmock
    auto n = g.createNode().lock();
    ASSERT_TRUE(n);
    n->addNodeBehaviour( std::unique_ptr<MockNodeBehaviour>{nodeMockBehaviour} );

    // inNodeInserted() notification
    EXPECT_CALL(*nodeMockBehaviour, mockInNodeInserted()).Times(1);
    auto n2 = g.createNode();
    auto e1 = g.createEdge(n2, n);
    EXPECT_CALL(*nodeMockBehaviour, mockInNodeInserted()).Times(0);

    // inNodeRemoved() notification
    EXPECT_CALL(*nodeMockBehaviour, mockInNodeRemoved()).Times(2); // Expecting two call for inNodeRemoved(arg1, arg2) just before removal an inNodeRemoved(void) after removal
    g.removeEdge(e1);
    EXPECT_CALL(*nodeMockBehaviour, mockInNodeRemoved()).Times(0);

    // outNodeInserted() notification
    EXPECT_CALL(*nodeMockBehaviour, mockOutNodeInserted()).Times(1);
    auto n3 = g.createNode();
    auto e2 = g.createEdge(n, n3);
    EXPECT_CALL(*nodeMockBehaviour, mockOutNodeInserted()).Times(0);

    // outNodeRemoved() notification
    EXPECT_CALL(*nodeMockBehaviour, mockOutNodeRemoved()).Times(2);  // Expecting two call for outNodeRemoved(arg1, arg2) just before removal an outNodeRemoved(void) after removal
    g.removeEdge(e2);
    EXPECT_CALL(*nodeMockBehaviour, mockOutNodeRemoved()).Times(0);
}
