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

#include "gtest/gtest.h"

// https://stackoverflow.com/a/2072890
inline bool ends_with(std::string const& value, std::string const& ending)
{
  if(ending.size() > value.size())
    return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

TEST(HgFontTest, getFontFilePath)
{
  hg::HgFont hgFont;

  uint_least8_t result;
  std::string filePath;

  filePath = hgFont.getFontFilePath("Some Unknown Font, Noto Sans", 16, 400,
      litehtml::font_style::fontStyleNormal, &result);
  EXPECT_EQ(hg::HgFont::FontMatches::allMatched, result);
  EXPECT_TRUE(ends_with(filePath, "NotoSans-Regular.ttf"));

  filePath = hgFont.getFontFilePath("Some Unknown Font, Noto Sans", 16, 400,
      litehtml::font_style::fontStyleItalic, &result);
  EXPECT_EQ(hg::HgFont::FontMatches::allMatched, result);
  EXPECT_TRUE(ends_with(filePath, "NotoSans-Italic.ttf"));

  filePath = hgFont.getFontFilePath("Some Unknown Font, Noto Sans", 16, 700,
      litehtml::font_style::fontStyleNormal, &result);
  EXPECT_EQ(hg::HgFont::FontMatches::allMatched, result);
  EXPECT_TRUE(ends_with(filePath, "NotoSans-Bold.ttf"));

  filePath = hgFont.getFontFilePath("Some Unknown Font, Noto Sans", 16, 700,
      litehtml::font_style::fontStyleItalic, &result);
  EXPECT_EQ(hg::HgFont::FontMatches::allMatched, result);
  EXPECT_TRUE(ends_with(filePath, "NotoSans-BoldItalic.ttf"));
}

TEST(HgFontTest, createFtFace)
{
  hg::HgFont hgFont;
  int pixelSize = 16;
  int weight = 400;
  litehtml::font_style fontStyle = litehtml::font_style::fontStyleNormal;

  uint_least8_t result;
  std::string filePath = hgFont.getFontFilePath(
      "Noto Sans", pixelSize, weight, fontStyle, &result);
  EXPECT_EQ(hg::HgFont::FontMatches::allMatched, result);
  EXPECT_TRUE(ends_with(filePath, "NotoSans-Regular.ttf"));

  EXPECT_TRUE(hgFont.createFtFace(filePath, pixelSize));
  EXPECT_EQ(hg::HgFont::f26Dot6ToInt(hgFont.xHeight()), 9);
  EXPECT_TRUE(hgFont.destroyFtFace());
}
