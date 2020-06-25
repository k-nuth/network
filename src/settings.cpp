// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/network/settings.hpp>

#include <kth/domain.hpp>
#include <kth/domain/multi_crypto_support.hpp>

namespace kth::network {

using namespace kth::asio;
using namespace kd::message;

// Common default values (no settings context).
settings::settings()
    : threads(0)
    , protocol_maximum(version::level::maximum)
    , protocol_minimum(version::level::minimum)
    , services(version::service::none)
#ifdef KTH_CURRENCY_BCH
    , invalid_services(0)
#else
    , invalid_services(176)
#endif    
    , relay_transactions(true)
    , validate_checksum(false)
    , inbound_connections(0)
    , outbound_connections(8)
    , manual_attempt_limit(0)
    , connect_batch_size(5)
    , connect_timeout_seconds(5)
    , channel_handshake_seconds(30)
    , channel_heartbeat_minutes(5)
    , channel_inactivity_minutes(10)
    , channel_expiration_minutes(60)
    , channel_germination_seconds(30)
    , host_pool_capacity(0)
    , hosts_file("hosts.cache")
    , self(unspecified_network_address)
    // , bitcoin_cash(false)

    // [log]
    , debug_file("debug.log")
    , error_file("error.log")
    , archive_directory("archive")
    , rotation_size(0)
    , minimum_free_space(0)
    , maximum_archive_size(0)
    , maximum_archive_files(0)
    , statistics_server(unspecified_network_address)
    , verbose(false)
    , use_ipv6(true) {}

// Use push_back due to initializer_list bug:
// stackoverflow.com/a/20168627/1172329
settings::settings(infrastructure::config::settings context)
    : settings() {
    // Handle deviations from common defaults.
    switch (context) {
        case infrastructure::config::settings::mainnet: {
// #ifdef LITECOIN
#ifdef KTH_CURRENCY_LTC
            inbound_port = 9333;
            // identifier = 0xdbb6c0fb; 
            identifier = netmagic::ltc_mainnet; 
            seeds.reserve(5);
            seeds.push_back({ "seed-a.litecoin.loshan.co.uk", 9333 });
            seeds.push_back({ "dnsseed.thrasher.io", 9333 });
            seeds.push_back({ "dnsseed.litecointools.com", 9333 });
            seeds.push_back({ "dnsseed.litecoinpool.org", 9333 });
            seeds.push_back({ "dnsseed.koin-project.com", 9333 });        
#else
            // identifier = 3652501241;
            inbound_port = 8333;

#ifdef KTH_CURRENCY_BCH
            identifier = netmagic::bch_mainnet; 
            seeds.reserve(5);
            seeds.push_back({ "seed.bitcoinabc.org", 8333 });
            seeds.push_back({ "seed-bch.bitcoinforks.org", 8333 });
            seeds.push_back({ "btccash-seeder.bitcoinunlimited.info", 8333 });
            seeds.push_back({ "seed.deadalnix.me", 8333 });
            seeds.push_back({ "seed.bchd.cash", 8333 });
#else
            identifier = netmagic::btc_mainnet; 
            seeds.reserve(6);
            seeds.push_back({ "seed.bitcoin.sipa.be", 8333 });
            seeds.push_back({ "dnsseed.bluematt.me", 8333 });
            seeds.push_back({ "dnsseed.bitcoin.dashjr.org", 8333 });
            seeds.push_back({ "seed.bitcoinstats.com", 8333 });
            seeds.push_back({ "seed.bitcoin.jonasschnelli.ch", 8333 });
            seeds.push_back({ "seed.voskuil.org", 8333 });
#endif // KTH_CURRENCY_BCH
#endif //KTH_CURRENCY_LTC
            break;
        }

        // Seeds based on satoshi client v0.14.0 plus voskuil.org.
        case infrastructure::config::settings::testnet: {

// #ifdef LITECOIN
#ifdef KTH_CURRENCY_LTC
            // identifier = 4056470269;
            identifier = netmagic::ltc_testnet;
            inbound_port = 19335;
            seeds.reserve(2);
            seeds.push_back({ "testnet-seed.litecointools.com", 19335 });
            seeds.push_back({ "seed-b.litecoin.loshan.co.uk", 19335 });
#else
            // identifier = 118034699;
            inbound_port = 18333;
#ifdef KTH_CURRENCY_BCH
            identifier = netmagic::bch_testnet;
            seeds.reserve(5);
            seeds.push_back({ "testnet-seed.bitcoinabc.org", 18333 });
            seeds.push_back({ "testnet-seed-bch.bitcoinforks.org", 18333 });
            seeds.push_back({ "testnet-seed.bitcoinunlimited.info", 18333 });
            seeds.push_back({ "testnet-seed.deadalnix.me", 18333 });
            seeds.push_back({ "testnet-seed.bchd.cash", 18333 });
#else
            identifier = netmagic::btc_testnet;
            // Seeds based on satoshi client v0.14.0 plus voskuil.org.
            seeds.reserve(5);
            seeds.push_back({ "testnet-seed.bitcoin.jonasschnelli.ch", 18333 });
            seeds.push_back({ "seed.tbtc.petertodd.org", 18333 });
            seeds.push_back({ "testnet-seed.bluematt.me", 18333 });
            seeds.push_back({ "testnet-seed.bitcoin.schildbach.de", 18333 });
            seeds.push_back({ "testnet-seed.voskuil.org", 18333 });
#endif //KTH_CURRENCY_BCH
#endif //KTH_CURRENCY_LTC
            break;
        }

        case infrastructure::config::settings::regtest: {
            identifier = 3669344250;
            inbound_port = 18444;

            // Regtest is private network only, so there is no seeding.
            break;
        }

        default:
        case config::settings::none: {}
    }
}

duration settings::connect_timeout() const {
    return seconds(connect_timeout_seconds);
}

duration settings::channel_handshake() const {
    return seconds(channel_handshake_seconds);
}

duration settings::channel_heartbeat() const {
    return minutes(channel_heartbeat_minutes);
}

duration settings::channel_inactivity() const {
    return minutes(channel_inactivity_minutes);
}

duration settings::channel_expiration() const {
    return minutes(channel_expiration_minutes);
}

duration settings::channel_germination() const {
    return seconds(channel_germination_seconds);
}

} // namespace kth::network
