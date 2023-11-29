
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0

#ifndef CATCH_RANDOM_INTEGER_HELPERS_HPP_INCLUDED
#define CATCH_RANDOM_INTEGER_HELPERS_HPP_INCLUDED

#include <cstddef>
#include <cstdint>

namespace Catch {
    namespace Detail {
        template <typename T>
        struct ExtendedMultResult {
            T upper;
            T lower;
            friend bool operator==( ExtendedMultResult const& lhs,
                                    ExtendedMultResult const& rhs ) {
                return lhs.upper == rhs.upper && lhs.lower == rhs.lower;
            }
        };

        // Returns 128 bit result of multiplying lhs and rhs
        constexpr ExtendedMultResult<std::uint64_t>
        ExtendedMult( std::uint64_t lhs, std::uint64_t rhs ) {
            // We use the simple long multiplication approach for
            // correctness, we can use platform specific builtins
            // for performance later.

            // Split the lhs and rhs into two 32bit "digits", so that we can
            // do 64 bit arithmetic to handle carry bits.
            //            32b    32b    32b    32b
            //     lhs                  L1     L2
            //   * rhs                  R1     R2
            //            ------------------------
            //                       |  R2 * L2  |
            //                 |  R2 * L1  |
            //                 |  R1 * L2  |
            //           |  R1 * L1  |
            //           -------------------------
            //           |  a  |  b  |  c  |  d  |

#define CarryBits( x ) ( x >> 32 )
#define Digits( x ) ( x & 0xFF'FF'FF'FF )

            auto r2l2 = Digits( rhs ) * Digits( lhs );
            auto r2l1 = Digits( rhs ) * CarryBits( lhs );
            auto r1l2 = CarryBits( rhs ) * Digits( lhs );
            auto r1l1 = CarryBits( rhs ) * CarryBits( lhs );

            // Sum to columns first
            auto d = Digits( r2l2 );
            auto c = CarryBits( r2l2 ) + Digits( r2l1 ) + Digits( r1l2 );
            auto b = CarryBits( r2l1 ) + CarryBits( r1l2 ) + Digits( r1l1 );
            auto a = CarryBits( r1l1 );

            // Propagate carries between columns
            c += CarryBits( d );
            b += CarryBits( c );
            a += CarryBits( b );

            // Remove the used carries
            c = Digits( c );
            b = Digits( b );
            a = Digits( a );

#undef CarryBits
#undef Digits

            return {
                a << 32 | b, // upper 64 bits
                c << 32 | d  // lower 64 bits
            };
        }

        template <std::size_t>
        struct SizedUnsignedType;
#define SizedUnsignedTypeHelper( TYPE )        \
    template <>                                \
    struct SizedUnsignedType<sizeof( TYPE )> { \
        using type = TYPE;                     \
    }

        SizedUnsignedTypeHelper( std::uint8_t );
        SizedUnsignedTypeHelper( std::uint16_t );
        SizedUnsignedTypeHelper( std::uint32_t );
        SizedUnsignedTypeHelper( std::uint64_t );
#undef SizedUnsignedTypeHelper

        template <std::size_t sz>
        using SizedUnsignedType_t = typename SizedUnsignedType<sz>::type;

        template <typename T>
        using DoubleWidthUnsignedType_t = SizedUnsignedType_t<2 * sizeof(T)>;



    } // namespace Detail
} // namespace Catch

#endif // CATCH_RANDOM_INTEGER_HELPERS_HPP_INCLUDED
