#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "dcmtk" for configuration "Release"
set_property(TARGET dcmtk APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(dcmtk PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/dcmtk.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "iphlpapi;ws2_32;netapi32;wsock32"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/dcmtk.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS dcmtk )
list(APPEND _IMPORT_CHECK_FILES_FOR_dcmtk "${_IMPORT_PREFIX}/lib/dcmtk.lib" "${_IMPORT_PREFIX}/bin/dcmtk.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
