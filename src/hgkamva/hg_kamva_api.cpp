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

#include <cassert>
#include <memory>

#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"

#include "litehtml.h"

#include "hgkamva/container/HgContainer.h"
#include "hgkamva/renderer/HgAggHtmlRenderer.h"
#include "hgkamva/renderer/HgHtmlRenderer.h"

using namespace hg;

// Internal functions.

inline HgHtmlRenderer* getHgHtmlRenderer(HgHtmlRendererPtr renderer)
{
  HgHtmlRenderer* hgRenderer = static_cast<HgHtmlRenderer*>(renderer);
  assert(hgRenderer);
  return hgRenderer;
}

inline std::shared_ptr<HgContainer> getHgContainer(HgHtmlRendererPtr renderer)
{
  return getHgHtmlRenderer(renderer)->getHgContainer();
}

inline std::shared_ptr<litehtml::context> getHtmlContext(
    HgHtmlRendererPtr renderer)
{
  return getHgHtmlRenderer(renderer)->getHtmlContext();
}

inline litehtml::document::ptr getHtmlDocument(HgHtmlRendererPtr renderer)
{
  return getHgHtmlRenderer(renderer)->getHtmlDocument();
}


// API functions.

int hgPixelFormatIdToColorBits(enum hgPixelFormatId pixFmtId)
{
  switch(pixFmtId) {
    case hgPixelFormatId::RGB24:
    case hgPixelFormatId::BGR24:
    case hgPixelFormatId::RGBA32:
    case hgPixelFormatId::BGRA32:
    case hgPixelFormatId::ARGB32:
    case hgPixelFormatId::ABGR32:
      return 8;
  }
  return 0;
}

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

void hgDeleteHtmlRenderer(HgHtmlRendererPtr renderer)
{
  delete getHgHtmlRenderer(renderer);
}

void hgHtmlRenderer_createHtmlDocumentFromUtf8(
    HgHtmlRendererPtr renderer, const char* htmlText)
{
  return getHgHtmlRenderer(renderer)->createHtmlDocumentFromUtf8(htmlText);
}

int hgHtmlRenderer_renderHtml(HgHtmlRendererPtr renderer, int width, int height)
{
  return getHgHtmlRenderer(renderer)->renderHtml(width, height);
}

void hgHtmlRenderer_drawHtml(HgHtmlRendererPtr renderer,
    unsigned char* buffer,
    int width,
    int height,
    int stride,
    int htmlX,
    int htmlY)
{
  return getHgHtmlRenderer(renderer)->drawHtml(
      buffer, width, height, stride, htmlX, htmlY);
}

void hgHtmlRenderer_setBackgroundColor(HgHtmlRendererPtr renderer,
    HgByte red,
    HgByte green,
    HgByte blue,
    HgByte alpha)
{
  return getHgHtmlRenderer(renderer)->setBackgroundColor(
      litehtml::web_color(red, green, blue, alpha));
}

// HgContainer methods.

HgBool hgContainer_parseAndLoadFontConfigFromMemory(
    HgHtmlRendererPtr renderer, const char* fontConfig, HgBool complain)
{
  return getHgContainer(renderer)->parseAndLoadFontConfigFromMemory(
      fontConfig, complain);
}

HgBool hgContainer_addFontDir(HgHtmlRendererPtr renderer, const char* dirPath)
{
  return getHgContainer(renderer)->addFontDir(dirPath);
}

void hgContainer_setFontTextCacheSize(HgHtmlRendererPtr renderer, int size)
{
  return getHgContainer(renderer)->setFontTextCacheSize(size);
}

void hgContainer_setDefaultFontName(
    HgHtmlRendererPtr renderer, const char* name)
{
  return getHgContainer(renderer)->setDefaultFontName(name);
}

void hgContainer_setDefaultFontSize(HgHtmlRendererPtr renderer, int size)
{
  return getHgContainer(renderer)->setDefaultFontSize(size);
}

void hgContainer_setDeviceWidth(HgHtmlRendererPtr renderer, int width)
{
  return getHgContainer(renderer)->setDeviceWidth(width);
}

void hgContainer_setDeviceHeight(HgHtmlRendererPtr renderer, double height)
{
  return getHgContainer(renderer)->setDeviceHeight(height);
}

void hgContainer_setDeviceDpiX(HgHtmlRendererPtr renderer, double dpi)
{
  return getHgContainer(renderer)->setDeviceDpiX(dpi);
}

void hgContainer_setDeviceDpiY(HgHtmlRendererPtr renderer, double dpi)
{
  return getHgContainer(renderer)->setDeviceDpiY(dpi);
}

void hgContainer_setDisplayAreaWidth(HgHtmlRendererPtr renderer, int width)
{
  return getHgContainer(renderer)->setDisplayAreaWidth(width);
}

void hgContainer_setDisplayAreaHeight(HgHtmlRendererPtr renderer, int height)
{
  return getHgContainer(renderer)->setDisplayAreaHeight(height);
}

void hgContainer_setDeviceMonochromeBits(HgHtmlRendererPtr renderer, int bits)
{
  return getHgContainer(renderer)->setDeviceMonochromeBits(bits);
}

void hgContainer_setDeviceColorBits(HgHtmlRendererPtr renderer, int bits)
{
  return getHgContainer(renderer)->setDeviceColorBits(bits);
}

void hgContainer_setDeviceColorIndex(HgHtmlRendererPtr renderer, int colorIndex)
{
  return getHgContainer(renderer)->setDeviceColorIndex(colorIndex);
}

void hgContainer_setDeviceMediaType(
    HgHtmlRendererPtr renderer, hgLitehtmlMediaType type)
{
  return getHgContainer(renderer)->setDeviceMediaType(
      static_cast<litehtml::media_type>(type));
}

int hgContainer_ptToPx(HgHtmlRendererPtr renderer, int pt)
{
  return getHgContainer(renderer)->pt_to_px(pt);
}

// litehtml::context methods.

void hgHtmlContext_loadMasterStylesheet(
    HgHtmlRendererPtr renderer, const char* str)
{
  return getHtmlContext(renderer)->load_master_stylesheet(str);
}


// litehtml::document methods.

int hgHtmlDocument_width(HgHtmlRendererPtr renderer)
{
  return getHtmlDocument(renderer)->width();
}

int hgHtmlDocument_height(HgHtmlRendererPtr renderer)
{
  return getHtmlDocument(renderer)->height();
}
