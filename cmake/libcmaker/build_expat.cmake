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
# Build, install and find Expat library
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Set vars for LibCMaker_Expat
#-----------------------------------------------------------------------

set(LIBCMAKER_EXPAT_SRC_DIR
  "${CMAKE_CURRENT_LIST_DIR}/LibCMaker_Expat"
)
# To use our FindEXPAT.cmake.
list(APPEND CMAKE_MODULE_PATH "${LIBCMAKER_EXPAT_SRC_DIR}/cmake")

set(EXPAT_lib_VERSION   "2.2.5")
set(EXPAT_DOWNLOAD_DIR  "${EXTERNAL_DOWNLOAD_DIR}")
set(EXPAT_UNPACKED_DIR  "${EXTERNAL_UNPACKED_DIR}")
set(EXPAT_BUILD_DIR     "${EXTERNAL_BIN_DIR}/build_expat")

# Library specific vars and options.
option(BUILD_tools "build the xmlwf tool for expat library" OFF)
option(BUILD_examples "build the examples for expat library" OFF)
option(BUILD_tests "build the tests for expat library" OFF)
# Option BUILD_shared is set in lib_cmaker_expat() by BUILD_SHARED_LIBS.
#option(BUILD_shared "build a shared expat library" ${BUILD_SHARED_LIBS})
option(BUILD_doc "build man page for xmlwf" OFF)
option(USE_libbsd "utilize libbsd (for arc4random_buf)" OFF)
# Option INSTALL is set in lib_cmaker_expat() by NOT SKIP_INSTALL_ALL.
#option(INSTALL "install expat files in cmake install target" ON)

# Configuration options.
set(XML_CONTEXT_BYTES 1024 CACHE STRING
  "Define to specify how much context to retain around the current parse point")
option(XML_DTD
  "Define to make parameter entity parsing functionality available" ON)
option(XML_NS "Define to make XML Namespaces functionality available" ON)
if(NOT WIN32)
  option(XML_DEV_URANDOM
    "Define to include code reading entropy from `/dev/urandom'." ON)
endif()


#-----------------------------------------------------------------------
# Build and install the Expat
#-----------------------------------------------------------------------

# Try to find already installed lib.
find_package(EXPAT QUIET)

if(NOT EXPAT_FOUND)
  cmr_print_status(
    "Expat is not installed, build and install it.")

  include(${LIBCMAKER_EXPAT_SRC_DIR}/lib_cmaker_expat.cmake)
  lib_cmaker_expat(
    VERSION       ${EXPAT_lib_VERSION}
    DOWNLOAD_DIR  ${EXPAT_DOWNLOAD_DIR}
    UNPACKED_DIR  ${EXPAT_UNPACKED_DIR}
    BUILD_DIR     ${EXPAT_BUILD_DIR}
  )
  
  find_package(EXPAT REQUIRED)
  
else()
  cmr_print_status(
    "Expat is installed, skip building and installing it.")
endif()
