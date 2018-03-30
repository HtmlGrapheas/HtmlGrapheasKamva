/*****************************************************************************
 * Project:  HtmlGrapheas
 * Purpose:  HTML text editor library
 * Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *   Copyright (c) 2017-2018 NikitaFeodonit
 *
 *    This file is part of the HtmlGrapheas project.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published
 *    by the Free Software Foundation, either version 3 of the License,
 *    or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program. If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include "hgkamva/container/HgFont.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>

#include "gtest/gtest.h"

#include "agg_pixfmt_rgb.h"
#include "agg_renderer_base.h"

#include "hgkamva/container/HgFontLibrary.h"
#include "hgkamva/util/StringUtil.h"

// Writing the buffer to a .PPM file, assuming it has RGB-structure, one byte
// per color component.
// http://www.antigrain.com/doc/basic_renderers/basic_renderers.agdoc.html
bool writePpmFile(const unsigned char* buf,
    unsigned width,
    unsigned height,
    const char* file_name)
{
  FILE* fd = fopen(file_name, "wb");
  if(fd) {
    fprintf(fd, "P6 %d %d 255 ", width, height);
    fwrite(buf, 1, width * height * 3, fd);
    fclose(fd);
    return true;
  }
  return false;
}

//https://stackoverflow.com/a/37575457
bool compareFiles(const std::string& filePath1, const std::string& filePath2)
{
  std::ifstream f1(filePath1, std::ifstream::binary | std::ifstream::ate);
  std::ifstream f2(filePath2, std::ifstream::binary | std::ifstream::ate);

  if(f1.fail() || f2.fail()) {
    return false;  // File problem.
  }

  if(f1.tellg() != f2.tellg()) {
    return false;  // Size mismatch.
  }

  // Seek back to beginning and use std::equal to compare contents.
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
      std::istreambuf_iterator<char>(),
      std::istreambuf_iterator<char>(f2.rdbuf()));
}

TEST(HgFontTest, HgFontTest)
{
  //////// Init part.

  // Get dirs from env vars.
  const char* testDir = std::getenv("HGRAPH_TEST_DIR");
  EXPECT_NE(testDir, nullptr);
  const char* fontDir = std::getenv("HGRAPH_TEST_FONT_DIR");
  EXPECT_NE(fontDir, nullptr);
  const char* dataDir = std::getenv("HGRAPH_TEST_DATA_DIR");
  EXPECT_NE(dataDir, nullptr);

  //// AGG init.

  enum
  {
    BYTES_PER_PIXEL = 3
  };

  // The AGG pixel format.
  using PixelFormat = agg::pixfmt_rgb24;

  // The AGG base renderer.
  using RendererBase = agg::renderer_base<PixelFormat>;

  //int frameWidth = bbox.mBboxW + 20;
  //int frameHeight = bbox.mBboxH + 20;
  int frameWidth = 250;
  int frameHeight = 50;
  int stride = frameWidth * BYTES_PER_PIXEL;

  unsigned char* frameBuf = new unsigned char[frameWidth * frameHeight * 3];
  EXPECT_NE(frameBuf, nullptr);

  // Attach with constructor or with method attach().
  //agg::rendering_buffer rbuf(frameBuf, frameWidth, frameHeight, stride);
  agg::rendering_buffer rbuf;
  rbuf.attach(frameBuf, frameWidth, frameHeight, stride);

  PixelFormat pixf(rbuf);
  RendererBase rbase(pixf);
  rbase.clear(PixelFormat::color_type(0, 0, 0));

  //// HtmlGrapheasKamva init.

  // Create HgFontLibrary and HgFont with it.
  hg::HgFontLibrary hgFontLibrary;
  hg::HgFont<PixelFormat> hgFont(hgFontLibrary.ftLibrary());

  //////// Test HgFontLibrary::addFontDir().

  EXPECT_TRUE(hgFontLibrary.addFontDir(fontDir));

  //////// Test HgFontLibrary::getFontFilePath().

  int pixelSize = 16;
  int weight = 400;
  litehtml::font_style fontStyle = litehtml::font_style::fontStyleNormal;

  uint_least8_t result;
  std::string filePath = hgFontLibrary.getFontFilePath(
      "Tinos", pixelSize, weight, fontStyle, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(hg::StringUtil::endsWith(filePath, "Tinos-Regular.ttf"));

  //////// Test HgFont::createFtFace().

  EXPECT_TRUE(hgFont.createFtFace(filePath, pixelSize));

  //////// Perform HgFont::layoutText() before getBbox() and drawText().

  hgFont.setDirection(HB_DIRECTION_LTR);
  hgFont.setScript(HB_SCRIPT_LATIN);
  hgFont.setLanguage("eng");
  hgFont.layoutText("This is some english text");

  //////// Test HgFont::getBbox().

  hg::HgFont<PixelFormat>::TextBbox bbox = hgFont.getBbox();
  EXPECT_EQ(bbox.mMinX, 0);
  EXPECT_EQ(bbox.mMaxX, 155);
  EXPECT_EQ(bbox.mMinY, -3);
  EXPECT_EQ(bbox.mMaxY, 10);
  EXPECT_EQ(bbox.mBboxW, 155);
  EXPECT_EQ(bbox.mBboxH, 13);
  EXPECT_EQ(bbox.mBaselineShift, 0);
  EXPECT_EQ(bbox.mBaselineOffset, 10);

  //////// Test HgFont::drawText().

  // Set text position.
  FT_Size ftSize = hgFont.ftFace()->size;
  int descent =
      hg::HgFont<PixelFormat>::f26Dot6ToInt(ftSize->metrics.descender);
  int x = 10;
  int y = frameHeight - 10 + descent;

  // drawText()
  hgFont.drawText(&rbase, x, y);

  // Write our picture to file.
  std::string fileName1 = "HgFontTest_1.ppm";
  std::string fileOutTest1 = std::string(testDir) + "/" + fileName1;
  writePpmFile(frameBuf, frameWidth, frameHeight, fileOutTest1.c_str());

  // Compare our file with prototype.
  std::string fileTest1 = std::string(dataDir) + "/" + fileName1;
  EXPECT_TRUE(compareFiles(fileTest1, fileOutTest1));

  //////// Repeat tests for new text.

  // Make cleaning before new text.
  hgFont.clearBuffer();
  rbase.clear(PixelFormat::color_type(0, 0, 0));

  // layoutText().
  hgFont.setDirection(HB_DIRECTION_LTR);
  hgFont.setScript(HB_SCRIPT_LATIN);
  hgFont.setLanguage("eng");
  hgFont.layoutText("some english");

  // getBbox().
  bbox = hgFont.getBbox();
  EXPECT_EQ(bbox.mMinX, 0);
  EXPECT_EQ(bbox.mMaxX, 82);
  EXPECT_EQ(bbox.mMinY, -3);
  EXPECT_EQ(bbox.mMaxY, 10);
  EXPECT_EQ(bbox.mBboxW, 82);
  EXPECT_EQ(bbox.mBboxH, 13);
  EXPECT_EQ(bbox.mBaselineShift, 0);
  EXPECT_EQ(bbox.mBaselineOffset, 10);

  // drawText().
  hgFont.drawText(&rbase, x, y);

  // Write our picture to file.
  std::string fileName2 = "HgFontTest_2.ppm";
  std::string fileOutTest2 = std::string(testDir) + "/" + fileName2;
  writePpmFile(frameBuf, frameWidth, frameHeight, fileOutTest2.c_str());

  // Compare our file with prototype.
  std::string fileTest2 = std::string(dataDir) + "/" + fileName2;
  EXPECT_TRUE(compareFiles(fileTest2, fileOutTest2));

  //////// Test HgFont::xHeight().

  EXPECT_EQ(hg::HgFont<PixelFormat>::f26Dot6ToInt(hgFont.xHeight()), 8);

  //////// Deinit part.

  EXPECT_TRUE(hgFont.destroyFtFace());
  delete[] frameBuf;
}
