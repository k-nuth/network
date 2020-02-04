// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_NETWORK_PROTOCOL_SEED_31402_HPP
#define KTH_NETWORK_PROTOCOL_SEED_31402_HPP

#include <memory>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/channel.hpp>
#include <bitcoin/network/define.hpp>
#include <bitcoin/network/protocols/protocol_timer.hpp>

namespace libbitcoin {
namespace network {

class p2p;

/**
 * Seeding protocol.
 * Attach this to a channel immediately following seed handshake completion.
 */
class BCT_API protocol_seed_31402
  : public protocol_timer, track<protocol_seed_31402>
{
public:
    typedef std::shared_ptr<protocol_seed_31402> ptr;

    /**
     * Construct a seed protocol instance.
     * @param[in]  network   The network interface.
     * @param[in]  channel   The channel on which to start the protocol.
     */
    protocol_seed_31402(p2p& network, channel::ptr channel);

    /**
     * Start the protocol.
     * @param[in]  handler   Invoked upon stop or complete.
     */
    virtual void start(event_handler handler);

protected:
    virtual void send_own_address(const settings& settings);

    virtual void handle_send_address(const code& ec);
    virtual void handle_send_get_address(const code& ec);
    virtual void handle_store_addresses(const code& ec);
    virtual void handle_seeding_complete(const code& ec,
        event_handler handler);

    virtual bool handle_receive_address(const code& ec,
        address_const_ptr address);
    ////virtual bool handle_receive_get_address(const code& ec,
    ////    get_address_const_ptr message);

    p2p& network_;
    const config::authority self_;
};

} // namespace network
} // namespace kth

#endif
