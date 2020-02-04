// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_NETWORK_SESSION_BATCH_HPP
#define KTH_NETWORK_SESSION_BATCH_HPP

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/channel.hpp>
#include <bitcoin/network/connector.hpp>
#include <bitcoin/network/define.hpp>
#include <bitcoin/network/sessions/session.hpp>
#include <bitcoin/network/settings.hpp>

namespace kth {
namespace network {

class p2p;

/// Intermediate base class for adding batch connect sequence.
class BCT_API session_batch
  : public session
{
protected:

    /// Construct an instance.
    session_batch(p2p& network, bool notify_on_connect);

    /// Create a channel from the configured number of concurrent attempts.
    virtual void connect(channel_handler handler);

private:
    // Connect sequence
    void new_connect(channel_handler handler);
    void start_connect(const code& ec, const authority& host,
        channel_handler handler);
    void handle_connect(const code& ec, channel::ptr channel,
        connector::ptr connector, channel_handler handler);

    const size_t batch_size_;
};

} // namespace network
} // namespace kth

#endif
