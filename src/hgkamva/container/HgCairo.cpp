#include "hgkamva/container/HgCairo.h"

#include <stdexcept>

#include <cairo/cairo-ft.h>

namespace hg
{
HgCairo::HgCairo(unsigned char* buffer,
    const cairo_format_t colorFormat,
    const unsigned int width,
    const unsigned int height,
    const int stride)
{
  mSurface = {cairo_image_surface_create_for_data(
                  buffer, colorFormat, width, height, stride),
      cairo_surface_destroy};
  checkStatus(cairo_surface_status, mSurface.get());

  mContext = {cairo_create(mSurface.get()), cairo_destroy};
  checkStatus(cairo_status, mContext.get());
}

HgCairo::~HgCairo() {}

void HgCairo::clear(const Color& color)
{
  cairo_save(mContext.get());
  cairo_set_source_rgba(
      mContext.get(), color.mRed, color.mGreen, color.mBlue, color.mAlpha);
  cairo_set_operator(mContext.get(), CAIRO_OPERATOR_SOURCE);
  cairo_paint(mContext.get());
  cairo_restore(mContext.get());
}

// static
HgCairo::FontFacePtr HgCairo::getFontFace(const FT_Face ftFace, const int ftLoadFlags)
{
  FontFacePtr fontFacePtr{
      cairo_ft_font_face_create_for_ft_face(ftFace, ftLoadFlags),
      cairo_font_face_destroy};
  checkPtrStatus(fontFacePtr);
  return fontFacePtr;
}

HgCairo::ScaledFontPtr HgCairo::getScaledFont(
    const FontFacePtr fontFace, const int pixelSize)
{
  cairo_save(mContext.get());
  cairo_set_font_face(mContext.get(), fontFace.get());
  cairo_set_font_size(mContext.get(), pixelSize);
  ScaledFontPtr scaledFont{
      cairo_scaled_font_reference(cairo_get_scaled_font(mContext.get())),
      cairo_scaled_font_destroy};
  cairo_restore(mContext.get());
  checkStatus(cairo_scaled_font_status, scaledFont.get());
  return scaledFont;
}

void HgCairo::showGlyphs(const GlyphVector& glyphs,
    const ScaledFontPtr scaledFont,
    const double x,
    const double y,
    const cairo_text_extents_t& extents,
    const Color& color)
{
  cairo_save(mContext.get());
  cairo_translate(mContext.get(), x, y);
  cairo_rectangle(mContext.get(), extents.x_bearing, extents.y_bearing,
      extents.width, extents.height);
  cairo_clip(mContext.get());
  cairo_set_source_rgba(
      mContext.get(), color.mRed, color.mGreen, color.mBlue, color.mAlpha);
  cairo_set_scaled_font(mContext.get(), scaledFont.get());
  // NOTE: See also: cairo_show_text_glyphs(), cairo_glyph_path()
  cairo_show_glyphs(mContext.get(), glyphs.data(), glyphs.size());
  cairo_restore(mContext.get());
}

double HgCairo::xHeight(const ScaledFontPtr scaledFont)
{
  cairo_save(mContext.get());
  cairo_set_scaled_font(mContext.get(), scaledFont.get());
  cairo_text_extents_t tex;
  cairo_text_extents(mContext.get(), "x", &tex);
  cairo_restore(mContext.get());
  return tex.height;
}

}  // namespace hg
