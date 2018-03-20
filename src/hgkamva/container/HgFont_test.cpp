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

#include <cstdlib>

#include "gtest/gtest.h"

#include "hgkamva/container/HgFontLibrary.h"
#include "hgkamva/util/StringUtil.h"

TEST(HgFontTest, createFtFace)
{
  const char* fontDir = std::getenv("HGRAPH_TEST_FONT_DIR");
  EXPECT_TRUE(fontDir);

  hg::HgFontLibrary hgFontLibrary;
  hg::HgFont hgFont(hgFontLibrary.ftLibrary());
  int pixelSize = 16;
  int weight = 400;
  litehtml::font_style fontStyle = litehtml::font_style::fontStyleNormal;

  EXPECT_TRUE(hgFontLibrary.addFontDir(fontDir));

  uint_least8_t result;
  std::string filePath = hgFontLibrary.getFontFilePath(
      "Tinos", pixelSize, weight, fontStyle, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(hg::StringUtil::endsWith(filePath, "Tinos-Regular.ttf"));

  EXPECT_TRUE(hgFont.createFtFace(filePath, pixelSize));
  EXPECT_EQ(hg::HgFont::f26Dot6ToInt(hgFont.xHeight()), 8);
  EXPECT_TRUE(hgFont.destroyFtFace());
}
