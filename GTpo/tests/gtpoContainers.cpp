/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the GTpo software.
//
// \file	gtpoContainers.cpp
// \author	benoit@qanava.org
// \date	2016 01 26
//-----------------------------------------------------------------------------

// STD headers
#include <iostream>

// GTpo headers
#include "../src/gtpoContainerAdapter.h"

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using gtpo::std_container_adapter;

//-----------------------------------------------------------------------------
// GTpo container adapter tests
//-----------------------------------------------------------------------------
TEST(GTpoContainerAdapter, stdVector)
{   
    {
        using IntVector = std::vector<int>;
        IntVector iv;
        int i = 42;
        EXPECT_TRUE( std_container_adapter<IntVector>::size(iv) == 0 );
        EXPECT_FALSE( std_container_adapter<IntVector>::contains(iv, i) );
        std_container_adapter<IntVector>::insert(i, iv);
        EXPECT_TRUE( std_container_adapter<IntVector>::size(iv) == 1 );
        EXPECT_TRUE( std_container_adapter<IntVector>::contains(iv, i) );
        std_container_adapter<IntVector>::remove(i, iv);
        EXPECT_TRUE( std_container_adapter<IntVector>::size(iv) == 0 );
        EXPECT_FALSE( std_container_adapter<IntVector>::contains(iv, i) );

        std_container_adapter<IntVector>::reserve(iv, 2);
    }

    {
        using SharedIntVector = std::vector< std::shared_ptr<int> >;
        SharedIntVector siv;
        auto si = std::make_shared<int>( 42 );
        EXPECT_TRUE( std_container_adapter<SharedIntVector>::size(siv) == 0 );
        EXPECT_FALSE( std_container_adapter<SharedIntVector>::contains(siv, si) );
        std_container_adapter<SharedIntVector>::insert(si, siv);
        EXPECT_TRUE( std_container_adapter<SharedIntVector>::size(siv) == 1 );
        EXPECT_TRUE( std_container_adapter<SharedIntVector>::contains(siv, si) );
        std_container_adapter<SharedIntVector>::remove(si, siv);

        std_container_adapter<SharedIntVector>::reserve(siv, 2);
    }
    {
        using WeakIntVector = std::vector< std::weak_ptr<int> >;
        auto si = std::make_shared<int>( 42 );
        auto wi = std::weak_ptr<int>{si};
        WeakIntVector wiv;
        EXPECT_TRUE( std_container_adapter<WeakIntVector>::size(wiv) == 0 );
        EXPECT_FALSE( std_container_adapter<WeakIntVector>::contains(wiv, wi) );
        std_container_adapter<WeakIntVector>::insert(wi, wiv);
        EXPECT_TRUE( std_container_adapter<WeakIntVector>::size(wiv) == 1 );
        EXPECT_TRUE( std_container_adapter<WeakIntVector>::contains(wiv, wi) );
        std_container_adapter<WeakIntVector>::remove(wi, wiv);

        std_container_adapter<WeakIntVector>::reserve(wiv, 2);
    }
}

TEST(GTpoContainerAdapter, stdListInsertRemove)
{
    using SharedIntList = std::list< std::shared_ptr<int> >;
    SharedIntList sil;
    auto si = std::make_shared<int>( 42 );
    std_container_adapter<SharedIntList>::insert(si, sil);
    std_container_adapter<SharedIntList>::remove(si, sil);

    using IntList = std::list<int>;
    IntList il;
    int i = 42;
    std_container_adapter<IntList>::insert(i, il);
    std_container_adapter<IntList>::remove(i, il);
}

TEST(GTpoContainerAdapter, stdSetInsertRemove)
{
    using SharedIntSet = std::set< std::shared_ptr<int> >;
    SharedIntSet sil;
    auto si = std::make_shared<int>( 42 );
    std_container_adapter<SharedIntSet>::insert(si, sil);
    std_container_adapter<SharedIntSet>::remove(si, sil);

    using IntSet = std::set<int>;
    IntSet il;
    int i = 42;
    std_container_adapter<IntSet>::insert(i, il);
    std_container_adapter<IntSet>::remove(i, il);
}

TEST(GTpoContainerAdapter, stdUnorderedSetInsertRemove)
{
    using SharedIntUnorderedSet = std::unordered_set< std::shared_ptr<int> >;
    SharedIntUnorderedSet sil;
    auto si = std::make_shared<int>( 42 );
    std_container_adapter<SharedIntUnorderedSet>::insert(si, sil);
    std_container_adapter<SharedIntUnorderedSet>::remove(si, sil);

    using IntUnorderedSet = std::unordered_set<int>;
    IntUnorderedSet il;
    int i = 42;
    std_container_adapter<IntUnorderedSet>::insert(i, il);
    std_container_adapter<IntUnorderedSet>::remove(i, il);
}





/*    using IntList = std::list< std::shared_ptr<int> >;
    IntList l;
    using IntVector = std::vector< std::shared_ptr<int> >;
    IntVector v;

    using IntSet = std::set< std::shared_ptr<int> >;
    IntSet s;
    using IntUnorderedSet = std::unordered_set< std::shared_ptr<int> >;
    IntUnorderedSet us;

    // It is rather more a "compile test" than a runtime one, since
    // we have 100+ lines of specialized template code for just
    // adding/removing items in so-called generic containers; STL
    // sucks, but we probably won a few ms over Qt containers
    l.push_back( std::make_shared<int>( 42 ) );
    gtpo::std_container_adapter<std::list, std::shared_ptr<int>>::remove( l.front(), l );
    auto cs = l.size();
    EXPECT_TRUE( ( cs == 1 && l.size() == 0 ) );

    v.push_back( std::make_shared<int>( 42 ) );
    cs = v.size();
    gtpo::std_container_adapter<std::vector, std::shared_ptr<int>>::remove( v[0], v );
    EXPECT_TRUE( ( cs == 1 && v.size() == 0 ) ) ;

    s.insert( std::make_shared<int>( 42 ) );
    cs = s.size();
    gtpo::std_container_adapter<std::set, std::shared_ptr<int>>::remove( *s.begin(), s );
    EXPECT_TRUE( ( cs == 1 && s.size() == 0 ) ) ;*/

