// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_NETWORK_SESSION_OUTBOUND_HPP
#define KTH_NETWORK_SESSION_OUTBOUND_HPP

#include <cstddef>
#include <memory>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/channel.hpp>
#include <bitcoin/network/define.hpp>
#include <bitcoin/network/sessions/session_batch.hpp>
#include <bitcoin/network/settings.hpp>

namespace libbitcoin {
namespace network {

class p2p;

/// Outbound connections session, thread safe.
class BCT_API session_outbound
  : public session_batch, track<session_outbound>
{
public:
    typedef std::shared_ptr<session_outbound> ptr;

    /// Construct an instance.
    session_outbound(p2p& network, bool notify_on_connect);

    /// Start the session.
    void start(result_handler handler) override;

protected:
    /// Overridden to implement pending outbound channels.
    void start_channel(channel::ptr channel,
        result_handler handle_started) override;

    /// Overridden to attach minimum service level for witness support.
    void attach_handshake_protocols(channel::ptr channel,
        result_handler handle_started) override;

    /// Override to attach specialized protocols upon channel start.
    virtual void attach_protocols(channel::ptr channel);

private:
    void new_connection(const code&);

    void handle_started(const code& ec, result_handler handler);
    void handle_connect(const code& ec, channel::ptr channel);

    void do_unpend(const code& ec, channel::ptr channel,
        result_handler handle_started);

    void handle_channel_stop(const code& ec, channel::ptr channel);
    void handle_channel_start(const code& ec, channel::ptr channel);
};

} // namespace network
} // namespace kth

#endif
