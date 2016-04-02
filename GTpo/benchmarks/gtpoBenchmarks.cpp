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
// \file	qtpoBenchmarks.cpp
// \author	benoit@qanava.org
// \date	2016 03 26
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

// Google Benchmark
#include <benchmark/benchmark.h>

static void BM_GraphCreationWithBehaviours(benchmark::State& state) {
    stpo::Graph g;
    while ( state.KeepRunning( ) ) {
        auto n1 = g.createNode();
        auto n2 = g.createNode();
        auto e = g.createEdge( n1, n2 );
    }
}

static void BM_GraphCreationWithoutBehaviours(benchmark::State& state) {
    stpo::Graph g;
    while ( state.KeepRunning( ) ) {
        auto n1 = g.createNode();
        auto n2 = g.createNode();
        auto e = g.createEdge( n1, n2 );
    }
}

BENCHMARK(BM_GraphCreationWithBehaviours);
BENCHMARK(BM_GraphCreationWithoutBehaviours);

BENCHMARK_MAIN()

