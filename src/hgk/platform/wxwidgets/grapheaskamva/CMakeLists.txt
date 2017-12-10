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

project(HtmlGrapheasKamvaWxWidgets CXX)
set(lib_NAME "grapheaskamva")

set(lib_HEADERS
  HtmlGrapheasKamvaWx.h
  PixelFormatConvertor.h
)

set(lib_SOURCES
  HtmlGrapheasKamvaWx.cpp
)

add_library(${lib_NAME} ${lib_SOURCES} ${lib_HEADERS})
set_property(TARGET ${lib_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${lib_NAME} PROPERTY C_STANDARD 11)
# Export HtmlGrapheasKamvaWxWidgets includes.
target_include_directories(${lib_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

# HtmlGrapheasKamva
target_link_libraries(${lib_NAME} PRIVATE htmlgrapheaskamva)

# wxWidgets
if(USING_WX_SUB_DIR)
  target_link_libraries(${lib_NAME} PRIVATE ${WX_lib_COMPONENTS})
else()
  target_compile_definitions(${lib_NAME} PRIVATE ${wxWidgets_DEFINITIONS})
  target_include_directories(${lib_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
  target_link_libraries(${lib_NAME} PRIVATE ${wxWidgets_LIBRARIES})
  # TODO:
  #COMPILE_DEFINITIONS_DEBUG ${wxWidgets_DEFINITIONS_DEBUG}
  #wxWidgets_CXX_FLAGS
endif()
