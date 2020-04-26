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
// \file	qtpo_config_tests.cpp
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

//-----------------------------------------------------------------------------
// GTpo topology tests with concrete support
//-----------------------------------------------------------------------------
namespace custom {

class Node;

struct Config final : public gtpo::config<Config>
{
    using graph_base = gtpo::empty;
    using node_base  = gtpo::empty;
    using edge_base  = gtpo::empty;
    using group_base = gtpo::empty;

    using final_group_t        = gtpo::group<Config>;
    using final_node_t         = custom::Node;
    using final_edge_t         = gtpo::edge<Config>;
    using final_group_edge_t   = gtpo::group_edge<Config>;

    using final_config = Config;
};

class Node : public gtpo::node<Config> {
public:
    Node() = default;

    std::string _label;
};

}

TEST(GTpoConcrete, compilation)
{
    using Graph = gtpo::graph<custom::Config>;

    Graph g;
    auto n = std::make_shared<custom::Node>();
    //n->_label = "test";
    //g.insert_node(n);

    EXPECT_TRUE(true);
}

