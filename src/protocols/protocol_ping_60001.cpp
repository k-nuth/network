// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin/network/protocols/protocol_ping_60001.hpp>

#include <cstdint>
#include <functional>
#include <string>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/channel.hpp>
#include <bitcoin/network/define.hpp>
#include <bitcoin/network/p2p.hpp>
#include <bitcoin/network/protocols/protocol_ping_31402.hpp>
#include <bitcoin/network/protocols/protocol_timer.hpp>

namespace kth {
namespace network {

#define CLASS protocol_ping_60001

using namespace bc::message;
using namespace std::placeholders;

protocol_ping_60001::protocol_ping_60001(p2p& network, channel::ptr channel)
  : protocol_ping_31402(network, channel),
    pending_(false),
    CONSTRUCT_TRACK(protocol_ping_60001)
{
}

// This is fired by the callback (i.e. base timer and stop handler).
void protocol_ping_60001::send_ping(const code& ec)
{
    if (stopped(ec))
        return;

    if (ec && ec != error::channel_timeout)
    {
        LOG_DEBUG(LOG_NETWORK)
            << "Failure in ping timer for [" << authority() << "] "
            << ec.message();
        stop(ec);
        return;
    }

    if (pending_)
    {
        LOG_DEBUG(LOG_NETWORK)
            << "Ping latency limit exceeded [" << authority() << "]";
        stop(error::channel_timeout);
        return;
    }

    pending_ = true;
    auto const nonce = pseudo_random::next();
    SUBSCRIBE3(pong, handle_receive_pong, _1, _2, nonce);
    SEND2(ping{ nonce }, handle_send_ping, _1, ping::command);
}

void protocol_ping_60001::handle_send_ping(const code& ec, const std::string&)
{
    if (stopped(ec))
        return;

    if (ec)
    {
        LOG_DEBUG(LOG_NETWORK)
            << "Failure sending ping to [" << authority() << "] "
            << ec.message();
        stop(ec);
        return;
    }
}

bool protocol_ping_60001::handle_receive_ping(const code& ec,
    ping_const_ptr message)
{
    if (stopped(ec))
        return false;

    if (ec)
    {
        LOG_DEBUG(LOG_NETWORK)
            << "Failure getting ping from [" << authority() << "] "
            << ec.message();
        stop(ec);
        return false;
    }

    SEND2(pong{ message->nonce() }, handle_send, _1, pong::command);
    return true;
}

bool protocol_ping_60001::handle_receive_pong(const code& ec,
    pong_const_ptr message, uint64_t nonce)
{
    if (stopped(ec))
        return false;

    if (ec)
    {
        LOG_DEBUG(LOG_NETWORK)
            << "Failure getting pong from [" << authority() << "] "
            << ec.message();
        stop(ec);
        return false;
    }

    pending_ = false;

    if (message->nonce() != nonce)
    {
        LOG_WARNING(LOG_NETWORK)
            << "Invalid pong nonce from [" << authority() << "]";
        stop(error::bad_stream);
        return false;
    }

    return false;
}

} // namespace network
} // namespace kth
