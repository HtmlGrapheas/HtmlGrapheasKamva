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

#include <string>

#include "gtest/gtest.h"

#include "agg_pixfmt_rgb.h"
//#include "agg_pixfmt_rgba.h"

#include "hgkamva/agg/container/HgAggPainter.h"
#include "hgkamva/container/HgFontLibrary.h"
#include "hgkamva/util/FileUtil.h"
#include "hgkamva/util/StringUtil.h"

#include <filesystem>
#include <string>

inline std::filesystem::path testDir;
inline std::filesystem::path fontDir;
inline std::filesystem::path dataDir;

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);

  // https://stackoverflow.com/a/55579815
  testDir =
      std::filesystem::absolute(std::filesystem::path(argv[0]).parent_path());
  fontDir = testDir / "fonts";
  dataDir = testDir / "data";

  return RUN_ALL_TESTS();
}

TEST(HgFontTest, HgFontTest)
{
  //////// Init part.

  EXPECT_TRUE(std::filesystem::exists(testDir));
  EXPECT_TRUE(std::filesystem::exists(fontDir));
  EXPECT_TRUE(std::filesystem::exists(dataDir));

  //// AGG init.

  enum
  {
    BYTES_PER_PIXEL = 3
    //BYTES_PER_PIXEL = 4
  };

  // The AGG pixel format.
  using PixelFormat = agg::pixfmt_rgb24;
  //using PixelFormat = agg::pixfmt_rgba32;

  //int frameWidth = bbox->mBboxW + 20;
  //int frameHeight = bbox->mBboxH + 20;
  int frameWidth = 250;
  int frameHeight = 50;
  int stride = frameWidth * BYTES_PER_PIXEL;

  unsigned char* frameBuf = new unsigned char[stride * frameHeight];
  EXPECT_NE(frameBuf, nullptr);

  hg::HgAggPainter<PixelFormat> hgAggPainter;
  hgAggPainter.attach(frameBuf, frameWidth, frameHeight, stride);

  litehtml::web_color backgroundColor(0, 0, 0);
  hgAggPainter.setPaintColor(backgroundColor);
  hgAggPainter.clear();

  //// HtmlGrapheasKamva init.

  // Init HgFontLibrary.
  hg::HgFontLibrary hgFontLibrary;

  std::filesystem::path fontConfFile = fontDir / "fonts.conf";
  EXPECT_TRUE(std::filesystem::exists(fontConfFile));
  std::string fontConfig = hg::FileUtil::readFile(fontConfFile);
  EXPECT_GE(fontConfig.size(), 0);

  EXPECT_TRUE(hgFontLibrary.parseAndLoadConfigFromMemory(fontConfig, true));

  EXPECT_TRUE(hgFontLibrary.addFontDir(fontDir));

  int pixelSize = 16;
  int weight = 400;
  litehtml::font_style fontStyle = litehtml::font_style::fontStyleNormal;

  uint_least8_t result;
  std::string filePath = hgFontLibrary.getFontFilePath(
      "Tinos", pixelSize, weight, fontStyle, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(hg::StringUtil::endsWith(filePath, "Tinos-Regular.ttf"));

  // Create HgFont with HgFontLibrary.
  hg::HgFont hgFont(hgFontLibrary.ftLibrary(), 1000);

  //////// Test HgFont::createFtFace().

  EXPECT_TRUE(hgFont.createFtFace(filePath, pixelSize));

  //////// Set text for rendering.

  std::string text = "This is some english text";

  //////// Set HarfBuzz params.

  hgFont.setDirection(HB_DIRECTION_LTR);
  hgFont.setScript(HB_SCRIPT_LATIN);
  hgFont.setLanguage("eng");

  //////// Test HgFont::getBbox().

  hg::HgFont::TextBboxPtr bbox = hgFont.getBbox(text);
  EXPECT_EQ(bbox->mMinX, 0);
  EXPECT_EQ(bbox->mMaxX, 155);
  EXPECT_EQ(bbox->mMinY, -3);
  EXPECT_EQ(bbox->mMaxY, 10);
  EXPECT_EQ(bbox->mBboxW, 155);
  EXPECT_EQ(bbox->mBboxH, 13);
  EXPECT_EQ(bbox->mBaselineShift, 0);
  EXPECT_EQ(bbox->mBaselineOffset, 10);

  //////// Test HgFont::drawText().

  // Set text position.
  FT_Size ftSize = hgFont.ftFace()->size;
  int x = 10;
  //int ascender = hg::HgFont::f26Dot6ToInt(ftSize->metrics.ascender);
  //int y = 10 + ascender;
  int descent = hg::HgFont::f26Dot6ToInt(ftSize->metrics.descender);
  int y = frameHeight - 10 + descent;

  // Set text color.
  litehtml::web_color color(128, 128, 128, 255);

  // drawText()
  hgFont.drawText(text, &hgAggPainter, x, y, color);

  // Write our picture to file.
  std::string fileName1 = "HgFontTest_1.ppm";
  std::filesystem::path fileOutTest1 = testDir / fileName1;
  hg::FileUtil::writePpmFile(
      frameBuf, frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1.c_str());

  // Compare our file with prototype.
  std::filesystem::path fileTest1 = dataDir / fileName1;
  EXPECT_TRUE(hg::FileUtil::compareFiles(fileTest1, fileOutTest1));

  //////// Repeat tests for new text.

  text = "some english";

  // Make cleaning before new text.
  hgFont.clearBuffer();
  hgAggPainter.setPaintColor(backgroundColor);
  hgAggPainter.clear();

  // Set HarfBuzz params.
  hgFont.setDirection(HB_DIRECTION_LTR);
  hgFont.setScript(HB_SCRIPT_LATIN);
  hgFont.setLanguage("eng");

  // getBbox().
  bbox = hgFont.getBbox(text);
  EXPECT_EQ(bbox->mMinX, 0);
  EXPECT_EQ(bbox->mMaxX, 82);
  EXPECT_EQ(bbox->mMinY, -3);
  EXPECT_EQ(bbox->mMaxY, 10);
  EXPECT_EQ(bbox->mBboxW, 82);
  EXPECT_EQ(bbox->mBboxH, 13);
  EXPECT_EQ(bbox->mBaselineShift, 0);
  EXPECT_EQ(bbox->mBaselineOffset, 10);

  // drawText().
  hgFont.drawText(text, &hgAggPainter, x, y, color);

  // Write our picture to file.
  std::string fileName2 = "HgFontTest_2.ppm";
  std::filesystem::path fileOutTest2 = testDir / fileName2;
  hg::FileUtil::writePpmFile(
      frameBuf, frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest2.c_str());

  // Compare our file with prototype.
  std::filesystem::path fileTest2 = dataDir / fileName2;
  EXPECT_TRUE(hg::FileUtil::compareFiles(fileTest2, fileOutTest2));

  //////// Test HgFont::xHeight().

  EXPECT_EQ(hg::HgFont::f26Dot6ToInt(hgFont.xHeight()), 8);

  //////// Deinit part.

  EXPECT_TRUE(hgFont.destroyFtFace());
  delete[] frameBuf;
}
