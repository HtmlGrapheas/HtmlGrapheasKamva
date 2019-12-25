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

#include "gtest/gtest.h"

#include "hgkamva/container/HgFontLibrary.h"
#include "hgkamva/util/FileUtil.h"
#include "hgkamva/util/Filesystem.h"
#include "hgkamva/util/StringUtil.h"

inline hg::filesystem::path testDir;
inline hg::filesystem::path fontDir;
//inline hg::filesystem::path dataDir;

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);

  // https://stackoverflow.com/a/55579815
  // See also:
  // https://stackoverflow.com/a/42772814
  // https://habr.com/ru/post/343622/
  testDir =
      hg::filesystem::absolute(hg::filesystem::path(argv[0]).parent_path());
  fontDir = testDir / "fonts";
  //  dataDir = testDir / "data";

  return RUN_ALL_TESTS();
}

TEST(HgFontLibraryTest, getFontFilePath)
{
  EXPECT_TRUE(hg::filesystem::exists(testDir));
  EXPECT_TRUE(hg::filesystem::exists(fontDir));
  //  EXPECT_TRUE(hg::filesystem::exists(dataDir));

  hg::HgFontLibrary hgFontLibrary;

  hg::filesystem::path fontConfFile = fontDir / "fonts.conf";
  EXPECT_TRUE(hg::filesystem::exists(fontConfFile));
  std::string fontConfig = hg::util::readFile(fontConfFile);
  EXPECT_FALSE(fontConfig.empty());

  EXPECT_TRUE(hgFontLibrary.parseAndLoadConfigFromMemory(fontConfig, true));

  EXPECT_TRUE(hgFontLibrary.addFontDir(fontDir));

  uint_least8_t result;
  hg::filesystem::path filePath;

  filePath = hgFontLibrary.getFontFilePath("Some Unknown Font, Tinos", 16, 400,
      litehtml::font_style::fontStyleNormal, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-Regular.ttf");

  filePath = hgFontLibrary.getFontFilePath("Some Unknown Font, Tinos", 16, 400,
      litehtml::font_style::fontStyleItalic, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-Italic.ttf");

  filePath = hgFontLibrary.getFontFilePath("Some Unknown Font, Tinos", 16, 700,
      litehtml::font_style::fontStyleNormal, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-Bold.ttf");

  filePath = hgFontLibrary.getFontFilePath("Some Unknown Font, Tinos", 16, 700,
      litehtml::font_style::fontStyleItalic, &result);
  EXPECT_EQ(hg::HgFontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-BoldItalic.ttf");
}
