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

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(HtmlGrapheasKamva CXX)
set(hgk_LIB_NAME "htmlgrapheaskamva")
set(hgk_SRC_DIR "${PROJECT_SOURCE_DIR}/src")


#-----------------------------------------------------------------------
# Configure external libs directories.
#-----------------------------------------------------------------------

set(EXTERNAL_SRC_DIR "${PROJECT_SOURCE_DIR}/external")
if(NOT EXTERNAL_DOWNLOAD_DIR)
  set(EXTERNAL_DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/.downloads")
endif()
if(NOT EXTERNAL_UNPACKED_SRC_DIR)
  set(EXTERNAL_UNPACKED_SRC_DIR "${PROJECT_BINARY_DIR}/external/sources")
endif()
if(NOT EXTERNAL_BIN_DIR)
  set(EXTERNAL_BIN_DIR "${PROJECT_BINARY_DIR}/external/build")
endif()
if(NOT EXTERNAL_INSTALL_DIR)
  set(EXTERNAL_INSTALL_DIR "${PROJECT_BINARY_DIR}/install")
endif()
set(CMAKE_INSTALL_PREFIX "${EXTERNAL_INSTALL_DIR}")


#-----------------------------------------------------------------------
# Configure to find_package.
#-----------------------------------------------------------------------

# Set CMake's search path for find_package(CONFIG mode), find_program(),
# find_library(), find_file(), and find_path() commands.
list(APPEND CMAKE_PREFIX_PATH "${EXTERNAL_INSTALL_DIR}")

if(ANDROID)
  list(APPEND CMAKE_FIND_ROOT_PATH "${EXTERNAL_INSTALL_DIR}")
endif()


#-----------------------------------------------------------------------
# Set path to LibCMaker root
#-----------------------------------------------------------------------

set(LIBCMAKER_SRC_DIR "${EXTERNAL_SRC_DIR}/LibCMaker")

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

#set(cmr_PRINT_DEBUG ON)


#-----------------------------------------------------------------------
# Build, install and find required libraries.
#-----------------------------------------------------------------------

if(MSVC)
  include(build_dirent)
endif()
include(build_expat)
include(build_freetype_with_harfbuzz)
include(build_fontconfig)
include(build_agg)
include(build_stlcache)


#-----------------------------------------------------------------------
# HtmlGrapheas
#-----------------------------------------------------------------------

set(HtmlGrapheas_DIR "${PROJECT_SOURCE_DIR}/HtmlGrapheas")

add_subdirectory(${HtmlGrapheas_DIR})


#-----------------------------------------------------------------------
# Link to the dependences.
#-----------------------------------------------------------------------

# HtmlGrapheasKamva
set(hgk_HEADERS
  ${hgk_SRC_DIR}/hgkamva/hg_kamva_api.h
  ${hgk_SRC_DIR}/hgkamva/hg_kamva_codes.h
  ${hgk_SRC_DIR}/hgkamva/hg_kamva_common.h

  ${hgk_SRC_DIR}/hgkamva/container/HgAggPainter.h
  ${hgk_SRC_DIR}/hgkamva/container/HgContainer.h
  ${hgk_SRC_DIR}/hgkamva/container/HgFont.h
  ${hgk_SRC_DIR}/hgkamva/container/HgFontLibrary.h
  ${hgk_SRC_DIR}/hgkamva/container/HgPainter.h
  ${hgk_SRC_DIR}/hgkamva/renderer/HgAggHtmlRenderer.h
  ${hgk_SRC_DIR}/hgkamva/renderer/HgHtmlRenderer.h
  ${hgk_SRC_DIR}/hgkamva/util/FileUtil.h
  ${hgk_SRC_DIR}/hgkamva/util/StringUtil.h
)

set(hgk_SOURCES
  ${hgk_SRC_DIR}/hgkamva/hg_kamva_api.cpp

  ${hgk_SRC_DIR}/hgkamva/container/HgContainer.cpp
  ${hgk_SRC_DIR}/hgkamva/container/HgFont.cpp
  ${hgk_SRC_DIR}/hgkamva/container/HgFontLibrary.cpp
  ${hgk_SRC_DIR}/hgkamva/renderer/HgHtmlRenderer.cpp
  ${hgk_SRC_DIR}/hgkamva/util/FileUtil.cpp
  ${hgk_SRC_DIR}/hgkamva/util/StringUtil.cpp
)

add_library(${hgk_LIB_NAME} ${hgk_SOURCES} ${hgk_HEADERS})
set_target_properties(${hgk_LIB_NAME} PROPERTIES
  CXX_STANDARD 11
  C_STANDARD 11
)
# Export HtmlGrapheasKamva includes.
target_include_directories(${hgk_LIB_NAME} PUBLIC ${hgk_SRC_DIR})

# HtmlGrapheas
target_link_libraries(${hgk_LIB_NAME} PRIVATE htmlgrapheas)

# FontConfig
target_include_directories(${hgk_LIB_NAME} PRIVATE ${FONTCONFIG_INCLUDE_DIR})
target_link_libraries(${hgk_LIB_NAME} PRIVATE ${FONTCONFIG_LIBRARY})

# Expat
target_include_directories(${hgk_LIB_NAME} PRIVATE ${EXPAT_INCLUDE_DIR})
target_link_libraries(${hgk_LIB_NAME} PRIVATE ${EXPAT_LIBRARY})

# FreeType
target_include_directories(${hgk_LIB_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${hgk_LIB_NAME} PRIVATE ${FREETYPE_LIBRARIES})

# HarfBuzz
if(WITH_HarfBuzz)
  target_include_directories(${hgk_LIB_NAME} PRIVATE ${HARFBUZZ_INCLUDE_DIRS})
  # From <freetype sources>/docs/CHANGES:
  #  Note that HarfBuzz depends on FreeType.
  target_link_libraries(${hgk_LIB_NAME} PRIVATE
    ${HARFBUZZ_LIBRARIES} ${FREETYPE_LIBRARIES})
endif()

# AGG
target_include_directories(${hgk_LIB_NAME} PRIVATE ${AGG_INCLUDE_DIRS})
target_link_libraries(${hgk_LIB_NAME} PRIVATE ${AGG_LIBRARY})

# STLCache
target_include_directories(${hgk_LIB_NAME} PRIVATE ${STLCACHE_INCLUDE_DIR})

#-----------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------

add_subdirectory(test)
