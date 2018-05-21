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

project(HtmlGrapheasKamvaAndroid CXX)
set(lib_NAME "htmlgrapheaskamvaandroid")
set(src_DIR "${PROJECT_SOURCE_DIR}/src")


#-----------------------------------------------------------------------
# Configure external libs directories.
#-----------------------------------------------------------------------

if(NOT EXTERNAL_INSTALL_DIR)
  set(EXTERNAL_INSTALL_DIR "${PROJECT_BINARY_DIR}/install")
endif()
set(CMAKE_INSTALL_PREFIX "${EXTERNAL_INSTALL_DIR}")


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

set(lib_HEADERS
)

set(lib_SOURCES
  ${src_DIR}/hgkamva/platform/android/HgKamvaApiJni.cpp
)

add_library(${lib_NAME} SHARED ${lib_SOURCES} ${lib_HEADERS})
set_target_properties(${lib_NAME} PROPERTIES
  CXX_STANDARD 11
  C_STANDARD 11
)

# HtmlGrapheasKamva
target_link_libraries(${lib_NAME} PRIVATE htmlgrapheaskamva)

# Android libs
target_link_libraries(${lib_NAME} PRIVATE android jnigraphics)

if(BUILD_SHARED_LIBS)
  # Copy installed external so-libs to include to apk-file.
  # We needed to make a copying twice
  # because of the behavior of the android gradle.
  add_custom_target(copy_installed_so_libs_before ALL
    COMMAND ${CMAKE_COMMAND} -E copy
      "*.so"
      "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    WORKING_DIRECTORY
      "${EXTERNAL_INSTALL_DIR}/lib"
  )
  add_dependencies(${lib_NAME} copy_installed_so_libs_before)
  
  add_custom_target(copy_installed_so_libs_after ALL
    COMMAND ${CMAKE_COMMAND} -E copy
      "*.so"
      "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    WORKING_DIRECTORY
      "${EXTERNAL_INSTALL_DIR}/lib"
  )
  add_dependencies(copy_installed_so_libs_after ${lib_NAME})
endif()


#-----------------------------------------------------------------------
# Android app assets data.
#-----------------------------------------------------------------------

if(HG_APP_ASSETS_DIR)
  set(HG_APP_ASSETS_FONT_DIR "${HG_APP_ASSETS_DIR}/fonts")
  set(HG_APP_ASSETS_DATA_DIR "${HG_APP_ASSETS_DIR}/data")

  add_custom_target(prepare_app_assets_fonts ALL
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${HG_APP_ASSETS_FONT_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      # Arimo font files
      "${HtmlGrapheasKamva_DIR}/fonts/Arimo/Arimo-BoldItalic.ttf"
      "${HtmlGrapheasKamva_DIR}/fonts/Arimo/Arimo-Bold.ttf"
      "${HtmlGrapheasKamva_DIR}/fonts/Arimo/Arimo-Italic.ttf"
      "${HtmlGrapheasKamva_DIR}/fonts/Arimo/Arimo-Regular.ttf"

      # Tinos font files
      "${HtmlGrapheasKamva_DIR}/fonts/Tinos/Tinos-BoldItalic.ttf"
      "${HtmlGrapheasKamva_DIR}/fonts/Tinos/Tinos-Bold.ttf"
      "${HtmlGrapheasKamva_DIR}/fonts/Tinos/Tinos-Italic.ttf"
      "${HtmlGrapheasKamva_DIR}/fonts/Tinos/Tinos-Regular.ttf"

      # To dir 'fonts'
      "${HG_APP_ASSETS_FONT_DIR}"
  )

  add_custom_target(prepare_app_assets_font_conf ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${EXTERNAL_INSTALL_DIR}/etc/fonts"

      # To dir 'fonts'
      "${HG_APP_ASSETS_FONT_DIR}"
  )

  add_custom_target(prepare_app_assets_data ALL
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${HG_APP_ASSETS_DATA_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HtmlGrapheasKamva_DIR}/test/data/master.css"
      "${HtmlGrapheasKamva_DIR}/test/data/test.html"

      # To dir 'data'
      "${HG_APP_ASSETS_DATA_DIR}"
  )
  
  add_dependencies(${lib_NAME}
    prepare_app_assets_fonts
    prepare_app_assets_font_conf
    prepare_app_assets_data
  )
endif()
