<!-- <a target="_blank" href="http://semver.org">![Version][badge.version]</a> -->
<!-- <a target="_blank" href="https://cirrus-ci.com/github/k-nuth/network">![Build Status][badge.Cirrus]</a> -->

# Knuth Network <a target="_blank" href="https://github.com/k-nuth/network/releases">![Github Releases][badge.release]</a> <a target="_blank" href="https://travis-ci.org/k-nuth/network">![Build status][badge.Travis]</a> <a target="_blank" href="https://ci.appveyor.com/projects/k-nuth/network">![Build Status][badge.Appveyor]</a> <a target="_blank" href="https://t.me/knuth_cash">![Telegram][badge.telegram]</a> <a target="_blank" href="https://k-nuth.slack.com/">![Slack][badge.slack]</a>

> Crypto P2P Network Library

Build steps
-----------

The *Knuth* libraries can be installed on Linux, macOS, FreeBSD, Windows and others. These binaries are pre-built for the most usual operating system/compiler combinations and hosted in an online repository. If there are no pre-built binaries for your platform, a build from source will be attempted.

So, for any platform, an installation can be performed in 2 simple steps:

1. Install the Knuth build helper:
```
pip install kthbuild --user --upgrade
```

2. Configure the Conan remote:
```
conan remote add kth https://api.bintray.com/conan/k-nuth/kth
```

3. Install the appropriate library:

```
conan install network/0.X@kth/stable 
```

In you want to tune the installation for better performance, please refer to the [documentation](https://kth.github.io/docfx/content/user_guide/installation.html#advanced-installation).


<!-- Links -->
[badge.Travis]: https://travis-ci.org/k-nuth/network.svg?branch=master
[badge.Appveyor]: https://ci.appveyor.com/api/projects/status/github/k-nuth/network?svg=true&branch=master
[badge.Cirrus]: https://api.cirrus-ci.com/github/k-nuth/network.svg?branch=master
[badge.version]: https://badge.fury.io/gh/k-nuth%2Fnetwork.svg
[badge.release]: https://img.shields.io/github/release/k-nuth/network.svg

[badge.telegram]: https://img.shields.io/badge/telegram-badge-blue.svg?logo=telegram
[badge.slack]: https://img.shields.io/badge/slack-badge-orange.svg?logo=slack

<!-- [badge.Gitter]: https://img.shields.io/badge/gitter-join%20chat-blue.svg -->

