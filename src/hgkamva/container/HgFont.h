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

#include <filesystem>
#include <memory>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb-ft.h>
#include <hb.h>

#include "litehtml.h"

#include <stlcache/stlcache.hpp>

#include "hgkamva/container/HgCairo.h"
#include "hgkamva/container/HgFontLibrary.h"

namespace hg
{
class HgFont;

using HgFontPtr = std::shared_ptr<HgFont>;

class HgFont
{
public:
  explicit HgFont() = delete;

  // TODO: Copy/move constructors/operators.
  explicit HgFont(FtLibraryPtr ftLibrary, const int textCacheSize = 1000);
  ~HgFont();

  bool createFtFace(
      const std::filesystem::path& fontFilePath, const int pixelSize);

  void resetBuffer();
  void clearBuffer();
  void setDirection(const hb_direction_t direction);
  void setScript(const hb_script_t script);
  void setLanguage(const std::string& language);

  const cairo_font_extents_t& getScaledFontExtents();
  HgCairo::TextExtentsPtr getTextExtents(const std::string& text);
  void drawText(HgCairoPtr cairo,
      const std::string& text,
      const double x,
      const double y,
      const litehtml::web_color& color);

  double xHeight();

  static FT_F26Dot6 intToF26Dot6(int pixelSize);
  static int f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels);

  // TODO: get size, strikeout and underline from FT structs if possible,
  // TODO: remove it here.
  void setPixelSize(int pixelSize) { mPixelSize = pixelSize; }
  void setUnderline(bool underline) { mUnderline = underline; }
  void setStrikeout(bool strikeout) { mStrikeout = strikeout; }
  int pixelSize() { return mPixelSize; }
  bool underline() { return mUnderline; }
  bool strikeout() { return mStrikeout; }

private:
  static constexpr int FT_64_INT = 64;
  static constexpr double FT_64_DOUBLE = static_cast<double>(FT_64_INT);

  using FtFacePtr = std::shared_ptr<FT_FaceRec_>;
  using HbBufferPtr = std::shared_ptr<hb_buffer_t>;
  using HbFontPtr = std::shared_ptr<hb_font_t>;

  struct TextLayout
  {
    explicit TextLayout(
        HgCairo::GlyphVectorPtr glyphs, HgCairo::TextExtentsPtr textExtents)
        : mGlyphs{glyphs}
        , mExtents{textExtents}
    {
    }

    HgCairo::GlyphVectorPtr mGlyphs;
    HgCairo::TextExtentsPtr mExtents;
    // TODO:
    //hb_direction_t mDirection;
    //hb_script_t mScript;
    //std::string mLanguage;
  };

  using TextLayoutPtr = std::shared_ptr<TextLayout>;
  using TextLayoutCache =
      stlcache::cache<std::string, TextLayoutPtr, stlcache::policy_lru>;
  using TextLayoutCachePtr = std::shared_ptr<TextLayoutCache>;

  static int forceUcs2Charmap(FT_Face ftf);
  TextLayoutPtr getTextLayout(const std::string& text);

  FtLibraryPtr mFtLibrary;
  FtFacePtr mFtFace;

  HbBufferPtr mHbBuffer;
  HbFontPtr mHbFont;

  HgCairo::ScaledFontPtr mCairoScaledFont;
  cairo_font_extents_t mScaledFontExtents;

  TextLayoutCachePtr mTextLayoutCache;

  int mPixelSize;
  bool mStrikeout;
  bool mUnderline;
  double mxHeight;
};  // class HgFont

// static
inline FT_F26Dot6 HgFont::intToF26Dot6(int pixelSize)
{
  return static_cast<FT_Long>(pixelSize) * FT_64_INT;
}

// static
inline int HgFont::f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels)
{
  return static_cast<int>(f26Dot6Pixels / FT_64_INT);
}

}  // namespace hg

#endif  // HG_FONT_H
