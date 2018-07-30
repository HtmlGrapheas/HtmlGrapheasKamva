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

#include "hgkamva/container/HgContainer.h"

#include "gtest/gtest.h"

#include "agg_pixfmt_rgb.h"
//#include "agg_pixfmt_rgba.h"

#include "hgkamva/container/HgAggPainter.h"
#include "hgkamva/util/FileUtil.h"

TEST(HgContainerTest, create_font)
{
  const char* fontDir = std::getenv("HGKamva_TEST_FONT_DIR");
  EXPECT_NE(fontDir, nullptr);

  std::string fontConfFile = std::string(fontDir) + "/fonts.conf";
  std::string fontConfig = hg::FileUtil::readFile(fontConfFile);
  EXPECT_GE(fontConfig.size(), 0);

  litehtml::font_metrics fm;
  hg::HgContainer container;
  container.setFontTextCacheSize(1000);
  EXPECT_TRUE(container.parseAndLoadFontConfigFromMemory(fontConfig, true));
  EXPECT_TRUE(container.addFontDir(fontDir));
  litehtml::uint_ptr hFont = container.create_font(
      "Tinos", 16, 400, litehtml::font_style::fontStyleNormal, 0, &fm);
  EXPECT_TRUE(hFont);

  EXPECT_EQ(fm.ascent, 15);
  EXPECT_EQ(fm.descent, -4);
  EXPECT_EQ(fm.height, 18);
  EXPECT_EQ(fm.x_height, 8);
  EXPECT_FALSE(fm.draw_spaces);

  EXPECT_EQ(container.text_width("This is some english text", hFont), 155);
  EXPECT_EQ(container.text_width("some english", hFont), 82);

  container.delete_font(hFont);
}

TEST(HgContainerTest, draw_text)
{
  //////// Init part.

  // Get dirs from env vars.
  const char* testDir = std::getenv("HGKamva_TEST_DIR");
  EXPECT_NE(testDir, nullptr);
  const char* fontDir = std::getenv("HGKamva_TEST_FONT_DIR");
  EXPECT_NE(fontDir, nullptr);
  const char* dataDir = std::getenv("HGKamva_TEST_DATA_DIR");
  EXPECT_NE(dataDir, nullptr);

  //// AGG init.

  enum
  {
    BYTES_PER_PIXEL = 3
    //BYTES_PER_PIXEL = 4
  };

  // The AGG pixel format.
  using PixelFormat = agg::pixfmt_rgb24;
  //using PixelFormat = agg::pixfmt_rgba32;

  unsigned int frameWidth = 250;
  unsigned int frameHeight = 50;
  int stride = frameWidth * BYTES_PER_PIXEL;

  unsigned char* frameBuf = new unsigned char[stride * frameHeight];
  EXPECT_NE(frameBuf, nullptr);

  hg::HgAggPainter<PixelFormat> hgAggPainter;
  hgAggPainter.attach(frameBuf, frameWidth, frameHeight, stride);

  litehtml::web_color backgroundColor(0, 0, 0);
  hgAggPainter.setPaintColor(backgroundColor);
  hgAggPainter.clear();

  //// HtmlGrapheasKamva init.

  litehtml::font_metrics fm;
  hg::HgContainer container;
  container.setFontTextCacheSize(1000);
  EXPECT_TRUE(container.addFontDir(fontDir));
  litehtml::uint_ptr hFont = container.create_font("Tinos", 16, 400,
      litehtml::font_style::fontStyleNormal,
      litehtml::font_decoration_linethrough
          | litehtml::font_decoration_underline,
      &fm);
  EXPECT_TRUE(hFont);

  //////// Test HgContainer::draw_text().

  // Set text position.
  int x = 10;
  int y = frameHeight - 10;
  litehtml::position pos(x, 0, 0, y);

  // Set text color.
  litehtml::web_color textColor(128, 128, 128);

  // draw_text()
  container.draw_text(reinterpret_cast<litehtml::uint_ptr>(&hgAggPainter),
      "This is some english text", hFont, textColor, pos);

  // Write our picture to file.
  std::string fileName1 = "HgContainer_1.ppm";
  std::string fileOutTest1 = std::string(testDir) + "/" + fileName1;
  hg::FileUtil::writePpmFile(
      frameBuf, frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1.c_str());

  // Compare our file with prototype.
  std::string fileTest1 = std::string(dataDir) + "/" + fileName1;
  EXPECT_TRUE(hg::FileUtil::compareFiles(fileTest1, fileOutTest1));

  //////// Repeat tests for new text.

  // Make cleaning before new text.
  hgAggPainter.setPaintColor(backgroundColor);
  hgAggPainter.clear();

  // draw_text()
  container.draw_text(reinterpret_cast<litehtml::uint_ptr>(&hgAggPainter),
      "some english", hFont, textColor, pos);

  // Write our picture to file.
  std::string fileName2 = "HgContainer_2.ppm";
  std::string fileOutTest2 = std::string(testDir) + "/" + fileName2;
  hg::FileUtil::writePpmFile(
      frameBuf, frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest2.c_str());

  // Compare our file with prototype.
  std::string fileTest2 = std::string(dataDir) + "/" + fileName2;
  EXPECT_TRUE(hg::FileUtil::compareFiles(fileTest2, fileOutTest2));

  //////// Deinit part.

  container.delete_font(hFont);
  delete[] frameBuf;
}

TEST(HgContainerTest, drawHtmlDocument)
{
  //////// Init part.

  // Get dirs from env vars.
  const char* testDir = std::getenv("HGKamva_TEST_DIR");
  EXPECT_NE(testDir, nullptr);
  const char* fontDir = std::getenv("HGKamva_TEST_FONT_DIR");
  EXPECT_NE(fontDir, nullptr);
  const char* dataDir = std::getenv("HGKamva_TEST_DATA_DIR");
  EXPECT_NE(dataDir, nullptr);

  //// AGG init.

  enum
  {
    BYTES_PER_PIXEL = 3
    //    BYTES_PER_PIXEL = 4
  };

  // The AGG pixel format.
  using PixelFormat = agg::pixfmt_rgb24;
  //  using PixelFormat = agg::pixfmt_rgba32;

  unsigned int frameWidth = 640;
  unsigned int frameHeight = 480;
  int stride = frameWidth * BYTES_PER_PIXEL;

  unsigned char* frameBuf = new unsigned char[stride * frameHeight];
  EXPECT_NE(frameBuf, nullptr);

  hg::HgAggPainter<PixelFormat> hgAggPainter;
  hgAggPainter.attach(frameBuf, frameWidth, frameHeight, stride);

  litehtml::web_color backgroundColor(255, 255, 255);
  hgAggPainter.setPaintColor(backgroundColor);
  hgAggPainter.clear();

  //// HtmlGrapheasKamva init.

  hg::HgContainer container;

  // Set fonts.
  container.setFontTextCacheSize(1000);
  container.setDefaultFontName("Tinos");
  container.setDefaultFontSize(24);
  EXPECT_TRUE(container.addFontDir(fontDir));

  // Set device parameters.
  container.setDeviceWidth(frameWidth);
  container.setDeviceHeight(frameHeight);
  container.setDeviceDpiX(96);
  container.setDeviceDpiY(96);
  container.setDisplayAreaWidth(frameWidth);
  container.setDisplayAreaHeight(frameHeight);
  container.setDeviceMonochromeBits(0);
  container.setDeviceColorBits(8);
  container.setDeviceColorIndex(256);
  container.setDeviceMediaType(litehtml::media_type_screen);

  //////// Draw HTML document.

  // Load master CSS.
  std::string masterCss =
      hg::FileUtil::readFile(std::string(dataDir) + "/" + "master.css");
  EXPECT_GE(masterCss.size(), 0);

  litehtml::context htmlContext;
  htmlContext.load_master_stylesheet(masterCss.c_str());

  // Create HTML document from UTF8 string.
  std::string htmlText =
      u8R"(
        <html>
          <body>
            <center><h1>Tile text</h1></center>
            <center><h3>Subtitle text</h3></center>
            This is <i>some</i> <b>English</b> <b><i>text</i></b>
            <blockquote>for HTML renderer test.</blockquote>
          </body>
        </html>
      )";
  litehtml::document::ptr htmlDocument =
      litehtml::document::createFromUTF8(htmlText.c_str(), &container,
          &htmlContext /*, litehtml::css* user_styles = 0 */);

  // Render HTML document.
  int bestWidth = htmlDocument->render(frameWidth);
  EXPECT_GE(bestWidth, 0);

  // Draw HTML document.
  litehtml::position clip(0, 0, frameWidth, frameHeight);
  htmlDocument->draw(
      reinterpret_cast<litehtml::uint_ptr>(&hgAggPainter), 0, 0, &clip);

  // Write our picture to file.
  std::string fileName1 = "HtmlDocument_1.ppm";
  std::string fileOutTest1 = std::string(testDir) + "/" + fileName1;
  hg::FileUtil::writePpmFile(
      frameBuf, frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1.c_str());

  // Compare our file with prototype.
  std::string fileTest1 = std::string(dataDir) + "/" + fileName1;
  EXPECT_TRUE(hg::FileUtil::compareFiles(fileTest1, fileOutTest1));

  //////// Deinit part.

  delete[] frameBuf;
}
