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

#include "hgkamva/hg_kamva_api.h"

//#include <cassert>
//#include <memory>

#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"

//#include "litehtml.h"

//#include "hgkamva/container/HgContainer.h"
#include "hgkamva/agg/renderer/HgAggHtmlRenderer.h"
//#include "hgkamva/renderer/HgHtmlRenderer.h"

using namespace hg;

// HgHtmlRenderer methods.

HgHtmlRendererPtr hgNewHtmlRendererRgb24()
{
  return new HgAggHtmlRenderer<agg::pixfmt_rgb24>();
}

HgHtmlRendererPtr hgNewHtmlRendererBgr24()
{
  return new HgAggHtmlRenderer<agg::pixfmt_bgr24>();
}

HgHtmlRendererPtr hgNewHtmlRendererRgba32()
{
  return new HgAggHtmlRenderer<agg::pixfmt_rgba32>();
}

HgHtmlRendererPtr hgNewHtmlRendererBgra32()
{
  return new HgAggHtmlRenderer<agg::pixfmt_bgra32>();
}

HgHtmlRendererPtr hgNewHtmlRendererArgb32()
{
  return new HgAggHtmlRenderer<agg::pixfmt_argb32>();
}

HgHtmlRendererPtr hgNewHtmlRendererAbgr32()
{
  return new HgAggHtmlRenderer<agg::pixfmt_abgr32>();
}
