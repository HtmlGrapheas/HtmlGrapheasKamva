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

#include <cstdlib>
#include <string>
#include <filesystem>

#include "gtest/gtest.h"

#include "hgkamva/container/HgFontLibrary.h"
#include "hgkamva/util/FileUtil.h"
#include "hgkamva/util/StringUtil.h"

inline std::filesystem::path testDir;
inline std::filesystem::path fontDir;
//inline std::filesystem::path dataDir;

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);

  // https://stackoverflow.com/a/55579815
  // See also:
  // https://stackoverflow.com/a/42772814
  // https://habr.com/ru/post/343622/
  testDir =
      std::filesystem::absolute(std::filesystem::path(argv[0]).parent_path());
  fontDir = testDir / "fonts";
//  dataDir = testDir / "data";

  return RUN_ALL_TESTS();
}

TEST(HgFontLibraryTest, getFontFilePath)
{
  EXPECT_TRUE(std::filesystem::exists(testDir));
  EXPECT_TRUE(std::filesystem::exists(fontDir));
//  EXPECT_TRUE(std::filesystem::exists(dataDir));

  hg::HgFontLibrary hgFontLibrary;

  std::filesystem::path fontConfFile = fontDir / "fonts.conf";
  EXPECT_TRUE(std::filesystem::exists(fontConfFile));
  std::string fontConfig = hg::FileUtil::readFile(fontConfFile);
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
