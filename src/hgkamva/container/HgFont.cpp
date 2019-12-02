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

#include <cassert>
#include <cstdio>
#include <limits>
#include <stdexcept>

#include <cairo/cairo-ft.h>

namespace hg
{
HgFont::HgFont(cairo_t* cairoContext, FT_Library ftLibrary, int textCacheSize)
    : mFtLibrary{ftLibrary}
    , mFtFace{nullptr}
    , mHbBuffer{hb_buffer_create()}
    , mHbFont{nullptr}
    , mCairoContext{cairoContext}
    , mCairoFontFace{nullptr}
    , mCairoScaledFont{nullptr}
    , mHbLaoutCache{std::make_shared<HbLaoutCache>(textCacheSize)}
    , mPixelSize{10}
    , mStrikeout{false}
    , mUnderline{false}
{
}

HgFont::~HgFont()
{
  destroyFtFace();
  hb_buffer_destroy(mHbBuffer);
}

bool HgFont::createFtFace(const std::string& fontFilePath, int pixelSize)
{
  // px = pt * DPI / 72
  mPixelSize = pixelSize;

  if(!destroyFtFace()) {
    throw std::logic_error("!destroyFtFace()");
  }

  if(FT_New_Face(mFtLibrary, fontFilePath.c_str(), 0, &mFtFace) != FT_Err_Ok) {
    throw std::logic_error("FT_New_Face() != FT_Err_Ok");
  }

  //  if(FT_Set_Pixel_Sizes(mFtFace, 0, mPixelSize) != FT_Err_Ok) {
  //    throw std::logic_error("FT_Set_Pixel_Sizes() != FT_Err_Ok");
  //  }

  // We ignore encoding.
  if(forceUcs2Charmap(mFtFace) != FT_Err_Ok) {
    throw std::logic_error("forceUcs2Charmap() != FT_Err_Ok");
  }

  if(!(mHbFont = hb_ft_font_create(mFtFace, nullptr))) {
    throw std::logic_error("hb_ft_font_create() returns nullptr");
  }

  if(!(mCairoFontFace =
             cairo_ft_font_face_create_for_ft_face(mFtFace, FT_LOAD_DEFAULT))) {
    throw std::logic_error(
        "cairo_ft_font_face_create_for_ft_face() returns nullptr");
  }

  cairo_save(mCairoContext);
  cairo_set_font_face(mCairoContext, mCairoFontFace);
  cairo_set_font_size(mCairoContext, mPixelSize);
  mCairoScaledFont =
      cairo_scaled_font_reference(cairo_get_scaled_font(mCairoContext));
  cairo_restore(mCairoContext);
  if(cairo_scaled_font_status(mCairoScaledFont) != CAIRO_STATUS_SUCCESS) {
    throw std::logic_error(
        "cairo_scaled_font_status() != CAIRO_STATUS_SUCCESS");
  }

  //    mFtRasterParams.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
  return true;
}

// See http://www.microsoft.com/typography/otspec/name.htm
//    for a list of some possible platform-encoding pairs.
//    We're interested in 0-3 aka 3-1 - UCS-2.
//    Otherwise, fail. If a font has some unicode map, but lacks
//    UCS-2 - it is a broken or irrelevant font. What exactly
//    Freetype will select on face load (it promises most wide
//    unicode, and if that will be slower that UCS-2 - left as
//    an excercise to check.
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
  return FT_Err_Invalid_Argument;
}

bool HgFont::destroyFtFace()
{
  if(mCairoScaledFont) {
    cairo_scaled_font_destroy(mCairoScaledFont);
    mCairoScaledFont = nullptr;
  }
  if(mCairoFontFace) {
    cairo_font_face_destroy(mCairoFontFace);
    mCairoFontFace = nullptr;
  }

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

typename HgFont::HbLaoutCacheItemPtr HgFont::layoutText(const std::string& text)
{
  // TODO: check buffer state.

  if(mHbLaoutCache->check(text)) {
    return mHbLaoutCache->fetch(text);
  }

  // Layout the text
  hb_buffer_add_utf8(mHbBuffer, text.c_str(), text.size(), 0, text.size());
  hb_shape(mHbFont, mHbBuffer, nullptr, 0);

  unsigned int glyphCount;
  hb_glyph_info_t* glyphInfo =
      hb_buffer_get_glyph_infos(mHbBuffer, &glyphCount);
  hb_glyph_position_t* glyphPos =
      hb_buffer_get_glyph_positions(mHbBuffer, &glyphCount);

  CairoGlyphArrayPtr cairoGlyphArray =
      std::make_shared<CairoGlyphArray>(glyphCount);
  double x = 0;
  double y = 0;
  for(unsigned int i = 0; i < glyphCount; ++i) {
    (*cairoGlyphArray)[i].index = glyphInfo[i].codepoint;
    (*cairoGlyphArray)[i].x =
        x + (static_cast<double>(glyphPos[i].x_offset) / FT_64_DOUBLE);
    (*cairoGlyphArray)[i].y =
        y - (static_cast<double>(glyphPos[i].y_offset) / FT_64_DOUBLE);
    x += static_cast<double>(glyphPos[i].x_advance) / FT_64_DOUBLE;
    y -= static_cast<double>(glyphPos[i].y_advance) / FT_64_DOUBLE;
  }

  CairoTextExtentsPtr cairoTextExtents =
      std::make_shared<cairo_text_extents_t>();
  cairo_scaled_font_glyph_extents(mCairoScaledFont, cairoGlyphArray->data(),
      glyphCount, cairoTextExtents.get());

  HbLaoutCacheItemPtr hbLaoutCacheItem =
      std::make_shared<HbLaoutCacheItem>(cairoGlyphArray, cairoTextExtents);

  mHbLaoutCache->insert(text, hbLaoutCacheItem);
  return hbLaoutCacheItem;
}

hg::HgFont::CairoTextExtentsPtr HgFont::getTextExtents(const std::string& text)
{
  return layoutText(text)->mCairoTextExtents;
}

void HgFont::drawText(
    const std::string& text, int x, int y, const litehtml::web_color& color)
{
  HbLaoutCacheItemPtr hbLayoutInfo = layoutText(text);

  // Move glyphs to position (x, y)
  CairoGlyphArray glyphs{*(hbLayoutInfo->mCairoGlyph)};
  for(unsigned int i = 0, size = glyphs.size(); i < size; ++i) {
    glyphs[i].x += x;
    glyphs[i].y += y;
  }

  cairo_save(mCairoContext);
  cairo_set_source_rgba(mCairoContext, color.red / 255.0, color.green / 255.0,
      color.blue / 255.0, color.alpha / 255.0);
  cairo_set_scaled_font(mCairoContext, mCairoScaledFont);
  // NOTE: See also: cairo_show_text_glyphs(), cairo_glyph_path()
  cairo_show_glyphs(mCairoContext, glyphs.data(), glyphs.size());
  cairo_restore(mCairoContext);
}

double HgFont::xHeight()
{
  cairo_save(mCairoContext);

  cairo_set_font_face(mCairoContext, mCairoFontFace);
  cairo_set_font_size(mCairoContext, mPixelSize);
  cairo_text_extents_t tex;
  cairo_text_extents(mCairoContext, "x", &tex);

  cairo_restore(mCairoContext);

  return tex.height;
}

}  // namespace hg
