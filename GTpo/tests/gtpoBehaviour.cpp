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
// \file	qtpoTests.cpp
// \author	benoit@qanava.org
// \date	2016 01 26
//-----------------------------------------------------------------------------

// STD headers
#include <list>
#include <memory>
#include <iostream>

// GTpo headers
#include <GTpo>
#include <GTpoStd>
#include <gtpoProgressNotifier.h>

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//-----------------------------------------------------------------------------
// GTpo Edge Set Behaviour test (using STpo)
//-----------------------------------------------------------------------------

TEST(GTpoBehaviour, stpoGroupAdjacentEdgesBehaviour)
{
    stpo::Graph g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();

    auto g1 = g.createGroup();
    auto sg1 = g1.lock();
    sg1->insertNode( n1 );
    sg1->insertNode( n2 );

    // Expecting 0 adjacent edges since no edge has been added
    auto& g1AdjacentEdgeSet = sg1->getAdjacentEdges();
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 0 );

    {   // Adding an edge at graph level, expecting edge to be inserted in group adjacent edge set
        g.createEdge( n1, n2 );
        EXPECT_EQ( g1AdjacentEdgeSet.size(), 1 );

        // Removing the edge at graph level, expecting group empty adjacent edge
        g.removeEdge( n1, n2 );
        EXPECT_EQ( g1AdjacentEdgeSet.size(), 0 );
    }

    {
        auto e2 = g.createEdge( n1, n2 );
        EXPECT_EQ( g1AdjacentEdgeSet.size(), 1 );

        // Removing node 1 from group, edge is still adjacent
        sg1->removeNode( n1 );
        EXPECT_EQ( g1AdjacentEdgeSet.size(), 1 );

        // Removing node 2 from group, edge is no longer adjacent to group
        sg1->removeNode( n2 );
        EXPECT_EQ( g1AdjacentEdgeSet.size(), 0 );
    }
}

TEST(GTpoBehaviour, stpoGroupAdjacentEdgesBehaviour2)
{
    stpo::Graph g;
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

    auto g1 = g.createGroup();
    auto sg1 = g1.lock();
    sg1->insertNode( n3 );
    sg1->insertNode( n4 );
    sg1->insertNode( n5 );

    // See manual topology section, it is the same topology
    // Check that group 1 adjacent edge set is {e2, e3, e4, e5}
    auto& g1AdjacentEdgeSet = sg1->getAdjacentEdges();
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e2 ) );
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e3 ) );
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e4 ) );
    EXPECT_TRUE( gtpo::find_weak_ptr( g1AdjacentEdgeSet, e5 ) );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 4 );

    // Removing a node from the group should remove its in/out edges from group adjacent edge set
    // Here, removing n4 should remove e3 and e5
    sg1->removeNode( n4 );
    EXPECT_EQ( g1AdjacentEdgeSet.size(), 3 );

    g.clear();
}


//-----------------------------------------------------------------------------
// GTpo Behaviour test (using STpo)
//-----------------------------------------------------------------------------

template < class Config >
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
    virtual void    nodeModified( WeakNode& ) noexcept override { mockNodeModified(); }
    virtual void    edgeInserted( WeakEdge& ) noexcept override { mockEdgeInserted(); }
    virtual void    edgeRemoved( WeakEdge& ) noexcept override { mockEdgeRemoved(); }
    virtual void    edgeModified( WeakEdge& ) noexcept override { mockEdgeModified(); }
    virtual void    groupInserted( WeakGroup& ) noexcept override { mockGroupInserted(); }
    virtual void    groupRemoved( WeakGroup& ) noexcept override { mockGroupRemoved(); }
    virtual void    groupModified( WeakGroup& ) noexcept override { mockGroupModified(); }

public:
    MOCK_METHOD0(mockNodeInserted, void(void));
    MOCK_METHOD0(mockNodeRemoved, void(void));
    MOCK_METHOD0(mockNodeModified, void(void));
    MOCK_METHOD0(mockEdgeInserted, void(void));
    MOCK_METHOD0(mockEdgeRemoved, void(void));
    MOCK_METHOD0(mockEdgeModified, void(void));
    MOCK_METHOD0(mockGroupInserted, void(void));
    MOCK_METHOD0(mockGroupRemoved, void(void));
    MOCK_METHOD0(mockGroupModified, void(void));
};

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
    virtual void    nodeModified( WeakNode& ) noexcept override { mockNodeModified(); }
    virtual void    edgeInserted( WeakEdge& ) noexcept override { mockEdgeInserted(); }
    virtual void    edgeRemoved( WeakEdge& ) noexcept override { mockEdgeRemoved(); }
    virtual void    edgeModified( WeakEdge& ) noexcept override { mockEdgeModified(); }
    virtual void    groupModified( WeakGroup& ) noexcept override { mockGroupModified(); }

public:
    MOCK_METHOD0(mockNodeInserted, void(void));
    MOCK_METHOD0(mockNodeRemoved, void(void));
    MOCK_METHOD0(mockNodeModified, void(void));
    MOCK_METHOD0(mockEdgeInserted, void(void));
    MOCK_METHOD0(mockEdgeRemoved, void(void));
    MOCK_METHOD0(mockEdgeModified, void(void));
    MOCK_METHOD0(mockGroupModified, void(void));
};

using testing::AtLeast;

TEST(GTpoBehaviour, stpoEchoBehaviour)
{
    stpo::Graph g;

    { // Testing graph behaviour
        using MockGraphBehaviour = GraphBehaviourMock< stpo::Graph::Configuration >;
        auto mockBehaviour = new MockGraphBehaviour();  // Wan't use unique_ptr here because of gmock

        // Test basic enable/disable behaviour property
        EXPECT_TRUE( mockBehaviour->isEnabled() );
        mockBehaviour->disable();
        EXPECT_TRUE( !mockBehaviour->isEnabled() );
        mockBehaviour->enable();

        // Behaviour notify virtual methods should be called when graph topology changes
        g.addBehaviour( std::unique_ptr<MockGraphBehaviour>(mockBehaviour) );

        // nodeInserted() notification
        EXPECT_CALL(*mockBehaviour, mockNodeInserted()).Times(AtLeast(1));
        auto n = g.createNode();

        // nodeModified() notification
        EXPECT_CALL(*mockBehaviour, mockNodeModified()).Times(AtLeast(1));
        // FIXME GTPO3
        //stpo::Graph::Configuration::setLabel( n.lock().get(), "test" );

        // nodeInserted() notification
        EXPECT_CALL(*mockBehaviour, mockNodeRemoved()).Times(AtLeast(1));
        g.removeNode( n );

        // edgeInserted() notification
        auto s = g.createNode();
        auto d = g.createNode();
        EXPECT_CALL(*mockBehaviour, mockEdgeInserted()).Times(AtLeast(1));
        auto e = g.createEdge(s, d);

        // edgeModified() notification
        EXPECT_CALL(*mockBehaviour, mockEdgeModified()).Times(AtLeast(1));
        // FIXME GTPO3
        //stpo::Graph::Configuration::setWeight( e.lock().get(), 0.5 );

        // edgeRemoved() notification
        EXPECT_CALL(*mockBehaviour, mockEdgeRemoved()).Times(AtLeast(1));
        g.removeEdge(e);

        // groupInserted() notification
        EXPECT_CALL(*mockBehaviour, mockGroupInserted()).Times(AtLeast(1));
        auto gg = g.createGroup();

        // groupModified() notification
        EXPECT_CALL(*mockBehaviour, mockGroupModified()).Times(AtLeast(2));
        if ( !s.expired() &&
             !gg.expired() ) {
            gg.lock()->insertNode( s );                 // First mockGroupModified() call
            EXPECT_EQ( gg.lock()->getNodeCount(), 1 );
            gg.lock()->removeNode( s );                 // Second mockGroupModified() call
            EXPECT_EQ( gg.lock()->getNodeCount(), 0 );
        }

        // groupRemoved() notification
        EXPECT_CALL(*mockBehaviour, mockGroupRemoved()).Times(AtLeast(1));
        g.removeGroup( gg );

        { // Testing group behaviour
            using MockGroupBehaviour = GroupBehaviourMock< stpo::Graph::Configuration >;
            auto group2 = g.createGroup();
            auto groupMockBehaviour = new MockGroupBehaviour(); // Can't use unique_ptr here because of gmock
            group2.lock()->addBehaviour( std::unique_ptr<MockGroupBehaviour>(groupMockBehaviour) );

            // nodeInserted() notification
            EXPECT_CALL(*groupMockBehaviour, mockNodeInserted()).Times(AtLeast(1));
            EXPECT_CALL(*groupMockBehaviour, mockGroupModified()).Times(AtLeast(1));
            group2.lock()->insertNode( s );

            // nodeModified() notification
            //stpo::Graph::Configuration::setNodeLabel( n.lock(), "test" );

            // nodeInserted() notification
            EXPECT_CALL(*groupMockBehaviour, mockNodeRemoved()).Times(AtLeast(1));
            group2.lock()->removeNode( s );
        }
    }
    g.clear();  // Mock behaviour will be automatically destroyed
}

