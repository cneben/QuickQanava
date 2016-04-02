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
#include <gtpoProtoSerializer.h>
#include <gtpoProgressNotifier.h>

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char **argv) {
    //::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    gtpo::ProtoSerializer<>::initProtocolBuffer();
    return RUN_ALL_TESTS();
}

//-----------------------------------------------------------------------------
// GTpo utils tests
//-----------------------------------------------------------------------------

TEST(GTpoUtils, utilsGenericInsertRemove)
{
    using IntList = std::list< std::shared_ptr<int> >;
    IntList l;
    using IntVector = std::vector< std::shared_ptr<int> >;
    IntVector v;
    using IntSet = std::unordered_set< std::shared_ptr<int> >;
    IntSet s;

    // It is rather more a "compile test" than a runtime one, since
    // we have 100+ lines of specialized template code for just
    // adding/removing items in so-called generic containers; STL
    // sucks, but we probably won a few ms over Qt containers
    l.push_back( std::make_shared<int>( 42 ) );
    auto cs = l.size();
    gtpo::DefaultConfig::remove<IntList>::from( l, l.front() );
    EXPECT_TRUE( ( cs == 1 && l.size() == 0 ) );

    v.push_back( std::make_shared<int>( 42 ) );
    cs = v.size();
    gtpo::DefaultConfig::remove<IntVector>::from( v, v[0] );
    EXPECT_TRUE( ( cs == 1 && v.size() == 0 ) ) ;

    s.insert( std::make_shared<int>( 42 ) );
    cs = s.size();
    gtpo::DefaultConfig::remove<IntSet>::from( s, *s.begin() );
    EXPECT_TRUE( ( cs == 1 && s.size() == 0 ) ) ;
}

//-----------------------------------------------------------------------------
// GTpo tests
//-----------------------------------------------------------------------------

TEST(GTpo, createNodeNodesCount)
{
    gtpo::GenGraph<> g;
    auto nc = g.getNodeCount();
    auto rnc = g.getRootNodeCount();
    g.createNode();
    EXPECT_TRUE( ( g.getNodeCount() == nc + 1 ) );
    EXPECT_TRUE( ( g.getRootNodeCount() == rnc + 1 ) );
    g.clear();
}

TEST(GTpo, removeNodeNodesCount)
{
    gtpo::GenGraph<> g;
    gtpo::GenGraph<>::WeakNode n = g.createNode();
    auto nc = g.getNodeCount();
    auto rnc = g.getRootNodeCount();
    g.installRootNode( n ); // Just a test, it should not be called by the user directly
    EXPECT_TRUE( g.isRootNode( n ) );
    EXPECT_TRUE( g.containsNode( n ) );
    g.removeNode( n );
    EXPECT_FALSE( g.containsNode( n ) );
    EXPECT_TRUE( ( g.getNodeCount() == nc - 1 ) );
    EXPECT_TRUE( ( g.getRootNodeCount() == rnc - 1 ) ) ;
    g.createNode();
    g.clear();
}

TEST(GTpo, createEdge)
{
    // Testing that inserting an edge remove dst from the root node set
    gtpo::GenGraph<> g;
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    EXPECT_TRUE( ( g.getRootNodeCount() == 2 ) );
    auto e = g.createEdge(n1, n2);
    EXPECT_TRUE( ( g.getRootNodeCount() == 1 ) );   // n2 is no longer a root node
    g.clear();

    // Testing that when creating a circuit with a root node, node is still
    // a root one after an edge has been created to itself.
    gtpo::GenGraph<> g2;
    auto g2n1 = g2.createNode();
    g2.createEdge( g2n1, g2n1 );
    EXPECT_TRUE( g2.getRootNodeCount() == 1 );
    g2.clear();
}

TEST(GTpo, topology1)
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

TEST(GTpo, topology2)
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

TEST(GTpo, parallelTopology)
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
// GTpo serialization tests
//-----------------------------------------------------------------------------

TEST(GTpo, protobufInOut)
{
    gtpo::GenGraph<> go;
    auto n1 = go.createNode();
    auto n2 = go.createNode();
    auto n3 = go.createNode();
    auto n4 = go.createNode();
    go.createEdge(n1, n2);
    go.createEdge(n2, n3);
    go.createEdge(n2, n4);
    go.createEdge(n4, n1);

    auto group1 = go.createGroup();
    group1.lock()->insertNode( n1 );

    gtpo::GenGraph<> gi;

    gtpo::ProtoSerializer<> ps;
    ps.serializeOutTo( go, std::string( "test.gtpo" ) );
    ps.serializeInFrom( std::string( "test.gtpo" ), gi );

    EXPECT_EQ( go.getNodeCount(), gi.getNodeCount() );
    EXPECT_EQ( go.getEdges().size(), gi.getEdges().size() );
    EXPECT_EQ( go.getGroups().size(), gi.getGroups().size() );
    EXPECT_TRUE( true );
}

//-----------------------------------------------------------------------------
// GTpo Progress Notifier Tests
//-----------------------------------------------------------------------------

TEST(GTpo, progressNotifier)
{
    {   // Initial values and progress reporting with a unique phase
        gtpo::ProgressNotifier progress;

        // Test default initial progress values
        EXPECT_DOUBLE_EQ( progress.getProgress(), 0. );
        EXPECT_DOUBLE_EQ( progress.getPhaseProgress(), 0. );

        // Test progress reporting with a single phase
        // Expectation: with no phase specified, setProgress() and
        // setPhaseProgress() should impact global progress directly.
        progress.setProgress( 0.5 );
        EXPECT_DOUBLE_EQ( progress.getProgress(), 0.5 );
        progress.setProgress( 0.7 );
        EXPECT_NEAR( progress.getProgress(), 0.7, 0.09 );
        EXPECT_NEAR( progress.getPhaseProgress(), 0.7, 0.09);
    }

    {   // Test progress reporting with multiple phases
        gtpo::ProgressNotifier progress;
        progress.reserveSubProgress( 2 );

        gtpo::IProgressNotifier& subProgress1 = progress.takeSubProgress( "Phase1" );
        subProgress1.setProgress( 0.5 );
        EXPECT_DOUBLE_EQ( subProgress1.getProgress(), 0.5 );

        gtpo::IProgressNotifier& subProgress2 = progress.takeSubProgress( "Phase2" );
        subProgress2.setProgress( 0.5 );
        EXPECT_DOUBLE_EQ( subProgress2.getProgress(), 0.5 );

        EXPECT_EQ( progress.getProgress(), 0.5 );  // progress = 0.5*0.5 + 0.5*0.5
    }

/*    {   // Test reporting with multiple levels
        gtpo::ProgressNotifier progress;
        gtpo::IProgressNotifier* progress2 = progress.createSubProgress();

        // Level 1
        progress.beginProgress();
        progress.setPhaseProgress( 0.5 );
        EXPECT_DOUBLE_EQ( progress.getProgress(), 0.25 );
        { // Level 2
            progress2->beginProgress();
            progress2->setPhaseProgress( 0.5 );
            EXPECT_DOUBLE_EQ( progress2->getProgress(), 0.5 );
            progress2->endProgress();
        }
        EXPECT_DOUBLE_EQ( progress.getProgress(), 0.5 );
        progress.endProgress();
    }

    {   // Test reporting with multiple levels with multiple phases
        gtpo::ProgressNotifier progress;
        gtpo::IProgressNotifier* progress2 = progress.createSubProgress();
        EXPECT_EQ( progress.getPhaseCount(), 2 );   // 2 phases: 1 for progress, 1 for progress 2
        progress.setPhaseCount( 2 );
        EXPECT_EQ( progress.getPhaseCount(), 3 );   // 3 phases: 2 for progress, 1 for progress 2

        // Level 1
        progress.beginProgress();
        progress.beginPhase();
        progress.setPhaseProgress( 0.5 );
        EXPECT_DOUBLE_EQ( progress.getPhaseProgress(), 0.5 );
        progress.beginPhase();
        progress.setPhaseProgress( 0.5 );
        EXPECT_DOUBLE_EQ( progress.getProgress(), 0.5 );
                // progress phase 1 = 1., phase 2 = 0.5, with 3 phases: progress = 0.333*1 + 0.333*0.5 = 0.5

        { // Level 2
            progress2->beginProgress();
            progress2->setPhaseProgress( 0.5 );
            EXPECT_DOUBLE_EQ( progress2->getProgress(), 0.5 );
            progress2->endProgress();
        }
        EXPECT_NEAR( progress.getProgress(), 0.66665, 0.0001 );  // 0.333 + 0.333 / 2 + 0.333 = 0.6666
        progress.endProgress();
    }

    {   // Test reporting with multiple levels with multiple phases at multiple levels
        gtpo::ProgressNotifier progress;
        gtpo::IProgressNotifier* progress2 = progress.createSubProgress();
        progress.setPhaseCount( 2 );
        progress2->setPhaseCount( 2 );

        // Level 1
        progress.beginProgress();
        progress.beginPhase();
        progress.setPhaseProgress( 0.5 );
        EXPECT_DOUBLE_EQ( progress.getPhaseProgress(), 0.5 );
        progress.beginPhase();
        progress.setPhaseProgress( 0.5 );
        EXPECT_DOUBLE_EQ( progress.getProgress(), 0.375 ); // 1*0.25 + 0.5*0.25 = 0.375

        { // Level 2
            progress2->beginProgress();
            progress2->beginPhase();
            progress2->setPhaseProgress( 0.5 );
            EXPECT_DOUBLE_EQ( progress2->getPhaseProgress(), 0.5 );
            progress2->beginPhase();
            progress2->setPhaseProgress( 0.5 );
            EXPECT_DOUBLE_EQ( progress2->getProgress(), 0.75 );
            progress2->endProgress();
        }
        EXPECT_DOUBLE_EQ( progress.getProgress(), 0.75 );
        progress.endProgress();
    }*/
}

//-----------------------------------------------------------------------------
// GTpo Edge Set Behaviour test (using STpo)
//-----------------------------------------------------------------------------

TEST(GTpo, stpoGroupAdjacentEdgesBehaviour)
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

TEST(GTpo, stpoGroupAdjacentEdgesBehaviour2)
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

    using WeakNode      = std::weak_ptr< typename Config::Node >;
    using WeakEdge      = std::weak_ptr< typename Config::Edge >;
    using WeakGroup     = std::weak_ptr< typename Config::Group >;

protected:
    virtual void    nodeInserted( WeakNode& ) override { mockNodeInserted(); }
    virtual void    nodeRemoved( WeakNode& ) override { mockNodeRemoved(); }
    virtual void    nodeModified( WeakNode& ) override { mockNodeModified(); }
    virtual void    edgeInserted( WeakEdge& ) override { mockEdgeInserted(); }
    virtual void    edgeRemoved( WeakEdge& ) override { mockEdgeRemoved(); }
    virtual void    edgeModified( WeakEdge& ) override { mockEdgeModified(); }
    virtual void    groupInserted( WeakGroup& ) override { mockGroupInserted(); }
    virtual void    groupRemoved( WeakGroup& ) override { mockGroupRemoved(); }
    virtual void    groupModified( WeakGroup& ) override { mockGroupModified(); }

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

    using WeakNode      = std::weak_ptr< typename Config::Node >;
    using WeakEdge      = std::weak_ptr< typename Config::Edge >;
    using WeakGroup     = std::weak_ptr< typename Config::Group >;

protected:
    virtual void    nodeInserted( WeakNode& ) override { mockNodeInserted(); }
    virtual void    nodeRemoved( WeakNode& ) override { mockNodeRemoved(); }
    virtual void    nodeModified( WeakNode& ) override { mockNodeModified(); }
    virtual void    edgeInserted( WeakEdge& ) override { mockEdgeInserted(); }
    virtual void    edgeRemoved( WeakEdge& ) override { mockEdgeRemoved(); }
    virtual void    edgeModified( WeakEdge& ) override { mockEdgeModified(); }
    virtual void    groupModified( WeakGroup& ) override { mockGroupModified(); }

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

TEST(GTpo, stpoEchoBehaviour)
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
        g.addBehaviour( mockBehaviour );

        // nodeInserted() notification
        EXPECT_CALL(*mockBehaviour, mockNodeInserted()).Times(AtLeast(1));
        auto n = g.createNode();

        // nodeModified() notification
        EXPECT_CALL(*mockBehaviour, mockNodeModified()).Times(AtLeast(1));
        stpo::Graph::Configuration::setNodeLabel( n.lock().get(), "test" );

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
        stpo::Graph::Configuration::setEdgeWeight( e.lock().get(), 0.5 );

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
            group2.lock()->addBehaviour( groupMockBehaviour );

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

//-----------------------------------------------------------------------------
// GTpo stpo tests
//-----------------------------------------------------------------------------

using namespace gtpo;

TEST(GTpo, stpoCreateNodeNodesCount)
{
    stpo::Graph g;
    auto nc = g.getNodeCount();
    auto rnc = g.getRootNodeCount();
    g.createNode();
    EXPECT_TRUE( g.getNodeCount() == nc + 1 );
    EXPECT_TRUE( g.getRootNodeCount() == rnc + 1 );
    g.clear();
}

TEST(GTpo, stpoRemoveNodeNodesCount)
{
    stpo::Graph g;
    stpo::Graph::WeakNode n = g.createNode();
    auto nc = g.getNodeCount();
    auto rnc = g.getRootNodeCount();
    g.removeNode( n );
    EXPECT_TRUE( g.getNodeCount() == nc - 1 );
    EXPECT_TRUE( g.getRootNodeCount() == rnc - 1 );
    g.clear();
}


