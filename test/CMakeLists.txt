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

  set(HGKamva_TEST_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  set(HGKamva_TEST_FONT_DIR "${HGKamva_TEST_DIR}/fonts")
  set(HGKamva_TEST_DATA_DIR "${HGKamva_TEST_DIR}/data")
  
  add_custom_target(clean_test_temp_dir
    COMMAND
      ${CMAKE_COMMAND} -E remove_directory "${HGKamva_TEST_DIR}/temp"
  )

  add_custom_target(prepare_test_fonts
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${HGKamva_TEST_FONT_DIR}"

    # Arimo font files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${PROJECT_SOURCE_DIR}/fonts/Arimo/Arimo-BoldItalic.ttf"
      "${HGKamva_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${PROJECT_SOURCE_DIR}/fonts/Arimo/Arimo-Bold.ttf"
      "${HGKamva_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${PROJECT_SOURCE_DIR}/fonts/Arimo/Arimo-Italic.ttf"
      "${HGKamva_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${PROJECT_SOURCE_DIR}/fonts/Arimo/Arimo-Regular.ttf"
      "${HGKamva_TEST_FONT_DIR}"

    # Tinos font files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${PROJECT_SOURCE_DIR}/fonts/Tinos/Tinos-BoldItalic.ttf"
      "${HGKamva_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${PROJECT_SOURCE_DIR}/fonts/Tinos/Tinos-Bold.ttf"
      "${HGKamva_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${PROJECT_SOURCE_DIR}/fonts/Tinos/Tinos-Italic.ttf"
      "${HGKamva_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${PROJECT_SOURCE_DIR}/fonts/Tinos/Tinos-Regular.ttf"
      "${HGKamva_TEST_FONT_DIR}"
  )
  
  add_custom_target(prepare_test_font_conf
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_INSTALL_PREFIX}/etc/fonts"
      "${HGKamva_TEST_FONT_DIR}"
  )

  set(HGKamva_TEST_GRAPHICS_DIR "${CMAKE_CURRENT_LIST_DIR}/data")
  if(MINGW)
    set(HGKamva_TEST_GRAPHICS_DIR "${HGKamva_TEST_GRAPHICS_DIR}/MinGW")
  endif()
  
  add_custom_target(prepare_test_data
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${HGKamva_TEST_DATA_DIR}"

    # Graphic data
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_TEST_GRAPHICS_DIR}/HgContainer_1.ppm"
      "${HGKamva_TEST_DATA_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_TEST_GRAPHICS_DIR}/HgContainer_2.ppm"
      "${HGKamva_TEST_DATA_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_TEST_GRAPHICS_DIR}/HgFontTest_1.ppm"
      "${HGKamva_TEST_DATA_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_TEST_GRAPHICS_DIR}/HgFontTest_2.ppm"
      "${HGKamva_TEST_DATA_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_TEST_GRAPHICS_DIR}/HtmlDocument_1.ppm"
      "${HGKamva_TEST_DATA_DIR}"

    # master.css
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_LIST_DIR}/data/master.css"
      "${HGKamva_TEST_DATA_DIR}"
  )

  macro(add_hg_test test_NAME test_SRC_FILES test_LIBS)
    add_executable(${test_NAME} "")
    target_sources(${test_NAME}
      PRIVATE
        ${test_SRC_FILES}
    )
    set_target_properties(${test_NAME} PROPERTIES
      CXX_STANDARD 11
      C_STANDARD 11
    )
    target_link_libraries(${test_NAME} PRIVATE hgraph)
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
        HGKamva_TEST_DIR=${HGKamva_TEST_DIR}
    )
    set_property(TEST ${test_NAME} APPEND PROPERTY
      ENVIRONMENT
        HGKamva_TEST_FONT_DIR=${HGKamva_TEST_FONT_DIR}
    )
    set_property(TEST ${test_NAME} APPEND PROPERTY
      ENVIRONMENT
        HGKamva_TEST_DATA_DIR=${HGKamva_TEST_DATA_DIR}
    )
  endmacro()

  # StringUtil tests.
  add_hg_test("StringUtil_test"
    ${private_src_DIR}/hgkamva/util/StringUtil_test.cpp
    hgkamva
  )

  # HgFontLibrary tests.
  add_hg_test("HgFontLibrary_test"
    ${private_src_DIR}/hgkamva/container/HgFontLibrary_test.cpp
    hgkamva
  )

  # HgFont tests.
  add_hg_test("HgFont_test"
    ${private_src_DIR}/hgkamva/container/HgFont_test.cpp
    hgkamva
  )

  # HgContainer tests.
  add_hg_test("HgContainer_test"
    ${private_src_DIR}/hgkamva/container/HgContainer_test.cpp
    hgkamva
  )

  # HgAggHtmlRenderer tests.
  add_hg_test("HgAggHtmlRenderer_test"
    ${private_src_DIR}/hgkamva/renderer/HgAggHtmlRenderer_test.cpp
    hgkamva
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
