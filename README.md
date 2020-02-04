# Knuth Network <a target="_blank" href="http://semver.org">![Version][badge.version]</a> <a target="_blank" href="https://travis-ci.org/k-nuth/network">![Travis status][badge.Travis]</a> <a target="_blank" href="https://ci.appveyor.com/project/k-nuth/network">![Appveyor status][badge.Appveyor]</a> <a target="_blank" href="https://gitter.im/k-nuth/Lobby">![Gitter Chat][badge.Gitter]</a>

> Crypto P2P Network Library

## Installation Requirements

- 64-bit machine.
- [Conan](https://www.conan.io/) package manager, version 1.4.0 or newer. See [Conan Installation](http://docs.conan.io/en/latest/installation.html#install-with-pip-recommended).

## Installation Procedure

The *Knuth* libraries can be installed on Linux, macOS, FreeBSD, Windows and others. These binaries are pre-built for the most usual operating system/compiler combinations and hosted in an online repository. If there are no pre-built binaries for your platform, a build from source will be attempted.

So, for any platform, an installation can be performed in 2 simple steps:

1. Configure the Conan remote:
```
conan remote add kth https://api.bintray.com/conan/k-nuth/kth
```

2. Install the appropriate library:

```
conan install network/0.X@kth/stable 
```

In you want to tune the installation for better performance, please refer to [this](https://kth.github.io/docfx/content/user_guide/installation.html#advanced-installation).


<!-- Links -->
[badge.Appveyor]: https://ci.appveyor.com/api/projects/status/github/k-nuth/network?svg=true&branch=dev
[badge.Gitter]: https://img.shields.io/badge/gitter-join%20chat-blue.svg
[badge.Travis]: https://travis-ci.org/k-nuth/network.svg?branch=master
[badge.version]: https://badge.fury.io/gh/kth%2Fnetwork.svg

[CMake]: http://www.cmake.org
[Doxygen]: http://www.doxygen.org
[eRuby]: http://en.wikipedia.org/wiki/ERuby
[Hana.docs]: http://boostorg.github.io/hana
[Hana.wiki]: https://github.com/boostorg/hana/wiki
[Homebrew formula]: https://github.com/Homebrew/homebrew-network/blob/master/Formula/hana.rb
