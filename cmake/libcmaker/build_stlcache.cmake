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
# Build, install and find STLCache library
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Set vars for LibCMaker_STLCache
#-----------------------------------------------------------------------

set(LIBCMAKER_STLCACHE_SRC_DIR
  "${CMAKE_CURRENT_LIST_DIR}/LibCMaker_STLCache"
)
# To use our FindSTLCache.cmake.
list(APPEND CMAKE_MODULE_PATH "${LIBCMAKER_STLCACHE_SRC_DIR}/cmake")

set(STLCACHE_lib_VERSION    "0.2.20180405")
set(STLCACHE_DOWNLOAD_DIR   "${EXTERNAL_DOWNLOAD_DIR}")
set(STLCACHE_UNPACKED_DIR   "${EXTERNAL_UNPACKED_DIR}")
set(STLCACHE_BUILD_DIR      "${EXTERNAL_BIN_DIR}/build_stlcache")

set(COPY_STLCACHE_CMAKE_BUILD_SCRIPTS ON)

# Library specific vars and options.


#-----------------------------------------------------------------------
# Build and install the STLCache
#-----------------------------------------------------------------------

# Try to find already installed lib.
find_package(STLCache QUIET)

if(NOT STLCACHE_FOUND)
  cmr_print_status(
    "STLCache is not installed, build and install it.")

  include(${LIBCMAKER_STLCACHE_SRC_DIR}/lib_cmaker_stlcache.cmake)
  lib_cmaker_stlcache(
    VERSION       ${STLCACHE_lib_VERSION}
    DOWNLOAD_DIR  ${STLCACHE_DOWNLOAD_DIR}
    UNPACKED_DIR  ${STLCACHE_UNPACKED_DIR}
    BUILD_DIR     ${STLCACHE_BUILD_DIR}
  )
  
  find_package(STLCache REQUIRED)
  
else()
  cmr_print_status(
    "STLCache is installed, skip building and installing it.")
endif()
