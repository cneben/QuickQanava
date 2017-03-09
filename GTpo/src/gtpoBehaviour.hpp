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
// \file	gtpoBehaviour.hpp
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#include "./gtpoUtils.h"

namespace gtpo { // ::gtpo

/* Virtual Behaviours Management *///------------------------------------------
template < class Behaviour, class SBehaviours  >
template < class T >
auto    Behaviourable< Behaviour, SBehaviours >::notifyBehaviours( void (Behaviour::*method)(T&), T& arg ) noexcept -> void
{
    // Note 20160314: See http://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
    // For calling pointer on template template parameter template keyword functions.
    // std::unique_ptr has no overload for .* or ->* operator
    // (*behaviour) == (*behaviour.get())
    // Equivalent to: ((*behaviour.get()).*method)(arg)
    for ( auto& behaviour : _behaviours )
        if ( behaviour )
            ((*behaviour).*method)(arg);
}

template < class Behaviour, class SBehaviours  >
template < class T, class T2 >
auto    Behaviourable< Behaviour, SBehaviours >::notifyBehaviours( void (Behaviour::*method)(T&, T2&), T& arg, T2& arg2 ) noexcept -> void
{
    // Note 20160314: See http://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
    // For calling pointer on template template parameter template keyword functions.
    // std::unique_ptr has no overload for .* or ->* operator
    // (*behaviour) == (*behaviour.get())
    // Equivalent to: ((*behaviour.get()).*method)(arg)
    for ( auto& behaviour : _behaviours )
        if ( behaviour )
            ((*behaviour).*method)(arg, arg2);
}

template < class Behaviour, class SBehaviours  >
auto    Behaviourable< Behaviour, SBehaviours >::notifyBehaviours0( void (Behaviour::*method)() ) noexcept -> void
{
    for ( auto& behaviour : _behaviours )
        if ( behaviour != nullptr )
            ((*behaviour).*method)();
}
//-----------------------------------------------------------------------------

} // ::gtpo

