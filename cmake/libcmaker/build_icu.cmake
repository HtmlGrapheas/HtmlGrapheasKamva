# ****************************************************************************
#  Project:  HtmlGrapheas
#  Purpose:  HTML text editor library
#  Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#    Copyright (c) 2017-2018 NikitaFeodonit
#
#    This file is part of the HtmlGrapheas project.
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published
#    by the Free Software Foundation, either version 3 of the License,
#    or (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#    See the GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program. If not, see <http://www.gnu.org/licenses/>.
# ****************************************************************************

include(cmr_print_status)

#-----------------------------------------------------------------------
# Build, install and find ICU library
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Set vars to LibCMaker_ICU
#-----------------------------------------------------------------------

set(LIBCMAKER_ICU_SRC_DIR
  "${CMAKE_CURRENT_LIST_DIR}/LibCMaker_ICU"
)
# To use our FindICU.cmake.
list(APPEND CMAKE_MODULE_PATH "${LIBCMAKER_ICU_SRC_DIR}/cmake")

set(ICU_lib_VERSION   "61.1")
set(ICU_DOWNLOAD_DIR  "${EXTERNAL_DOWNLOAD_DIR}")
set(ICU_UNPACKED_DIR  "${EXTERNAL_UNPACKED_DIR}")
set(ICU_BUILD_DIR     "${EXTERNAL_BIN_DIR}/build_icu")

set(COPY_ICU_CMAKE_BUILD_SCRIPTS ON)

# Library specific vars and options.

# Enable cross compiling
set(ICU_CROSS_COMPILING OFF)
if(IOS OR ANDROID OR WINDOWS_STORE)
  set(ICU_CROSS_COMPILING ON)
endif()
# Specify an absolute path to the build directory of an ICU built for the current platform
set(ICU_CROSS_BUILDROOT "")
if(ICU_CROSS_COMPILING)
  if(NOT HOST_TOOLS_BUILD_DIR)
    cmr_print_error(
      "Please set HOST_TOOLS_BUILD_DIR with path to built host tools to cross compilation."
    )
  endif()
  set(ICU_CROSS_BUILDROOT
    "${HOST_TOOLS_BUILD_DIR}/external/build/build_icu_host_tools/icu-${ICU_lib_VERSION}/source"
  )
endif()
# Compile with strict compiler options
set(ICU_ENABLE_STRICT ON)
# Enable auto cleanup of libraries
set(ICU_ENABLE_AUTO_CLEANUP OFF)
# Enable draft APIs (and internal APIs)
set(ICU_ENABLE_DRAFT ON)
# Add a version suffix to symbols
set(ICU_ENABLE_RENAMING ON)
# Enable function and data tracing
set(ICU_ENABLE_TRACING OFF)
# Enable plugins
set(ICU_ENABLE_PLUGINS OFF)
# Disable dynamic loading
set(ICU_DISABLE_DYLOAD OFF)
# Use rpath when linking
set(ICU_ENABLE_RPATH OFF)
# Build ICU extras
set(ICU_ENABLE_EXTRAS OFF) # TODO: not released
if(ICU_CROSS_COMPILING)
  set(ICU_ENABLE_EXTRAS OFF)
endif()
# Build ICU's icuio library
set(ICU_ENABLE_ICUIO ON)
# Build ICU's Paragraph Layout library. icu-le-hb must be available via find_package(icu-le-hb). See http://harfbuzz.org
set(ICU_ENABLE_LAYOUTEX OFF) # TODO: not released
# ...
#set(ICU_ENABLE_LAYOUT OFF)
# Build ICU's tools
set(ICU_ENABLE_TOOLS ON)
if(ICU_CROSS_COMPILING)
  set(ICU_ENABLE_TOOLS OFF)
endif()
# Specify how to package ICU data. Possible values: files, archive, library, static, auto. See http://userguide.icu-project.org/icudata for more info
set(ICU_DATA_PACKAGING "auto") # TODO: 'files' mode is not released
# Tag a suffix to the library names
set(ICU_LIBRARY_SUFFIX "")
# Build ICU tests
set(ICU_ENABLE_TESTS OFF) # TODO: not released
# Build ICU samples
set(ICU_ENABLE_SAMPLES OFF) # TODO: not released


#-----------------------------------------------------------------------
# Build and install the ICU
#-----------------------------------------------------------------------

# Try to find already installed lib.
find_package(ICU ${ICU_lib_VERSION} CONFIG QUIET)

if(NOT ICU_FOUND)
  cmr_print_status(
    "ICU is not installed, build and install it.")

  include(${LIBCMAKER_ICU_SRC_DIR}/lib_cmaker_icu.cmake)
  lib_cmaker_icu(
    VERSION       ${ICU_lib_VERSION}
    DOWNLOAD_DIR  ${ICU_DOWNLOAD_DIR}
    UNPACKED_DIR  ${ICU_UNPACKED_DIR}
    BUILD_DIR     ${ICU_BUILD_DIR}
  )
  
  find_package(ICU ${ICU_lib_VERSION} REQUIRED CONFIG)
  
else()
  cmr_print_status(
    "ICU is installed, skip building and installing it.")
endif()
