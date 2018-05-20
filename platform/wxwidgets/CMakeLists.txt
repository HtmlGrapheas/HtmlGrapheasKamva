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

project(HtmlGrapheasKamvaWx CXX)
set(lib_NAME "htmlgrapheaskamvawx")
set(src_DIR "${PROJECT_SOURCE_DIR}/src")

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
# wxWidgets platform
#-----------------------------------------------------------------------

set(lib_HEADERS
  ${src_DIR}/hgkamva/platform/wxwidgets/HgKamvaWxWindow.h
  ${src_DIR}/hgkamva/platform/wxwidgets/wxPixelFormatToHgHtmlRenderer.h
)

set(lib_SOURCES
  ${src_DIR}/hgkamva/platform/wxwidgets/HgKamvaWxWindow.cpp
)

add_library(${lib_NAME} ${lib_SOURCES} ${lib_HEADERS})
set_target_properties(${lib_NAME} PROPERTIES
  CXX_STANDARD 11
  C_STANDARD 11
)
# Export HtmlGrapheasKamvaWx includes.
target_include_directories(${lib_NAME} PUBLIC ${src_DIR})

# HtmlGrapheasKamva
target_link_libraries(${lib_NAME} PUBLIC htmlgrapheaskamva)

# wxWidgets
if(WX_USE_FIND_PACKAGE_MODULE)
  target_compile_definitions(${lib_NAME} PUBLIC ${wxWidgets_DEFINITIONS})
  target_include_directories(${lib_NAME} PUBLIC ${wxWidgets_INCLUDE_DIRS})
  target_link_libraries(${lib_NAME} PUBLIC ${wxWidgets_LIBRARIES})
  # TODO:
  #COMPILE_DEFINITIONS_DEBUG ${wxWidgets_DEFINITIONS_DEBUG}
  #wxWidgets_CXX_FLAGS
else()
  target_link_libraries(${lib_NAME} PUBLIC ${WX_lib_COMPONENTS})
endif()
