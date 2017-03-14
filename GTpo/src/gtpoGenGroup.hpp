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
// This file is a part of the GTpo software.
//
// \file	gtpoGroup.hpp
// \author	benoit@destrat.io
// \date	2016 03 22
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

/* Group Nodes Management *///-------------------------------------------------
template < class Config >
auto GenGroup< Config >::hasNode( const WeakNode& node ) const noexcept -> bool
{
    if ( node.expired() )
        return false;
    SharedNode groupNode = node.lock();
    if ( groupNode == nullptr )
        return false;
    auto groupNodeIter = std::find_if( _nodes.begin(), _nodes.end(),
                                        [=](const WeakNode& groupNode ){ return ( compare_weak_ptr<>( node, groupNode ) ); } );
    return groupNodeIter != _nodes.end();
}
//-----------------------------------------------------------------------------

} // ::gtpo
