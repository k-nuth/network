// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_NETWORK_SESSION_MANUAL_HPP
#define KTH_NETWORK_SESSION_MANUAL_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/channel.hpp>
#include <bitcoin/network/connector.hpp>
#include <bitcoin/network/define.hpp>
#include <bitcoin/network/sessions/session.hpp>
#include <bitcoin/network/settings.hpp>

namespace kth {
namespace network {

class p2p;

/// Manual connections session, thread safe.
class BCT_API session_manual
  : public session, track<session_manual>
{
public:
    typedef std::shared_ptr<session_manual> ptr;
    typedef std::function<void(const code&, channel::ptr)> channel_handler;

    /// Construct an instance.
    session_manual(p2p& network, bool notify_on_connect);

    /// Start the manual session.
    void start(result_handler handler) override;

    /// Maintain connection to a node.
    virtual void connect(const std::string& hostname, uint16_t port);

    /// Maintain connection to a node with callback on first connect.
    virtual void connect(const std::string& hostname, uint16_t port,
        channel_handler handler);

protected:
    /// Override to attach specialized protocols upon channel start.
    virtual void attach_protocols(channel::ptr channel);

private:
    void start_connect(const code& ec, const std::string& hostname,
        uint16_t port, uint32_t attempts, channel_handler handler);

    void handle_started(const code& ec, result_handler handler);
    void handle_connect(const code& ec, channel::ptr channel,
        const std::string& hostname, uint16_t port, uint32_t remaining,
        connector::ptr connector, channel_handler handler);

    void handle_channel_start(const code& ec, const std::string& hostname,
        uint16_t port, channel::ptr channel, channel_handler handler);
    void handle_channel_stop(const code& ec, const std::string& hostname,
        uint16_t port);
};

} // namespace network
} // namespace kth

#endif
