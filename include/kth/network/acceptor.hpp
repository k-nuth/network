// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_NETWORK_ACCEPTOR_HPP
#define KTH_NETWORK_ACCEPTOR_HPP

#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <kth/domain.hpp>
#include <kth/network/channel.hpp>
#include <kth/network/define.hpp>
#include <kth/network/settings.hpp>

namespace kth::network {

/// Create inbound socket connections.
/// This class is thread safe against stop.
/// This class is not safe for concurrent listening attempts.
class BCT_API acceptor
  : public enable_shared_from_base<acceptor>, noncopyable, track<acceptor>
{
public:
    typedef std::shared_ptr<acceptor> ptr;
    typedef std::function<void(code const&, channel::ptr)> accept_handler;

    /// Construct an instance.
    acceptor(threadpool& pool, settings const& settings);

    /// Validate acceptor stopped.
    ~acceptor();

    /// Start the listener on the specified port.
    virtual code listen(uint16_t port);

    /// Accept the next connection available, until canceled.
    virtual void accept(accept_handler handler);

    /// Cancel outstanding accept attempt.
    virtual void stop(code const& ec);

private:
    virtual bool stopped() const;

    void handle_accept(const boost_code& ec, socket::ptr socket,
        accept_handler handler);

    // These are thread safe.
    std::atomic<bool> stopped_;
    threadpool& pool_;
    settings const& settings_;
    mutable dispatcher dispatch_;

    // These are protected by mutex.
    asio::acceptor acceptor_;
    mutable shared_mutex mutex_;
};

} // namespace network
} // namespace kth

#endif
