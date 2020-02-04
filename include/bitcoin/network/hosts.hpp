// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_NETWORK_HOSTS_HPP
#define KTH_NETWORK_HOSTS_HPP

#include <atomic>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/define.hpp>
#include <bitcoin/network/settings.hpp>

namespace libbitcoin {
namespace network {

/// This class is thread safe.
/// The hosts class manages a thread-safe dynamic store of network addresses.
/// The store can be loaded and saved from/to the specified file path.
/// The file is a line-oriented set of config::authority serializations.
/// Duplicate addresses and those with zero-valued ports are disacarded.
class BCT_API hosts
  : noncopyable
{
public:
    typedef std::shared_ptr<hosts> ptr;
    typedef message::network_address address;
    typedef handle0 result_handler;

    /// Construct an instance.
    hosts(const settings& settings);

    /// Load hosts file if found.
    virtual code start();

    // Save hosts to file.
    virtual code stop();

    virtual size_t count() const;
    virtual code fetch(address& out) const;
    virtual code fetch(address::list& out) const;
    virtual code remove(const address& host);
    virtual code store(const address& host);
    virtual void store(const address::list& hosts, result_handler handler);

private:
    typedef boost::circular_buffer<address> list;
    typedef list::iterator iterator;

    iterator find(const address& host);

    const size_t capacity_;

    // These are protected by a mutex.
    list buffer_;
    std::atomic<bool> stopped_;
    mutable upgrade_mutex mutex_;

    // HACK: we use this because the buffer capacity cannot be set to zero.
    const bool disabled_;
    const boost::filesystem::path file_path_;
};

} // namespace network
} // namespace kth

#endif

