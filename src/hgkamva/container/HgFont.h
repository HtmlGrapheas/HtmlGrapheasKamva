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

#include "litehtml.h"

#include <stlcache/stlcache.hpp>

#include "hgkamva/container/HgPainter.h"

namespace hg
{
class HgFont
{
public:
  struct TextBbox
  {
    int mMinX;
    int mMaxX;
    int mMinY;
    int mMaxY;

    int mBboxW;
    int mBboxH;
    int mBaselineShift;
    int mBaselineOffset;
  };

  using TextBboxPtr = std::shared_ptr<TextBbox>;

private:
  static constexpr int FT_64 = 64;

  struct FtRasterParamsUser
  {
    HgPainter* mHgPainter;

    int mGlyphX;
    int mGlyphY;

    // Sizing part.
    int mMinSpanX;
    int mMaxSpanX;
    int mMinY;
    int mMaxY;

    litehtml::web_color mColor;
  };

  static void blendFtSpanFunc(
      int y, int count, const FT_Span* spans, void* user);
  static void sizerFtSpanFunc(
      int y, int count, const FT_Span* spans, void* user);

  using GlyphInfoArray = std::vector<hb_glyph_info_t>;
  using GlyphInfoArrayPtr = std::shared_ptr<GlyphInfoArray>;
  using GlyphPositionArray = std::vector<hb_glyph_position_t>;
  using GlyphPositionArrayPtr = std::shared_ptr<GlyphPositionArray>;

  struct HbLaoutCacheItem
  {
    explicit HbLaoutCacheItem(unsigned int glyphCount,
        GlyphInfoArrayPtr glyphInfo,
        GlyphPositionArrayPtr glyphPos)
        : mGlyphCount(glyphCount)
        , mGlyphInfo(glyphInfo)
        , mGlyphPos(glyphPos)
    {
    }

    unsigned int mGlyphCount;
    GlyphInfoArrayPtr mGlyphInfo;
    GlyphPositionArrayPtr mGlyphPos;
    // TODO:
    //hb_direction_t mDirection;
    //hb_script_t mScript;
    //std::string mLanguage;
  };

  using HbLaoutCacheItemPtr = std::shared_ptr<HbLaoutCacheItem>;
  using HbLaoutCache =
      stlcache::cache<std::string, HbLaoutCacheItemPtr, stlcache::policy_lru>;

  using TextBboxCache =
      stlcache::cache<std::string, TextBboxPtr, stlcache::policy_lru>;

  using TextRenderCache =
      stlcache::cache<std::string, HgPainterPtr, stlcache::policy_lru>;

public:
  explicit HgFont() = delete;
  explicit HgFont(FT_Library ftLibrary, int textCacheSize = 1000);
  ~HgFont();

  bool createFtFace(const std::string& fontFilePath, int pixelSize);
  bool destroyFtFace();

  void resetBuffer();
  void clearBuffer();
  void setDirection(hb_direction_t direction);
  void setScript(hb_script_t script);
  void setLanguage(const std::string& language);

  TextBboxPtr getBbox(const std::string& text);
  void drawText(const std::string& text,
      HgPainter* hgPainter,
      int x,
      int y,
      const litehtml::web_color& color);

  int forceUcs2Charmap(FT_Face ftf);

  const FT_Face ftFace() const { return mFtFace; }
  FT_F26Dot6 xHeight() const;

  static FT_F26Dot6 intToF26Dot6(int pixelSize);
  static int f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels);

private:
  HbLaoutCacheItemPtr layoutText(const std::string& text);

private:
  FT_Library mFtLibrary;
  FT_Face mFtFace;
  FT_Raster_Params mFtRasterParams;
  FtRasterParamsUser mFtRasterParamsUser;

  hb_buffer_t* mHbBuffer;
  hb_font_t* mHbFont;

  std::shared_ptr<HbLaoutCache> mHbLaoutCache;
  std::shared_ptr<TextBboxCache> mTextBboxCache;
  std::shared_ptr<TextRenderCache> mTextRenderCache;

  // TODO: get size, strikeout and underline from FT structs if possible,
  // TODO: remove it here.
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

inline FT_F26Dot6 HgFont::intToF26Dot6(int pixelSize)
{
  return static_cast<FT_Long>(pixelSize) * FT_64;
}

inline int HgFont::f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels)
{
  return static_cast<int>(f26Dot6Pixels / FT_64);
}

}  // namespace hg

#endif  // HG_FONT_H
