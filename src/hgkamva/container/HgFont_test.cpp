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

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <cairo/cairo.h>

#include "gtest/gtest.h"

#include "hgkamva/container/HgCairo.h"
#include "hgkamva/container/HgFont.h"
#include "hgkamva/container/HgFontLibrary.h"
#include "hgkamva/util/FileUtil.h"
#include "hgkamva/util/StringUtil.h"

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

  //// Cairo init.

  enum
  {
    //BYTES_PER_PIXEL = 3
    BYTES_PER_PIXEL = 4
  };

  const cairo_format_t colorFormat = CAIRO_FORMAT_ARGB32;
  const unsigned int frameWidth = 250;
  const unsigned int frameHeight = 50;
  //const int stride = frameWidth * BYTES_PER_PIXEL;
  const int stride = cairo_format_stride_for_width(colorFormat, frameWidth);

  std::vector<unsigned char> frameBuf(stride * frameHeight);

  hg::HgCairoPtr cairo = std::make_shared<hg::HgCairo>(
      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride);
  cairo->clear(hg::HgCairo::Color{0, 0, 0, 1});

  //// HtmlGrapheasKamva init.

  // Init HgFontLibrary.
  hg::HgFontLibrary hgFontLibrary;

  std::filesystem::path fontConfFile = fontDir / "fonts.conf";
  EXPECT_TRUE(std::filesystem::exists(fontConfFile));
  std::string fontConfig = hg::util::readFile(fontConfFile);
  EXPECT_FALSE(fontConfig.empty());

  EXPECT_TRUE(hgFontLibrary.parseAndLoadConfigFromMemory(fontConfig, true));

  EXPECT_TRUE(hgFontLibrary.addFontDir(fontDir));

  // NOTE: px = pt * DPI / 72
  int pixelSize = 16;
  int weight = 400;
  litehtml::font_style fontStyle = litehtml::font_style::fontStyleNormal;

  uint_least8_t result;
  std::filesystem::path filePath = hgFontLibrary.getFontFilePath(
      "Tinos", pixelSize, weight, fontStyle, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-Regular.ttf");

  // Create HgFont with HgFontLibrary.
  hg::HgFont hgFont(cairo, hgFontLibrary.ftLibrary(), 1000);

  //////// Test HgFont::createFtFace().

  EXPECT_TRUE(hgFont.createFtFace(filePath, pixelSize));

  //////// Set text for rendering.

  std::string text = "This is some english text";

  //////// Set HarfBuzz params.

  hgFont.setDirection(HB_DIRECTION_LTR);
  hgFont.setScript(HB_SCRIPT_LATIN);
  hgFont.setLanguage("eng");

  //////// Test HgFont::getBbox().

  hg::HgCairo::TextExtentsPtr extents = hgFont.getTextExtents(text);
  EXPECT_DOUBLE_EQ(extents->x_bearing, 0);
  EXPECT_DOUBLE_EQ(extents->y_bearing, -11);
  EXPECT_DOUBLE_EQ(extents->width, 159.765625);
  EXPECT_DOUBLE_EQ(extents->height, 14);
  EXPECT_DOUBLE_EQ(extents->x_advance, 158.765625);
  EXPECT_DOUBLE_EQ(extents->y_advance, 0);

  //////// Test HgFont::drawText().

  // Set text position.
  int x = 10;
  //int y = 10 + hgFont.getScaledFontExtents().ascent;
  int y = frameHeight - 10 - hgFont.getScaledFontExtents().descent;

  // Set text color.
  litehtml::web_color color(128, 128, 128, 255);

  // drawText()
  hgFont.drawText(text, x, y, color);

  // Write our picture to file.
  std::string fileName1 = "HgFontTest_1.ppm";
  std::filesystem::path fileOutTest1 = testDir / fileName1;
  hg::util::writePpmFile(
      frameBuf.data(), frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1);

  // Compare our file with prototype.
  std::filesystem::path fileTest1 = dataDir / fileName1;
  EXPECT_TRUE(hg::util::compareFiles(fileTest1, fileOutTest1));

  //////// Repeat tests for new text.

  text = "some english";

  // Make cleaning before new text.
  hgFont.clearBuffer();
  cairo->clear(hg::HgCairo::Color{0, 0, 0, 1});

  // Set HarfBuzz params.
  hgFont.setDirection(HB_DIRECTION_LTR);
  hgFont.setScript(HB_SCRIPT_LATIN);
  hgFont.setLanguage("eng");

  // getBbox().
  extents = hgFont.getTextExtents(text);
  EXPECT_DOUBLE_EQ(extents->x_bearing, 0);
  EXPECT_DOUBLE_EQ(extents->y_bearing, -11);
  EXPECT_DOUBLE_EQ(extents->width, 84.046875);
  EXPECT_DOUBLE_EQ(extents->height, 14);
  EXPECT_DOUBLE_EQ(extents->x_advance, 84.046875);
  EXPECT_DOUBLE_EQ(extents->y_advance, 0);

  // drawText().
  hgFont.drawText(text, x, y, color);

  // Write our picture to file.
  std::string fileName2 = "HgFontTest_2.ppm";
  std::filesystem::path fileOutTest2 = testDir / fileName2;
  hg::util::writePpmFile(frameBuf.data(), frameWidth, frameHeight,
      BYTES_PER_PIXEL, fileOutTest2.c_str());

  // Compare our file with prototype.
  std::filesystem::path fileTest2 = dataDir / fileName2;
  EXPECT_TRUE(hg::util::compareFiles(fileTest2, fileOutTest2));

  //////// Test HgFont::xHeight().

  EXPECT_DOUBLE_EQ(hgFont.xHeight(), 8.0);
}
