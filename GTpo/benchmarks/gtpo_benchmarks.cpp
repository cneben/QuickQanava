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
#include "../src/algorithm.h"
#include "../src/generator.h"

// Google Benchmark
#include <benchmark/benchmark.h>

/*
 * Available benchmarks:
 * --benchmark_report_aggregates_only={true|false}
 *
 *
 *
 */


struct config_raw final :  public gtpo::config<config_raw>
{
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
    auto n1 = g.create_node();
    auto n2 = g.create_node();
    auto e = g.create_edge( n1, n2 );

    auto g1 = g.create_group();
    g.group_node(n1, g1);
    g.ungroup_node(n1, g1);

    g.remove_group(g1);
    g.remove_edge(e);
    g.remove_node(n2);
    g.remove_node(n1);
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


static std::vector<std::unique_ptr<gtpo::graph<>>>  bin_trees;

static void BM_linearize_dfs_on_tree(benchmark::State& state)
{
    const auto g = bin_trees[state.range(0)].get();
    for (auto _ : state) {
        auto r = gtpo::linearize_dfs(*g);
    }
}
static void BM_linearize_tree_dfs_rec(benchmark::State& state)
{
    const auto g = bin_trees[state.range(0)].get();
    for (auto _ : state) {
        auto r = gtpo::linearize_tree_dfs_rec(*g);
    }
}
BENCHMARK(BM_linearize_dfs_on_tree)->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
  })->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
  })->DenseRange(0, 14, 1);
BENCHMARK(BM_linearize_tree_dfs_rec)->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
  })->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
  })->DenseRange(0, 14, 1);

int main(int argc, char** argv) {
    // Generate CSV with following command:
    // ./gtpo_benchmarks --benchmark_filter=BM_linearize  --benchmark_report_aggregates_only=true --benchmark_repetitions=4 --benchmark_out_format=csv  --benchmark_out=linearize_dfs_tree.csv

    // Generate candidate trees
    for ( int depth = 0; depth < 15; depth++ ) {
        auto t = std::make_unique<gtpo::graph<>>();
        gtpo::complete_tree_rec(*t, depth, 2);
        bin_trees.emplace_back(std::move(t));
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}

