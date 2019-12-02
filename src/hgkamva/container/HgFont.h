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

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb-ft.h>
#include <hb.h>

#include <cairo/cairo.h>

#include "litehtml.h"

#include <stlcache/stlcache.hpp>

namespace hg
{
class HgFont
{
public:
  using CairoTextExtentsPtr = std::shared_ptr<cairo_text_extents_t>;

private:
  static constexpr int FT_64_INT = 64;
  static constexpr double FT_64_DOUBLE = static_cast<double>(FT_64_INT);

  using GlyphInfoArray = std::vector<hb_glyph_info_t>;
  using GlyphInfoArrayPtr = std::shared_ptr<GlyphInfoArray>;
  using GlyphPositionArray = std::vector<hb_glyph_position_t>;
  using GlyphPositionArrayPtr = std::shared_ptr<GlyphPositionArray>;
  using CairoGlyphArray = std::vector<cairo_glyph_t>;
  using CairoGlyphArrayPtr = std::shared_ptr<CairoGlyphArray>;

  struct HbLaoutCacheItem
  {
    explicit HbLaoutCacheItem(
        CairoGlyphArrayPtr cairoGlyph, CairoTextExtentsPtr cairoTextExtents)
        : mCairoGlyph{cairoGlyph}
        , mCairoTextExtents{cairoTextExtents}
    {
    }

    CairoGlyphArrayPtr mCairoGlyph;
    CairoTextExtentsPtr mCairoTextExtents;
    // TODO:
    //hb_direction_t mDirection;
    //hb_script_t mScript;
    //std::string mLanguage;
  };

  using HbLaoutCacheItemPtr = std::shared_ptr<HbLaoutCacheItem>;
  using HbLaoutCache =
      stlcache::cache<std::string, HbLaoutCacheItemPtr, stlcache::policy_lru>;

public:
  explicit HgFont() = delete;

  explicit HgFont(
      cairo_t* cairoContext, FT_Library ftLibrary, int textCacheSize = 1000);
  ~HgFont();

  bool createFtFace(const std::string& fontFilePath, int pixelSize);
  bool destroyFtFace();

  void resetBuffer();
  void clearBuffer();
  void setDirection(hb_direction_t direction);
  void setScript(hb_script_t script);
  void setLanguage(const std::string& language);

  hg::HgFont::CairoTextExtentsPtr getTextExtents(const std::string& text);
  void drawText(
      const std::string& text, int x, int y, const litehtml::web_color& color);

  int forceUcs2Charmap(FT_Face ftf);

  const FT_Face ftFace() const { return mFtFace; }
  double xHeight();

  static FT_F26Dot6 intToF26Dot6(int pixelSize);
  static int f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels);

private:
  HbLaoutCacheItemPtr layoutText(const std::string& text);

private:
  FT_Library mFtLibrary;
  FT_Face mFtFace;

  hb_buffer_t* mHbBuffer;
  hb_font_t* mHbFont;

  cairo_t* mCairoContext;
  cairo_font_face_t* mCairoFontFace;
  cairo_scaled_font_t* mCairoScaledFont;

  std::shared_ptr<HbLaoutCache> mHbLaoutCache;

public:
  // TODO: get size, strikeout and underline from FT structs if possible,
  // TODO: remove it here.
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

inline FT_F26Dot6 HgFont::intToF26Dot6(int pixelSize)
{
  return static_cast<FT_Long>(pixelSize) * FT_64_INT;
}

inline int HgFont::f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels)
{
  return static_cast<int>(f26Dot6Pixels / FT_64_INT);
}

}  // namespace hg

#endif  // HG_FONT_H
