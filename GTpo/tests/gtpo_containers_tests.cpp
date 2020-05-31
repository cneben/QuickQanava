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
// This file is a part of the GTpo software.
//
// \file	gtpo_containers_tests.cpp
// \author	benoit@qanava.org
// \date	2016 01 26
//-----------------------------------------------------------------------------

// STD headers
#include <iostream>

// GTpo headers
#include "../src/container_adapter.h"

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
