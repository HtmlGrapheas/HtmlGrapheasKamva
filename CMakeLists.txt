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

cmake_minimum_required(VERSION 3.3)

# FindThreads work in environments, where only CXX is enabled, since CMake 3.4.
project(HtmlGrapheasKamva CXX C)
set(lib_NAME "hgkamva")

include(GNUInstallDirs)


#-----------------------------------------------------------------------
# Configure external libs directories
#-----------------------------------------------------------------------

set(EXTERNAL_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/external")
if(NOT EXTERNAL_DOWNLOAD_DIR)
  set(EXTERNAL_DOWNLOAD_DIR "${CMAKE_CURRENT_LIST_DIR}/.downloads")
endif()
if(NOT EXTERNAL_UNPACKED_DIR)
  set(EXTERNAL_UNPACKED_DIR "${PROJECT_BINARY_DIR}/external/sources")
endif()
if(NOT EXTERNAL_BIN_DIR)
  set(EXTERNAL_BIN_DIR "${PROJECT_BINARY_DIR}/external/build")
endif()
if(NOT EXTERNAL_INSTALL_DIR)
  set(EXTERNAL_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")
endif()


#-----------------------------------------------------------------------
# Configure to find_package()
#-----------------------------------------------------------------------

# Set CMake's search path for find_*() commands.
list(APPEND CMAKE_PREFIX_PATH "${EXTERNAL_INSTALL_DIR}")

if(ANDROID)
  list(APPEND CMAKE_FIND_ROOT_PATH "${EXTERNAL_INSTALL_DIR}")
endif()


#-----------------------------------------------------------------------
# Set path to LibCMaker root
#-----------------------------------------------------------------------

set(libcmaker_root_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake/libcmaker")
set(LIBCMAKER_SRC_DIR "${libcmaker_root_DIR}/LibCMaker")

list(APPEND CMAKE_MODULE_PATH
#  "${CMAKE_CURRENT_LIST_DIR}/cmake"
  "${libcmaker_root_DIR}"
  "${LIBCMAKER_SRC_DIR}/cmake/modules"
)


#-----------------------------------------------------------------------
# Set vars to HtmlGrapheasKamva
#-----------------------------------------------------------------------

# Standard CMake vars.
#set(BUILD_SHARED_LIBS ON)

# Library specific vars.


#-----------------------------------------------------------------------
# Build, install and find required libraries
#-----------------------------------------------------------------------

if(MSVC)
  include(build_dirent)
endif()
include(build_stlcache)
include(build_expat)
include(build_icu)
include(build_freetype_with_harfbuzz)
include(build_fontconfig)
include(build_agg)


#-----------------------------------------------------------------------
# HtmlGrapheas
#-----------------------------------------------------------------------

set(HGraph_DIR "${CMAKE_CURRENT_LIST_DIR}/HtmlGrapheas")

add_subdirectory(${HGraph_DIR} "${PROJECT_BINARY_DIR}/HtmlGrapheas")


#-----------------------------------------------------------------------
# HtmlGrapheasKamva
#-----------------------------------------------------------------------

set(private_src_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(interface_src_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(build_src_DIR
  "$<BUILD_INTERFACE:${private_src_DIR}>"
)
set(install_src_DIR
  "$<INSTALL_INTERFACE:${interface_src_DIR}>"
)
set(public_src_DIR "${build_src_DIR}${install_src_DIR}")

add_library(${lib_NAME} "")
set_target_properties(${lib_NAME} PROPERTIES
  CXX_STANDARD 11
  C_STANDARD 11
)
# Export HtmlGrapheasKamva include dirs.
target_include_directories(${lib_NAME} PUBLIC ${public_src_DIR})

target_sources(${lib_NAME}
  PRIVATE
    ${private_src_DIR}/hgkamva/hg_kamva_api.cpp
  
    ${private_src_DIR}/hgkamva/container/HgContainer.cpp
    ${private_src_DIR}/hgkamva/container/HgFont.cpp
    ${private_src_DIR}/hgkamva/container/HgFontLibrary.cpp
    ${private_src_DIR}/hgkamva/renderer/HgHtmlRenderer.cpp
    ${private_src_DIR}/hgkamva/util/FileUtil.cpp
    ${private_src_DIR}/hgkamva/util/StringUtil.cpp

  PUBLIC
    # TODO: all are public?
    ${public_src_DIR}/hgkamva/hg_kamva_api.h
    ${public_src_DIR}/hgkamva/hg_kamva_codes.h
    ${public_src_DIR}/hgkamva/hg_kamva_common.h
  
    ${public_src_DIR}/hgkamva/container/HgAggPainter.h
    ${public_src_DIR}/hgkamva/container/HgContainer.h
    ${public_src_DIR}/hgkamva/container/HgFont.h
    ${public_src_DIR}/hgkamva/container/HgFontLibrary.h
    ${public_src_DIR}/hgkamva/container/HgPainter.h
    ${public_src_DIR}/hgkamva/renderer/HgAggHtmlRenderer.h
    ${public_src_DIR}/hgkamva/renderer/HgHtmlRenderer.h
    ${public_src_DIR}/hgkamva/util/FileUtil.h
    ${public_src_DIR}/hgkamva/util/StringUtil.h
)


#-----------------------------------------------------------------------
# Link to the libraries
#-----------------------------------------------------------------------

# HtmlGrapheas
target_link_libraries(${lib_NAME} PRIVATE hgraph)

# AGG
target_include_directories(${lib_NAME} PRIVATE ${AGG_INCLUDE_DIRS})
target_link_libraries(${lib_NAME} PRIVATE ${AGG_LIBRARY})

# FontConfig
target_include_directories(${lib_NAME} PRIVATE ${FONTCONFIG_INCLUDE_DIR})
target_link_libraries(${lib_NAME} PRIVATE ${FONTCONFIG_LIBRARY})

# FreeType
target_include_directories(${lib_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${lib_NAME} PRIVATE ${FREETYPE_LIBRARIES})

# HarfBuzz
if(WITH_HarfBuzz)
  target_include_directories(${lib_NAME} PRIVATE ${HARFBUZZ_INCLUDE_DIRS})
  # From <freetype sources>/docs/CHANGES:
  #  Note that HarfBuzz depends on FreeType.
  target_link_libraries(${lib_NAME} PRIVATE
    ${HARFBUZZ_LIBRARIES} ${FREETYPE_LIBRARIES})
endif()

# ICU
target_link_libraries(${lib_NAME} PRIVATE ICU::i18n)

# Expat
target_include_directories(${lib_NAME} PRIVATE ${EXPAT_INCLUDE_DIR})
target_link_libraries(${lib_NAME} PRIVATE ${EXPAT_LIBRARY})

# STLCache
target_include_directories(${lib_NAME} PRIVATE ${STLCACHE_INCLUDE_DIR})


#-----------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------

add_subdirectory(test)
