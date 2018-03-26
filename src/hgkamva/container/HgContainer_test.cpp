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

  EXPECT_EQ(container.text_width("This is some english text", hFont), 226);
  EXPECT_EQ(container.text_width("some english", hFont), 72);

  container.delete_font(hFont);
}
