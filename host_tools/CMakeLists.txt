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

project(HtmlGrapheasKamvaHostTools CXX)
set(lib_NAME "hgkamva_host_tools")

if(NOT HGKamva_DIR)
  message(FATAL_ERROR
    "Please set HGKamva_DIR with path to HtmlGrapheasKamva library.")
endif()
if(NOT HOST_TOOLS_STAMP_FILE_NAME)
  message(FATAL_ERROR
    "Please set HOST_TOOLS_STAMP_FILE_NAME with name of stamp file.")
endif()


#-----------------------------------------------------------------------
# Configure external libs directories
#-----------------------------------------------------------------------

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
  set(EXTERNAL_INSTALL_DIR "${PROJECT_BINARY_DIR}/install")
endif()
set(CMAKE_INSTALL_PREFIX "${EXTERNAL_INSTALL_DIR}")


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

set(libcmaker_root_DIR "${HGKamva_DIR}/cmake/libcmaker")
set(LIBCMAKER_SRC_DIR "${libcmaker_root_DIR}/LibCMaker")

list(APPEND CMAKE_MODULE_PATH
  "${libcmaker_root_DIR}"
  "${LIBCMAKER_SRC_DIR}/cmake/modules"
)


#-----------------------------------------------------------------------
# Set vars to HtmlGrapheasKamvaHostTools
#-----------------------------------------------------------------------

# Standard CMake vars.
#set(BUILD_SHARED_LIBS ON)

# Library specific vars.


#-----------------------------------------------------------------------
# Build, install and find required libraries
#-----------------------------------------------------------------------

include(build_icu_host_tools)


#-----------------------------------------------------------------------
# HtmlGrapheasKamvaHostTools
#-----------------------------------------------------------------------

add_custom_target(${lib_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E touch
    "${PROJECT_BINARY_DIR}/${HOST_TOOLS_STAMP_FILE_NAME}"
  COMMAND ${CMAKE_COMMAND} -E echo
    "Host tools are ready."
)
