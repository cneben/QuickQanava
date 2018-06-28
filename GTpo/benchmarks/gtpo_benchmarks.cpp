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
// \file	qtpo_benchmarks.cpp
// \author	benoit@qanava.org
// \date	2016 03 26
//-----------------------------------------------------------------------------

// STD headers
#include <list>
#include <memory>
#include <iostream>

// GTpo headers
#include <GTpo>

// Google Benchmark
#include <benchmark/benchmark.h>

struct config_raw final :  public gtpo::config<config_raw>
{
    using final_group_t        = gtpo::group<config_raw>;
    using final_node_t         = gtpo::node<config_raw>;
    using final_edge_t         = gtpo::edge<config_raw>;
    using final_group_edge_t   = gtpo::group_edge<config_raw>;
};
using graph_raw = gtpo::graph<config_raw>;

struct config_adjacent final :  public gtpo::config<config_adjacent>
{
    using graph_behaviours = std::tuple< gtpo::graph_group_adjacent_edges_behaviour<config_adjacent> >;
    using group_behaviours = std::tuple< gtpo::group_adjacent_edges_behaviour<config_adjacent> >;
    using node_behaviours = std::tuple< >;
};
using graph_adjacent = gtpo::graph<config_adjacent>;


struct config_no_dynamic final :  public gtpo::config<config_no_dynamic>
{
    using final_config = config_no_dynamic;
    using graph_behaviours = std::tuple< >;
    using group_behaviours = std::tuple< >;
    using node_behaviours = std::tuple< >;
};
using graph_no_dynamic = gtpo::graph<config_no_dynamic>;


using graph_complete = gtpo::graph<>;

namespace impl {  // ::impl

template < typename graph_t >
void    benchmark(graph_t& g) {
    auto n1 = g.createNode();
    auto n2 = g.createNode();
    auto e = g.createEdge( n1, n2 );

    auto g1 = g.createGroup();
    g.groupNode(n1, g1);
    g.ungroupNode(n1, g1);

    g.removeGroup(g1);
    g.removeEdge(e);
    g.removeNode(n2);
    g.removeNode(n1);
}

} // :impl

static void BM_GraphRaw(benchmark::State& state)
{
    graph_raw g;
    for (auto _ : state) {
        impl::benchmark(g);
    }
}

static void BM_GraphAdjacent(benchmark::State& state)
{
    graph_adjacent g;
    for (auto _ : state) {
        impl::benchmark(g);
    }
}


static void BM_GraphNoDynamic(benchmark::State& state)
{
    graph_no_dynamic g;
    for (auto _ : state) {
        impl::benchmark(g);
    }
}

static void BM_GraphComplete(benchmark::State& state)
{
    graph_complete g;
    for (auto _ : state) {
        impl::benchmark(g);
    }
}

BENCHMARK(BM_GraphRaw);
BENCHMARK(BM_GraphAdjacent);
BENCHMARK(BM_GraphNoDynamic);
BENCHMARK(BM_GraphComplete);

BENCHMARK_MAIN()

