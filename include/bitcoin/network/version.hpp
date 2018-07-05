///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2015 libbitcoin-network developers (see COPYING).
//
//        GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
//
///////////////////////////////////////////////////////////////////////////////
#ifndef LIBBITCOIN_NETWORK_VERSION_HPP_
#define LIBBITCOIN_NETWORK_VERSION_HPP_

/**
 * The semantic version of this repository as: [major].[minor].[patch]
 * For interpretation of the versioning scheme see: http://semver.org
 */


#ifdef BITPRIM_PROJECT_VERSION
#define BITPRIM_NETWORK_VERSION BITPRIM_PROJECT_VERSION
#else
#define BITPRIM_NETWORK_VERSION "0.0.0"
#endif

namespace libbitcoin { namespace network {
char const* version();
}} /*namespace libbitcoin::network*/

#endif //LIBBITCOIN_NETWORK_VERSION_HPP_
