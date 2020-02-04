///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2015 libbitcoin-network developers (see COPYING).
//
//        GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
//
///////////////////////////////////////////////////////////////////////////////
#ifndef KTH_NETWORK_VERSION_HPP_
#define KTH_NETWORK_VERSION_HPP_

/**
 * The semantic version of this repository as: [major].[minor].[patch]
 * For interpretation of the versioning scheme see: http://semver.org
 */


#ifdef KTH_PROJECT_VERSION
#define KTH_NETWORK_VERSION KTH_PROJECT_VERSION
#else
#define KTH_NETWORK_VERSION "0.0.0"
#endif

namespace libbitcoin { namespace network {
char const* version();
}} /*namespace libbitcoin::network*/

#endif //KTH_NETWORK_VERSION_HPP_
