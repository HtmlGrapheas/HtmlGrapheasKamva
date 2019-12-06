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

#ifndef HG_FONT_LIBRARY_H
#define HG_FONT_LIBRARY_H

#include <cstdint>
#include <filesystem>
#include <string>

#include <fontconfig/fontconfig.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "litehtml.h"

namespace hg
{
class HgFontLibrary;

using HgFontLibraryPtr = std::shared_ptr<HgFontLibrary>;

using FcConfigPtr = std::shared_ptr<FcConfig>;
using FcPatternPtr = std::shared_ptr<FcPattern>;
using FtLibraryPtr = std::shared_ptr<FT_LibraryRec_>;

class HgFontLibrary
{
public:
  enum FontMatches : uint_least8_t
  {
    allMatched = 0,
    notMatchedFaceName = 1 << 0,
    notMatchedFontStyle = 1 << 1,
    notMatchedPixelSize = 1 << 2,
    notMatchedWeight = 1 << 3,
  };

  // TODO: Copy/move constructors/operators.
  explicit HgFontLibrary();
  ~HgFontLibrary();

  bool parseAndLoadConfigFromMemory(
      const std::string& fontConfig, const bool complain);
  bool addFontDir(const std::filesystem::path& dirPath);

  std::filesystem::path getFontFilePath(const std::string& names,
      const int pixelSize,
      const int weight,
      const litehtml::font_style fontStyle,
      uint_least8_t* result) const;

  FtLibraryPtr ftLibrary() { return mFtLibrary; }

private:
  int weightToFcWeight(const int weigh) const;
  int fontStyleToFcSlant(const litehtml::font_style fontStyle) const;

private:
  FcConfigPtr mFcConfig;
  FtLibraryPtr mFtLibrary;
};  // class HgFontLibrary

inline int HgFontLibrary::weightToFcWeight(const int weight) const
{
  if(weight >= 0 && weight < 150)
    return FC_WEIGHT_THIN;
  else if(weight >= 150 && weight < 250)
    return FC_WEIGHT_EXTRALIGHT;
  else if(weight >= 250 && weight < 350)
    return FC_WEIGHT_LIGHT;
  else if(weight >= 350 && weight < 450)
    return FC_WEIGHT_NORMAL;
  else if(weight >= 450 && weight < 550)
    return FC_WEIGHT_MEDIUM;
  else if(weight >= 550 && weight < 650)
    return FC_WEIGHT_SEMIBOLD;
  else if(weight >= 650 && weight < 750)
    return FC_WEIGHT_BOLD;
  else if(weight >= 750 && weight < 850)
    return FC_WEIGHT_EXTRABOLD;
  else if(weight >= 950)
    return FC_WEIGHT_BLACK;
  else
    return FC_WEIGHT_NORMAL;
}

inline int HgFontLibrary::fontStyleToFcSlant(
    const litehtml::font_style fontStyle) const
{
  switch(fontStyle) {
    case litehtml::fontStyleItalic:
      return FC_SLANT_ITALIC;
    case litehtml::fontStyleNormal:
    default:
      return FC_SLANT_ROMAN;
  }
}

}  // namespace hg

#endif  // HG_FONT_LIBRARY_H
