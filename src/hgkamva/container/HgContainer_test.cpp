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
#include "agg_renderer_base.h"

#include "hgkamva/container/HgRenderer.h"
#include "hgkamva/util/FileUtil.h"

TEST(HgContainerTest, create_font)
{
  const char* fontDir = std::getenv("HGRAPH_TEST_FONT_DIR");
  EXPECT_NE(fontDir, nullptr);

  litehtml::font_metrics fm;
  hg::HgContainer container;
  EXPECT_TRUE(container.addFontDir(fontDir));
  litehtml::uint_ptr hFont = container.create_font(
      "Tinos", 16, 400, litehtml::font_style::fontStyleNormal, 0, &fm);
  EXPECT_NE(hFont, nullptr);

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
  const char* testDir = std::getenv("HGRAPH_TEST_DIR");
  EXPECT_NE(testDir, nullptr);
  const char* fontDir = std::getenv("HGRAPH_TEST_FONT_DIR");
  EXPECT_NE(fontDir, nullptr);
  const char* dataDir = std::getenv("HGRAPH_TEST_DATA_DIR");
  EXPECT_NE(dataDir, nullptr);

  // TODO: move to common file.h
  //// AGG init.

  enum
  {
    BYTES_PER_PIXEL = 3
  };

  // The AGG pixel format.
  using PixelFormat = agg::pixfmt_rgb24;

  // The AGG base renderer.
  using RendererBase = agg::renderer_base<PixelFormat>;

  using RenderBaseColorType = typename RendererBase::color_type;

  int frameWidth = 250;
  int frameHeight = 50;
  int stride = frameWidth * BYTES_PER_PIXEL;

  unsigned char* frameBuf = new unsigned char[frameWidth * frameHeight * 3];
  EXPECT_NE(frameBuf, nullptr);

  agg::rendering_buffer rbuf;
  rbuf.attach(frameBuf, frameWidth, frameHeight, stride);

  PixelFormat pixf(rbuf);
  RendererBase rbase(pixf);
  rbase.clear(RenderBaseColorType(0, 0, 0));

  //// HtmlGrapheasKamva init.

  hg::HgRenderer hgRenderer;
  hgRenderer.setBlendHLineFunc(
      [&rbase](int x1, int y, int x2, const litehtml::web_color& color,
          unsigned char cover) -> void {
        RenderBaseColorType rbaseColor;
        rbaseColor.clear();
        rbaseColor.opacity(color.alpha / 255.0);
        rbaseColor.r = color.red;
        rbaseColor.g = color.green;
        rbaseColor.b = color.blue;

        rbase.blend_hline(x1, y, x2, rbaseColor, cover);
      });

  hgRenderer.setCopyHLineFunc([&rbase](int x1, int y, int x2,
                                  const litehtml::web_color& color) -> void {
    RenderBaseColorType rbaseColor;
    rbaseColor.clear();
    rbaseColor.opacity(color.alpha / 255.0);
    rbaseColor.r = color.red;
    rbaseColor.g = color.green;
    rbaseColor.b = color.blue;

    rbase.copy_hline(x1, y, x2, rbaseColor);
  });

  litehtml::font_metrics fm;
  hg::HgContainer container;
  EXPECT_TRUE(container.addFontDir(fontDir));
  litehtml::uint_ptr hFont = container.create_font("Tinos", 16, 400,
      litehtml::font_style::fontStyleNormal,
      litehtml::font_decoration_linethrough
          | litehtml::font_decoration_underline,
      &fm);
  EXPECT_NE(hFont, nullptr);

  //////// Test HgContainer::draw_text().

  // Set text position.
  int x = 10;
  int y = frameHeight - 10;
  litehtml::position pos(x, 0, 0, y);

  // Set text color.
  litehtml::web_color color(128, 128, 128, 255);

  // draw_text()
  container.draw_text(
      &hgRenderer, "This is some english text", hFont, color, pos);

  // Write our picture to file.
  std::string fileName1 = "HgContainer_1.ppm";
  std::string fileOutTest1 = std::string(testDir) + "/" + fileName1;
  hg::FileUtil::writePpmFile(
      frameBuf, frameWidth, frameHeight, fileOutTest1.c_str());

  // Compare our file with prototype.
  std::string fileTest1 = std::string(dataDir) + "/" + fileName1;
  EXPECT_TRUE(hg::FileUtil::compareFiles(fileTest1, fileOutTest1));

  //////// Repeat tests for new text.

  // Make cleaning before new text.
  rbase.clear(RenderBaseColorType(0, 0, 0));

  // draw_text()
  container.draw_text(&hgRenderer, "some english", hFont, color, pos);

  // Write our picture to file.
  std::string fileName2 = "HgContainer_2.ppm";
  std::string fileOutTest2 = std::string(testDir) + "/" + fileName2;
  hg::FileUtil::writePpmFile(
      frameBuf, frameWidth, frameHeight, fileOutTest2.c_str());

  // Compare our file with prototype.
  std::string fileTest2 = std::string(dataDir) + "/" + fileName2;
  EXPECT_TRUE(hg::FileUtil::compareFiles(fileTest2, fileOutTest2));

  //////// Deinit part.

  container.delete_font(hFont);
  delete[] frameBuf;
}
