// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin/network/protocols/protocol_timer.hpp>

#include <functional>
#include <memory>
#include <string>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/channel.hpp>
#include <bitcoin/network/p2p.hpp>
#include <bitcoin/network/protocols/protocol_events.hpp>

namespace libbitcoin {
namespace network {

#define CLASS protocol_timer
using namespace std::placeholders;

protocol_timer::protocol_timer(p2p& network, channel::ptr channel,
    bool perpetual, const std::string& name)
  : protocol_events(network, channel, name),
    perpetual_(perpetual)
{
}

// Start sequence.
// ----------------------------------------------------------------------------

// protected:
void protocol_timer::start(const asio::duration& timeout,
    event_handler handle_event)
{
    // The deadline timer is thread safe.
    timer_ = std::make_shared<deadline>(pool(), timeout);
    protocol_events::start(BIND2(handle_notify, _1, handle_event));
    reset_timer();
}

void protocol_timer::handle_notify(const code& ec, event_handler handler)
{
    if (ec == error::channel_stopped)
        timer_->stop();

    handler(ec);
}

// Timer.
// ----------------------------------------------------------------------------

// protected:
void protocol_timer::reset_timer()
{
    if (stopped())
        return;

    timer_->start(BIND1(handle_timer, _1));
}

void protocol_timer::handle_timer(const code& ec)
{
    if (stopped())
        return;

    LOG_DEBUG(LOG_NETWORK)
        << "Fired protocol_" << name() << " timer on [" << authority() << "] "
        << ec.message();

    // The handler completes before the timer is reset.
    set_event(error::channel_timeout);

    // A perpetual timer resets itself until the channel is stopped.
    if (perpetual_)
        reset_timer();
}

} // namespace network
} // namespace kth
