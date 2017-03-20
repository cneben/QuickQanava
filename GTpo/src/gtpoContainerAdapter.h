/*
    This file is part of QuickContainers library.

    Copyright (C) 2016  Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickContainers library.
//
// \file    gtpoContainerAdapter.h
// \author  benoit@destrat.io
// \date    2017 03 04
//-----------------------------------------------------------------------------

#ifndef gtpoContainerAdapter_h
#define gtpoContainerAdapter_h

// Std headers
#include <cstddef>          // std::size_t
#include <list>
#include <vector>
#include <set>
#include <unordered_set>
#include <memory>

// Gtpo headers
#include "./gtpoUtils.h"

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
    static void             insert( std::shared_ptr<T> t, std::vector<std::shared_ptr<T>>& c ) { c.emplace_back( t ); }
    static constexpr void   insert( std::shared_ptr<T> t, std::vector<std::shared_ptr<T>>& c, int i ) { c.insert( i, t ); }
    static void             remove( const std::shared_ptr<T>& t, std::vector<std::shared_ptr<T>>& c )
    {   // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
        c.erase( std::remove(c.begin(), c.end(), t), c.end());
    }
    inline static   std::size_t size( std::vector<std::shared_ptr<T>>& c ) { return c.size(); }
    inline static   bool        contains( const std::vector<std::shared_ptr<T>>& c, const std::shared_ptr<T>& t ) { return std::find(std::begin(c), std::end(c), t) != std::end(c); }
    inline static   void        reserve( std::vector<std::shared_ptr<T>>& c, std::size_t s) { c.reserve(s); }
};

template < typename T >
struct std_container_adapter< std::vector<std::weak_ptr<T>> > {
    static void             insert( std::weak_ptr<T> t, std::vector<std::weak_ptr<T>>& c ) { c.emplace_back( t ); }
    static constexpr void   insert( std::weak_ptr<T> t, std::vector<std::weak_ptr<T>>& c, int i ) { c.insert( i, t ); }
    static void             remove( const std::weak_ptr<T>& t, std::vector<std::weak_ptr<T>>& c )
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
    static void             insert( T t, std::list<T>& c ) { c.emplace_back( t ); }
    static constexpr void   insert( T t, std::list<T>& c, int i ) { c.insert( i, t ); }
    static void             remove( const T& t, std::list<T>& c )
    {   // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
        c.erase( std::remove(c.begin(), c.end(), t), c.end());
    }
};

template < typename T >
struct std_container_adapter< std::list<std::shared_ptr<T>> > {
    static void             insert( std::shared_ptr<T> t, std::list<std::shared_ptr<T>>& c ) { c.emplace_back( t ); }
    static constexpr void   insert( std::shared_ptr<T> t, std::list<std::shared_ptr<T>>& c, int i ) { c.insert( i, t ); }
    static void             remove( const std::shared_ptr<T>& t, std::list<std::shared_ptr<T>>& c )
    {   // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
        c.erase( std::remove(c.begin(), c.end(), t), c.end());
    }
};

template < typename T >
struct std_container_adapter< std::set<T> > {
    static void             insert( T t, std::set<T>& c ) { c.insert( t ); }
    static constexpr void   insert( T t, std::set<T>& c, int i ) { c.insert( i, t ); }
    static void             remove( const T& t, std::set<T>& c ) { c.erase(t); }
};

template < typename T >
struct std_container_adapter< std::set<std::shared_ptr<T>> > {
    static void             insert( std::shared_ptr<T> t, std::set<std::shared_ptr<T>>& c ) { c.insert( t ); }
    static constexpr void   insert( std::shared_ptr<T> t, std::set<std::shared_ptr<T>>& c, int i ) { c.insert( i, t ); }
    static void             remove( const std::shared_ptr<T>& t, std::set<std::shared_ptr<T>>& c ) { c.erase(t); }
};

template < typename T >
struct std_container_adapter< std::unordered_set<T> > {
    static void             insert( T t, std::unordered_set<T>& c ) { c.insert( t ); }
    static constexpr void   insert( T t, std::unordered_set<T>& c, int i ) { c.insert( i, t ); }
    static void             remove( const T& t, std::unordered_set<T>& c ) { c.erase(t); }
};

template < typename T >
struct std_container_adapter< std::unordered_set<std::shared_ptr<T>> > {
    static void             insert( std::shared_ptr<T> t, std::unordered_set<std::shared_ptr<T>>& c ) { c.insert( t ); }
    static constexpr void   insert( std::shared_ptr<T> t, std::unordered_set<std::shared_ptr<T>>& c, int i ) { c.insert( i, t ); }
    static void             remove( const std::shared_ptr<T>& t, std::unordered_set<std::shared_ptr<T>>& c ) { c.erase(t); }
};

} // ::gtpo

#endif // gtpoContainerAdapter_h

