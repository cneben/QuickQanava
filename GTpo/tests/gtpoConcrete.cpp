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
// \file	qtpoConcrete.cpp
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

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//-----------------------------------------------------------------------------
// GTpo topology tests with concrete support
//-----------------------------------------------------------------------------

class Node;
class Edge;
class Graph;
class Group;

struct MyNode {
    std::string     _label;
};

class CustomConfig final :  public gtpo::GraphConfig,
                            public gtpo::StdContainerAccessors
{
public:
    using UserNode      = MyNode;
    //using FinalEdge     = stpo::Edge;
    //using FinalGroup    = stpo::Group;

    using FinalNode     = gtpo::GenNode<CustomConfig>;
    using FinalEdge     = gtpo::GenEdge<CustomConfig>;
    using FinalGroup    = gtpo::GenGroup<CustomConfig>;

    using GraphBehaviours = std::tuple< gtpo::GraphGroupAjacentEdgesBehaviour< CustomConfig> >;
    using GroupBehaviours = std::tuple< gtpo::GroupAdjacentEdgesBehaviour< CustomConfig > >;

    template <class...Ts>
    using NodeContainer = std::vector<Ts...>;

    template <class...Ts>
    using EdgeContainer = std::vector<Ts...>;

    template <class T>
    using SearchContainer = std::unordered_set<T>;
};

TEST(GTpoConcrete, compilation)
{
    using CustomGraph = gtpo::GenGraph<CustomConfig>;

    CustomGraph g;
    auto n = std::make_shared<CustomGraph::Node>();
    n->data()._label = "test";
    g.insertNode(n);

    EXPECT_TRUE( true );
}

