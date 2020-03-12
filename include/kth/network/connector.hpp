// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_NETWORK_CONNECTOR_HPP
#define KTH_NETWORK_CONNECTOR_HPP

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <kth/domain.hpp>
#include <kth/network/channel.hpp>
#include <kth/network/define.hpp>
#include <kth/network/settings.hpp>

namespace kth {
namespace network {

/// Create outbound socket connections.
/// This class is thread safe against stop.
/// This class is not safe for concurrent connection attempts.
class BCT_API connector
  : public enable_shared_from_base<connector>, noncopyable, track<connector>
{
public:
    typedef std::shared_ptr<connector> ptr;
    typedef std::function<void(code const& ec, channel::ptr)> connect_handler;

    /// Construct an instance.
    connector(threadpool& pool, const settings& settings);

    /// Validate connector stopped.
    ~connector();

    /// Try to connect to the endpoint.
    virtual void connect(const config::endpoint& endpoint,
        connect_handler handler);

    /// Try to connect to the authority.
    virtual void connect(const config::authority& authority,
        connect_handler handler);

    /// Try to connect to host:port.
    virtual void connect(const std::string& hostname, uint16_t port,
        connect_handler handler);

    /// Cancel outstanding connection attempt.
    void stop(code const& ec);

private:
    typedef std::shared_ptr<asio::query> query_ptr;

    bool stopped() const;

    void handle_resolve(const boost_code& ec, asio::iterator iterator,
        connect_handler handler);
    void handle_connect(const boost_code& ec, asio::iterator iterator,
        socket::ptr socket, connect_handler handler);
    void handle_timer(code const& ec, socket::ptr socket,
        connect_handler handler);

    // These are thread safe
    std::atomic<bool> stopped_;
    threadpool& pool_;
    const settings& settings_;
    mutable dispatcher dispatch_;

    // These are protected by mutex.
    query_ptr query_;
    deadline::ptr timer_;
    asio::resolver resolver_;
    mutable upgrade_mutex mutex_;
};

} // namespace network
} // namespace kth

#endif
