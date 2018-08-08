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
# Build the host tools of the ICU library
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
if(BUILD_FOR_WINXP OR CMAKE_GENERATOR_TOOLSET STREQUAL "v141_xp")
  # This is the last ICU4C release that works on Windows XP and Windows Vista.
  set(ICU_lib_VERSION   "58.2")
endif()
set(ICU_DOWNLOAD_DIR  "${EXTERNAL_DOWNLOAD_DIR}")
set(ICU_UNPACKED_DIR  "${EXTERNAL_UNPACKED_DIR}/host_tools_sources")
set(ICU_BUILD_DIR     "${EXTERNAL_BIN_DIR}/build_icu_host_tools")

set(COPY_ICU_CMAKE_BUILD_SCRIPTS ON)
set(BUILD_HOST_TOOLS ON)

# Library specific vars and options.

# Enable cross compiling
set(ICU_CROSS_COMPILING OFF)
if(IOS OR ANDROID OR WINDOWS_STORE)
  set(ICU_CROSS_COMPILING ON)
endif()
# Specify an absolute path to the build directory of an ICU built for the current platform
set(ICU_CROSS_BUILDROOT "")
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
# Build the host tools of the ICU library
#-----------------------------------------------------------------------

# Try to find already existed host tools.
set(ICU_HOST_TOOLS_FOUND ON)

set(tmp_ICU_CROSS_BUILDROOT
  "${PROJECT_BINARY_DIR}/external/build/build_icu_host_tools/icu-61.1/source"
)

if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/derb)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/escapesrc)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/genbrk)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/genccode)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gencfu)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gencmn)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gencnval)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gendict)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gennorm2)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/genrb)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gensprep)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gentest)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/icuinfo)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/icupkg)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/makeconv)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()
if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/pkgdata)
  set(ICU_HOST_TOOLS_FOUND OFF)
endif()

if(NOT ICU_HOST_TOOLS_FOUND)
  cmr_print_status("ICU host tools is not built, build it.")

  include(${LIBCMAKER_ICU_SRC_DIR}/lib_cmaker_icu.cmake)
  lib_cmaker_icu(
    VERSION       ${ICU_lib_VERSION}
    DOWNLOAD_DIR  ${ICU_DOWNLOAD_DIR}
    UNPACKED_DIR  ${ICU_UNPACKED_DIR}
    BUILD_DIR     ${ICU_BUILD_DIR}
  )

  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/derb)
    message(FATAL_ERROR "The program 'derb' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/escapesrc)
    message(FATAL_ERROR "The program 'escapesrc' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/genbrk)
    message(FATAL_ERROR "The program 'genbrk' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/genccode)
    message(FATAL_ERROR "The program 'genccode' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gencfu)
    message(FATAL_ERROR "The program 'gencfu' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gencmn)
    message(FATAL_ERROR "The program 'gencmn' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gencnval)
    message(FATAL_ERROR "The program 'gencnval' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gendict)
    message(FATAL_ERROR "The program 'gendict' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gennorm2)
    message(FATAL_ERROR "The program 'gennorm2' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/genrb)
    message(FATAL_ERROR "The program 'genrb' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gensprep)
    message(FATAL_ERROR "The program 'gensprep' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/gentest)
    message(FATAL_ERROR "The program 'gentest' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/icuinfo)
    message(FATAL_ERROR "The program 'icuinfo' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/icupkg)
    message(FATAL_ERROR "The program 'icupkg' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/makeconv)
    message(FATAL_ERROR "The program 'makeconv' is not found.")
  endif()
  if(NOT EXISTS ${tmp_ICU_CROSS_BUILDROOT}/bin/pkgdata)
    message(FATAL_ERROR "The program 'pkgdata' is not found.")
  endif()

else()
  cmr_print_status("ICU host tools is built, skip building it.")
endif()
