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

#include "hgkamva/container/HgFont.h"

#include <cstdio>
#include <limits>

namespace hg
{
HgFont::HgFont(FT_Library ftLibrary)
    : mFtLibrary(ftLibrary)
    , mFtFace(nullptr)
    , mHbFont(nullptr)
    , mGlyphCount(0)
    , mGlyphInfo(nullptr)
    , mGlyphPos(nullptr)
    , mPixelSize(0)
    , mStrikeout(false)
    , mUnderline(false)
{
  mHbBuffer = hb_buffer_create();
}

HgFont::~HgFont()
{
  destroyFtFace();
  hb_buffer_destroy(mHbBuffer);
}

bool HgFont::createFtFace(const std::string& fontFilePath, int pixelSize)
{
  bool ret = destroyFtFace()
      && (FT_New_Face(mFtLibrary, fontFilePath.c_str(), 0, &mFtFace)
                 == FT_Err_Ok)
      && (FT_Set_Pixel_Sizes(mFtFace, 0, pixelSize) == FT_Err_Ok)
      && (forceUcs2Charmap(mFtFace) == FT_Err_Ok)  // We ignore encoding.
      && (mHbFont = hb_ft_font_create(mFtFace, NULL));

  if(ret) {
    // Set up rendering via spanners.
    mFtRasterParams.target = 0;
    mFtRasterParams.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
    mFtRasterParams.user = &mFtRasterParamsUser;
    mFtRasterParams.black_spans = 0;
    mFtRasterParams.bit_set = 0;
    mFtRasterParams.bit_test = 0;
  } else {
    destroyFtFace();
  }
  return ret;
}

/*  See http://www.microsoft.com/typography/otspec/name.htm
    for a list of some possible platform-encoding pairs.
    We're interested in 0-3 aka 3-1 - UCS-2.
    Otherwise, fail. If a font has some unicode map, but lacks
    UCS-2 - it is a broken or irrelevant font. What exactly
    Freetype will select on face load (it promises most wide
    unicode, and if that will be slower that UCS-2 - left as
    an excercise to check. */
int HgFont::forceUcs2Charmap(FT_Face ftf)
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

bool HgFont::destroyFtFace()
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

void HgFont::resetBuffer()
{
  hb_buffer_reset(mHbBuffer);
}

void HgFont::clearBuffer()
{
  // Clean up the buffer, but don't kill it just yet.
  hb_buffer_clear_contents(mHbBuffer);
}

void HgFont::setDirection(hb_direction_t direction)
{
  // NOTE: see also hb_script_get_horizontal_direction()
  hb_buffer_set_direction(mHbBuffer, direction);
}

void HgFont::setScript(hb_script_t script)
{
  hb_buffer_set_script(mHbBuffer, script);  // see hb-unicode.h
}

void HgFont::setLanguage(const std::string& language)
{
  // For ISO 639 Code see
  // http://www.loc.gov/standards/iso639-2/php/code_list.php
  hb_buffer_set_language(
      mHbBuffer, hb_language_from_string(language.c_str(), language.size()));
}

void HgFont::layoutText(const std::string& text)
{
  // TODO: check buffer state.

  // Layout the text
  hb_buffer_add_utf8(mHbBuffer, text.c_str(), text.size(), 0, text.size());
  hb_shape(mHbFont, mHbBuffer, NULL, 0);
  mGlyphInfo = hb_buffer_get_glyph_infos(mHbBuffer, &mGlyphCount);
  mGlyphPos = hb_buffer_get_glyph_positions(mHbBuffer, &mGlyphCount);
}

typename HgFont::TextBbox HgFont::getBbox()
{
  // TODO: check buffer state.

  static constexpr int Int_MIN = std::numeric_limits<int>::min();
  static constexpr int Int_MAX = std::numeric_limits<int>::max();

  // Calculate string bounding box in pixels.
  mFtRasterParams.gray_spans = aggSpannerSizer;

  // See http://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html

  TextBbox bbox;

  // Largest coordinate a pixel has been set at, or the pen was advanced to.
  bbox.mMaxX = Int_MIN;
  // Smallest coordinate a pixel has been set at, or the pen was advanced to.
  bbox.mMinX = Int_MAX;
  // This is max topside bearing along the string.
  bbox.mMaxY = Int_MIN;
  // This is max value of (height - topbearing) along the string.
  bbox.mMinY = Int_MAX;
  /*  Naturally, the above comments swap their meaning between
              horizontal and vertical scripts, since the pen changes the axis
              it is advanced along. However, their differences still make up
              the bounding box for the string. Also note that all this is
              in FT coordinate system where y axis points upwards.
           */

  int sizerX = 0;
  int sizerY = 0;  // In FT coordinate system.

  FT_Error ftErr;
  for(unsigned j = 0; j < mGlyphCount; ++j) {
    if((ftErr = FT_Load_Glyph(mFtFace, mGlyphInfo[j].codepoint, 0))) {
      std::printf(
          "load %08x failed ftErr=%d.\n", mGlyphInfo[j].codepoint, ftErr);
    } else {
      if(mFtFace->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
        std::printf("glyph->format = %4s\n",
            reinterpret_cast<char*>(&mFtFace->glyph->format));
      } else {
        int gx = sizerX + (mGlyphPos[j].x_offset / FT_64);
        // Note how the sign differs from the rendering pass.
        int gy = sizerY + (mGlyphPos[j].y_offset / FT_64);

        mFtRasterParamsUser.mMinSpanX = Int_MAX;
        mFtRasterParamsUser.mMaxSpanX = Int_MIN;
        mFtRasterParamsUser.mMinY = Int_MAX;
        mFtRasterParamsUser.mMaxY = Int_MIN;

        if((ftErr = FT_Outline_Render(
                mFtLibrary, &mFtFace->glyph->outline, &mFtRasterParams))) {
          std::printf("FT_Outline_Render() failed err=%d\n", ftErr);
        }

        if(mFtRasterParamsUser.mMinSpanX != INT_MAX) {
          // Update values if the spanner was actually called.
          if(bbox.mMinX > mFtRasterParamsUser.mMinSpanX + gx)
            bbox.mMinX = mFtRasterParamsUser.mMinSpanX + gx;

          if(bbox.mMaxX < mFtRasterParamsUser.mMaxSpanX + gx)
            bbox.mMaxX = mFtRasterParamsUser.mMaxSpanX + gx;

          if(bbox.mMinY > mFtRasterParamsUser.mMinY + gy)
            bbox.mMinY = mFtRasterParamsUser.mMinY + gy;

          if(bbox.mMaxY < mFtRasterParamsUser.mMaxY + gy)
            bbox.mMaxY = mFtRasterParamsUser.mMaxY + gy;
        } else {
          // The spanner wasn't called at all - an empty glyph, like space.
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

    sizerX += mGlyphPos[j].x_advance / FT_64;
    // Note how the sign differs from the rendering pass.
    sizerY += mGlyphPos[j].y_advance / FT_64;
  }
  // Still have to take into account last glyph's advance. Or not?
  if(bbox.mMinX > sizerX)
    bbox.mMinX = sizerX;
  if(bbox.mMaxX < sizerX)
    bbox.mMaxX = sizerX;
  if(bbox.mMinY > sizerY)
    bbox.mMinY = sizerY;
  if(bbox.mMaxY < sizerY)
    bbox.mMaxY = sizerY;

  // The bounding box.
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

void HgFont::drawText(
    HgRenderer* hgRenderer, int x, int y, litehtml::web_color color)
{
  /* About params int x and int y:
     The pen/baseline start coordinates in window coordinate system
              - with those text placement in the window is controlled.
              - note that for RTL scripts pen still goes LTR */

  // Set rendering spanner.
  mFtRasterParams.gray_spans = aggSpannerBlend;

  // Initialize rendering part of the FtRasterParamsUser.
  mFtRasterParamsUser.mHgRenderer = hgRenderer;
  mFtRasterParamsUser.mColor = color;

  // Render.
  FT_Error ftErr;
  for(unsigned j = 0; j < mGlyphCount; ++j) {
    if((ftErr = FT_Load_Glyph(mFtFace, mGlyphInfo[j].codepoint, 0))) {
      printf("load %08x failed ftErr=%d.\n", mGlyphInfo[j].codepoint, ftErr);
    } else {
      if(mFtFace->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
        std::printf("glyph->format = %4s\n",
            reinterpret_cast<char*>(&mFtFace->glyph->format));
      } else {
        mFtRasterParamsUser.mGlyphX = x + (mGlyphPos[j].x_offset / FT_64);
        mFtRasterParamsUser.mGlyphY = y - (mGlyphPos[j].y_offset / FT_64);

        if((ftErr = FT_Outline_Render(
                mFtLibrary, &mFtFace->glyph->outline, &mFtRasterParams)))
          printf("FT_Outline_Render() failed err=%d\n", ftErr);
      }
    }

    x += mGlyphPos[j].x_advance / FT_64;
    y -= mGlyphPos[j].y_advance / FT_64;
  }
}

/*  This spanner is for obtaining exact bounding box for the string.
    Unfortunately this can't be done without rendering it (or pretending to).
    After this runs, we get min and max values of coordinates used. */
// static
void HgFont::aggSpannerSizer(int y, int count, const FT_Span* spans, void* user)
{
  FtRasterParamsUser* ftUser = static_cast<FtRasterParamsUser*>(user);

  if(y < ftUser->mMinY)
    ftUser->mMinY = y;
  if(y > ftUser->mMaxY)
    ftUser->mMaxY = y;

  for(int i = 0; i < count; i++) {
    if(spans[i].x + spans[i].len > ftUser->mMaxSpanX)
      ftUser->mMaxSpanX = spans[i].x + spans[i].len;
    if(spans[i].x < ftUser->mMinSpanX)
      ftUser->mMinSpanX = spans[i].x;
  }
}

// static
void HgFont::aggSpannerBlend(int y, int count, const FT_Span* spans, void* user)
{
  FtRasterParamsUser* ftUser = static_cast<FtRasterParamsUser*>(user);

  for(int i = 0; i < count; ++i) {
    ftUser->mHgRenderer->blendHLine(ftUser->mGlyphX + spans[i].x,
        ftUser->mGlyphY - y, ftUser->mGlyphX + spans[i].x + spans[i].len - 1,
        ftUser->mColor, spans[i].coverage);
  }
}

FT_F26Dot6 HgFont::xHeight() const
{
  if(!mFtFace || FT_Load_Char(mFtFace, 'x', 0) != FT_Err_Ok) {
    return 0;
  }
  return mFtFace->glyph->metrics.height;
}

}  // namespace hg
