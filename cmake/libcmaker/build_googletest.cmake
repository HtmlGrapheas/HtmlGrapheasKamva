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

include(cmr_print_status)

#-----------------------------------------------------------------------
# Build, install and find Google Test library
#-----------------------------------------------------------------------

# Add this in the project's root CMakeLists.txt.
#option(BUILD_TESTING "Build the testing tree." ON)
#if(BUILD_TESTING)
#  enable_testing()
#endif()


#-----------------------------------------------------------------------
# Set vars for LibCMaker_GoogleTest.
#-----------------------------------------------------------------------

set(LIBCMAKER_GOOGLETEST_SRC_DIR
  "${CMAKE_CURRENT_LIST_DIR}/LibCMaker_GoogleTest"
)
# To use our FindGTest.cmake.
list(APPEND CMAKE_MODULE_PATH "${LIBCMAKER_GOOGLETEST_SRC_DIR}/cmake")

set(GOOGLETEST_lib_VERSION    "1.8.20180314")
set(GOOGLETEST_DOWNLOAD_DIR   "${EXTERNAL_DOWNLOAD_DIR}")
set(GOOGLETEST_UNPACKED_DIR   "${EXTERNAL_UNPACKED_DIR}")
set(GOOGLETEST_BUILD_DIR      "${EXTERNAL_BIN_DIR}/build_googletest")

# Library specific vars and options.

#-----------------------------------------------------------------------
# Common Google Test and Google Mock options
#
option(BUILD_GTEST "Builds the googletest subproject" ON)
#Note that googlemock target already builds googletest
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

#-----------------------------------------------------------------------
# Google Test options
#
# When other libraries are using a shared version of runtime libraries,
# Google Test also has to use one.
option(
  gtest_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  ON
)
option(gtest_build_tests "Build all of gtest's own tests." OFF)
option(gtest_build_samples "Build gtest's sample programs." OFF)
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." OFF)
option(
  gtest_hide_internal_symbols
  "Build gtest with internal symbols hidden in shared libraries."
  OFF
)

#-----------------------------------------------------------------------
# Google Mock options
#
option(gmock_build_tests "Build all of Google Mock's own tests." OFF)


#-----------------------------------------------------------------------
# Build and install the Google Test
#-----------------------------------------------------------------------

# From "FindGTest.cmake":
#   If compiling with MSVC, this variable can be set to ``MT`` or
#   ``MD`` (the default) to enable searching a GTest build tree
if(MSVC)
  set(GTEST_MSVC_SEARCH "MT")
endif()

# Try to find already installed lib.
find_package(GTest QUIET)

if(NOT GTEST_FOUND)
  cmr_print_status(
    "Google Test is not installed, build and install it.")

  include(
    ${LIBCMAKER_GOOGLETEST_SRC_DIR}/lib_cmaker_googletest.cmake)
  lib_cmaker_googletest(
    VERSION       ${GOOGLETEST_lib_VERSION}
    DOWNLOAD_DIR  ${GOOGLETEST_DOWNLOAD_DIR}
    UNPACKED_DIR  ${GOOGLETEST_UNPACKED_DIR}
    BUILD_DIR     ${GOOGLETEST_BUILD_DIR}
  )

  find_package(GTest REQUIRED)

else()
  cmr_print_status(
    "Google Test is installed, skip building and installing it.")
endif()
