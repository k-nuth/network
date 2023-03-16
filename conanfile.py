# Copyright (c) 2016-2023 Knuth Project developers.
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

import os
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
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

        "march_id": "ANY",
        "march_strategy": ["download_if_possible", "optimized", "download_or_fail"],

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

        "march_id": "_DUMMY_",
        "march_strategy": "download_if_possible",

        "verbose": False,
        "cxxflags": "_DUMMY_",
        "cflags": "_DUMMY_",
        "glibcxx_supports_cxx11_abi": "_DUMMY_",
        "cxxflags": "_DUMMY_",
        "cflags": "_DUMMY_",
        "glibcxx_supports_cxx11_abi": "_DUMMY_",
        "cmake_export_compile_commands": False,
        "log": "spdlog",
    }

    # generators = "cmake"
    exports = "conan_*", "ci_utils/*"
    exports_sources = "src/*", "CMakeLists.txt", "cmake/*", "kth-networkConfig.cmake.in", "knuthbuildinfo.cmake", "include/*", "test/*"
    package_files = "build/lkth-network.a"
    # build_policy = "missing"

    def build_requirements(self):
        if self.options.tests:
            self.test_requires("catch2/3.3.1")

    def requirements(self):
        self.requires("domain/0.X@%s/%s" % (self.user, self.channel))

    def validate(self):
        KnuthConanFile.validate(self)
        if self.info.settings.compiler.cppstd:
            check_min_cppstd(self, "20")

    def config_options(self):
        KnuthConanFile.config_options(self)

    def configure(self):
        KnuthConanFile.configure(self)

        #TODO(fernando): move to kthbuild
        self.options["*"].log = self.options.log
        self.output.info("Compiling with log: %s" % (self.options.log,))

    def package_id(self):
        KnuthConanFile.package_id(self)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = self.cmake_toolchain_basis()
        # tc.variables["CMAKE_VERBOSE_MAKEFILE"] = True
        #TODO(fernando): move to kthbuild
        tc.variables["LOG_LIBRARY"] = self.options.log
        tc.variables["CONAN_DISABLE_CHECK_COMPILER"] = option_on_off(True)
        tc.generate()
        tc = CMakeDeps(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
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

        if self.settings.arch == "armv7":
            self.cpp_info.libs.append("atomic")
