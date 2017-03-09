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
// \file	gtpoTests.cpp
// \author	benoit@qanava.org
// \date	2016 01 26
//-----------------------------------------------------------------------------

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// GTpo headers
#include "../src/gtpoUtils.h"

int main(int argc, char **argv) {
    //::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
#ifdef GTPO_HAS_PROTOBUF
    gtpo::ProtoSerializer<>::initProtocolBuffer();
#endif
    return RUN_ALL_TESTS();
}

TEST(GTpoUtils, assertThrow)
{
    EXPECT_THROW( gtpo::assert_throw(false, "" ), gtpo::bad_topology_error );
    EXPECT_NO_THROW( gtpo::assert_throw(true, "" ) );
}


