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
// This file is a part of the GTpo software library.
//
// \file	generator.h
// \author	benoit@destrat.io
// \date	2018 07 12
//-----------------------------------------------------------------------------

#ifndef gtpo_generator_h
#define gtpo_generator_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <iterator>         // std::forward_iterator_tag

#include <stack>        // FIXME: move to .hpp...

// GTpo headers
#include "./utils.h"
#include "./config.h"
#include "./edge.h"
#include "./node.h"
#include "./group.h"

namespace gtpo { // ::gtpo

/*
 * Some generation code here is inspired from NetworkX distributed under a BSD licence.
 *
 * https://github.com/networkx/networkx/blob/master/networkx/generators/random_graphs.py
 *
 */

template <class config_t>
class graph;

/* Tree Generation Algorithms *///---------------------------------------------
/*! \brief .
 *
 */
template <class graph_t>
complete_tree() -> graph_t
{

}
//-----------------------------------------------------------------------------


/* Graph Generation Algorithms *///--------------------------------------------
/*! \brief Generate a fully connected directed graph.
 *
 */
template <class graph_t>
complete_graph() -> graph_t
{

}

/*! \brief .
 *
 */
template <class graph_t>
gnp_random_graph() -> graph_t
{

}

/*! \brief Shortcut to gnp_random_graph() generation function.
 *
 */
template <class graph_t>
erdos_renyi_graph() -> graph_t
{
    return gnp_random_graph<graph_t>();
}
//-----------------------------------------------------------------------------

} // ::gtpo

#include "./generator.hpp"

#endif // gtpo_generator_h

