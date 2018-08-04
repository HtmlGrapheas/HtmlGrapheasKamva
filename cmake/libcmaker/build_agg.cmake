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
# Build, install and find AGG library
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Set vars to LibCMaker_AGG
#-----------------------------------------------------------------------

set(LIBCMAKER_AGG_SRC_DIR
  "${CMAKE_CURRENT_LIST_DIR}/LibCMaker_AGG"
)
# To use our FindAgg.cmake.
list(APPEND CMAKE_MODULE_PATH "${LIBCMAKER_AGG_SRC_DIR}/cmake")

set(AGG_lib_VERSION   "2.4.128")
set(AGG_DOWNLOAD_DIR  "${EXTERNAL_DOWNLOAD_DIR}")
set(AGG_UNPACKED_DIR  "${EXTERNAL_UNPACKED_DIR}")
set(AGG_BUILD_DIR     "${EXTERNAL_BIN_DIR}/build_agg")

set(AGG_DIR "${EXTERNAL_INSTALL_DIR}")
set(ENV{AGG_DIR} "${AGG_DIR}")
set(AGG_DIR_BIN "${AGG_DIR}/bin")

set(NOT_ADD_AGG_PLATFORM ON)
set(SKIP_BUILD_AGG_EXAMPLES ON)
set(SKIP_BUILD_AGG_MYAPP ON)

# Library specific vars and options.
option(agg_USE_GPC "Use Gpc Boolean library" OFF)
option(agg_USE_FREETYPE "Use Freetype library" OFF)
option(agg_USE_EXPAT "Use Expat library" OFF)
option(agg_USE_SDL_PLATFORM "Use SDL as platform" OFF)
option(agg_USE_PACK "Package Agg" OFF)
option(agg_USE_AGG2D "Agg 2D graphical context" OFF)
option(agg_USE_DEBUG "For debug version" OFF)
option(agg_USE_AGG2D_FREETYPE "Agg 2D graphical context uses freetype" OFF)    

# TODO: set ENV{FREETYPE_DIR} to AGG if agg_USE_FREETYPE==ON


#-----------------------------------------------------------------------
# Build and install the AGG
#-----------------------------------------------------------------------

# Try to find already installed lib.
find_package(Agg CONFIG QUIET)

if(NOT Agg_FOUND)
  cmr_print_status(
    "AGG is not installed, build and install it.")

  include(${LIBCMAKER_AGG_SRC_DIR}/lib_cmaker_agg.cmake)
  lib_cmaker_agg(
    VERSION       ${AGG_lib_VERSION}
    DOWNLOAD_DIR  ${AGG_DOWNLOAD_DIR}
    UNPACKED_DIR  ${AGG_UNPACKED_DIR}
    BUILD_DIR     ${AGG_BUILD_DIR}
  )

  find_package(Agg REQUIRED CONFIG)

else()
  cmr_print_status(
    "AGG is installed, skip building and installing it.")
endif()

#include(${AGG_USE_FILE})
