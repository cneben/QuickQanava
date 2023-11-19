/*
 Copyright (c) 2008-2023, Benoit AUTHEMAN All rights reserved.

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
// \file	observers_tests.cpp
// \author	benoit@qanava.org
// \date	2016 01 26
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

using testing::AtLeast;

//-----------------------------------------------------------------------------
// GTpo Graph observer tests
//-----------------------------------------------------------------------------

class GraphObserverMock : public gtpo::graph_observer<QQuickItem, qan::Node, qan::Edge, qan::Group>
{
public:
    GraphObserverMock() { }
    virtual ~GraphObserverMock() { std::cerr << "Deleting mock graph observer..." << std::endl; }

protected:
    virtual void    on_node_inserted(qan::Node&) noexcept override { std::cerr << "GraphObserverMock::on_node_inserted()" << std::endl; mockNodeInserted(); }
    virtual void    on_node_removed(qan::Node&) noexcept override { std::cerr << "GraphObserverMock::on_node_removed()" << std::endl; mockNodeRemoved(); }
    virtual void    on_edge_inserted(qan::Edge&) noexcept override { std::cerr << "GraphObserverMock::on_edge_inserted()" << std::endl; mockEdgeInserted(); }
    virtual void    on_edge_removed(qan::Edge&) noexcept override { std::cerr << "GraphObserverMock::on_edge_removed()" << std::endl; mockEdgeRemoved(); }
    virtual void    on_group_inserted(qan::Node&) noexcept override { std::cerr << "GraphObserverMock::on_group_inserted()" << std::endl; mockGroupInserted(); }
    virtual void    on_group_removed(qan::Node&) noexcept override { std::cerr << "GraphObserverMock::on_group_removed()" << std::endl; mockGroupRemoved(); }

public:
    MOCK_METHOD0(mockNodeInserted, void(void));
    MOCK_METHOD0(mockNodeRemoved, void(void));
    MOCK_METHOD0(mockEdgeInserted, void(void));
    MOCK_METHOD0(mockEdgeRemoved, void(void));
    MOCK_METHOD0(mockGroupInserted, void(void));
    MOCK_METHOD0(mockGroupRemoved, void(void));
};


TEST(gtpo_graph_observer, enable)
{
    qan::Graph g;

    auto mockObserver = new GraphObserverMock();

    // Test basic enable/disable observer property
    EXPECT_TRUE(mockObserver->isEnabled());
    mockObserver->disable();
    EXPECT_TRUE(!mockObserver->isEnabled());

    // FIXME: checker move et copy et == ...
    delete mockObserver;
}


TEST(gtpo_graph_observer, basic)
{
    qan::Graph g;

    // Observer notify virtual methods should be called when graph topology changes
    g.add_observer(std::make_unique<GraphObserverMock>());
    auto mockObserver = reinterpret_cast<GraphObserverMock*>(g.getObservers().at(0).get());

    // node_inserted() notification
    EXPECT_CALL(*mockObserver, mockNodeInserted()).Times(1);
    auto n = g.create_node();
    EXPECT_CALL(*mockObserver, mockNodeInserted()).Times(AtLeast(1));  // Reset counter...

    // node_inserted() notification
    EXPECT_CALL(*mockObserver, mockNodeRemoved()).Times(AtLeast(1));
    g.remove_node( n );

    // edge_inserted() notification
    auto s = g.create_node();
    auto d = g.create_node();
    EXPECT_CALL(*mockObserver, mockEdgeInserted()).Times(1);
    auto e = g.insert_edge(s, d);

    // edge_removed() notification
    EXPECT_CALL(*mockObserver, mockEdgeRemoved()).Times(1);
    g.remove_edge(e);

    // group_inserted() notification
    EXPECT_CALL(*mockObserver, mockGroupInserted()).Times(1);
    auto gg = new qan::Group{};
    g.insert_group(gg);

    // group_removed() notification
    EXPECT_CALL(*mockObserver, mockGroupRemoved()).Times(1);
    g.remove_group(gg);

    testing::Mock::AllowLeak(mockObserver);  // Let gtpo delete observers
    // FIXME: 20220424: Well it is actually not deleted !
}


//-----------------------------------------------------------------------------
// GTpo Node observer tests
//-----------------------------------------------------------------------------

class NodeObserverMock : public gtpo::node_observer<qan::Node, qan::Edge>
{
public:
    NodeObserverMock() { }
    virtual ~NodeObserverMock() { }

protected:
    virtual void    on_in_node_inserted(qan::Node&, qan::Node&, const qan::Edge&) noexcept override { mockInNodeInserted(); }
    virtual void    on_in_node_removed(qan::Node&, qan::Node&, const qan::Edge&) noexcept override { mockInNodeRemoved(); }
    virtual void    on_in_node_removed(qan::Node& ) noexcept override { mockInNodeRemoved(); }

    virtual void    on_out_node_inserted(qan::Node&, qan::Node&, const qan::Edge& ) noexcept override { mockOutNodeInserted(); }
    virtual void    on_out_node_removed(qan::Node&, qan::Node&, const qan::Edge&) noexcept override { mockOutNodeRemoved(); }
    virtual void    on_out_node_removed(qan::Node&) noexcept override { mockOutNodeRemoved(); }

public:
    MOCK_METHOD0(mockInNodeInserted, void(void));
    MOCK_METHOD0(mockInNodeRemoved, void(void));
    MOCK_METHOD0(mockOutNodeInserted, void(void));
    MOCK_METHOD0(mockOutNodeRemoved, void(void));
};

TEST(gtpo_node_observer, basic)
{
    qan::Graph g;
    auto n = g.create_node();
    ASSERT_NE(n, nullptr);
    ASSERT_TRUE(g.insert_node(n));
    n->add_observer(std::make_unique<NodeObserverMock>());
    auto mockObserver = reinterpret_cast<NodeObserverMock*>(n->getObservers().at(0).get());

    // in_node_inserted() notification
    EXPECT_CALL(*mockObserver, mockInNodeInserted()).Times(1);
    auto n2 = g.create_node();
    g.insert_node(n2);
    auto e1 = g.insert_edge(n2, n);
    EXPECT_CALL(*mockObserver, mockInNodeInserted()).Times(0);

    // in_node_removed() notification
    EXPECT_CALL(*mockObserver, mockInNodeRemoved()).Times(2); // Expecting two call for in_node_removed(arg1, arg2) just before removal an in_node_removed(void) after removal
    g.remove_edge(e1);
    EXPECT_CALL(*mockObserver, mockInNodeRemoved()).Times(0);

    // out_node_inserted() notification
    EXPECT_CALL(*mockObserver, mockOutNodeInserted()).Times(1);
    auto n3 = g.create_node();
    auto e2 = g.insert_edge(n, n3);
    EXPECT_CALL(*mockObserver, mockOutNodeInserted()).Times(0);

    // out_node_removed() notification
    EXPECT_CALL(*mockObserver, mockOutNodeRemoved()).Times(2);  // Expecting two call for out_node_removed(arg1, arg2) just before removal an out_node_removed(void) after removal
    g.remove_edge(e2);
    EXPECT_CALL(*mockObserver, mockOutNodeRemoved()).Times(0);
}
