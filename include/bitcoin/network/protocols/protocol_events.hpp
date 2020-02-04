// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_NETWORK_PROTOCOL_EVENTS_HPP
#define KTH_NETWORK_PROTOCOL_EVENTS_HPP

#include <string>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/channel.hpp>
#include <bitcoin/network/define.hpp>
#include <bitcoin/network/protocols/protocol.hpp>

namespace libbitcoin {
namespace network {

class p2p;

/**
 * Base class for stateful protocol implementation, thread and lock safe.
 */
class BCT_API protocol_events
  : public protocol
{
protected:

    /**
     * Construct a protocol instance.
     * @param[in]  network   The network interface.
     * @param[in]  channel   The channel on which to start the protocol.
     * @param[in]  name      The instance name for logging purposes.
     */
    protocol_events(p2p& network, channel::ptr channel,
        const std::string& name);

    /**
     * Start the protocol with no event handler.
     */
    virtual void start();

    /**
     * Start the protocol.
     * The event handler may be invoked one or more times.
     * @param[in]  handler  The handler to call at each completion event.
     */
    virtual void start(event_handler handler);

    /**
     * Invoke the event handler.
     * @param[in]  ec  The error code of the preceding operation.
     */
    virtual void set_event(const code& ec);

    /**
     * Determine if the event handler has been cleared.
     */
    virtual bool stopped() const;

    /**
     * Determine if the code is a stop code or the handler has been cleared.
     */
    virtual bool stopped(const code& ec) const;

private:
    void handle_started(completion_handler handler);
    void handle_stopped(const code& ec);
    void do_set_event(const code& ec);

    bc::atomic<event_handler> handler_;
};

} // namespace network
} // namespace kth

#endif
