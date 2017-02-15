cmake_minimum_required(VERSION 3.4)

# bitprim-network
#==============================================================================
project(bitprim-network
        VERSION 3.0.0
        LANGUAGES CXX C)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Check for baseline language coverage in the compiler for the C++11 standard.
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Process options.
#==============================================================================
# Implement --with-tests and declare WITH_TESTS.
#------------------------------------------------------------------------------
option(WITH_TESTS "Compile with unit tests." ON)

# Inherit --enable-shared and define BOOST_TEST_DYN_LINK.
#------------------------------------------------------------------------------
option(ENABLE_SHARED "" OFF)

# Implement --with-litecoin.
#------------------------------------------------------------------------------
option(WITH_LITECOIN "Compile with Litecoin support." OFF)
if (WITH_LITECOIN)
  add_definitions(-DLITECOIN)
endif()

# Check dependencies.
#==============================================================================
# Require Boost of at least version 1.56.0 and output ${boost_CPPFLAGS/LDFLAGS}.
#------------------------------------------------------------------------------
if (NOT ENABLE_SHARED)
  set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(
        Boost 1.54.0 REQUIRED
        COMPONENTS unit_test_framework)

# Require bitprim-core of at least version 3.0.0 and output ${bitprim_core_CPPFLAGS/LIBS/PKG}.
#------------------------------------------------------------------------------
if (NOT TARGET bitprim-core)
  find_package(bitprim-core 3.0.0 REQUIRED)
endif()

# Set flags.
#==============================================================================
include(CheckCXXCompilerFlag)

function(_add_c_compile_flag _Flag _Var)
  check_cxx_compiler_flag(${_Flag} ${_Var})
  if (${_Var})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_Flag}" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_Flag}" )
  endif()
endfunction()

function(_add_cxx_compile_flag _Flag _Var)
  check_cxx_compiler_flag(${_Flag} ${_Var})
  if (${_Var})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_Flag}" )
  endif()
endfunction()

function(_add_link_flag _Flag _Var)
  check_cxx_compiler_flag(${_Flag} ${_Var})
  if (${_Var})
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${_Flag}" )
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${_Flag}" )
  endif()
endfunction()

# Warn on all stuff.
#------------------------------------------------------------------------------
if (NOT MSVC)
  _add_c_compile_flag(-Wall _has_all_warning_flag)
else()
  _add_c_compile_flag(-W4 _has_all_warning_flag)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
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
if (MSVC)
  add_definitions(-D_WIN32_WINNT=0x0600)
endif()

# Build
#==============================================================================
function(_group_sources target sources_dir)
  file(GLOB_RECURSE _headers
          ${sources_dir}/include/*.h ${sources_dir}/include/*.hpp)
  target_sources(${target} PRIVATE ${_headers})

  get_target_property(sources ${target} SOURCES)
  foreach (source ${sources})
    get_filename_component(group ${source} ABSOLUTE)
    get_filename_component(group ${group} DIRECTORY)
    file(RELATIVE_PATH group "${sources_dir}" "${group}")
    if (group)
      if (MSVC)
        string(REPLACE "/" "\\" group "${group}")
      endif()
      source_group("${group}" FILES "${source}")
    endif()
  endforeach()

  set_target_properties(${target} PROPERTIES FOLDER "network")
endfunction()

# src/bitprim-network.la => ${libdir}
#------------------------------------------------------------------------------
set(MODE STATIC)
if (ENABLE_SHARED)
  set(MODE SHARED)
endif()

#add_library(bitprim-network ${MODE}
#  src/acceptor.cpp
#  src/channel.cpp
#  src/connections.cpp
#  src/connector.cpp
#  src/const_buffer.cpp
#  src/hosts.cpp
#  src/locked_socket.cpp
#  src/logging.cpp
#  src/message_subscriber.cpp
#  src/p2p.cpp
#  src/pending_channels.cpp
#  src/pending_sockets.cpp
#  src/proxy.cpp
#  src/settings.cpp
#  src/socket.cpp
#  src/protocols/protocol.cpp
#  src/protocols/protocol_address_31402.cpp
#  src/protocols/protocol_events.cpp
#  src/protocols/protocol_ping_31402.cpp
#  src/protocols/protocol_ping_60001.cpp
#  src/protocols/protocol_seed_31402.cpp
#  src/protocols/protocol_timer.cpp
#  src/protocols/protocol_version_31402.cpp
#  src/protocols/protocol_version_70002.cpp
#  src/sessions/session.cpp
#  src/sessions/session_batch.cpp
#  src/sessions/session_inbound.cpp
#  src/sessions/session_manual.cpp
#  src/sessions/session_outbound.cpp
#  src/sessions/session_seed.cpp)

add_library(bitprim-network ${MODE}
        src/channel.cpp
        src/collections/connections.cpp
        src/collections/hosts.cpp
        src/collections/pending_channels.cpp
        src/collections/pending_sockets.cpp
        src/p2p.cpp
        src/protocols/protocol.cpp
        src/protocols/protocol_address_31402.cpp
        src/protocols/protocol_events.cpp
        src/protocols/protocol_ping_31402.cpp
        src/protocols/protocol_ping_60001.cpp
        src/protocols/protocol_seed_31402.cpp
        src/protocols/protocol_timer.cpp
        src/protocols/protocol_version_31402.cpp
        src/protocols/protocol_version_70002.cpp
        src/proxy.cpp
        src/sessions/session.cpp
        src/sessions/session_batch.cpp
        src/sessions/session_inbound.cpp
        src/sessions/session_manual.cpp
        src/sessions/session_outbound.cpp
        src/sessions/session_seed.cpp
        src/settings.cpp
        src/utility/acceptor.cpp
        src/utility/connector.cpp
        src/utility/const_buffer.cpp
        src/utility/locked_socket.cpp
        src/utility/message_subscriber.cpp
        src/utility/socket.cpp)

target_include_directories(bitprim-network PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_compile_definitions(bitprim-network PUBLIC -DBCT_STATIC)

target_link_libraries(bitprim-network bitprim-core)

_group_sources(bitprim-network "${CMAKE_CURRENT_LIST_DIR}")

# Tests
#==============================================================================
function(_add_tests target)
  if (ENABLE_SHARED)
    target_compile_definitions(${target} PRIVATE -DBOOST_TEST_DYN_LINK)
  endif()
  target_include_directories(${target} SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
  target_link_libraries(${target} PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  foreach (_test_name ${ARGN})
    add_test(
            NAME test.network.${_test_name}
            COMMAND ${target}
            --run_test=${_test_name}
            --show_progress=no
            --detect_memory_leak=0
            --report_level=no
            --build_info=yes)
  endforeach()
endfunction()

if (WITH_TESTS)
  enable_testing()
endif()

# local: test/bitprim_network_test
#------------------------------------------------------------------------------
if (WITH_TESTS)
  add_executable(bitprim_network_test
          test/main.cpp
          test/p2p.cpp)
  target_link_libraries(bitprim_network_test PUBLIC bitprim-network)
  _group_sources(bitprim_network_test "${CMAKE_CURRENT_LIST_DIR}/test")

  _add_tests(bitprim_network_test
          empty_tests)
endif()

# Install
#==============================================================================
install(TARGETS bitprim-network
        EXPORT bitprim-network
        ARCHIVE DESTINATION lib)

#set(_bitprim_headers
#  # include_bitcoin_HEADERS =
#  bitcoin/network.hpp
#  # include_bitcoin_network_HEADERS =
#  bitcoin/network/acceptor.hpp
#  bitcoin/network/channel.hpp
#  bitcoin/network/connections.hpp
#  bitcoin/network/connector.hpp
#  bitcoin/network/const_buffer.hpp
#  bitcoin/network/define.hpp
#  bitcoin/network/hosts.hpp
#  bitcoin/network/locked_socket.hpp
#  bitcoin/network/logging.hpp
#  bitcoin/network/message_subscriber.hpp
#  bitcoin/network/p2p.hpp
#  bitcoin/network/pending_channels.hpp
#  bitcoin/network/pending_sockets.hpp
#  bitcoin/network/proxy.hpp
#  bitcoin/network/settings.hpp
#  bitcoin/network/socket.hpp
#  bitcoin/network/version.hpp
#  # include_bitcoin_network_protocols_HEADERS =
#  bitcoin/network/protocols/protocol.hpp
#  bitcoin/network/protocols/protocol_address_31402.hpp
#  bitcoin/network/protocols/protocol_events.hpp
#  bitcoin/network/protocols/protocol_ping_31402.hpp
#  bitcoin/network/protocols/protocol_ping_60001.hpp
#  bitcoin/network/protocols/protocol_seed_31402.hpp
#  bitcoin/network/protocols/protocol_timer.hpp
#  bitcoin/network/protocols/protocol_version_31402.hpp
#  bitcoin/network/protocols/protocol_version_70002.hpp
#  # include_bitcoin_network_sessions_HEADERS =
#  bitcoin/network/sessions/session.hpp
#  bitcoin/network/sessions/session_batch.hpp
#  bitcoin/network/sessions/session_inbound.hpp
#  bitcoin/network/sessions/session_manual.hpp
#  bitcoin/network/sessions/session_outbound.hpp
#  bitcoin/network/sessions/session_seed.hpp)

set(_bitprim_headers
        bitcoin/network/channel.hpp
        bitcoin/network/collections/connections.hpp
        bitcoin/network/collections/hosts.hpp
        bitcoin/network/collections/pending_channels.hpp
        bitcoin/network/collections/pending_sockets.hpp
        bitcoin/network/define.hpp
        bitcoin/network/p2p.hpp
        bitcoin/network/protocols/protocol.hpp
        bitcoin/network/protocols/protocol_address_31402.hpp
        bitcoin/network/protocols/protocol_events.hpp
        bitcoin/network/protocols/protocol_ping_31402.hpp
        bitcoin/network/protocols/protocol_ping_60001.hpp
        bitcoin/network/protocols/protocol_seed_31402.hpp
        bitcoin/network/protocols/protocol_timer.hpp
        bitcoin/network/protocols/protocol_version_31402.hpp
        bitcoin/network/protocols/protocol_version_70002.hpp
        bitcoin/network/proxy.hpp
        bitcoin/network/sessions/session.hpp
        bitcoin/network/sessions/session_batch.hpp
        bitcoin/network/sessions/session_inbound.hpp
        bitcoin/network/sessions/session_manual.hpp
        bitcoin/network/sessions/session_outbound.hpp
        bitcoin/network/sessions/session_seed.hpp
        bitcoin/network/settings.hpp
        bitcoin/network/utility/acceptor.hpp
        bitcoin/network/utility/connector.hpp
        bitcoin/network/utility/const_buffer.hpp
        bitcoin/network/utility/locked_socket.hpp
        bitcoin/network/utility/message_subscriber.hpp
        bitcoin/network/utility/socket.hpp
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
