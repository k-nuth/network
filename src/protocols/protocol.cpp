// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/network/protocols/protocol.hpp>

#include <cstdint>
#include <string>
#include <kth/domain.hpp>
#include <kth/network/channel.hpp>
#include <kth/network/p2p.hpp>

namespace kth {
namespace network {

#define NAME "protocol"

protocol::protocol(p2p& network, channel::ptr channel, const std::string& name)
  : pool_(network.thread_pool()),
    dispatch_(network.thread_pool(), NAME),
    channel_(channel),
    name_(name)
{
}

config::authority protocol::authority() const
{
    return channel_->authority();
}

const std::string& protocol::name() const
{
    return name_;
}

uint64_t protocol::nonce() const
{
    return channel_->nonce();
}

version_const_ptr protocol::peer_version() const
{
    return channel_->peer_version();
}

void protocol::set_peer_version(version_const_ptr value)
{
    channel_->set_peer_version(value);
}

uint32_t protocol::negotiated_version() const
{
    return channel_->negotiated_version();
}

void protocol::set_negotiated_version(uint32_t value)
{
    channel_->set_negotiated_version(value);
}

threadpool& protocol::pool()
{
    return pool_;
}

// Stop the channel.
void protocol::stop(const code& ec)
{
    channel_->stop(ec);
}

// protected
void protocol::handle_send(const code& ec, const std::string& command)
{
    // Send and receive failures are logged by the proxy.
    // This provides a convenient location for override if desired.
}

} // namespace network
} // namespace kth
