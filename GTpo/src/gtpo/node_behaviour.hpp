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
// \file	node_behaviour.hpp
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#include "./utils.h"

namespace gtpo { // ::gtpo

/* Notification Helper Methods *///--------------------------------------------
template < class config_t >
template < class node_t, class edge_t >
auto    behaviourable_node< config_t >::notify_in_node_inserted( node_t target, node_t node, const edge_t& edge ) noexcept -> void
{
    this->notify_static_behaviours( [&](auto& behaviour) noexcept { behaviour.in_node_inserted( target, node, edge ); } );
}

template < class config_t >
template < class node_t, class edge_t >
auto    behaviourable_node< config_t >::notify_in_node_removed( node_t target, node_t node, const edge_t& edge ) noexcept -> void
{
    this->notify_static_behaviours( [&](auto& behaviour) noexcept { behaviour.in_node_removed( target, node, edge ); } );
}

template < class config_t >
template < class node_t >
auto    behaviourable_node< config_t >::notify_in_node_removed( node_t target ) noexcept -> void
{
    this->notify_static_behaviours( [&](auto& behaviour) noexcept { behaviour.in_node_removed( target ); } );
}

template < class config_t >
template < class node_t, class edge_t >
auto    behaviourable_node< config_t >::notify_out_node_inserted( node_t target, node_t node, const edge_t& edge ) noexcept -> void
{
    this->notify_static_behaviours( [&](auto& behaviour) noexcept { behaviour.out_node_inserted( target, node, edge ); } );
}

template < class config_t >
template < class node_t, class edge_t >
auto    behaviourable_node< config_t >::notify_out_node_removed( node_t target, node_t node, const edge_t& edge ) noexcept -> void
{
    this->notify_static_behaviours( [&](auto& behaviour) noexcept { behaviour.out_node_removed( target, node, edge ); } );
}

template < class config_t >
template < class node_t >
auto    behaviourable_node< config_t >::notify_out_node_removed( node_t target ) noexcept -> void
{
    this->notify_static_behaviours( [&](auto& behaviour) noexcept { behaviour.out_node_removed(target); } );
}
//-----------------------------------------------------------------------------

} // ::gtpo

