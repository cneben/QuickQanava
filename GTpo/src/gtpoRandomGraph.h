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
// This file is a part of the GTpo software library.
//
// \file	gtpoRandomGraph.h
// \author	benoit@destrat.io
// \date	2016 02 10
//-----------------------------------------------------------------------------

#ifndef gtpoRandomGraph_h
#define gtpoRandomGraph_h

// STD headers
#include <random>
#include <cstdlib>
#include <sstream>

// GTPO headers
#include "./gtpoUtils.h"

class RandomGraph
{
public:
    struct RandomConfig {
        std::string nodeClassName   = "gtpo::Node";
        std::string edgeClassName   = "gtpo::Edge";
        int nodeCount               = 10;
        std::pair<int,int>          outNodeRng{ 1, 3 };

        std::pair<double, double>   xRng{ 0., 500. };
        std::pair<double, double>   yRng{ 0., 500. };
        std::pair<double, double>   widthRng{ 70., 120. };
        std::pair<double, double>   heightRng{ 40., 50. };

        auto    isValid() const -> bool {
            return ( ( std::abs( widthRng.first - widthRng.second ) ) > 0.000001 &&
                     ( std::abs( heightRng.first -  heightRng.second ) > 0.000001 ) );  // We do not need precise epsilon comparison here
        }
    };

    template < class Graph >
    static void generate( Graph& graph, RandomConfig& config )
    {
        if ( !config.isValid() )
            return;

        std::vector< typename Graph::WeakNode > nodes;
        for ( int n = 0; n < config.nodeCount; ++n ) {
            auto node = graph.createNode( config.nodeClassName );
            if ( !node.expired() )
                nodes.push_back( node );
        }
        if ( nodes.size() < 2 )
            return;
        std::random_device rd;
        std::mt19937 gen( 200 );
        std::uniform_int_distribution< int > outNodeDist( config.outNodeRng.first, config.outNodeRng.second );
        std::uniform_int_distribution< int > nodeIndexDist( 0, (int)nodes.size()-1 );
        for ( auto source: nodes ) {
            int outNodeCount = outNodeDist( gen );
            for ( int outNode = 0; outNode < outNodeCount; ++outNode ) {
                auto destination = nodes[ nodeIndexDist( gen ) ];
                if ( !gtpo::compare_weak_ptr( source, destination ) )
                graph.createEdge( config.edgeClassName, source, destination );
            }
        }

        std::uniform_real_distribution< double > xDist( config.xRng.first, config.xRng.second );
        std::uniform_real_distribution< double > yDist( config.yRng.first, config.yRng.second );
        std::uniform_real_distribution< double > widthDist( config.widthRng.first, config.widthRng.second );
        std::uniform_real_distribution< double > heightDist( config.heightRng.first, config.heightRng.second );
        int nodeIndex = 0;
        for ( auto weakNode: nodes ) {
            typename Graph::SharedNode node = weakNode.lock();
            if ( node ) {
                std::ostringstream ostr; ostr << nodeIndex++;
                std::string nodeLabel{ std::string( "Node #" ) + ostr.str() };
                Graph::Configuration::setNodeLabel( node.get(), nodeLabel );
                Graph::Configuration::setNodeX( node.get(), xDist( gen ) );
                Graph::Configuration::setNodeY( node.get(), yDist( gen ) );
                Graph::Configuration::setNodeWidth( node.get(), widthDist( gen ) );
                Graph::Configuration::setNodeHeight( node.get(), heightDist( gen ) );
            }
        }
    }
};

#endif // gtpoRandomGraph_h
