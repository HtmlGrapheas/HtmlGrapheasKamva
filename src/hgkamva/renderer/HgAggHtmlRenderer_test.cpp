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

#include "hgkamva/renderer/HgAggHtmlRenderer.h"

#include "gtest/gtest.h"

#include "agg_pixfmt_rgb.h"
//#include "agg_pixfmt_rgba.h"

#include "hgkamva/util/FileUtil.h"

TEST(HgAggHtmlRenderer, drawHtml)
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
    //BYTES_PER_PIXEL = 4
  };

  // The AGG pixel format.
  using PixelFormat = agg::pixfmt_rgb24;
  //using PixelFormat = agg::pixfmt_rgba32;

  unsigned int frameWidth = 640;
  unsigned int frameHeight = 480;
  int stride = frameWidth * BYTES_PER_PIXEL;

  unsigned char* frameBuf =
      new unsigned char[frameWidth * frameHeight * BYTES_PER_PIXEL];
  EXPECT_NE(frameBuf, nullptr);

  //// HtmlGrapheasKamva init.

  hg::HgAggHtmlRenderer<PixelFormat> hgAggHtmlRenderer;
  std::shared_ptr<hg::HgContainer> hgContainer =
      hgAggHtmlRenderer.getHgContainer();

  // Load font config.
  std::string fontConfFile = std::string(fontDir) + "/fonts.conf";
  std::string fontConfig = hg::FileUtil::readFile(fontConfFile);
  EXPECT_GE(fontConfig.size(), 0);

  bool loadedFontConfig =
      hgContainer->parseAndLoadFontConfigFromMemory(fontConfig, true);
  assert(loadedFontConfig);

  // Set fonts.
  hgContainer->setFontTextCacheSize(1000);
  hgContainer->setDefaultFontName("Tinos");
  hgContainer->setDefaultFontSize(24);
  EXPECT_TRUE(hgContainer->addFontDir(fontDir));

  // Set device parameters.
  hgContainer->setDeviceDpiX(96);
  hgContainer->setDeviceDpiY(96);
  hgContainer->setDeviceMonochromeBits(0);
  hgContainer->setDeviceColorBits(8);
  hgContainer->setDeviceColorIndex(256);
  hgContainer->setDeviceMediaType(litehtml::media_type_screen);

  //////// Draw HTML document.

  // Load master CSS.
  std::string masterCss =
      hg::FileUtil::readFile(std::string(dataDir) + "/" + "master.css");
  EXPECT_GE(masterCss.size(), 0);

  hgAggHtmlRenderer.getHtmlContext()->load_master_stylesheet(masterCss.c_str());

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
  hgAggHtmlRenderer.createHtmlDocumentFromUtf8(htmlText);

  // Render HTML document.
  int bestWidth = hgAggHtmlRenderer.renderHtml(frameWidth, frameHeight);
  EXPECT_GE(bestWidth, 0);

  // Set background color.
  hgAggHtmlRenderer.setBackgroundColor(litehtml::web_color(255, 255, 255));

  // Draw HTML document.
  hgAggHtmlRenderer.drawHtml(frameBuf, frameWidth, frameHeight, stride, 0, 0);

  // Write our picture to file.
  std::string fileName1 = "HgAggHtmlRenderer_1.ppm";
  std::string fileOutTest1 = std::string(testDir) + "/" + fileName1;
  hg::FileUtil::writePpmFile(
      frameBuf, frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1.c_str());

  // Compare our file with prototype.
  std::string fileTest1 = std::string(dataDir) + "/" + "HtmlDocument_1.ppm";
  EXPECT_TRUE(hg::FileUtil::compareFiles(fileTest1, fileOutTest1));

  //////// Deinit part.

  delete[] frameBuf;
}
