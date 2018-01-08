///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2015 libbitcoin-network developers (see COPYING).
//
//        GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
//
///////////////////////////////////////////////////////////////////////////////
#ifndef LIBBITCOIN_NETWORK_VERSION_HPP
#define LIBBITCOIN_NETWORK_VERSION_HPP

/**
 * The semantic version of this repository as: [major].[minor].[patch]
 * For interpretation of the versioning scheme see: http://semver.org
 */

#define LIBBITCOIN_NETWORK_VERSION "0.5.0"
#define LIBBITCOIN_NETWORK_MAJOR_VERSION 0
#define LIBBITCOIN_NETWORK_MINOR_VERSION 5
#define LIBBITCOIN_NETWORK_PATCH_VERSION 0

// #define STR_HELPER(x) #x
// #define STR(x) STR_HELPER(x)
// #define LIBBITCOIN_NETWORK_VERSION STR(LIBBITCOIN_NETWORK_MAJOR_VERSION) "." STR(LIBBITCOIN_NETWORK_MINOR_VERSION) "." STR(LIBBITCOIN_NETWORK_PATCH_VERSION)
// #undef STR
// #undef STR_HELPER

#ifdef BITPRIM_BUILD_NUMBER
#define BITPRIM_NETWORK_VERSION BITPRIM_BUILD_NUMBER
#else
#define BITPRIM_NETWORK_VERSION "v0.0.0"
#endif

namespace libbitcoin { namespace network {
char const* version();
}} /*namespace libbitcoin::network*/


#endif
