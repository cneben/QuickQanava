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
// \author	benoit@qanava.org
// \date	2016 02 08
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

template < class Behaviour >
template < class T >
auto    Behaviourable< Behaviour >::notifyBehaviours( void (Behaviour::*method)(T&), T& arg ) -> void
{
    // Note 20160314: See http://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
    // For calling pointer on memg++ template template parameter template keywordber functions.
    // std::unique_ptr has no overload for .* or ->* operator
    // (*behaviour) == (*behaviour.get())
    // Equivalent to: ((*behaviour.get()).*method)(arg)
    for ( auto& behaviour : _behaviours )
        if ( behaviour != nullptr )
            ((*behaviour).*method)(arg);
}

} // ::gtpo

