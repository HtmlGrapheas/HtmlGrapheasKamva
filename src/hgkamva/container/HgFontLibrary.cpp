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
{
  mFcConfig = FcInitLoadConfig();
  FT_Init_FreeType(&mFtLibrary);
}

HgFontLibrary::~HgFontLibrary()
{
  FT_Done_FreeType(mFtLibrary);
  FcConfigDestroy(mFcConfig);
}

bool HgFontLibrary::parseAndLoadConfigFromMemory(
    const std::string& fontConfig, bool complain)
{
  const FcChar8* config = reinterpret_cast<const FcChar8*>(fontConfig.c_str());
  return FcConfigParseAndLoadFromMemory(mFcConfig, config, complain)
      && FcConfigSetCurrent(mFcConfig);
}

bool HgFontLibrary::addFontDir(const std::string& dirPath)
{
  const FcChar8* dir = reinterpret_cast<const FcChar8*>(dirPath.c_str());
  return FcConfigAppFontAddDir(mFcConfig, dir);
}

std::string HgFontLibrary::getFontFilePath(const std::string& names,
    int pixelSize,
    int weight,
    litehtml::font_style fontStyle,
    uint_least8_t* result) const
{
  std::string ret = "";

  litehtml::string_vector fonts;
  litehtml::split_string(names, fonts, ",");
  for(litehtml::string_vector::iterator i = fonts.begin(); i != fonts.end();
      i++) {
    litehtml::trim(*i);
  }

  FcPattern* pat = FcPatternCreate();

  for(litehtml::string_vector::iterator i = fonts.begin(); i != fonts.end();
      i++) {
    const FcChar8* fcFamily = reinterpret_cast<const FcChar8*>(i->c_str());
    FcPatternAddString(pat, FC_FAMILY, fcFamily);
  }

  int fcSlant = fontStyleToFcSlant(fontStyle);
  int fcWeight = weightToFcWeight(weight);

  FcPatternAddInteger(pat, FC_SLANT, fcSlant);
  FcPatternAddInteger(pat, FC_PIXEL_SIZE, pixelSize);
  FcPatternAddInteger(pat, FC_WEIGHT, fcWeight);

  FcConfigSubstitute(mFcConfig, pat, FcMatchPattern);
  FcDefaultSubstitute(pat);

  // Find the font.
  FcResult fcResult;
  FcPattern* fontPat = FcFontMatch(mFcConfig, pat, &fcResult);
  if(fontPat) {
    if(FcResultMatch == fcResult) {
      FcChar8* file = nullptr;
      if(FcPatternGetString(fontPat, FC_FILE, 0, &file) == FcResultMatch) {
        // Found the font file, this might be a fallback font.
        ret = reinterpret_cast<char*>(file);

        if(result) {
          FcChar8* retFamily = nullptr;
          int retSlant = -1;
          int retPixelSize = -1;
          int retWeight = -1;
          *result = FontMatches::allMatched;

          if(FcPatternGetString(fontPat, FC_FAMILY, 0, &retFamily)
              == FcResultMatch) {
            bool found = false;
            for(litehtml::string_vector::iterator i = fonts.begin();
                i != fonts.end(); i++) {
              if(0 == i->compare(reinterpret_cast<const char*>(retFamily))) {
                found = true;
                break;
              }
            }
            if(!found) {
              *result |= FontMatches::notMatchedFaceName;
            }
          }

          if(FcPatternGetInteger(fontPat, FC_SLANT, 0, &retSlant)
                  == FcResultMatch
              && retSlant != fcSlant) {
            *result |= FontMatches::notMatchedFontStyle;
          }
          if(FcPatternGetInteger(fontPat, FC_PIXEL_SIZE, 0, &retPixelSize)
                  == FcResultMatch
              && retPixelSize != pixelSize) {
            *result |= FontMatches::notMatchedPixelSize;
          }
          if(FcPatternGetInteger(fontPat, FC_WEIGHT, 0, &retWeight)
                  == FcResultMatch
              && retWeight != fcWeight) {
            *result |= FontMatches::notMatchedWeight;
          }
        }
      }
    }
    FcPatternDestroy(fontPat);
  }
  FcPatternDestroy(pat);

  return ret;
}

}  // namespace hg
