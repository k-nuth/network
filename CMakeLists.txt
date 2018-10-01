#
# Copyright (c) 2017 Bitprim developers (see AUTHORS)
#
# This file is part of Bitprim.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.4)

# bitprim-network
#==============================================================================
project(bitprim-network
        VERSION 0
        LANGUAGES CXX C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Check for baseline language coverage in the compiler for the C++11 standard.
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Process options.
#==============================================================================

# Implement --use-conan
#------------------------------------------------------------------------------
option(USE_CONAN "Use Conan Build Tool." OFF)
option(NO_CONAN_AT_ALL "Conan totally disabled." OFF)

if (NO_CONAN_AT_ALL)
  set(USE_CONAN OFF)
endif()

# Inherit --enable-shared and define BOOST_TEST_DYN_LINK.
#------------------------------------------------------------------------------
option(ENABLE_SHARED "" OFF)

# Implement POSITION_INDEPENDENT_CODE
#------------------------------------------------------------------------------
option(ENABLE_POSITION_INDEPENDENT_CODE "Enable POSITION_INDEPENDENT_CODE property" ON)

# Implement --with-tests and declare WITH_TESTS.
#------------------------------------------------------------------------------
option(WITH_TESTS "Compile with unit tests." ON)

# Implement --with-litecoin.
#------------------------------------------------------------------------------
# option(WITH_LITECOIN "Compile with Litecoin support." OFF)


# Implement --use-domain and declare USE_DOMAIN.
#------------------------------------------------------------------------------
option(USE_DOMAIN "Domain enabled." OFF)
if (USE_DOMAIN)
  message(STATUS "Bitprim: Using Domain enabled")
  add_definitions(-DBITPRIM_USE_DOMAIN)
endif()


set(BITPRIM_PROJECT_VERSION "-" CACHE STRING "Specify the Bitprim Project Version.")
# message(${BITPRIM_PROJECT_VERSION})


set(MICROARCHITECTURE "x86_64" CACHE STRING "Specify the Cryptocurrency (x86_64|...).")
message( STATUS "Bitprim: Compiling for ${MICROARCHITECTURE}")
# add_definitions(-DBITPRIM_MICROARCHITECTURE=${MICROARCHITECTURE})
# add_definitions(-DBITPRIM_MICROARCHITECTURE_STR="${MICROARCHITECTURE}")


set(CURRENCY "BCH" CACHE STRING "Specify the Cryptocurrency (BCH|BTC|LTC).")

if (${CURRENCY} STREQUAL "BCH")
  add_definitions(-DBITPRIM_CURRENCY_BCH)
elseif (${CURRENCY} STREQUAL "BTC")
  add_definitions(-DBITPRIM_CURRENCY_BTC)
elseif (${CURRENCY} STREQUAL "LTC")
  add_definitions(-DBITPRIM_CURRENCY_LTC)
else()
  message(FATAL_ERROR "Invalid Cryptocurrency: ${CURRENCY}")
endif()

message(STATUS "Bitprim: Cryptocurrency: ${CURRENCY}")


# Conan infrastructure
#==============================================================================
if(EXISTS ${CMAKE_SOURCE_DIR}/ci_utils/cmake/bitprimbuildinfo.cmake)
    include(${CMAKE_SOURCE_DIR}/ci_utils/cmake/bitprimbuildinfo.cmake)
else()
    message( STATUS "bitprimbuildinfo doent exists")
endif()



# Check dependencies.
#==============================================================================
#------------------------------------------------------------------------------
if (NO_CONAN_AT_ALL)
  # Require Boost of at least version 1.56.0 and output ${boost_CPPFLAGS/LDFLAGS}.
  #------------------------------------------------------------------------------
  if (NOT ENABLE_SHARED)
    set(Boost_USE_STATIC_LIBS ON)
  endif()

  if (MSVC)
    find_package(
      Boost 1.56.0 REQUIRED
      COMPONENTS atomic chrono date_time filesystem iostreams locale log log_setup program_options regex system thread unit_test_framework
    )
  else()
    find_package(
      Boost 1.56.0 REQUIRED
      COMPONENTS chrono date_time filesystem iostreams locale log program_options regex system thread unit_test_framework
    )
  endif()
endif()

if (NOT USE_CONAN)
    # Require bitprim-core of at least version 0 and output ${bitprim_core_CPPFLAGS/LIBS/PKG}.
    #------------------------------------------------------------------------------
    if (NOT TARGET bitprim-core)
            find_package(bitprim-core 0 REQUIRED)
    endif()
endif()


# Set flags.
#==============================================================================
include(CheckCXXCompilerFlag)

# Including common functions
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/ci_utils/cmake)

include(BitprimTools)


# Warn on all stuff.
#------------------------------------------------------------------------------
# message("CMAKE_CXX_COMPILER_VERSION")
# message(CMAKE_CXX_COMPILER_VERSION)

if (MINGW)
  # if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.9")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wa,-mbig-obj")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
  # endif()
endif()

# Warn on all stuff.
#------------------------------------------------------------------------------
if (NOT MSVC)
  _add_c_compile_flag(-Wall _has_all_warning_flag)
else()
  _add_c_compile_flag(-W4 _has_all_warning_flag)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Warn on extra stuff.
#------------------------------------------------------------------------------
if (NOT MSVC)
  _add_c_compile_flag(-Wextra _has_extra_warning_flag)
endif()

# Be really annoying.
#------------------------------------------------------------------------------
_add_c_compile_flag(-Wpedantic _has_pedantic_warning_flag)
if (_has_pedantic_warning_flag)
  _add_c_compile_flag(-pedantic _has_pedantic_flag)
endif()

# Conform to style.
#------------------------------------------------------------------------------
_add_cxx_compile_flag(-Wno-missing-braces _has_no_missing_braces_warning_flag)

# Conflict in stdlib under clang. Enabled in clang only.
#------------------------------------------------------------------------------
_add_cxx_compile_flag(-Wno-mismatched-tags _has_no_mismatched_tags_warning_flag)

# Clean up boost 1.55 headers. Enabled in gcc only.
#------------------------------------------------------------------------------
_add_c_compile_flag(-Wno-deprecated-declarations _has_no_deprecated_declarations_warning_flag)

# Protect stack.
#------------------------------------------------------------------------------
_add_link_flag(-fstack-protector _has_stack_protector_flag)

# Protect stack comprehensively.
#------------------------------------------------------------------------------
_add_link_flag(-fstack-protector-all _has_stack_protector_all_flag)

# Hide internal functions from external libs. Enabled in gcc only.
#------------------------------------------------------------------------------
_add_cxx_compile_flag(-fvisibility-hidden _has_visibility_hidden_flag)

# Hide inlines from external libs. Enabled in gcc only.
#------------------------------------------------------------------------------
_add_cxx_compile_flag(-fvisibility-inlines-hidden _has_visibility_inlines_hidden_flag)

# Target Windows Vista. Enabled in msvc only.
#------------------------------------------------------------------------------
if (MSVC) # OR MINGW)
  add_definitions(-D_WIN32_WINNT=0x0600)
endif()

# Build
#==============================================================================

# src/bitprim-network.la => ${libdir}
#------------------------------------------------------------------------------
set(MODE STATIC)
if (ENABLE_SHARED)
  set(MODE SHARED)
endif()

add_library(bitprim-network ${MODE}
        src/protocols/protocol.cpp
        src/protocols/protocol_address_31402.cpp
        src/protocols/protocol_events.cpp
        src/protocols/protocol_ping_31402.cpp
        src/protocols/protocol_ping_60001.cpp
        src/protocols/protocol_reject_70002.cpp
        src/protocols/protocol_seed_31402.cpp
        src/protocols/protocol_timer.cpp
        src/protocols/protocol_version_31402.cpp
        src/protocols/protocol_version_70002.cpp

        src/sessions/session.cpp
        src/sessions/session_batch.cpp
        src/sessions/session_inbound.cpp
        src/sessions/session_manual.cpp
        src/sessions/session_outbound.cpp
        src/sessions/session_seed.cpp

        src/acceptor.cpp
        src/channel.cpp
        src/connector.cpp
        src/hosts.cpp
        src/message_subscriber.cpp
        src/p2p.cpp
        src/proxy.cpp
        src/settings.cpp
        src/version.cpp
)


if (ENABLE_POSITION_INDEPENDENT_CODE)
  set_property(TARGET bitprim-network PROPERTY POSITION_INDEPENDENT_CODE ON)
endif(ENABLE_POSITION_INDEPENDENT_CODE)

target_include_directories(bitprim-network PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)


target_compile_definitions(bitprim-network PUBLIC -DBITPRIM_PROJECT_VERSION="\\"${BITPRIM_PROJECT_VERSION}\\"") #TODO(fernando): manage with Conan????

if (NOT ENABLE_SHARED)
    target_compile_definitions(bitprim-network PUBLIC -DBCT_STATIC -DBC_STATIC)
endif()

if (NOT USE_CONAN)
  target_link_libraries(bitprim-network PUBLIC bitprim-core)
endif()

if (NOT NO_CONAN_AT_ALL)
  target_link_libraries(bitprim-network PUBLIC ${CONAN_LIBS})
endif()

# if (NOT NO_CONAN_AT_ALL)
#   target_link_libraries(bitprim-network PUBLIC ${CONAN_LIBS})
# else()
#   target_link_libraries(bitprim-network bitprim-core)
# endif()

if (USE_CONAN)
  if (MINGW)
    target_link_libraries(bitprim-network PUBLIC ws2_32 wsock32) #TODO(fernando): manage with Conan
  endif()
endif()


_group_sources(bitprim-network "${CMAKE_CURRENT_LIST_DIR}")

# Tests
#==============================================================================
if (WITH_TESTS)
  enable_testing()
endif()

# local: test/bitprim_network_test
#------------------------------------------------------------------------------
if (WITH_TESTS)
    add_executable(bitprim_network_test
          test/main.cpp
          test/p2p.cpp
          test/user_agent_dummy.cpp)

    target_link_libraries(bitprim_network_test PUBLIC bitprim-network)

    _group_sources(bitprim_network_test "${CMAKE_CURRENT_LIST_DIR}/test")

    _add_tests(bitprim_network_test 
      empty_tests 
      # p2p_tests
    )
endif()







# Install
#==============================================================================
install(TARGETS bitprim-network
        EXPORT bitprim-network
        ARCHIVE DESTINATION lib)

set(_bitprim_headers
        bitcoin/network/protocols/protocol.hpp
        bitcoin/network/protocols/protocol_address_31402.hpp
        bitcoin/network/protocols/protocol_events.hpp
        bitcoin/network/protocols/protocol_ping_31402.hpp
        bitcoin/network/protocols/protocol_ping_60001.hpp
        bitcoin/network/protocols/protocol_reject_70002.hpp
        bitcoin/network/protocols/protocol_seed_31402.hpp
        bitcoin/network/protocols/protocol_timer.hpp
        bitcoin/network/protocols/protocol_version_31402.hpp
        bitcoin/network/protocols/protocol_version_70002.hpp

        bitcoin/network/sessions/session.hpp
        bitcoin/network/sessions/session_batch.hpp
        bitcoin/network/sessions/session_inbound.hpp
        bitcoin/network/sessions/session_manual.hpp
        bitcoin/network/sessions/session_outbound.hpp
        bitcoin/network/sessions/session_seed.hpp

        bitcoin/network/acceptor.hpp
        bitcoin/network/channel.hpp
        bitcoin/network/connector.hpp
        bitcoin/network/define.hpp
        bitcoin/network/hosts.hpp
        bitcoin/network/message_subscriber.hpp
        bitcoin/network/p2p.hpp
        bitcoin/network/proxy.hpp
        bitcoin/network/settings.hpp
        bitcoin/network/version.hpp
        bitcoin/network.hpp)

foreach (_header ${_bitprim_headers})
  get_filename_component(_directory "${_header}" DIRECTORY)
  install(FILES "include/${_header}" DESTINATION "include/${_directory}")
endforeach()

# Export
#==============================================================================
include(CMakePackageConfigHelpers)

export(EXPORT bitprim-network
        FILE "${CMAKE_CURRENT_BINARY_DIR}/bitprim-networkTargets.cmake")
export(PACKAGE bitprim-network)

if (UNIX)
  set(_config_package_location "lib/cmake")
elseif (WIN32)
  set(_config_package_location "cmake")
endif()

install(EXPORT bitprim-network
        FILE bitprim-networkTargets.cmake
        DESTINATION ${_config_package_location})

configure_file(bitprim-networkConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/bitprim-networkConfig.cmake"
        @ONLY)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/bitprim-networkConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/bitprim-networkConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/bitprim-networkConfigVersion.cmake"
        DESTINATION ${_config_package_location})
