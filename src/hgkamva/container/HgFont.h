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

#include <cstdio>
#include <limits>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb-ft.h>
#include <hb.h>

#include "agg_renderer_base.h"

#include "litehtml.h"

namespace hg
{
template <class PixelFormat>
class HgFont
{
private:
  static constexpr int FT_64 = 64;

  struct AggSpannerBaton
  {
    agg::renderer_base<PixelFormat>* mRenderBase;
    int mGlyphX;
    int mGlyphY;

    // Sizing part.
    int mMinSpanX;
    int mMaxSpanX;
    int mMinY;
    int mMaxY;
  };

  static void aggSpannerBlend(
      int y, int count, const FT_Span* spans, void* user);
  static void aggSpannerSizer(
      int y, int count, const FT_Span* spans, void* user);

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

  explicit HgFont() = delete;
  explicit HgFont(FT_Library ftLibrary);
  virtual ~HgFont();

  bool createFtFace(const std::string& fontFilePath, int pixelSize);
  bool destroyFtFace();

  void resetBuffer();
  void setDirection(hb_direction_t direction);
  void setScript(hb_script_t script);
  void setLanguage(const std::string& language);

  void layoutText(const std::string& text);
  TextBbox getBbox();

  int forceUcs2Charmap(FT_Face ftf);

  const FT_Face ftFace() const { return mFtFace; }
  FT_F26Dot6 xHeight() const;

  static FT_F26Dot6 intToF26Dot6(int pixelSize);
  static int f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels);

private:
  FT_Library mFtLibrary;
  FT_Face mFtFace;

  hb_buffer_t* mHbBuffer;
  hb_font_t* mHbFont;

  unsigned int mGlyphCount;
  hb_glyph_info_t* mGlyphInfos;
  hb_glyph_position_t* mGlyphPositions;

  AggSpannerBaton mSpannerBaton;
  FT_Raster_Params mFtRasterParams;

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

template <class PixelFormat>
inline FT_F26Dot6 HgFont<PixelFormat>::intToF26Dot6(int pixelSize)
{
  return static_cast<FT_Long>(pixelSize) * FT_64;
}

template <class PixelFormat>
inline int HgFont<PixelFormat>::f26Dot6ToInt(FT_F26Dot6 f26Dot6Pixels)
{
  return static_cast<int>(f26Dot6Pixels / FT_64);
}

template <class PixelFormat>
HgFont<PixelFormat>::HgFont(FT_Library ftLibrary)
    : mFtLibrary(ftLibrary)
    , mFtFace(nullptr)
    , mHbFont(nullptr)
    , mGlyphCount(0)
    , mGlyphInfos(nullptr)
    , mGlyphPositions(nullptr)
    , mPixelSize(0)
    , mStrikeout(false)
    , mUnderline(false)
{
  mHbBuffer = hb_buffer_create();
}

template <class PixelFormat>
HgFont<PixelFormat>::~HgFont()
{
  destroyFtFace();
  hb_buffer_destroy(mHbBuffer);
}

template <class PixelFormat>
bool HgFont<PixelFormat>::createFtFace(
    const std::string& fontFilePath, int pixelSize)
{
  bool ret = destroyFtFace()
      && (FT_New_Face(mFtLibrary, fontFilePath.c_str(), 0, &mFtFace)
                 == FT_Err_Ok)
      && (FT_Set_Pixel_Sizes(mFtFace, 0, pixelSize) == FT_Err_Ok)
      && (forceUcs2Charmap(mFtFace) == FT_Err_Ok)  // We ignore encoding.
      && (mHbFont = hb_ft_font_create(mFtFace, NULL));

  if(ret) {
    /* set up rendering via spanners */
    //agg_spanner_baton_t<PixelFormat> mStuffbaton;

    //FT_Raster_Params mFtRasterParams;
    mFtRasterParams.target = 0;
    mFtRasterParams.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
    mFtRasterParams.user = &mSpannerBaton;
    mFtRasterParams.black_spans = 0;
    mFtRasterParams.bit_set = 0;
    mFtRasterParams.bit_test = 0;
  } else {
    destroyFtFace();
  }
  return ret;
}

template <class PixelFormat>
bool HgFont<PixelFormat>::destroyFtFace()
{
  if(mHbFont) {
    hb_font_destroy(mHbFont);
    mHbFont = nullptr;
  }
  if(mFtFace) {
    FT_Done_Face(mFtFace);
    mFtFace = nullptr;
  }
  return true;
}

template <class PixelFormat>
void HgFont<PixelFormat>::resetBuffer()
{
  hb_buffer_reset(mHbBuffer);
}

template <class PixelFormat>
void HgFont<PixelFormat>::setDirection(hb_direction_t direction)
{
  // NOTE: see also hb_script_get_horizontal_direction()
  hb_buffer_set_direction(mHbBuffer, direction);
}

template <class PixelFormat>
void HgFont<PixelFormat>::setScript(hb_script_t script)
{
  hb_buffer_set_script(mHbBuffer, script);  // see hb-unicode.h
}

template <class PixelFormat>
void HgFont<PixelFormat>::setLanguage(const std::string& language)
{
  // For ISO 639 Code see
  // http://www.loc.gov/standards/iso639-2/php/code_list.php
  hb_buffer_set_language(
      mHbBuffer, hb_language_from_string(language.c_str(), language.size()));
}

template <class PixelFormat>
void HgFont<PixelFormat>::layoutText(const std::string& text)
{
  // TODO: check buffer state.

  // Layout the text
  hb_buffer_add_utf8(mHbBuffer, text.c_str(), text.size(), 0, text.size());
  hb_shape(mHbFont, mHbBuffer, NULL, 0);
  mGlyphInfos = hb_buffer_get_glyph_infos(mHbBuffer, &mGlyphCount);
  mGlyphPositions = hb_buffer_get_glyph_positions(mHbBuffer, &mGlyphCount);
}

template <class PixelFormat>
typename HgFont<PixelFormat>::TextBbox HgFont<PixelFormat>::getBbox()
{
  // TODO: check buffer state.

  static constexpr int Int_MIN = std::numeric_limits<int>::min();
  static constexpr int Int_MAX = std::numeric_limits<int>::max();

  /* Calculate string bounding box in pixels */
  mFtRasterParams.gray_spans = aggSpannerSizer;

  /* See
             http://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html */

  TextBbox bbox;

  /* largest coordinate a pixel has been set at,
             or the pen was advanced to. */
  bbox.mMaxX = Int_MIN;
  /* smallest coordinate a pixel has been set at,
             or the pen was advanced to. */
  bbox.mMinX = Int_MAX;
  /* this is max topside bearing along the string. */
  bbox.mMaxY = Int_MIN;
  /* this is max value of (height - topbearing) along the string. */
  bbox.mMinY = Int_MAX;
  /*  Naturally, the above comments swap their meaning between
              horizontal and vertical scripts, since the pen changes the axis
              it is advanced along. However, their differences still make up
              the bounding box for the string. Also note that all this is
              in FT coordinate system where y axis points upwards.
           */

  int sizerX = 0;
  int sizerY = 0; /* in FT coordinate system. */

  FT_Error ftErr;
  for(unsigned j = 0; j < mGlyphCount; ++j) {
    if((ftErr = FT_Load_Glyph(mFtFace, mGlyphInfos->codepoint, 0))) {
      std::printf(
          "load %08x failed fterr=%d.\n", mGlyphInfos->codepoint, ftErr);
    } else {
      if(mFtFace->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
        std::printf("glyph->format = %4s\n", (char*) &mFtFace->glyph->format);
      } else {
        int gx = sizerX + (mGlyphPositions->x_offset / FT_64);
        /* note how the sign differs from the rendering pass */
        int gy = sizerY + (mGlyphPositions->y_offset / FT_64);

        mSpannerBaton.mMinSpanX = Int_MAX;
        mSpannerBaton.mMaxSpanX = Int_MIN;
        mSpannerBaton.mMinY = Int_MAX;
        mSpannerBaton.mMaxY = Int_MIN;

        if((ftErr = FT_Outline_Render(
                mFtLibrary, &mFtFace->glyph->outline, &mFtRasterParams))) {
          std::printf("FT_Outline_Render() failed err=%d\n", ftErr);
        }

        if(mSpannerBaton.mMinSpanX != INT_MAX) {
          /* Update values if the spanner was actually called. */
          if(bbox.mMinX > mSpannerBaton.mMinSpanX + gx)
            bbox.mMinX = mSpannerBaton.mMinSpanX + gx;

          if(bbox.mMaxX < mSpannerBaton.mMaxSpanX + gx)
            bbox.mMaxX = mSpannerBaton.mMaxSpanX + gx;

          if(bbox.mMinY > mSpannerBaton.mMinY + gy)
            bbox.mMinY = mSpannerBaton.mMinY + gy;

          if(bbox.mMaxY < mSpannerBaton.mMaxY + gy)
            bbox.mMaxY = mSpannerBaton.mMaxY + gy;
        } else {
          /* The spanner wasn't called at all - an empty glyph,
                         like space. */
          if(bbox.mMinX > gx)
            bbox.mMinX = gx;
          if(bbox.mMaxX < gx)
            bbox.mMaxX = gx;
          if(bbox.mMinY > gy)
            bbox.mMinY = gy;
          if(bbox.mMaxY < gy)
            bbox.mMaxY = gy;
        }
      }
    }

    sizerX += mGlyphPositions->x_advance / FT_64;
    /* note how the sign differs from the rendering pass */
    sizerY += mGlyphPositions->y_advance / FT_64;
  }
  /* Still have to take into account last glyph's advance. Or not? */
  if(bbox.mMinX > sizerX)
    bbox.mMinX = sizerX;
  if(bbox.mMaxX < sizerX)
    bbox.mMaxX = sizerX;
  if(bbox.mMinY > sizerY)
    bbox.mMinY = sizerY;
  if(bbox.mMaxY < sizerY)
    bbox.mMaxY = sizerY;

  /* The bounding box */
  bbox.mBboxW = bbox.mMaxX - bbox.mMinX;
  bbox.mBboxH = bbox.mMaxY - bbox.mMinY;

  /* Two offsets below position the bounding box with respect
             to the 'origin', which is sort of origin of string's first glyph.

              baseline_offset - offset perpendecular to the baseline
                                to the topmost (horizontal),
                                or leftmost (vertical) pixel drawn.

              baseline_shift  - offset along the baseline, from the first
                                drawn glyph's origin to the leftmost
                                (horizontal), or topmost (vertical) pixel drawn.

              Thus those offsets allow positioning the bounding box to fit
              the rendered string, as they are in fact offsets from the point
              given to the renderer, to the top left corner of the bounding box.

              NB: baseline is defined as y==0 for horizontal and x==0 for
              vertical scripts.
              (0,0) here is where the first glyph's origin ended up after
              shaping, not taking into account glyph_pos[0].xy_offset
              (yeah, my head hurts too).
          */

  if(HB_DIRECTION_IS_HORIZONTAL(hb_buffer_get_direction(mHbBuffer))) {
    bbox.mBaselineOffset = bbox.mMaxY;
    bbox.mBaselineShift = bbox.mMinX;
  }
  if(HB_DIRECTION_IS_VERTICAL(hb_buffer_get_direction(mHbBuffer))) {
    bbox.mBaselineOffset = bbox.mMinX;
    bbox.mBaselineShift = bbox.mMaxY;
  }

  return bbox;
}

/*  See http://www.microsoft.com/typography/otspec/name.htm
    for a list of some possible platform-encoding pairs.
    We're interested in 0-3 aka 3-1 - UCS-2.
    Otherwise, fail. If a font has some unicode map, but lacks
    UCS-2 - it is a broken or irrelevant font. What exactly
    Freetype will select on face load (it promises most wide
    unicode, and if that will be slower that UCS-2 - left as
    an excercise to check. */
template <class PixelFormat>
int HgFont<PixelFormat>::forceUcs2Charmap(FT_Face ftf)
{
  for(int i = 0; i < ftf->num_charmaps; i++) {
    if(((ftf->charmaps[i]->platform_id == 0)
           && (ftf->charmaps[i]->encoding_id == 3))
        || ((ftf->charmaps[i]->platform_id == 3)
               && (ftf->charmaps[i]->encoding_id == 1))) {
      return (FT_Set_Charmap(ftf, ftf->charmaps[i]));
    }
  }
  return (-1);
}

// static
template <class PixelFormat>
void HgFont<PixelFormat>::aggSpannerBlend(
    int y, int count, const FT_Span* spans, void* user)
{
  typedef typename agg::renderer_base<PixelFormat>::color_type rbase_color_type;

  AggSpannerBaton* baton = static_cast<AggSpannerBaton*>(user);

  rbase_color_type color;
  color.clear();
  color.opacity(1.0);
  color.r = 128;
  color.g = 128;
  color.b = 128;

  for(int i = 0; i < count; ++i) {
    baton->mRenderBase->blend_hline(baton->mGlyphX + spans[i].x,
        baton->mGlyphY - y, baton->mGlyphX + spans[i].x + spans[i].len - 1,
        color, spans[i].coverage);
  }
}

/*  This spanner is for obtaining exact bounding box for the string.
    Unfortunately this can't be done without rendering it (or pretending to).
    After this runs, we get min and max values of coordinates used. */
// static
template <class PixelFormat>
void HgFont<PixelFormat>::aggSpannerSizer(
    int y, int count, const FT_Span* spans, void* user)
{
  AggSpannerBaton* baton = static_cast<AggSpannerBaton*>(user);

  if(y < baton->mMinY)
    baton->mMinY = y;
  if(y > baton->mMaxY)
    baton->mMaxY = y;
  for(int i = 0; i < count; i++) {
    if(spans[i].x + spans[i].len > baton->mMaxSpanX)
      baton->mMaxSpanX = spans[i].x + spans[i].len;
    if(spans[i].x < baton->mMinSpanX)
      baton->mMinSpanX = spans[i].x;
  }
}

template <class PixelFormat>
FT_F26Dot6 HgFont<PixelFormat>::xHeight() const
{
  if(!mFtFace || FT_Load_Char(mFtFace, 'x', 0) != FT_Err_Ok) {
    return 0;
  }
  return mFtFace->glyph->metrics.height;
}

}  // namespace hg

#endif  // HG_FONT_H
