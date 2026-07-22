#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "msquic::msquic" for configuration "Release"
set_property(TARGET msquic::msquic APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(msquic::msquic PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libmsquic.so.2.6.0"
  IMPORTED_SONAME_RELEASE "libmsquic.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS msquic::msquic )
list(APPEND _IMPORT_CHECK_FILES_FOR_msquic::msquic "${_IMPORT_PREFIX}/lib/libmsquic.so.2.6.0" )

# Import target "msquic::platform" for configuration "Release"
set_property(TARGET msquic::platform APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(msquic::platform PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libmsquic_platform.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS msquic::platform )
list(APPEND _IMPORT_CHECK_FILES_FOR_msquic::platform "${_IMPORT_PREFIX}/lib/libmsquic_platform.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
