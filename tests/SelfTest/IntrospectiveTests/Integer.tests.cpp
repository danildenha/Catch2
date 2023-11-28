
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0

#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_random_integer_helpers.hpp>

namespace {
    template <typename Int>
    static void
    CommutativeMultCheck( Int a, Int b, Int upper_result, Int lower_result ) {
        using Catch::Detail::ExtendedMult;
        using Catch::Detail::ExtendedMultResult;
        CHECK( ExtendedMult( a, b ) ==
               ExtendedMultResult<Int>{ upper_result, lower_result } );
        CHECK( ExtendedMult( b, a ) ==
               ExtendedMultResult<Int>{ upper_result, lower_result } );
    }
} // namespace

TEST_CASE( "ExtendedMult 64x64", "[Integer][approvals]" ) {
    using Catch::Detail::ExtendedMult;
    using Catch::Detail::ExtendedMultResult;

    // a x 0 == 0
    CommutativeMultCheck<uint64_t>( 0x12345678, 0, 0, 0 );

    // bit carried from low half to upper half
    CommutativeMultCheck<uint64_t>( uint64_t( 1 ) << 63, 2, 1, 0 );

    // bits in upper half on one side, bits in lower half on other side
    CommutativeMultCheck<uint64_t>( 0xcdcd'dcdc'0000'0000,
                                    0x0000'0000'aeae'aeae,
                                    0x0000'0000'8c6e'5a77,
                                    0x7391'a588'0000'0000 );

    // Some input numbers without interesting patterns
    CommutativeMultCheck<uint64_t>( 0xaaaa'aaaa'aaaa'aaaa,
                                    0xbbbb'bbbb'bbbb'bbbb,
                                    0x7d27'd27d'27d2'7d26,
                                    0xd82d'82d8'2d82'd82e );

    CommutativeMultCheck<uint64_t>( 0x7d27'd27d'27d2'7d26,
                                    0xd82d'82d8'2d82'd82e,
                                    0x69af'd991'8256'b953,
                                    0x8724'8909'fcb6'8cd4 );

    CommutativeMultCheck<uint64_t>( 0xdead'beef'dead'beef,
                                    0xfeed'feed'feed'feef,
                                    0xddbf'680b'2b0c'b558,
                                    0x7a36'b06f'2ce9'6321 );

    CommutativeMultCheck<uint64_t>( 0xddbf'680b'2b0c'b558,
                                    0x7a36'b06f'2ce9'6321,
                                    0x69dc'96c9'294b'fc7f,
                                    0xd038'39fa'a3dc'6858 );

    CommutativeMultCheck<uint64_t>( 0x61c8'8646'80b5'83eb,
                                    0x61c8'8646'80b5'83eb,
                                    0x2559'92d3'8220'8bbe,
                                    0xdf44'2d22'ce48'59b9 );
}
