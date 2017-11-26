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

// Based on the code from:
// http://mrl.nyu.edu/~ajsecord/downloads/wxAGG-1.1.tgz

#include "HtmlGrapheasKamvaWx.h"

#include "agg_bounding_rect.h"
#include "agg_renderer_base.h"
#include "agg_renderer_primitives.h"
#include "agg_renderer_scanline.h"
#include "agg_trans_affine.h"

#include "agg_freetype_harfbuzz.h"

namespace GUI
{
HtmlGrapheasKamvaWx::HtmlGrapheasKamvaWx(wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style)
    : AGGWindow(parent, id, pos, size, style)
{
}

HtmlGrapheasKamvaWx::~HtmlGrapheasKamvaWx()
{
}

void HtmlGrapheasKamvaWx::draw()
{
  /// The AGG base renderer
  typedef agg::renderer_base<PixelFormat::AGGType> RendererBase;

  /// The AGG primitives renderer
  typedef agg::renderer_primitives<RendererBase> RendererPrimitives;

  /// The AGG solid renderer
  typedef agg::renderer_scanline_aa_solid<RendererBase> RendererSolid;

  PixelFormat::AGGType pixf(rBuf);
  RendererBase rbase(pixf);
  RendererPrimitives rprim(rbase);
  RendererSolid rsolid(rbase);

  // Draw a rectangle against a white background
  rbase.clear(PixelFormat::AGGType::color_type(0, 0, 0));
  rprim.fill_color(PixelFormat::AGGType::color_type(64, 64, 200));
  rprim.solid_rectangle(rBuf.width() / 4, rBuf.height() / 4,
      3 * rBuf.width() / 4, 3 * rBuf.height() / 4);

  // Text drawing with FreeType and HarfBuzz to AGG buffer
  agg_ft_hb_draw<PixelFormat::AGGType>(rbase);
}
}
