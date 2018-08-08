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
# Build, install and find Dirent library
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Set vars for LibCMaker_Dirent
#-----------------------------------------------------------------------

set(LIBCMAKER_DIRENT_SRC_DIR
  "${CMAKE_CURRENT_LIST_DIR}/LibCMaker_Dirent"
)
# To use our FindDirent.cmake.
list(APPEND CMAKE_MODULE_PATH "${LIBCMAKER_DIRENT_SRC_DIR}/cmake")

set(DIRENT_lib_VERSION    "1.23.1")
set(DIRENT_DOWNLOAD_DIR   "${EXTERNAL_DOWNLOAD_DIR}")
set(DIRENT_UNPACKED_DIR   "${EXTERNAL_UNPACKED_DIR}")
set(DIRENT_BUILD_DIR      "${EXTERNAL_BIN_DIR}/build_dirent")

set(COPY_DIRENT_CMAKE_BUILD_SCRIPTS ON)

# Library specific vars and options.


#-----------------------------------------------------------------------
# Build and install the Dirent
#-----------------------------------------------------------------------

# Try to find already installed lib.
find_package(Dirent QUIET)

if(NOT DIRENT_FOUND)
  cmr_print_status(
    "Dirent is not installed, build and install it.")

  include(${LIBCMAKER_DIRENT_SRC_DIR}/lib_cmaker_dirent.cmake)
  lib_cmaker_dirent(
    VERSION       ${DIRENT_lib_VERSION}
    DOWNLOAD_DIR  ${DIRENT_DOWNLOAD_DIR}
    UNPACKED_DIR  ${DIRENT_UNPACKED_DIR}
    BUILD_DIR     ${DIRENT_BUILD_DIR}
  )

  find_package(Dirent REQUIRED)

else()
  cmr_print_status(
    "Dirent is installed, skip building and installing it.")
endif()
