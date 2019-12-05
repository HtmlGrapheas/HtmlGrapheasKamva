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

namespace hg
{
HgFontLibrary::HgFontLibrary()
    : mFcConfig{FcInitLoadConfig(), FcConfigDestroy}
{
  FT_Library frLibrary;
  FT_Init_FreeType(&frLibrary);
  mFtLibrary = {frLibrary, FT_Done_FreeType};
}

HgFontLibrary::~HgFontLibrary() {}

bool HgFontLibrary::parseAndLoadConfigFromMemory(
    const std::string& fontConfig, bool complain)
{
  const FcChar8* config = reinterpret_cast<const FcChar8*>(fontConfig.c_str());
  return FcConfigParseAndLoadFromMemory(mFcConfig.get(), config, complain)
      && FcConfigSetCurrent(mFcConfig.get());
}

bool HgFontLibrary::addFontDir(const std::filesystem::path& dirPath)
{
  const FcChar8* dir = reinterpret_cast<const FcChar8*>(dirPath.c_str());
  return FcConfigAppFontAddDir(mFcConfig.get(), dir);
}

std::filesystem::path HgFontLibrary::getFontFilePath(const std::string& names,
    int pixelSize,
    int weight,
    litehtml::font_style fontStyle,
    uint_least8_t* result) const
{
  std::filesystem::path ret;

  litehtml::string_vector fonts;
  litehtml::split_string(names, fonts, ",");
  for(auto& font : fonts) {
    litehtml::trim(font);
  }

  FcPatternPtr pat{FcPatternCreate(), FcPatternDestroy};

  for(const auto& font : fonts) {
    const FcChar8* fcFamily = reinterpret_cast<const FcChar8*>(font.c_str());
    FcPatternAddString(pat.get(), FC_FAMILY, fcFamily);
  }

  int fcSlant = fontStyleToFcSlant(fontStyle);
  int fcWeight = weightToFcWeight(weight);

  FcPatternAddInteger(pat.get(), FC_SLANT, fcSlant);
  FcPatternAddInteger(pat.get(), FC_PIXEL_SIZE, pixelSize);
  FcPatternAddInteger(pat.get(), FC_WEIGHT, fcWeight);

  FcConfigSubstitute(mFcConfig.get(), pat.get(), FcMatchPattern);
  FcDefaultSubstitute(pat.get());

  // Find the font.
  FcResult fcResult;
  FcPatternPtr fontPat{
      FcFontMatch(mFcConfig.get(), pat.get(), &fcResult), FcPatternDestroy};

  if(fontPat && FcResultMatch == fcResult) {
    FcChar8* file = nullptr;
    if(FcPatternGetString(fontPat.get(), FC_FILE, 0, &file) == FcResultMatch) {
      // Found the font file, this might be a fallback font.
      ret = reinterpret_cast<char*>(file);

      if(result) {
        FcChar8* retFamily = nullptr;
        int retSlant = -1;
        int retPixelSize = -1;
        int retWeight = -1;
        *result = FontMatches::allMatched;

        if(FcPatternGetString(fontPat.get(), FC_FAMILY, 0, &retFamily)
            == FcResultMatch) {
          bool found = false;
          for(const auto& font : fonts) {
            if(0 == font.compare(reinterpret_cast<const char*>(retFamily))) {
              found = true;
              break;
            }
          }
          if(!found) {
            *result |= FontMatches::notMatchedFaceName;
          }
        }

        if(FcPatternGetInteger(fontPat.get(), FC_SLANT, 0, &retSlant)
                == FcResultMatch
            && retSlant != fcSlant) {
          *result |= FontMatches::notMatchedFontStyle;
        }
        if(FcPatternGetInteger(fontPat.get(), FC_PIXEL_SIZE, 0, &retPixelSize)
                == FcResultMatch
            && retPixelSize != pixelSize) {
          *result |= FontMatches::notMatchedPixelSize;
        }
        if(FcPatternGetInteger(fontPat.get(), FC_WEIGHT, 0, &retWeight)
                == FcResultMatch
            && retWeight != fcWeight) {
          *result |= FontMatches::notMatchedWeight;
        }
      }
    }
  }

  return ret;
}

}  // namespace hg
