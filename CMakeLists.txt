# ****************************************************************************
#  Project:  HtmlGrapheas
#  Purpose:  HTML text editor library
#  Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#    Copyright (c) 2017 NikitaFeodonit
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

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(HtmlGrapheasKamva CXX)
set(hgk_LIB_NAME "htmlgrapheaskamva")

# External libs directories
set(EXTERNAL_SRC_DIR "${PROJECT_SOURCE_DIR}/external")
if(NOT EXTERNAL_DOWNLOAD_DIR)
  set(EXTERNAL_DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/.downloads")
endif()
if(NOT EXTERNAL_UNPACKED_SRC_DIR)
  set(EXTERNAL_UNPACKED_SRC_DIR "${PROJECT_BINARY_DIR}/external/sources")
endif()
set(EXTERNAL_BIN_DIR "${PROJECT_BINARY_DIR}/external/build")
set(EXTERNAL_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")

# Path to LibCMaker root
set(LIBCMAKER_SRC_DIR "${EXTERNAL_SRC_DIR}/LibCMaker")

set(CMAKE_COLOR_MAKEFILE ON)
list(APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmake"
  "${LIBCMAKER_SRC_DIR}/cmake/modules"
)


#-----------------------------------------------------------------------
# Set standard CMake vars for LibCMaker libs.
#-----------------------------------------------------------------------

#set(BUILD_SHARED_LIBS ON)

#set(SKIP_INSTALL_HEADERS ON)
#set(SKIP_INSTALL_LIBRARIES ON)
#set(SKIP_INSTALL_ALL ON)


#-----------------------------------------------------------------------
# Set common useful vars for LibCMaker libs.
#-----------------------------------------------------------------------

set(cmr_PRINT_DEBUG ON)


#-----------------------------------------------------------------------
# FreeType and HarfBuzz libraries
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Set vars for LibCMaker_FreeType and LibCMaker_HarfBuzz.
#-----------------------------------------------------------------------

# Needed for lib_cmaker_harfbuzz() to build HarfBuzz with FreeType
set(LIBCMAKER_FREETYPE_SRC_DIR "${EXTERNAL_SRC_DIR}/LibCMaker_FreeType")
# To use our FindFreetype.cmake
list(APPEND CMAKE_MODULE_PATH "${LIBCMAKER_FREETYPE_SRC_DIR}/cmake/")

set(FT_lib_VERSION "2.8.1")

set(FT_DOWNLOAD_DIR "${EXTERNAL_DOWNLOAD_DIR}")
set(FT_UNPACKED_SRC_DIR "${EXTERNAL_UNPACKED_SRC_DIR}")
set(FT_BUILD_DIR "${EXTERNAL_BIN_DIR}/build_ft")

# Library specific vars.
set(FREETYPE_NO_DIST ON)

set(WITH_ZLIB OFF)
set(WITH_BZip2 OFF)
set(WITH_PNG OFF)
set(WITH_HarfBuzz ON)

if(WITH_HarfBuzz)
  # Needed for lib_cmaker_freetype() to build HarfBuzz
  set(LIBCMAKER_HARFBUZZ_SRC_DIR
    "${EXTERNAL_SRC_DIR}/LibCMaker_HarfBuzz")
  # To find FindHarfBuzz.cmake
  list(APPEND CMAKE_MODULE_PATH "${LIBCMAKER_HARFBUZZ_SRC_DIR}/cmake/")

  set(HB_lib_VERSION "1.6.3")

  set(HB_DOWNLOAD_DIR "${EXTERNAL_DOWNLOAD_DIR}")
  set(HB_UNPACKED_SRC_DIR "${EXTERNAL_UNPACKED_SRC_DIR}")
  set(HB_BUILD_DIR "${EXTERNAL_BIN_DIR}/build_hb")

  # ENV{FREETYPE_DIR} is not needed to find Freetype in CONFIG mode,
  # but need to find FreeType in lib_cmaker_harfbuzz().
  set(ENV{FREETYPE_DIR} "${EXTERNAL_INSTALL_DIR}")
  # Needed to find HarfBuzz in lib_cmaker_freetype() and here.
  set(ENV{HARFBUZZ_DIR} "${EXTERNAL_INSTALL_DIR}")
endif()

#-----------------------------------------------------------------------
# Build and install the FreeType (and HarfBuzz in lib_cmaker_freetype()).
#-----------------------------------------------------------------------

include(${EXTERNAL_SRC_DIR}/LibCMaker_FreeType/lib_cmaker_freetype.cmake)
lib_cmaker_freetype(
  VERSION ${FT_lib_VERSION}
  DOWNLOAD_DIR ${FT_DOWNLOAD_DIR}
  UNPACKED_SRC_DIR ${FT_UNPACKED_SRC_DIR}
  BUILD_DIR ${FT_BUILD_DIR}
)


#-----------------------------------------------------------------------
# AGG library
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Set vars for LibCMaker_AGG.
#-----------------------------------------------------------------------

set(AGG_lib_VERSION "2.4.127")

set(AGG_DOWNLOAD_DIR "${EXTERNAL_DOWNLOAD_DIR}")
set(AGG_UNPACKED_SRC_DIR "${EXTERNAL_UNPACKED_SRC_DIR}")
set(AGG_BUILD_DIR "${EXTERNAL_BIN_DIR}/build_agg")

set(SKIP_BUILD_AGG_EXAMPLES ON)
set(SKIP_BUILD_AGG_MYAPP ON)

# Library specific vars and options.
set(AGG_DIR "${EXTERNAL_INSTALL_DIR}")
set(ENV{AGG_DIR} "${AGG_DIR}")
set(AGG_DIR_BIN "${AGG_DIR}/bin")

# Var antigrain_SOURCE_DIR is set in lib_cmaker_agg() from UNPACKED_SRC_DIR.
# Var antigrain_BINARY_DIR is set in lib_cmaker_agg() from CMAKE_INSTALL_PREFIX.

option(agg_USE_GPC "Use Gpc Boolean library" OFF)
option(agg_USE_FREETYPE "Use Freetype library" OFF)
option(agg_USE_EXPAT "Use Expat library" OFF)
option(agg_USE_SDL_PLATFORM "Use SDL as platform" OFF)
option(agg_USE_PACK "Package Agg" OFF)
option(agg_USE_AGG2D "Agg 2D graphical context" OFF)
option(agg_USE_DEBUG "For debug version" OFF)
option(agg_USE_AGG2D_FREETYPE "Agg 2D graphical context uses freetype" OFF)    

# TODO: set ENV{FREETYPE_DIR} for AGG if agg_USE_FREETYPE==ON


#-----------------------------------------------------------------------
# Build and install the AGG.
#-----------------------------------------------------------------------

include(${EXTERNAL_SRC_DIR}/LibCMaker_AGG/lib_cmaker_agg.cmake)
lib_cmaker_agg(
  VERSION ${AGG_lib_VERSION}
  DOWNLOAD_DIR ${AGG_DOWNLOAD_DIR}
  UNPACKED_SRC_DIR ${AGG_UNPACKED_SRC_DIR}
  BUILD_DIR ${AGG_BUILD_DIR}
)


#-----------------------------------------------------------------------
# Configure to find_package.
#-----------------------------------------------------------------------

# Set CMake's search path for find_package(), find_program(), find_library(),
# find_file(), and find_path() commands.
list(APPEND CMAKE_PREFIX_PATH "${EXTERNAL_INSTALL_DIR}")

if(ANDROID)
  list(APPEND CMAKE_FIND_ROOT_PATH "${EXTERNAL_INSTALL_DIR}")
endif()


#-----------------------------------------------------------------------
# find_package
#-----------------------------------------------------------------------

# FreeType
find_package(Freetype REQUIRED)

# HarfBuzz
if(WITH_HarfBuzz)
  find_package(HarfBuzz REQUIRED)
endif()

# AGG
# AggConfig.cmake set the cache var AGG_LIBRARY_DIRS,
# which used in parent project in link_directories().
# TODO: rewrite AGG's CMake scripts to exclude link_directories().
find_package(Agg REQUIRED CONFIG)
include(${AGG_USE_FILE})


#-----------------------------------------------------------------------
# Link to the dependences.
#-----------------------------------------------------------------------

# HtmlGrapheasKamva
add_library(${hgk_LIB_NAME}
  ${PROJECT_SOURCE_DIR}/src/hgk/dummy.cpp
  ${PROJECT_SOURCE_DIR}/src/hgk/agg_freetype_harfbuzz.h
)
set_target_properties(${hgk_LIB_NAME} PROPERTIES
  CXX_STANDARD 11
  C_STANDARD 11
)
# Export HtmlGrapheasKamva includes.
target_include_directories(${hgk_LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/hgk)

# FreeType
# TODO: PUBLIC change to PRIVATE for inc_dirs
target_include_directories(${hgk_LIB_NAME} PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${hgk_LIB_NAME} PUBLIC ${FREETYPE_LIBRARIES})

# HarfBuzz
if(WITH_HarfBuzz)
  # TODO: PUBLIC change to PRIVATE for inc_dirs
  target_include_directories(${hgk_LIB_NAME} PUBLIC ${HARFBUZZ_INCLUDE_DIRS})
  # From <freetype sources>/docs/CHANGES:
  #  Note that HarfBuzz depends on FreeType.
  target_link_libraries(${hgk_LIB_NAME} PUBLIC
    ${HARFBUZZ_LIBRARIES} ${FREETYPE_LIBRARIES})
endif()

# AGG
# Export AGG includes.
target_include_directories(${hgk_LIB_NAME} PUBLIC ${AGG_INCLUDE_DIRS})
target_link_libraries(${hgk_LIB_NAME} PUBLIC ${AGG_LIBRARIES})


#-----------------------------------------------------------------------
# Platforms
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# wxWidgets
#-----------------------------------------------------------------------

if(wxWidgets_FOUND)
  set(wxAGG_SRC_DIR "${PROJECT_SOURCE_DIR}/external/wxAGG") # TODO: remove it
  add_subdirectory(src/hgk/platform/wxwidgets/grapheaskamva)
endif()


#-----------------------------------------------------------------------
# Android
#-----------------------------------------------------------------------

if(ANDROID)
  add_subdirectory(src/hgk/platform/android/main/cpp/grapheaskamva)
endif()
