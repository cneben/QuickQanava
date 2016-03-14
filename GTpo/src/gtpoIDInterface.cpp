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
// \file	gtpoIDInterface.cpp
// \author	benoit@qanava.org
// \date	2016 01 22
//-----------------------------------------------------------------------------

// GTpo headers
#include "./gtpoIDInterface.h"

namespace gtpo { // ::gtpo

/* SimpleIDInterface object/id Mapping *///------------------------------------
int     SimpleIDInterface::registerObject( void* object, int id )
{
    if ( id == -1 ) // Eventually, generate an id if none is given
        id = ++_maxId;
    else
        _maxId = qMax( _maxId, id );

    // Check that an object isn't already registered with the given id
    if ( _idObjectMap.contains( id ) )
        return -1;

    _objectIdMap.insert( object, id );
    _idObjectMap.insert( id, object );
    return id;
}

int     SimpleIDInterface::getId( void* object )
{
    int id = _objectIdMap.value( object, -1 );
    if ( id == -1 )
        return registerObject( object );
    return id;
}

void*   SimpleIDInterface::getObject( int id )
{
    return _idObjectMap.value( id, nullptr );
}

void    SimpleIDInterface::clear( )
{
    _idObjectMap.clear( );
    _maxId = 0;
}
//-----------------------------------------------------------------------------

} // ::gtpo

