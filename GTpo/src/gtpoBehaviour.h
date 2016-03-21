/*
    The MIT License (MIT)

    Copyright (c) 2016 Benoit AUTHEMAN

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the GTpo software library.
//
// \file	gtpoBehaviour.h
// \author	benoit@qanava.org
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpoBehaviour_h
#define gtpoBehaviour_h

// STD headers
#include <functional>       // std::function
#include <fstream>          // std::ofstream

// Qan.Topo headers
// Nil

namespace gtpo { // ::gtpo

/*! \brief Define a node observer interface.
 *
 */
template < class Node >
class NodeBehaviour
{
public:
    NodeBehaviour() = default;
    ~NodeBehaviour() = default;
    explicit NodeBehaviour( const NodeBehaviour& ) = default;
    NodeBehaviour& operator=( const NodeBehaviour& ) = default;

    //! Called immediatly after \c node has been inserted.
    virtual auto nodeInserted( Node node ) -> void { (void)node; }
    //! Called when \c node is about to be removed.
    virtual auto nodeRemoved( Node node ) -> void { (void)node; }
    /*! \brief Called immedialty after \c node has been modified.
     *
     * Creating, inserting or removing a node does not generate a nodeModified() call.
     *
     * A call to nodeModified() usually follow a node property modification trought
     * the gtpo::GenGraph::Config PropertiesConfig interface.
     */
    virtual auto    nodeModified( Node node ) -> void { (void)node; }
};

/*! \brief Define an edge observer interface.
 *
 */
template < class Edge >
class EdgeBehaviour
{
public:
    EdgeBehaviour() = default;
    ~EdgeBehaviour() = default;
    explicit EdgeBehaviour( const EdgeBehaviour& ) = default;
    EdgeBehaviour& operator=( const EdgeBehaviour& ) = default;

    //! Called immediatly after \c edge has been inserted.
    virtual auto    edgeInserted( Edge edge ) -> void { (void)edge; }
    //! Called when \c edge is about to be removed.
    virtual auto    edgeRemoved( Edge edge ) -> void { (void)edge; }
    /*! \brief Called immediatly after \c edge has been modified.
     *
     * Creating, inserting or removing an edge does not generate an edgeModified() call.
     *
     * A call to edgeModified() usually follow an edge property modification trought
     * the gtpo::GenGraph::Config PropertiesConfig interface.
     */
    virtual auto    edgeModified( Edge edge ) -> void { (void)edge; }
};

/*! \brief Define an observer interface to catch changes in a gtpo::GenGraph.
 *
 */
template < class Node, class Edge >
class GraphBehaviour :  public NodeBehaviour< Node >,
                        public EdgeBehaviour< Edge >
{
public:
    GraphBehaviour() = default;
    ~GraphBehaviour() = default;
    explicit GraphBehaviour( const GraphBehaviour& ) = default;
    GraphBehaviour& operator=( const GraphBehaviour& ) = delete;
};

} // ::gtpo

#include "./gtpoBehaviour.hpp"

#endif // gtpoBehaviour_h

