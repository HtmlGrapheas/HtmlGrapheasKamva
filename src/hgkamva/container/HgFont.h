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

#ifndef HG_FONT_H
#define HG_FONT_H

#include <cstdint>
#include <memory>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb-ft.h>
#include <hb.h>

#include <fontconfig/fontconfig.h>

#include "litehtml.h"

namespace hg
{
class HgFont
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

  explicit HgFont();
  virtual ~HgFont();

  std::string getFontFilePath(const std::string& names,
      int pixelSize,
      int weight,
      litehtml::font_style fontStyle,
      uint_least8_t* result) const;

  virtual bool createFtFace(const std::string& fontFilePath, int pixelSize);
  virtual bool destroyFtFace();

  virtual const FT_Face ftFace() const { return mFtFace; }
  virtual FT_F26Dot6 xHeight() const;

  static FT_F26Dot6 intToF26Dot6(int pixelSize);
  static int f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels);

private:
  int weightToFcWeight(int weigh) const;
  int fontStyleToFcSlant(litehtml::font_style fontStyle) const;

private:
  FcConfig* mFcConfig;

  FT_Library mFtLibrary;
  FT_Face mFtFace;

  hb_buffer_t* mHbBuffer;
  hb_font_t* mHbFont;

  // TODO: get size, strikeout and underline from FT structs if possible, remove it here.
public:
  void setPixelSize(int pixelSize) { mPixelSize = pixelSize; }
  void setUnderline(bool underline) { mUnderline = underline; }
  void setStrikeout(bool strikeout) { mStrikeout = strikeout; }
  int pixelSize() { return mPixelSize; }
  bool underline() { return mUnderline; }
  bool strikeout() { return mStrikeout; }
private:
  int mPixelSize;
  bool mStrikeout;
  bool mUnderline;
};  // class HgFont

inline int HgFont::weightToFcWeight(int weight) const
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

inline int HgFont::fontStyleToFcSlant(litehtml::font_style fontStyle) const
{
  switch(fontStyle) {
    case litehtml::fontStyleItalic:
      return FC_SLANT_ITALIC;
    case litehtml::fontStyleNormal:
    default:
      return FC_SLANT_ROMAN;
  }
}

inline FT_F26Dot6 HgFont::intToF26Dot6(int pixelSize)
{
  return static_cast<FT_Long>(pixelSize) * 64;
}

inline int HgFont::f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels)
{
  return static_cast<int>(f26Dot6Pixels / 64);
}

}  // namespace hg

#endif  // HG_FONT_H
