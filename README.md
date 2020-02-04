# Bitprim Network <a target="_blank" href="https://gitter.im/bitprim/Lobby">![Gitter Chat][badge.Gitter]</a>

*Bitcoin P2P Network Library*

| **master(linux/osx)** | **conan-build-win(linux/osx)**   | **master(windows)**   | **conan-build-win(windows)** |
|:------:|:-:|:-:|:-:|
| [![Build Status](https://travis-ci.org/k-nuth/network.svg)](https://travis-ci.org/k-nuth/network)       | [![Build StatusB](https://travis-ci.org/k-nuth/network.svg?branch=conan-build-win)](https://travis-ci.org/k-nuth/network?branch=conan-build-win)  | [![Appveyor Status](https://ci.appveyor.com/api/projects/status/github/k-nuth/network?svg=true)](https://ci.appveyor.com/project/k-nuth/network)  | [![Appveyor StatusB](https://ci.appveyor.com/api/projects/status/github/k-nuth/network?branch=conan-build-win&svg=true)](https://ci.appveyor.com/project/k-nuth/network?branch=conan-build-win)  |

Make sure you have installed [kth-domain](https://github.com/k-nuth/core) beforehand according to its build instructions. 

```
$ git clone https://github.com/k-nuth/network.git
$ cd kth-network
$ mkdir build
$ cd build
$ cmake .. -DWITH_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-std=c++11" 
$ make -j2
$ sudo make install
```

kth-network is now installed in `/usr/local/`.

**About Bitprim Network**

Bitprim Network is a partial implementation of the Bitcoin P2P network protocol. Excluded are all protocols that require access to a blockchain. The [bitprim-node](https://github.com/k-nuth/node) library extends the P2P networking capability and incorporates [kth-blockchain](https://github.com/k-nuth/blockchain) in order to implement a full node. The [bitprim-explorer](https://github.com/k-nuth/explorer) library uses the P2P networking capability to post transactions to the P2P network.

[badge.Gitter]: https://img.shields.io/badge/gitter-join%20chat-blue.svg
