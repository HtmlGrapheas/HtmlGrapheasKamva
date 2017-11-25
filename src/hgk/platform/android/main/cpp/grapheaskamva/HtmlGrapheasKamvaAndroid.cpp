/*****************************************************************************
 * Project:  HtmlGrapheas
 * Purpose:  HTML text editor library
 * Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *   Copyright (c) 2017 NikitaFeodonit
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

#include "HtmlGrapheasKamvaAndroid.h"

#include <iostream>

#include <android/bitmap.h>

#include "agg_basics.h"
#include "agg_bounding_rect.h"
#include "agg_pixfmt_rgb_packed.h"
#include "agg_pixfmt_rgba.h"
#include "agg_renderer_base.h"
#include "agg_renderer_primitives.h"
#include "agg_renderer_scanline.h"
#include "agg_trans_affine.h"

#include "agg_freetype_harfbuzz.h"

namespace fgr
{
// static
bool HtmlGrapheasKamvaAndroid::drawIntoBitmap(void* p_pixels,
    uint32_t width,
    uint32_t height,
    uint32_t stride,
    uint32_t format)
{
  switch (format) {
    case ANDROID_BITMAP_FORMAT_RGB_565: {
      return drawIntoBitmap_pixfmt<agg::pixfmt_rgb565>(
          p_pixels, width, height, stride);
    }
    case ANDROID_BITMAP_FORMAT_RGBA_8888: {
      return drawIntoBitmap_pixfmt<agg::pixfmt_rgba32>(
          p_pixels, width, height, stride);
    }
    default: {
      std::cout << "HtmlGrapheasKamvaAndroid::drawIntoBitmap(), unsupported "
                   "bitmap format\n";
      return false;
    }
  }
}

// static
template <class PixelFormat>
bool HtmlGrapheasKamvaAndroid::drawIntoBitmap_pixfmt(
    void* p_pixels, uint32_t width, uint32_t height, uint32_t stride)
{
  typedef typename PixelFormat::color_type ColorType;
  typedef agg::renderer_base<PixelFormat> RendererBase;
  typedef agg::renderer_primitives<RendererBase> RendererPrimitives;

  agg::rendering_buffer rbuf;
  rbuf.attach(static_cast<agg::int8u*>(p_pixels), width, height, stride);

  PixelFormat pixf(rbuf);
  RendererBase rbase(pixf);
  RendererPrimitives rprim(rbase);

  rbase.clear(ColorType(0, 0, 0));
  rprim.fill_color(ColorType(64, 64, 200));
  rprim.solid_rectangle(rbuf.width() / 4, rbuf.height() / 4,
      3 * rbuf.width() / 4, 3 * rbuf.height() / 4);

  // Text drawing with FreeType and HarfBuzz to AGG buffer
  std::cout << "START agg_ft_hb_draw()\n";
//  agg_ft_hb_draw<PixelFormat>(rbase);

  return true;
}

}  // namespace fgr
