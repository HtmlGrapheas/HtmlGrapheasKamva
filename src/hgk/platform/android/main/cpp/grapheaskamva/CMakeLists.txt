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

project(HtmlGrapheasKamvaAndroid CXX)

#-----------------------------------------------------------------------
# HtmlGrapheasKamva
#-----------------------------------------------------------------------

if(NOT HtmlGrapheasKamva_DIR)
  message(FATAL_ERROR
    "Please set HtmlGrapheasKamva_DIR with path to HtmlGrapheasKamva library.")
endif()

add_subdirectory(
  "${HtmlGrapheasKamva_DIR}"
  "${PROJECT_BINARY_DIR}/HtmlGrapheasKamva"
)


#-----------------------------------------------------------------------
# Android platform
#-----------------------------------------------------------------------

set(lib_NAME "grapheaskamva")

set(lib_HEADERS
  HtmlGrapheasKamvaAndroid.h
)

set(lib_SOURCES
  HtmlGrapheasKamvaAndroid.cpp
  HtmlGrapheasKamvaAndroidJni.cpp
)

# TODO: rewrite AGG's CMake scripts to exclude link_directories().
# AGG_LIBRARY_DIRS is cache var from AggConfig.cmake
# by find_package(Agg REQUIRED CONFIG) in HtmlGrapheasKamva.
link_directories(${AGG_LIBRARY_DIRS})

add_library(${lib_NAME} SHARED ${lib_SOURCES} ${lib_HEADERS})
set_property(TARGET ${lib_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${lib_NAME} PROPERTY C_STANDARD 11)

# HtmlGrapheasKamva
target_link_libraries(${lib_NAME} PRIVATE htmlgrapheaskamva)

# Android libs
target_link_libraries(${lib_NAME} PRIVATE android jnigraphics)
