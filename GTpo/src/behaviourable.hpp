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
// \file	behaviourable.hpp
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#include "./utils.h"

namespace gtpo { // ::gtpo

/* Virtual Behaviours Management *///------------------------------------------
template < class behaviour_t, class static_behaviours_t  >
template < class T >
auto    behaviourable< behaviour_t, static_behaviours_t >::notify_dynamic_behaviours( void (behaviour_t::*method)(T&), T& arg ) noexcept -> void
{
    // Note 20160314: See http://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
    // For calling pointer on template template parameter template keyword functions.
    // std::unique_ptr has no overload for .* or ->* operator
    // (*behaviour) == (*behaviour.get())
    // Equivalent to: ((*behaviour.get()).*method)(arg)
    for ( auto& behaviour : _dynamic_behaviours )
        if ( behaviour )
            ((*behaviour).*method)(arg);
}

template < class behaviour_t, class static_behaviours_t  >
template < class T, class T2 >
auto    behaviourable< behaviour_t, static_behaviours_t >::notify_dynamic_behaviours( void (behaviour_t::*method)(T&, T2&), T& arg, T2& arg2 ) noexcept -> void
{
    // Note 20160314: See http://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
    // For calling pointer on template template parameter template keyword functions.
    // std::unique_ptr has no overload for .* or ->* operator
    // (*behaviour) == (*behaviour.get())
    // Equivalent to: ((*behaviour.get()).*method)(arg)
    for ( auto& behaviour : _dynamic_behaviours )
        if ( behaviour )
            ((*behaviour).*method)(arg, arg2);
}

template < class behaviour_t, class static_behaviours_t  >
template < class T, class T2, class T3 >
auto    behaviourable< behaviour_t, static_behaviours_t >::notify_dynamic_behaviours( void (behaviour_t::*method)(T&, T2&, const T3&), T& arg, T2& arg2, const T3& arg3 ) noexcept -> void
{
    // Note 20160314: See http://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
    // For calling pointer on template template parameter template keyword functions.
    // std::unique_ptr has no overload for .* or ->* operator
    // (*behaviour) == (*behaviour.get())
    // Equivalent to: ((*behaviour.get()).*method)(arg)
    for ( auto& behaviour : _dynamic_behaviours )
        if ( behaviour )
            ((*behaviour).*method)(arg, arg2, arg3);
}

template < class behaviour_t, class static_behaviours_t  >
auto    behaviourable< behaviour_t, static_behaviours_t >::notify_dynamic_behaviours0( void (behaviour_t::*method)() ) noexcept -> void
{
    for ( auto& behaviour : _dynamic_behaviours )
        if ( behaviour )
            ((*behaviour).*method)();
}
//-----------------------------------------------------------------------------

} // ::gtpo

