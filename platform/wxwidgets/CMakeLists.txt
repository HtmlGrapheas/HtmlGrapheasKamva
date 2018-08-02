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

project(HtmlGrapheasKamvaWx CXX)
set(lib_NAME "hgkamva_wx")


#-----------------------------------------------------------------------
# HtmlGrapheasKamvaWx
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
# Export HtmlGrapheasKamvaWx include dirs.
target_include_directories(${lib_NAME} PUBLIC ${public_src_DIR})

target_sources(${lib_NAME}
  PRIVATE
    ${private_src_DIR}/hgkamva/platform/wxwidgets/HgKamvaWxWindow.cpp

  PUBLIC
    # TODO: all are public?
    ${public_src_DIR}/hgkamva/platform/wxwidgets/HgKamvaWxWindow.h
    ${public_src_DIR}/hgkamva/platform/wxwidgets/wxPixelFormatToHgHtmlRenderer.h
)


#-----------------------------------------------------------------------
# Link to the libraries
#-----------------------------------------------------------------------

# HtmlGrapheasKamva
target_link_libraries(${lib_NAME} PUBLIC hgkamva)

# wxWidgets
if(WX_USE_FIND_PACKAGE_MODULE)
  target_compile_definitions(${lib_NAME} PRIVATE ${wxWidgets_DEFINITIONS})
  target_include_directories(${lib_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
  target_link_libraries(${lib_NAME} PRIVATE ${wxWidgets_LIBRARIES})
  # TODO:
  #COMPILE_DEFINITIONS_DEBUG ${wxWidgets_DEFINITIONS_DEBUG}
  #wxWidgets_CXX_FLAGS
else()
  target_link_libraries(${lib_NAME} PRIVATE ${WX_lib_COMPONENTS})
endif()
