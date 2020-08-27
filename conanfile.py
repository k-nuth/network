# Copyright (c) 2016-2020 Knuth Project developers.
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

import os
from conans import CMake
from kthbuild import option_on_off, march_conan_manip, pass_march_to_compiler
from kthbuild import KnuthConanFile

class KnuthNetworkConan(KnuthConanFile):
    def recipe_dir(self):
        return os.path.dirname(os.path.abspath(__file__))

    name = "network"
    license = "http://www.boost.org/users/license.html"
    url = "https://github.com/k-nuth/network"
    description = "Crypto P2P Network Library"
    settings = "os", "compiler", "build_type", "arch"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "tests": [True, False],
        "currency": ['BCH', 'BTC', 'LTC'],
        "microarchitecture": "ANY",
        "fix_march": [True, False],
        "march_id": "ANY",

        "verbose": [True, False],
        "cxxflags": "ANY",
        "cflags": "ANY",
        "glibcxx_supports_cxx11_abi": "ANY",
        "cmake_export_compile_commands": [True, False],
        "log": ["boost", "spdlog", "binlog"]
    }

    default_options = {
        "shared": False,
        "fPIC": True,
        "tests": False,
        "currency": "BCH",
        "microarchitecture": "_DUMMY_", 
        "fix_march": False,
        "march_id": "_DUMMY_",
        "verbose": False,
        "cxxflags": "_DUMMY_",
        "cflags": "_DUMMY_",
        "glibcxx_supports_cxx11_abi": "_DUMMY_",
        "cxxflags": "_DUMMY_",
        "cflags": "_DUMMY_",
        "glibcxx_supports_cxx11_abi": "_DUMMY_",
        "cmake_export_compile_commands": False,
        "log": "boost",
    }

    generators = "cmake"
    exports = "conan_*", "ci_utils/*"
    exports_sources = "src/*", "CMakeLists.txt", "cmake/*", "kth-networkConfig.cmake.in", "knuthbuildinfo.cmake", "include/*", "test/*"
    package_files = "build/lkth-network.a"
    build_policy = "missing"


    def requirements(self):
        self.requires("domain/0.X@%s/%s" % (self.user, self.channel))

        if self.options.tests:
            self.requires("catch2/2.13.0@")

    def config_options(self):
        KnuthConanFile.config_options(self)

    def configure(self):
        KnuthConanFile.configure(self)

        #TODO(fernando): move to kthbuild
        self.options["*"].log = self.options.log
        self.output.info("Compiling with log: %s" % (self.options.log,))


    def package_id(self):
        KnuthConanFile.package_id(self)

    def build(self):
        cmake = self.cmake_basis()

        #TODO(fernando): move to kthbuild
        cmake.definitions["LOG_LIBRARY"] = self.options.log

        cmake.configure(source_dir=self.source_folder)
        if not self.options.cmake_export_compile_commands:
            cmake.build()
            if self.options.tests:
                cmake.test()

    def imports(self):
        self.copy("*.h", "", "include")

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.hpp", dst="include", src="include")
        self.copy("*.ipp", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.libs = ["kth-network"]

