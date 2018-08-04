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

project(HtmlGrapheasKamvaAndroid CXX)
set(lib_NAME "hgkamva_android")

if(NOT HGKamva_DIR)
  message(FATAL_ERROR
    "Please set HGKamva_DIR with path to HtmlGrapheasKamva library.")
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
#  "${CMAKE_CURRENT_LIST_DIR}/cmake"
  "${LIBCMAKER_SRC_DIR}/cmake/modules"
)


#-----------------------------------------------------------------------
# Set vars to HtmlGrapheasKamvaAndroid
#-----------------------------------------------------------------------

# Standard CMake vars.
#set(BUILD_SHARED_LIBS ON)

# Library specific vars.


#-----------------------------------------------------------------------
# HtmlGrapheasKamva
#-----------------------------------------------------------------------

add_subdirectory("${HGKamva_DIR}" "${PROJECT_BINARY_DIR}/HtmlGrapheasKamva")


#-----------------------------------------------------------------------
# HtmlGrapheasKamvaAndroid
#-----------------------------------------------------------------------

set(private_src_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

add_library(${lib_NAME} SHARED "")
set_target_properties(${lib_NAME} PROPERTIES
  CXX_STANDARD 11
  C_STANDARD 11
)
target_include_directories(${lib_NAME} PRIVATE ${public_src_DIR})

target_sources(${lib_NAME}
  PRIVATE
    ${private_src_DIR}/hgkamva/platform/android/HgKamvaApiJni.cpp
)


#-----------------------------------------------------------------------
# Link to the libraries
#-----------------------------------------------------------------------

# HtmlGrapheasKamva
target_link_libraries(${lib_NAME} PRIVATE hgkamva)

# Android libs
target_link_libraries(${lib_NAME} PRIVATE android jnigraphics)

if(BUILD_SHARED_LIBS)
  # Copy installed external so-libs to include to apk-file.
  # We needed to make a copying twice
  # because of the behavior of the android gradle.
  add_custom_target(copy_installed_so_libs_before
    COMMAND ${CMAKE_COMMAND} -E copy
      "*.so"
      "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    WORKING_DIRECTORY
      "${EXTERNAL_INSTALL_DIR}/lib"
  )
  add_dependencies(${lib_NAME} copy_installed_so_libs_before)
  
  add_custom_target(copy_installed_so_libs_after
    COMMAND ${CMAKE_COMMAND} -E copy
      "*.so"
      "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    WORKING_DIRECTORY
      "${EXTERNAL_INSTALL_DIR}/lib"
  )
  add_dependencies(copy_installed_so_libs_after ${lib_NAME})
endif()


#-----------------------------------------------------------------------
# Android app assets data
#-----------------------------------------------------------------------

if(HG_APP_ASSETS_DIR)
  set(HG_APP_ASSETS_FONT_DIR "${HG_APP_ASSETS_DIR}/fonts")
  set(HG_APP_ASSETS_DATA_DIR "${HG_APP_ASSETS_DIR}/data")

  add_custom_target(prepare_app_assets_fonts
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${HG_APP_ASSETS_FONT_DIR}"

    # Arimo font files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Arimo/Arimo-BoldItalic.ttf"
      "${HG_APP_ASSETS_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Arimo/Arimo-Bold.ttf"
      "${HG_APP_ASSETS_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Arimo/Arimo-Italic.ttf"
      "${HG_APP_ASSETS_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Arimo/Arimo-Regular.ttf"
      "${HG_APP_ASSETS_FONT_DIR}"

    # Tinos font files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Tinos/Tinos-BoldItalic.ttf"
      "${HG_APP_ASSETS_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Tinos/Tinos-Bold.ttf"
      "${HG_APP_ASSETS_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Tinos/Tinos-Italic.ttf"
      "${HG_APP_ASSETS_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Tinos/Tinos-Regular.ttf"
      "${HG_APP_ASSETS_FONT_DIR}"
  )

  add_custom_target(prepare_app_assets_font_conf
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_INSTALL_PREFIX}/etc/fonts"
      "${HG_APP_ASSETS_FONT_DIR}"
  )

  add_custom_target(prepare_app_assets_data
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${HG_APP_ASSETS_DATA_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/test/data/master.css"
      "${HG_APP_ASSETS_DATA_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/test/data/test.html"
      "${HG_APP_ASSETS_DATA_DIR}"
  )
  
  add_dependencies(${lib_NAME}
    prepare_app_assets_fonts
    prepare_app_assets_font_conf
    prepare_app_assets_data
  )
endif()
