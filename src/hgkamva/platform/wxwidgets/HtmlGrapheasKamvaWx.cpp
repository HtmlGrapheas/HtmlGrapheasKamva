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

// Based on the code from:
// http://mrl.nyu.edu/~ajsecord/downloads/wxAGG-1.1.tgz

/**
 * @author Adrian Secord
 * @link http://mrl.nyu.edu/~ajsecord
 * @link http://mrl.nyu.edu/~ajsecord/downloads/wxAGG-1.1.tgz
 */

#include "HtmlGrapheasKamvaWx.h"

#include "PixelFormatConvertor.h"

#include "agg_bounding_rect.h"
#include "agg_pixfmt_rgb.h"
#include "agg_renderer_base.h"
#include "agg_renderer_primitives.h"
#include "agg_renderer_scanline.h"
#include "agg_rendering_buffer.h"
#include "agg_trans_affine.h"

#include "hgkamva/agg_freetype_harfbuzz.h"

namespace GUI
{
BEGIN_EVENT_TABLE(HtmlGrapheasKamvaWx, wxWindow)
EVT_PAINT(HtmlGrapheasKamvaWx::onPaint)
EVT_SIZE(HtmlGrapheasKamvaWx::onSize)
EVT_ERASE_BACKGROUND(HtmlGrapheasKamvaWx::onEraseBackground)
END_EVENT_TABLE()

HtmlGrapheasKamvaWx::HtmlGrapheasKamvaWx(wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style)
    : wxWindow(parent, id, pos, size, style, wxT("HtmlGrapheasKamvaWx"))
    , bitmap(NULL)
{
}

HtmlGrapheasKamvaWx::~HtmlGrapheasKamvaWx()
{
  memDC.SelectObject(wxNullBitmap);
  delete bitmap;
}

void HtmlGrapheasKamvaWx::init(const int width, const int height)
{
  // The conversion between wxWidgets' pixel format and AGG's pixel format.
  typedef PixelFormatConvertor<wxNativePixelFormat> PixelFormat;

  // The wxWidgets "pixel data" type, an accessor to the raw pixels.
  typedef wxPixelData<wxBitmap, PixelFormat::wxWidgetsType> PixelData;

  // The AGG base renderer.
  typedef agg::renderer_base<PixelFormat::AGGType> RendererBase;

  // The AGG primitives renderer.
  typedef agg::renderer_primitives<RendererBase> RendererPrimitives;

  // The AGG solid renderer.
  typedef agg::renderer_scanline_aa_solid<RendererBase> RendererSolid;

  memDC.SelectObject(wxNullBitmap);
  delete bitmap;

  bitmap =
      new wxBitmap(width, height, PixelFormat::wxWidgetsType::BitsPerPixel);
  memDC.SelectObject(*bitmap);

  // Attach the AGG rendering buffer to the bitmap
  // and call the user draw() code.

  // Draw the bitmap.
  // Get raw access to the wxWidgets bitmap -- this locks the pixels and
  // unlocks on destruction.
  PixelData data(*bitmap);
  assert(data);

  // This cast looks like it is ignoring byte-ordering, but the
  // pixel format already explicitly handles that.
  assert(data.GetPixels().IsOk());
  wxAlphaPixelFormat::ChannelType* pd =
      (wxAlphaPixelFormat::ChannelType*) &data.GetPixels().Data();

  // wxWidgets always returns a pointer to the first row of pixels, whether
  // that is stored at the beginning of the buffer (stride > 0) or at the
  // end of the buffer (stride < 0).  AGG always wants a pointer to the
  // beginning of the buffer, no matter what the stride.  (AGG does handle
  // negative strides correctly.)
  // Upshot: if the stride is negative, rewind the pointer from the end of
  // the buffer to the beginning.
  const int stride = data.GetRowStride();
  if (stride < 0)
    pd += (data.GetHeight() - 1) * stride;

  // AGG's rendering buffer, pointing into the bitmap.
  agg::rendering_buffer rbuf;
  rbuf.attach(pd, data.GetWidth(), data.GetHeight(), stride);

  // Draw into the bitmap using AGG.
  PixelFormat::AGGType pixf(rbuf);
  RendererBase rbase(pixf);
  RendererPrimitives rprim(rbase);
  RendererSolid rsolid(rbase);

  // Draw a rectangle against a background.
  rbase.clear(PixelFormat::AGGType::color_type(0, 0, 0));
  rprim.fill_color(PixelFormat::AGGType::color_type(64, 64, 200));
  rprim.solid_rectangle(rbuf.width() / 4, rbuf.height() / 4,
      3 * rbuf.width() / 4, 3 * rbuf.height() / 4);

  // Text drawing with FreeType and HarfBuzz to AGG buffer.
  agg_ft_hb_draw<PixelFormat::AGGType>(rbase);

  // Request a full redraw of the window.
  Refresh(false);
}

void HtmlGrapheasKamvaWx::onSize(wxSizeEvent& event)
{
  const wxSize size = GetClientSize();
  if (bitmap && size.GetWidth() == bitmap->GetWidth()
      && size.GetHeight() == bitmap->GetHeight())
    return;

  init(size.GetWidth(), size.GetHeight());
}

void HtmlGrapheasKamvaWx::onPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);

  wxCoord width, height;
  dc.GetSize(&width, &height);
  if (!bitmap || bitmap->GetWidth() != width || bitmap->GetHeight() != height)
    init(width, height);

  // Iterate over regions needing repainting.
  wxRegionIterator regions(GetUpdateRegion());
  wxRect rect;
  while (regions) {
    rect = regions.GetRect();
    dc.Blit(rect.x, rect.y, rect.width, rect.height, &memDC, rect.x, rect.y);
    ++regions;
  }
}

void HtmlGrapheasKamvaWx::onEraseBackground(wxEraseEvent& WXUNUSED(event))
{
  // Do nothing to "avoid flashing in MSW"  Grr.
}
}
