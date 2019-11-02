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

cmake_minimum_required(VERSION 3.4)

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

if(WX_USE_FIND_PACKAGE_MODULE)
  find_package(wxWidgets REQUIRED)
endif()

# HtmlGrapheasKamva
target_link_libraries(${lib_NAME} PUBLIC hgkamva)

# wxWidgets
if(WX_USE_FIND_PACKAGE_MODULE)
  #include(${wxWidgets_USE_FILE})
  #target_link_libraries(${lib_NAME} ${wxWidgets_LIBRARIES})

  target_compile_definitions(${lib_NAME} PRIVATE ${wxWidgets_DEFINITIONS})
  target_compile_definitions(${lib_NAME} PRIVATE
      $<$<CONFIG:Debug>:${wxWidgets_DEFINITIONS_DEBUG}>
  )
  target_compile_options(${lib_NAME} PRIVATE ${wxWidgets_CXX_FLAGS})
  target_include_directories(${lib_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
  target_link_libraries(${lib_NAME} PRIVATE ${wxWidgets_LIBRARIES})

  if(MINGW AND NOT BUILD_SHARED_LIBS)
    if(NOT MINGW_HOME)
      message(FATAL_ERROR "Please set MINGW_HOME to MinGW home path.")
    endif()

    set(mingw_ARCH "x86_64")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
      set(mingw_ARCH "i686")
    endif()
    set(mingw_PREFIX "${mingw_ARCH}-w64-mingw32")
    list(APPEND CMAKE_PREFIX_PATH "${MINGW_HOME}/${mingw_PREFIX}")

    find_library(comdlg32_LIB comdlg32)
    if(comdlg32_LIB)
      target_link_libraries(${lib_NAME} PRIVATE ${comdlg32_LIB})
    endif()

    find_library(gdi32_LIB gdi32)
    if(gdi32_LIB)
      target_link_libraries(${lib_NAME} PRIVATE ${gdi32_LIB})
    endif()

    find_library(ole32_LIB ole32)
    if(ole32_LIB)
      target_link_libraries(${lib_NAME} PRIVATE ${ole32_LIB})
    endif()

    find_library(oleacc_LIB oleacc)
    if(oleacc_LIB)
      target_link_libraries(${lib_NAME} PRIVATE ${oleacc_LIB})
    endif()

    find_library(oleaut32_LIB oleaut32)
    if(oleaut32_LIB)
      target_link_libraries(${lib_NAME} PRIVATE ${oleaut32_LIB})
    endif()

    find_library(uuid_LIB uuid)
    if(uuid_LIB)
      target_link_libraries(${lib_NAME} PRIVATE ${uuid_LIB})
    endif()

    find_library(uxtheme_LIB uxtheme)
    if(uxtheme_LIB)
      target_link_libraries(${lib_NAME} PRIVATE ${uxtheme_LIB})
    endif()

    find_library(winspool_LIB winspool)
    if(winspool_LIB)
      target_link_libraries(${lib_NAME} PRIVATE ${winspool_LIB})
    endif()
  endif()
else()
  target_link_libraries(${lib_NAME} PRIVATE ${WX_lib_COMPONENTS})
endif()
