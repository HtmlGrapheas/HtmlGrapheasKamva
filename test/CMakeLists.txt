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

# Testing with Google Test.
if(BUILD_TESTING)
  include(build_googletest)

  # TODO: rewrite AGG's CMake scripts to exclude link_directories().
  # AGG_LIBRARY_DIRS is cache var from AggConfig.cmake
  # by find_package(Agg REQUIRED CONFIG) in HtmlGrapheasKamva.
  link_directories(${AGG_LIBRARY_DIRS})

  # HgFont tests.
  set(test_NAME "HgFont_test")
  add_executable(${test_NAME}
    ${HtmlGrapheas_DIR}/src/hg/containers/agg/HgFont_test.cpp
  )
  target_link_libraries(${test_NAME} PRIVATE
    htmlgrapheaskamva
    GTest::GTest GTest::Main
  )
  add_test(NAME ${test_NAME} COMMAND ${test_NAME})

  # AggContainer tests.
#  set(test_NAME "AggContainer_test")
#  add_executable(${test_NAME}
#    ${HtmlGrapheas_DIR}/src/hg/containers/agg/AggContainer_test.cpp
#  )
#  target_link_libraries(${test_NAME} PRIVATE
#    htmlgrapheaskamva
#    GTest::GTest GTest::Main
#  )
#  add_test(NAME ${test_NAME} COMMAND ${test_NAME})
endif()


# For running test, 1st build all:
#   cmake --build .
# Then run test with this command:
#   cmake -E env CTEST_OUTPUT_ON_FAILURE=1 cmake --build . --target test
# Or with this command:
#   ctest --verbose
# Env var CTEST_OUTPUT_ON_FAILURE or key --verbose are for show test outputs,
# see
# https://stackoverflow.com/a/37123943
# https://stackoverflow.com/a/38386596

# For Visual Studio:
# https://stackoverflow.com/a/31124523
# https://stackoverflow.com/a/37123943
# cmake -E env CTEST_OUTPUT_ON_FAILURE=1 cmake --build . --target RUN_TESTS
