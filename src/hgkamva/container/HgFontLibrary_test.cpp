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

#include "hgkamva/container/HgFontLibrary.h"

#include <cstdlib>
#include <string>

#include "gtest/gtest.h"

#include "hgkamva/util/FileUtil.h"
#include "hgkamva/util/StringUtil.h"

TEST(HgFontLibraryTest, getFontFilePath)
{
  //const char* testDir = std::getenv("HGRAPH_TEST_DIR");
  //EXPECT_TRUE(testDir);
  const char* fontDir = std::getenv("HGRAPH_TEST_FONT_DIR");
  EXPECT_NE(fontDir, nullptr);

  hg::HgFontLibrary hgFontLibrary;

  std::string confFile = std::string(fontDir) + "/fonts.conf";
  std::string fontConfig = hg::FileUtil::readFile(confFile);
  EXPECT_GE(fontConfig.size(), 0);

  EXPECT_TRUE(hgFontLibrary.parseAndLoadConfigFromMemory(fontConfig, true));

  EXPECT_TRUE(hgFontLibrary.addFontDir(fontDir));

  uint_least8_t result;
  std::string filePath;

  filePath = hgFontLibrary.getFontFilePath("Some Unknown Font, Tinos", 16, 400,
      litehtml::font_style::fontStyleNormal, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(hg::StringUtil::endsWith(filePath, "Tinos-Regular.ttf"));

  filePath = hgFontLibrary.getFontFilePath("Some Unknown Font, Tinos", 16, 400,
      litehtml::font_style::fontStyleItalic, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(hg::StringUtil::endsWith(filePath, "Tinos-Italic.ttf"));

  filePath = hgFontLibrary.getFontFilePath("Some Unknown Font, Tinos", 16, 700,
      litehtml::font_style::fontStyleNormal, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(hg::StringUtil::endsWith(filePath, "Tinos-Bold.ttf"));

  filePath = hgFontLibrary.getFontFilePath("Some Unknown Font, Tinos", 16, 700,
      litehtml::font_style::fontStyleItalic, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(hg::StringUtil::endsWith(filePath, "Tinos-BoldItalic.ttf"));
}
