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

  set(HGRAPH_TEST_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  set(HGRAPH_TEST_FONT_DIR "${HGRAPH_TEST_DIR}/fonts")
  set(HGRAPH_TEST_DATA_DIR "${HGRAPH_TEST_DIR}/data")
  
  add_custom_target(clean_test_temp_dir ALL
    COMMAND
      ${CMAKE_COMMAND} -E remove_directory "${HGRAPH_TEST_DIR}/temp"
  )

  add_custom_target(prepare_test_fonts ALL
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${HGRAPH_TEST_FONT_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      # Arimo font files
      "${PROJECT_SOURCE_DIR}/fonts/Arimo/Arimo-BoldItalic.ttf"
      "${PROJECT_SOURCE_DIR}/fonts/Arimo/Arimo-Bold.ttf"
      "${PROJECT_SOURCE_DIR}/fonts/Arimo/Arimo-Italic.ttf"
      "${PROJECT_SOURCE_DIR}/fonts/Arimo/Arimo-Regular.ttf"

      # Tinos font files
      "${PROJECT_SOURCE_DIR}/fonts/Tinos/Tinos-BoldItalic.ttf"
      "${PROJECT_SOURCE_DIR}/fonts/Tinos/Tinos-Bold.ttf"
      "${PROJECT_SOURCE_DIR}/fonts/Tinos/Tinos-Italic.ttf"
      "${PROJECT_SOURCE_DIR}/fonts/Tinos/Tinos-Regular.ttf"

      # To dir 'fonts'
      "${HGRAPH_TEST_FONT_DIR}"
  )
  
  add_custom_target(prepare_test_font_conf ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_INSTALL_PREFIX}/etc/fonts"

      # To dir 'fonts'
      "${HGRAPH_TEST_FONT_DIR}"
  )

  set(HGRAPH_TEST_GRAPHICS_DIR "${PROJECT_SOURCE_DIR}/test/data")
  if(MINGW)
    set(HGRAPH_TEST_GRAPHICS_DIR "${HGRAPH_TEST_GRAPHICS_DIR}/MinGW")
  endif()
  
  add_custom_target(prepare_test_data ALL
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${HGRAPH_TEST_DATA_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      # Graphic data
      "${HGRAPH_TEST_GRAPHICS_DIR}/HgContainer_1.ppm"
      "${HGRAPH_TEST_GRAPHICS_DIR}/HgContainer_2.ppm"
      "${HGRAPH_TEST_GRAPHICS_DIR}/HgFontTest_1.ppm"
      "${HGRAPH_TEST_GRAPHICS_DIR}/HgFontTest_2.ppm"
      "${HGRAPH_TEST_GRAPHICS_DIR}/HtmlDocument_1.ppm"

      # master.css
      "${PROJECT_SOURCE_DIR}/test/data/master.css"

      # To dir 'data'
      "${HGRAPH_TEST_DATA_DIR}"
  )

  macro(add_hg_test test_NAME test_SRC_FILES test_LIBS)
    add_executable(${test_NAME} ${test_SRC_FILES})
    set_target_properties(${test_NAME} PROPERTIES
      CXX_STANDARD 11
      C_STANDARD 11
    )
    target_link_libraries(${test_NAME} PRIVATE htmlgrapheas)
    target_include_directories(${test_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
    if(WITH_HarfBuzz)
      target_include_directories(${test_NAME} PRIVATE ${HARFBUZZ_INCLUDE_DIRS})
    endif()
    target_include_directories(${test_NAME} PRIVATE ${AGG_INCLUDE_DIRS})
    target_link_libraries(${test_NAME} PRIVATE
      ${test_LIBS}
      GTest::GTest GTest::Main
    )
    
    add_test(NAME ${test_NAME} COMMAND ${test_NAME})
    
    # https://cmake.org/pipermail/cmake-developers/2013-June/019217.html
    set_property(TEST ${test_NAME} PROPERTY
      ENVIRONMENT
        HGRAPH_TEST_DIR=${HGRAPH_TEST_DIR}
    )
    set_property(TEST ${test_NAME} APPEND PROPERTY
      ENVIRONMENT
        HGRAPH_TEST_FONT_DIR=${HGRAPH_TEST_FONT_DIR}
    )
    set_property(TEST ${test_NAME} APPEND PROPERTY
      ENVIRONMENT
        HGRAPH_TEST_DATA_DIR=${HGRAPH_TEST_DATA_DIR}
    )
  endmacro()

  # StringUtil tests.
  add_hg_test("StringUtil_test"
    ${hgk_SRC_DIR}/hgkamva/util/StringUtil_test.cpp
    htmlgrapheaskamva
  )

  # HgFontLibrary tests.
  add_hg_test("HgFontLibrary_test"
    ${hgk_SRC_DIR}/hgkamva/container/HgFontLibrary_test.cpp
    htmlgrapheaskamva
  )

  # HgFont tests.
  add_hg_test("HgFont_test"
    ${hgk_SRC_DIR}/hgkamva/container/HgFont_test.cpp
    htmlgrapheaskamva
  )

  # HgContainer tests.
  add_hg_test("HgContainer_test"
    ${hgk_SRC_DIR}/hgkamva/container/HgContainer_test.cpp
    htmlgrapheaskamva
  )

  # HgAggHtmlRenderer tests.
  add_hg_test("HgAggHtmlRenderer_test"
    ${hgk_SRC_DIR}/hgkamva/renderer/HgAggHtmlRenderer_test.cpp
    htmlgrapheaskamva
  )

endif()


# For running test, 1st build all:
#   cmake .. -DBUILD_TESTING=ON
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
