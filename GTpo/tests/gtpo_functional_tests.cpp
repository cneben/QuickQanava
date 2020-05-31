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
// \file	gtpo_functional_tests.cpp
// \author	benoit@qanava.org
// \date	2018 08 15
//-----------------------------------------------------------------------------

// STD headers
#include <list>
#include <memory>
#include <iostream>

// GTpo headers
#include <GTpo>
#include <../src/functional.h>

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>


//-----------------------------------------------------------------------------
// gtpo::map
//-----------------------------------------------------------------------------

TEST(gtpo_functional, map_precond)
{
    gtpo::graph<> src;
    //const auto f = [](const auto& src_node, auto& dst_node) -> void { static_cast<void>(src_node); static_cast<void>(dst_node); };
    const auto f2 = [](const auto& src_node) -> gtpo::graph<>::shared_node_t {
        static_cast<void>(src_node);
        return std::make_shared<typename gtpo::graph<>::node_t>();
    };
    gtpo::graph<> dst;
    dst.create_node();
    auto r = gtpo::map<gtpo::graph<>, gtpo::graph<>, decltype(f2)>(src, dst, f2);
    EXPECT_FALSE(r);    // dst is non empty, gtpo::filter should check that precondition and return false.
}

TEST(gtpo_functional, map)
{
    gtpo::graph<> src, dst;
    // Just create an empty copy of source node to a destination node
    const auto f2 = [](const auto& src_node) -> gtpo::graph<>::shared_node_t {
        static_cast<void>(src_node);
        return std::make_shared<typename gtpo::graph<>::node_t>();
    };
    src.create_node();
    auto r = gtpo::map<gtpo::graph<>, gtpo::graph<>, decltype(f2)>(src, dst, f2);
    EXPECT_EQ(src.get_node_count(), dst.get_node_count());    // dst is non empty, gtpo::filter should check that precondition and return false.
}

TEST(gtpo_functional, map_copy)
{
    gtpo::graph<> src, dst;

    //src.create_node();
    // Just create an empty copy of source node to a destination node
//    const auto f2 = [](const auto& src_node) -> gtpo::graph<>::shared_node_t {
 //       static_cast<void>(src_node);
 //       return std::make_shared<typename gtpo::graph<>::node_t>();
 //   };
    //gtpo::copy2(src, dst, f2);
    gtpo::copy2(src, dst);
    EXPECT_EQ(src.get_node_count(), dst.get_node_count());    // dst is non empty, gtpo::filter should check that precondition and return false.
}


//-----------------------------------------------------------------------------
// gtpo::copy   (gtpo::copy is built internally with gtpo::map).
//-----------------------------------------------------------------------------

TEST(gtpo_functional, copy_precond)
{
    { // Test precondition "dst must be empty"
        gtpo::graph<> src;
        gtpo::graph<> dst;
        dst.create_node();
        auto r = gtpo::copy(src, dst);
        EXPECT_FALSE(r);    // Excpect false, precondition "dst must be empty" is no met.
    }
}

TEST(gtpo_functional, copy)
{
    { // Basic: a copy of an empty graph is an empty graph
        gtpo::graph<> src;
        gtpo::graph<> dst;
        auto r = gtpo::copy(src, dst);
        EXPECT_TRUE(r);
        EXPECT_TRUE(dst.is_empty());
    }
    { // Basic: src and dst should have the same number of nodes and edges after copy
        gtpo::graph<> src;
        gtpo::graph<> dst;
        auto n1 = src.create_node();
        auto n2 = src.create_node();
        src.create_edge(n1, n2);
        auto r = gtpo::copy(src, dst);
        EXPECT_TRUE(r);
        EXPECT_EQ(src.get_node_count(), dst.get_node_count());
        EXPECT_EQ(src.get_edge_count(), dst.get_edge_count());

        // FIXME: take groups into account too
    }
}

//-----------------------------------------------------------------------------
// gtpo::filter
//-----------------------------------------------------------------------------

TEST(gtpo_functional, filter_precond)
{
    gtpo::graph<> src;
    const auto f = [](const auto& node) -> bool { static_cast<void>(node); return false; };
    gtpo::graph<> dst;
    dst.create_node();
    auto r = gtpo::filter<gtpo::graph<>, gtpo::graph<>, decltype(f)>(src, dst, f);
    EXPECT_FALSE(r);    // dst is non empty, gtpo::filter should check that precondition and return false.
}

TEST(gtpo_functional, filter)
{
    { // Test filter with all nodes filtered
        gtpo::graph<> src;
        src.create_node();
        const auto f = [](const auto& node) -> bool { static_cast<void>(node); return false; };
        gtpo::graph<> dst;
        gtpo::filter<>(src, dst, f);
        EXPECT_EQ(dst.get_nodes().size(), 0);   // Filter functor always filter node, we should end up with no nodes
    }

    { // Test filter with only some nodes filtered
        gtpo::graph<> src;
        src.create_node();
        src.create_node();
        const auto f = [](const auto& node) -> bool {
            static_cast<void>(node);
            static int i = 0;
            return (i++ % 2) == 0;  // Select only even nodes
        };
        gtpo::graph<> dst;
        gtpo::filter<>(src, dst, f);
        EXPECT_EQ(dst.get_nodes().size(), 1);   // Filter functor always filter node, we should end up with no nodes
    }

    { // Test if topology (edges) selection works for only a subset of selected nodes
        gtpo::graph<> src;
        auto n1 = src.create_node().lock();
        auto n2 = src.create_node().lock();
        auto n3 = src.create_node();
        src.create_edge(n1, n2);
        src.create_edge(n1, n3);
        src.create_edge(n3, n2);
        const auto f = [n1, n2](const auto& node) -> bool {
            return node == n1 || node == n2;
        };
        gtpo::graph<> dst;
        gtpo::filter<>(src, dst, f);
        EXPECT_EQ(dst.get_nodes().size(), 2);   // Filter functor should have filtered 2 on 3 nodes
        EXPECT_EQ(dst.get_edges().size(), 1);   // Edges involving n3 should have been filtered too
    }
}

