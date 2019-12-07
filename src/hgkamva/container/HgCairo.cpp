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
  SurfacePtr surface = {cairo_image_surface_create_for_data(
                            buffer, colorFormat, width, height, stride),
      cairo_surface_destroy};
  checkStatus(cairo_surface_status, surface.get());

  mContext = {cairo_create(surface.get()), cairo_destroy};
  checkStatus(cairo_status, mContext.get());
}

HgCairo::~HgCairo() {}

void HgCairo::clear(const Color& color /*= Color{}*/)
{
  cairo_save(mContext.get());
  cairo_set_source_rgba(
      mContext.get(), color.mRed, color.mGreen, color.mBlue, color.mAlpha);
  cairo_set_operator(mContext.get(), CAIRO_OPERATOR_SOURCE);
  cairo_paint(mContext.get());
  cairo_restore(mContext.get());
}

void HgCairo::drawLine(const double x1,
    const double y1,
    const double x2,
    const double y2,
    const double width,
    const Color& color /*= Color{}*/)
{
  cairo_set_source_rgba(
      mContext.get(), color.mRed, color.mGreen, color.mBlue, color.mAlpha);
  cairo_move_to(mContext.get(), x1, y1);
  cairo_line_to(mContext.get(), x2, y2);
  cairo_set_line_width(mContext.get(), width);
  cairo_stroke(mContext.get());
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

// static
HgCairo::ScaledFontPtr HgCairo::getScaledFont(
    const FT_Face ftFace, const int ftLoadFlags, const int pixelSize)
{
  FontFacePtr fontFace{
      cairo_ft_font_face_create_for_ft_face(ftFace, ftLoadFlags),
      cairo_font_face_destroy};
  checkPtrStatus(fontFace);

  cairo_matrix_t fontMatrix;
  cairo_matrix_init_scale(&fontMatrix, pixelSize, pixelSize);
  cairo_matrix_t ctm;
  cairo_matrix_init_identity(&ctm);

  FontOptionsPtr fontOptions{
      cairo_font_options_create(), cairo_font_options_destroy};
  checkStatus(cairo_font_options_status, fontOptions.get());
  cairo_font_options_set_antialias(fontOptions.get(), CAIRO_ANTIALIAS_GRAY);
  cairo_font_options_set_hint_style(fontOptions.get(), CAIRO_HINT_STYLE_MEDIUM);
  cairo_font_options_set_hint_metrics(
      fontOptions.get(), CAIRO_HINT_METRICS_DEFAULT);
  // CSS font-variation-settings
  // (in Chrome browser!) see http://css.yoksel.ru/opentype-variable-fonts/
  //cairo_font_options_set_variations(options, "wght=200, wdth=140.5");

  ScaledFontPtr scaledFont{cairo_scaled_font_create(fontFace.get(), &fontMatrix,
                               &ctm, fontOptions.get()),
      cairo_scaled_font_destroy};
  checkStatus(cairo_scaled_font_status, scaledFont.get());

  return scaledFont;
}

// static
double HgCairo::xHeight(const ScaledFontPtr scaledFont)
{
  cairo_text_extents_t textExtents;
  cairo_scaled_font_text_extents(scaledFont.get(), "x", &textExtents);
  return textExtents.height;
}

}  // namespace hg
