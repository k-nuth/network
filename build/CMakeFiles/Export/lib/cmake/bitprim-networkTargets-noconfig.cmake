#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bitprim-network" for configuration ""
set_property(TARGET bitprim-network APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(bitprim-network PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libbitprim-network.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS bitprim-network )
list(APPEND _IMPORT_CHECK_FILES_FOR_bitprim-network "${_IMPORT_PREFIX}/lib/libbitprim-network.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
