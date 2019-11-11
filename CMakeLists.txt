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

cmake_minimum_required(VERSION 3.8)
# C++17 compiler:
#   GCC 8
#   Clang 8 (Android NDK r19)
#   Apple Clang 11.0.0, Xcode 11.0
#   MSVC 19.15, VS 2017 15.8
# See: https://en.cppreference.com/w/cpp/compiler_support


project(HtmlGrapheasKamva CXX)
set(lib_NAME "hgkamva")

include(GNUInstallDirs)


#-----------------------------------------------------------------------
# Set vars to HtmlGrapheasKamva
#-----------------------------------------------------------------------

if(NOT FT_WITH_HARFBUZZ)
  message(FATAL_ERROR
    "Please set FT_WITH_HARFBUZZ=ON and build FreeType with HarfBuzz."
  )
endif()


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
  CXX_STANDARD 17
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

    ${public_src_DIR}/hgkamva/container/HgContainer.h
    ${public_src_DIR}/hgkamva/container/HgFont.h
    ${public_src_DIR}/hgkamva/container/HgFontLibrary.h
    ${public_src_DIR}/hgkamva/container/HgPainter.h
    ${public_src_DIR}/hgkamva/renderer/HgHtmlRenderer.h
    ${public_src_DIR}/hgkamva/util/FileUtil.h
    ${public_src_DIR}/hgkamva/util/StringUtil.h
)

if(HG_AGG_CONTAINER)
  target_sources(${lib_NAME}
    PRIVATE
      ${private_src_DIR}/hgkamva/agg/hg_agg_kamva_api.cpp
    PUBLIC
      ${public_src_DIR}/hgkamva/agg/container/HgAggPainter.h
      ${public_src_DIR}/hgkamva/agg/renderer/HgAggHtmlRenderer.h
  )
  target_compile_definitions(${lib_NAME} PUBLIC HG_AGG_CONTAINER)
else()
  target_sources(${lib_NAME}
    PRIVATE
      ${private_src_DIR}/hgkamva/cairo/hg_cairo_kamva_api.cpp
    PUBLIC
      ${public_src_DIR}/hgkamva/cairo/container/HgCairoPainter.h
      ${public_src_DIR}/hgkamva/cairo/renderer/HgCairoHtmlRenderer.h
  )
endif()


#-----------------------------------------------------------------------
# Link to the libraries
#-----------------------------------------------------------------------

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(STLCache REQUIRED)
if(MSVC)
  find_package(Dirent REQUIRED)
endif()
find_package(EXPAT REQUIRED)
find_package(Freetype REQUIRED)
find_package(HarfBuzz REQUIRED)
find_package(Fontconfig REQUIRED)
find_package(Pixman REQUIRED)
find_package(Cairo REQUIRED)
if(HG_AGG_CONTAINER)
  find_package(Agg REQUIRED CONFIG)
endif()

# HtmlGrapheas
target_link_libraries(${lib_NAME} PRIVATE hgraph)

if(HG_AGG_CONTAINER)
  # AGG
  target_include_directories(${lib_NAME} PRIVATE ${AGG_INCLUDE_DIRS})
  target_link_libraries(${lib_NAME} PRIVATE ${AGG_LIBRARY})
endif()

# Cairo
target_link_libraries(${lib_NAME} PRIVATE Cairo::Cairo)

# Pixman
target_link_libraries(${lib_NAME} PRIVATE Pixman::Pixman)

# FontConfig
target_link_libraries(${lib_NAME} PRIVATE Fontconfig::Fontconfig)

# FreeType
target_link_libraries(${lib_NAME} PRIVATE Freetype::Freetype)

# HarfBuzz
target_include_directories(${lib_NAME} PRIVATE ${HARFBUZZ_INCLUDE_DIR})
# From <freetype sources>/docs/CHANGES:
#  Note that HarfBuzz depends on FreeType.
target_link_libraries(${lib_NAME} PRIVATE
  ${HARFBUZZ_LIBRARY} Freetype::Freetype
)

# Expat
target_include_directories(${lib_NAME} PRIVATE ${EXPAT_INCLUDE_DIR})
target_link_libraries(${lib_NAME} PRIVATE ${EXPAT_LIBRARY})

# STLCache
target_include_directories(${lib_NAME} PRIVATE ${STLCACHE_INCLUDE_DIR})

# libpng
target_link_libraries(${lib_NAME} PRIVATE PNG::PNG)

# zlib
target_link_libraries(${lib_NAME} PRIVATE ZLIB::ZLIB)  # icluded in PNG::PNG

# Threads, pthread
if(CMAKE_USE_PTHREADS_INIT)
  target_link_libraries(${lib_NAME} PRIVATE Threads::Threads)
endif()

# android_support
if(ANDROID AND ANDROID_NATIVE_API_LEVEL LESS 21)
  target_link_libraries(${lib_NAME} PRIVATE android_support)
endif()


#-----------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------

add_subdirectory(test)
