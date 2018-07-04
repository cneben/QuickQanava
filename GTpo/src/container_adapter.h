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
// This file is a part of the QuickContainers library.
//
// \file    container_adapter.h
// \author  benoit@destrat.io
// \date    2017 03 04
//-----------------------------------------------------------------------------

#ifndef gtpo_container_adapter_h
#define gtpo_container_adapter_h

// Std headers
#include <cstddef>          // std::size_t
#include <list>
#include <vector>
#include <set>
#include <unordered_set>
#include <memory>

// Gtpo headers
#include "./utils.h"

namespace gtpo { // ::gtpo

template < typename T >
struct std_container_adapter { };

template < typename T >
struct std_container_adapter< std::vector<T> > {
    inline static void  insert( T t, std::vector<T>& c ) { c.emplace_back( t ); }
    inline static void  insert( T t, std::vector<T>& c, int i ) { c.insert( i, t ); }
    inline static void  remove( const T& t, std::vector<T>& c )
    {   // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
        c.erase( std::remove(c.begin(), c.end(), t), c.end());
    }
    inline static   std::size_t size( std::vector<T>& c ) { return c.size(); }
    inline static   bool        contains( const std::vector<T>& c, const T& t ) { return std::find(std::begin(c), std::end(c), t) != std::end(c); }
    inline static   void        reserve( std::vector<T>& c, std::size_t s) { c.reserve(s); }
};

template < typename T >
struct std_container_adapter< std::vector<std::shared_ptr<T>> > {
    inline static void          insert( std::shared_ptr<T> t, std::vector<std::shared_ptr<T>>& c ) { c.emplace_back( t ); }
    static constexpr void       insert( std::shared_ptr<T> t, std::vector<std::shared_ptr<T>>& c, int i ) { c.insert( i, t ); }
    inline static void          remove( const std::shared_ptr<T>& t, std::vector<std::shared_ptr<T>>& c )
    {   // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
        c.erase( std::remove(c.begin(), c.end(), t), c.end());
    }
    inline static   std::size_t size( std::vector<std::shared_ptr<T>>& c ) { return c.size(); }
    inline static   bool        contains( const std::vector<std::shared_ptr<T>>& c, const std::shared_ptr<T>& t ) { return std::find(std::begin(c), std::end(c), t) != std::end(c); }
    inline static   void        reserve( std::vector<std::shared_ptr<T>>& c, std::size_t s) { c.reserve(s); }
};

template < typename T >
struct std_container_adapter< std::vector<std::weak_ptr<T>> > {
    inline static void      insert( std::weak_ptr<T> t, std::vector<std::weak_ptr<T>>& c ) { c.emplace_back( t ); }
    static constexpr void   insert( std::weak_ptr<T> t, std::vector<std::weak_ptr<T>>& c, int i ) { c.insert( i, t ); }
    inline static void      remove( const std::weak_ptr<T>& t, std::vector<std::weak_ptr<T>>& c )
    {
        c.erase( std::remove_if(c.begin(), c.end(), [=](const std::weak_ptr<T>& wp){
            return gtpo::compare_weak_ptr( wp, t );
        }), c.end());
    }
    inline static   std::size_t size( std::vector<std::weak_ptr<T>>& c ) { return c.size(); }
    inline static   bool        contains( const std::vector<std::weak_ptr<T>>& c, const std::weak_ptr<T>& t ) {
        return std::find_if( std::begin(c), std::end(c), [=](const auto& wp){
            return gtpo::compare_weak_ptr( wp, t );
        }) != std::end(c);
    }
    inline static   void        reserve( std::vector<std::weak_ptr<T>>& c, std::size_t s) { c.reserve(s); }
};

template < typename T >
struct std_container_adapter< std::list<T> > {
    inline static void             insert( T t, std::list<T>& c ) { c.emplace_back( t ); }
    inline static constexpr void   insert( T t, std::list<T>& c, int i ) { c.insert( i, t ); }
    inline static void             remove( const T& t, std::list<T>& c )
    {   // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
        c.erase( std::remove(c.begin(), c.end(), t), c.end());
    }
};

template < typename T >
struct std_container_adapter< std::list<std::shared_ptr<T>> > {
    inline static void             insert( std::shared_ptr<T> t, std::list<std::shared_ptr<T>>& c ) { c.emplace_back( t ); }
    inline static constexpr void   insert( std::shared_ptr<T> t, std::list<std::shared_ptr<T>>& c, int i ) { c.insert( i, t ); }
    inline static void             remove( const std::shared_ptr<T>& t, std::list<std::shared_ptr<T>>& c )
    {   // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
        c.erase( std::remove(c.begin(), c.end(), t), c.end());
    }
};

template < typename T >
struct std_container_adapter< std::set<T> > {
    inline static void             insert( T t, std::set<T>& c ) { c.insert( t ); }
    inline static constexpr void   insert( T t, std::set<T>& c, int i ) { c.insert( i, t ); }
    inline static void             remove( const T& t, std::set<T>& c ) { c.erase(t); }
};

template < typename T >
struct std_container_adapter< std::set<std::shared_ptr<T>> > {
    inline static void             insert( std::shared_ptr<T> t, std::set<std::shared_ptr<T>>& c ) { c.insert( t ); }
    inline static constexpr void   insert( std::shared_ptr<T> t, std::set<std::shared_ptr<T>>& c, int i ) { c.insert( i, t ); }
    inline static void             remove( const std::shared_ptr<T>& t, std::set<std::shared_ptr<T>>& c ) { c.erase(t); }
};

template < typename T >
struct std_container_adapter< std::unordered_set<T> > {
    inline static void             insert( T t, std::unordered_set<T>& c ) { c.insert( t ); }
    inline static constexpr void   insert( T t, std::unordered_set<T>& c, int i ) { c.insert( i, t ); }
    inline static void             remove( const T& t, std::unordered_set<T>& c ) { c.erase(t); }
};

template < typename T >
struct std_container_adapter< std::unordered_set<std::shared_ptr<T>> > {
    inline static void             insert( std::shared_ptr<T> t, std::unordered_set<std::shared_ptr<T>>& c ) { c.insert( t ); }
    inline static constexpr void   insert( std::shared_ptr<T> t, std::unordered_set<std::shared_ptr<T>>& c, int i ) { c.insert( i, t ); }
    inline static void             remove( const std::shared_ptr<T>& t, std::unordered_set<std::shared_ptr<T>>& c ) { c.erase(t); }
};

} // ::gtpo

#endif // gtpo_container_adapter_h

