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

void HgCairo::save()
{
  cairo_save(mContext.get());
}

void HgCairo::restore()
{
  cairo_restore(mContext.get());
}

void HgCairo::clip(
    const double x, const double y, const double width, const double height)
{
  cairo_rectangle(mContext.get(), x, y, width, height);
  cairo_clip(mContext.get());
}

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
  cairo_save(mContext.get());
  cairo_set_source_rgba(
      mContext.get(), color.mRed, color.mGreen, color.mBlue, color.mAlpha);
  cairo_move_to(mContext.get(), x1, y1);
  cairo_line_to(mContext.get(), x2, y2);
  cairo_set_line_width(mContext.get(), width);
  cairo_stroke(mContext.get());
  cairo_restore(mContext.get());
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
int HgCairo::formatBitsPerPixel(cairo_format_t format)
{
  switch(format) {
    case CAIRO_FORMAT_RGBA128F:
      return 128;
    case CAIRO_FORMAT_RGB96F:
      return 96;
    case CAIRO_FORMAT_ARGB32:
    case CAIRO_FORMAT_RGB30:
    case CAIRO_FORMAT_RGB24:
      return 32;
    case CAIRO_FORMAT_RGB16_565:
      return 16;
    case CAIRO_FORMAT_A8:
      return 8;
    case CAIRO_FORMAT_A1:
      return 1;
    case CAIRO_FORMAT_INVALID:
    default:
      return 0;
  }
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

void HgCairo::rasterCopy(int diffX, int diffY)
{
  if(diffX == 0 && diffY == 0) {
    return;
  }

  cairo_surface_t* surface = cairo_get_target(mContext.get());
  checkStatus(cairo_surface_status, surface);
  cairo_surface_flush(surface);

  unsigned char* data = cairo_image_surface_get_data(surface);
  int height = cairo_image_surface_get_height(surface);
  int stride = cairo_image_surface_get_stride(surface);

  int srcY1, srcY2, dstY;
  if(diffY > 0) {  // To up.
    srcY1 = diffY * stride;
    srcY2 = height * stride;
    dstY = 0;
  } else {  // To down.
    srcY1 = 0;
    srcY2 = (height - -diffY) * stride;
    dstY = height * stride;
  }

  if(diffX == 0) {  // Vertical copying
    int dstY = (diffY > 0) ? 0 : -diffY * stride;
    std::copy(data + srcY1, data + srcY2, data + dstY);

  } else {
    int width = cairo_image_surface_get_width(surface);
    int bpp = stride / width;  // Bytes per pixel.

    int srcX1, srcX2, dstX;
    if(diffX > 0) {  // To left.
      srcX1 = diffX * bpp;
      srcX2 = width * bpp;
      dstX = 0;
    } else {  // To right.
      srcX1 = 0;
      srcX2 = (width - -diffX) * bpp;
      dstX = -diffX * bpp;
    }

    if(diffY > 0) {
      for(int currSrcY = srcY1, currDstY = dstY; currSrcY < srcY2;
          currSrcY += stride, currDstY += stride) {
        std::copy(data + currSrcY + srcX1, data + currSrcY + srcX2,
            data + currDstY + dstX);
      }
    } else {
      for(int currSrcY = srcY2 - stride, currDstY = dstY - stride;
          currSrcY >= srcY1; currSrcY -= stride, currDstY -= stride) {
        std::copy(data + currSrcY + srcX1, data + currSrcY + srcX2,
            data + currDstY + dstX);
      }
    }
  }

  cairo_surface_mark_dirty(surface);
}

}  // namespace hg
