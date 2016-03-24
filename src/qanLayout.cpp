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
// This file is a part of the Qanava software.
//
// \file	qanLayout.cpp
// \author	benoit@qanava.org
// \date	2015 09 07
//-----------------------------------------------------------------------------

// Qanava headers
#include "./qanLayout.h"

namespace qan { // ::qan

/* Layout Topology Utilities *///----------------------------------------------
/*void    Layout::collectNodeGroupRootNodes( qan::NodeList& nodes, qan::Node::List& rootNodes )
{
    foreach ( qan::Node* node, nodes )
    {
        if ( node->getInDegree( ) == 0 )
            rootNodes.append( node );
        else
        {
            // A node is a root node in this layout if none of its super node are member of the layout nodes.
            bool isRoot = true;
            foreach ( qan::Edge* inEdge, node->getInEdges( ) )
            {
                if ( nodes.contains( inEdge->getSrc( ) ) )
                {
                    isRoot = false;
                    break;
                }
            }
            if ( isRoot )
                rootNodes.append( node );
        }
    }
}

int     Layout::getNodeGroupInDegree( qan::NodeList& nodes, qan::Node& node )
{
    Q_ASSERT( nodes.contains( &node ) );
    if ( node.getInDegree( ) == 0 )
        return 0;

    // In degree is the count of super nodes which are member of the layout nodes.
    int inDegree = 0;
    foreach ( qan::Edge* inEdge, node.getInEdges( ) )
    {
        if ( nodes.contains( inEdge->getSrc( ) ) )
            inDegree++;
    }
    return inDegree;
}

void    Layout::collectNodeGroupInNodes( qan::NodeList& nodes, qan::Node& node, qan::Node::List& groupInNodes )
{
    foreach ( qan::Edge* inEdge, node.getInEdges( ) )
    {
        if ( nodes.contains( inEdge->getSrc( ) ) )
            groupInNodes.append( inEdge->getSrc( ) );
    }
}

void    Layout::collectNodeGroupOutNodes( qan::NodeList& nodes, qan::Node& node, qan::Node::List& groupOutNodes )
{
    foreach ( qan::Edge* outEdge, node.getOutEdges( ) )
    {
        if ( nodes.contains( outEdge->getDst( ) ) )
            groupOutNodes.append( outEdge->getDst( ) );
    }
}*/
//-----------------------------------------------------------------------------

} // ::qan
