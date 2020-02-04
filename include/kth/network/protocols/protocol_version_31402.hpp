// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_NETWORK_PROTOCOL_VERSION_31402_HPP
#define KTH_NETWORK_PROTOCOL_VERSION_31402_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <kth/domain.hpp>
#include <kth/network/channel.hpp>
#include <kth/network/define.hpp>
#include <kth/network/protocols/protocol_timer.hpp>
#include <kth/network/settings.hpp>

namespace kth {
namespace network {

class p2p;

class BCT_API protocol_version_31402
  : public protocol_timer, track<protocol_version_31402>
{
public:
    typedef std::shared_ptr<protocol_version_31402> ptr;

    /**
     * Construct a version protocol instance using configured minimums.
     * @param[in]  network   The network interface.
     * @param[in]  channel   The channel for the protocol.
     */
    protocol_version_31402(p2p& network, channel::ptr channel);

    /**
     * Construct a version protocol instance.
     * @param[in]  network           The network interface.
     * @param[in]  channel           The channel for the protocol.
     * @param[in]  own_version       This node's maximum version.
     * @param[in]  own_services      This node's advertised services.
     * @param[in]  invalid_services  The disallowed peers services.
     * @param[in]  minimum_version   This required minimum version.
     * @param[in]  minimum_services  This required minimum services.
     */
    protocol_version_31402(p2p& network, channel::ptr channel,
        uint32_t own_version, uint64_t own_services, uint64_t invalid_services,
        uint32_t minimum_version, uint64_t minimum_services);

    /**
     * Start the protocol.
     * @param[in]  handler  Invoked upon stop or receipt of version and verack.
     */
    virtual void start(event_handler handler);

protected:
    virtual message::version version_factory() const;
    virtual bool sufficient_peer(version_const_ptr message);

    virtual bool handle_receive_version(const code& ec,
        version_const_ptr version);
    virtual bool handle_receive_verack(const code& ec, verack_const_ptr);

    p2p& network_;
    const uint32_t own_version_;
    const uint64_t own_services_;
    const uint64_t invalid_services_;
    const uint32_t minimum_version_;
    const uint64_t minimum_services_;
};

} // namespace network
} // namespace kth

#endif

