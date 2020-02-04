// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin/network/user_agent.hpp>

namespace libbitcoin {
namespace network {

std::string get_user_agent() {
    //return "/kth:" KTH_VERSION "/";
    return "/kth-dummy:0.0.0/";
}

} // namespace network
} // namespace kth