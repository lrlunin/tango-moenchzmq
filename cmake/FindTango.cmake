#[=======================================================================[.rst:
FindTango
---------

Find Tango library

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Tango::Tango``
  The Tango library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

    ``Tango_FOUND``
    True if the system has the Tango library.

Cache Variables
^^^^^^^^^^^^^^^

The following cache vairables can be used to control the behaviour of this find
module.

    ``Tango_USE_PKG_CONFIG``
    Set to OFF to disable using pkg-config to find Tango.
    ``Tango_FORCE_STATIC``
    Force Tango to use static libraries

The following cache variables may also be set:

    ``Tango_INCLUDE_DIR``
    The directory containing ``tango/tango.h``.
    ``Tango_LIBRARY``
    The path to the Tango library.

#]=======================================================================]

function(_tango_find_version)
    if (NOT Tango_INCLUDE_DIR)
        set(Tango_VERSION Tango_VERSION-NOTFOUND PARENT_SCOPE)
        return()
    endif()

# File containing version information.  We should find it under tango for 9.3.x
# releases and under tango/common for post 9.4.0 releases.
    find_file(tango_const_include_file
        NAMES tango_const.h
        PATHS
            ${Tango_INCLUDE_DIR}/tango
            ${Tango_INCLUDE_DIR}/tango/common
        NO_DEFAULT_PATH
    )

    if (NOT tango_const_include_file)
        message(WARNING "Could not fine tango_const.h under ${Tango_INCLUDE_DIR}")
        set(Tango_VERSION Tango_VERSION-NOTFOUND PARENT_SCOPE)
        return()
    endif()

    file(STRINGS ${tango_const_include_file} version_info
        REGEX "^#define[ \t]+TANGO_VERSION_(MAJOR|MINOR|PATCH).*")
    unset(tango_const_include_file CACHE)

    list(LENGTH version_info version_info_length)

    if (NOT version_info_length EQUAL 3)
        message(WARNING "Could not find version information in ${tango_const_include_file}")
        set(Tango_VERSION Tango_VERSION-NOTFOUND PARENT_SCOPE)
        return()
    endif()

    list(GET version_info 0 version_major_info)
    list(GET version_info 1 version_minor_info)
    list(GET version_info 2 version_patch_info)

    string(REGEX REPLACE "^#define[ \t]+TANGO_VERSION_MAJOR[ \t]+([0-9]+)$" "\\1" version_major ${version_major_info})
    string(REGEX REPLACE "^#define[ \t]+TANGO_VERSION_MINOR[ \t]+([0-9]+)$" "\\1" version_minor ${version_minor_info})
    string(REGEX REPLACE "^#define[ \t]+TANGO_VERSION_PATCH[ \t]+([0-9]+)$" "\\1" version_patch ${version_patch_info})

    set(Tango_VERSION ${version_major}.${version_minor}.${version_patch} CACHE INTERNAL "Tango Version")
    set(Tango_VERSION_MAJOR ${version_major} PARENT_SCOPE)
    set(Tango_VERSION_MINOR ${version_minor} PARENT_SCOPE)
    set(Tango_VERSION_PATCH ${version_patch} PARENT_SCOPE)
endfunction()

if (WIN32)
    set(_tango_default_use_pkg_config OFF)
else()
    set(_tango_default_use_pkg_config ON)
endif()
option(Tango_USE_PKG_CONFIG "Use pkg-config to find Tango" ${_tango_default_use_pkg_config})
unset(_tango_default_use_pkg_config)

option(Tango_FORCE_STATIC "Statically link Tango" OFF)
if(Tango_FORCE_STATIC AND Tango_USE_PKG_CONFIG)
    message(STATUS "Cannot set -DTango_FORCE_STATIC and -DTango_USE_PKG_CONFIG at the same time.  Forcing -DTango_USE_PKG_CONFIG to OFF")
    set(Tango_USE_PKG_CONFIG OFF)
endif()

if (NOT Tango_USE_PKG_CONFIG OR Tango_FIND_QUIETLY)
    set(_tango_pkg_config_quiet QUIET)
endif()
find_package(PkgConfig ${_tango_pkg_config_quiet})
unset(_tango_pkg_config_quiet)
if (PKG_CONFIG_FOUND)
    if (Tango_FIND_REQUIRED AND NOT Tango_FIND_QUIETLY AND Tango_USE_PKG_CONFIG)
        pkg_search_module(_Tango_PKG tango IMPORTED_TARGET)
    else()
        pkg_search_module(_Tango_PKG tango QUIET IMPORTED_TARGET)
    endif()
endif()

# This will not find the header file for the (Windows?) 9.3.5 release, however,
# I don't think this device server will work with the 9.3.5 release.
find_path(Tango_INCLUDE_DIR
    NAMES tango/tango.h
    PATHS "" ${_Tango_PKG_INCLUDE_DIRS}
)

_tango_find_version()

if (_Tango_PKG_FOUND AND Tango_USE_PKG_CONFIG)
    if (NOT TARGET Tango::Tango)
        add_library(Tango::Tango ALIAS PkgConfig::_Tango_PKG)
    endif()
    if (NOT Tango_FIND_QUIETLY)
        message(STATUS "Tango found via pkg-config")
    endif()
    set(Tango_FOUND TRUE)
    return()
endif()

if (NOT _Tango_PKG_FOUND AND Tango_USE_PKG_CONFIG AND NOT Tango_FIND_QUIETLY)
    message(STATUS "Tango not found via pkg-config, falling back to cmake find")
endif()

if (WIN32)
    set(_tango_release_names tango)
    set(_tango_debug_names tangod)
    set(_tango_static_release_names libtango)
    set(_tango_static_debug_names libtangod)
else()
    set(_tango_release_names tango)
    set(_tango_debug_names tango)
    set(_tango_static_release_names tango)
    set(_tango_static_debug_names tango)
endif()

find_library(Tango_LIBRARY_RELEASE
    NAMES ${_tango_release_names}
    PATHS "" ${_Tango_PKG_LIBRARY_DIRS}
)

find_library(Tango_LIBRARY_RELEASE
    NAMES ${_tango_static_release_names}
    PATHS "" ${_Tango_PKG_LIBRARY_DIRS}
)

find_library(Tango_LIBRARY_DEBUG
    NAMES ${_tango_debug_names}
    PATHS "" ${_Tango_PKG_LIBRARY_DIRS}
)

find_library(Tango_LIBRARY_DEBUG
    NAMES ${_tango_static_debug_names}
    PATHS "" ${_Tango_PKG_LIBRARY_DIRS}
)

find_library(Tango_static_LIBRARY_RELEASE
    NAMES ${_tango_static_release_names}
    PATHS "" ${_Tango_PKG_LIBRARY_DIRS}
)

find_library(Tango_static_LIBRARY_DEBUG
    NAMES ${_tango_static_debug_names}
    PATHS "" ${_Tango_PKG_LIBRARY_DIRS}
)

unset(_tango_release_names)
unset(_tango_debug_names)
unset(_tango_static_release_names)
unset(_tango_static_debug_names)

include(SelectLibraryConfigurations)
select_library_configurations(Tango)
select_library_configurations(Tango_static)

if(Tango_LIBRARY STREQUAL Tango_static_LIBRARY)
    set(Tango_IS_STATIC TRUE)
endif()

if (Tango_static_LIBRARY)
    set(Tango_static_FOUND TRUE)
endif()

if(Tango_FIND_QUIETLY)
    set(_tango_quiet QUIET)
endif()

find_package(cppzmq ${_tango_quiet})
find_package(omniORB4 ${_tango_quiet}
    COMPONENTS COS4 Dynamic4)
unset(_tango_quiet)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Tango
    FOUND_VAR Tango_FOUND
    REQUIRED_VARS
        Tango_LIBRARY
        Tango_INCLUDE_DIR
        cppzmq_FOUND
        omniORB4_FOUND
        omniORB4_COS4_FOUND
        omniORB4_Dynamic4_FOUND
    VERSION_VAR Tango_VERSION
)

if (Tango_FOUND)
    mark_as_advanced(Tango_INCLUDE_DIR)
    mark_as_advanced(Tango_LIBRARY)
    mark_as_advanced(Tango_LIBRARY_RELEASE)
    mark_as_advanced(Tango_LIBRARY_DEBUG)
endif()

if (Tango_static_FOUND)
    mark_as_advanced(Tango_static_LIBRARY)
    mark_as_advanced(Tango_static_LIBRARY_RELEASE)
    mark_as_advanced(Tango_static_LIBRARY_DEBUG)
endif()

function(_tango_add_target prefix targetsuffix is_static)
    if (NOT TARGET Tango::Tango)
        add_library(Tango::Tango UNKNOWN IMPORTED)
    endif()

    if (${prefix}_LIBRARY_RELEASE)
        set_property(TARGET Tango::Tango APPEND PROPERTY
            IMPORTED_CONFIGURATIONS RELEASE
        )
        set_target_properties(Tango::Tango PROPERTIES
            IMPORTED_LOCATION_RELEASE "${${prefix}_LIBRARY_RELEASE}"
        )
    endif()
    if (${prefix}_LIBRARY_DEBUG)
        set_property(TARGET Tango::Tango APPEND PROPERTY
            IMPORTED_CONFIGURATIONS DEBUG
        )
        set_target_properties(Tango::Tango PROPERTIES
            IMPORTED_LOCATION_DEBUG "${${prefix}_LIBRARY_DEBUG}"
        )
    endif()

    set(_tango_inc_dirs "${Tango_INCLUDE_DIR}")
    # For the 9.3.6 release the header files include each other without the "tango/" prefix, however,
    # our device server is including "tango/tango.h" so we need both directories in our include dirs.
    if (Tango_VERSION VERSION_LESS 9.4.0)
        list(APPEND _tango_inc_dirs "${Tango_INCLUDE_DIR}/tango")
    endif()

    set(_tango_dependents
        cppzmq::cppzmq${targetsuffix}
        omniORB4::omniORB4${targetsuffix}
        omniORB4::COS4${targetsuffix}
        omniORB4::Dynamic4${targetsuffix})

    if (WIN32)
        if(NOT is_static)
            set(_tango_definitions /DTANGO_HAS_DLL /DLOG4TANGO_HAS_DLL)
        else()
            set(_tango_definitions /D_WINSTATIC)
            list(APPEND _tango_dependents comctl32.lib)
        endif()
    endif()

    set_target_properties(Tango::Tango PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_tango_inc_dirs}"
        INTERFACE_COMPILE_OPTIONS "${_tango_definitions}"
        INTERFACE_LINK_LIBRARIES "${_tango_dependents}"
        )

    unset(_tango_inc_dirs)
    unset(_tango_dependents)
    unset(_tango_defintions)
endfunction()

if (Tango_FORCE_STATIC AND NOT Tango_static_FOUND)
    message(FATAL_ERROR "Could not find static Tango when forcing static")
endif()

if (Tango_FORCE_STATIC)
    _tango_add_target(Tango_static "-static" TRUE)
elseif (Tango_FOUND)
    _tango_add_target(Tango "" "${Tango_IS_STATIC}")
endif()
