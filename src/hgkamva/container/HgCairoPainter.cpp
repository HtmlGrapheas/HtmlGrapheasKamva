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

#include "hgkamva/container/HgCairoPainter.h"

// TODO: remove unneeded
#include <cstdlib>
#include <memory>
#include <vector>

#include "hgkamva/container/HgPainter.h"

namespace hg
{

HgCairoPainter::HgCairoPainter()
    : HgPainter()
//    , mPixelFormat(mRenderingBuffer)
{
}

void HgCairoPainter::attachNewBuffer(
    unsigned int width, unsigned int height, int stride)
{
  // stride == width * bytesPerPixel
  // stride may be negative.
  mBuffer = std::make_shared<Buffer>(abs(stride) * height);
  attach(mBuffer->data(), width, height, stride);
}

//const typename HgCairoPainter::RendererBase&
//HgCairoPainter::getRendererBase() const
//{
//  return mRendererBase;
//}

HgPainterPtr HgCairoPainter::newHgCachePainter(
    unsigned int width, unsigned int height)
{
  int parentStride = cairo_image_surface_get_stride(mCairoWrapper->getSurface());
  int parentWidth = cairo_image_surface_get_width(mCairoWrapper->getSurface());
  int bytesPerPixel = parentStride / parentWidth;
  int stride = width * bytesPerPixel;

  HgCairoPainter* cairoPainter = new HgCairoPainter();
  cairoPainter->attachNewBuffer(width, height, stride);
  return std::shared_ptr<HgPainter>(cairoPainter);
}

void HgCairoPainter::attach(
    unsigned char* buffer, unsigned int width, unsigned int height, int stride)
{
  mCairoWrapper = std::make_shared<CairoWrapper>(buffer, width, height, stride);
}

void HgCairoPainter::setPaintColor(
    const litehtml::web_color& color)
{
  cairo_set_source_rgba(mCairoWrapper->getContext(), color.red / 255.0,
      color.green / 255.0, color.blue / 255.0, color.alpha / 255.0);
}

bool HgCairoPainter::clipBox(int x1, int y1, int x2, int y2)
{
  return mRendererBase.clip_box(x1, y1, x2, y2);
}

void HgCairoPainter::resetClipping(bool visibility)
{
  mRendererBase.reset_clipping(visibility);
}

void HgCairoPainter::clear()
{
  mRendererBase.clear(mPaintColor);
}

void HgCairoPainter::blendHLine(
    int x1, int y, int x2, unsigned char cover)
{
  mRendererBase.blend_hline(x1, y, x2, mPaintColor, cover);
}

void HgCairoPainter::copyHLine(int x1, int y, int x2)
{
  mRendererBase.copy_hline(x1, y, x2, mPaintColor);
}

void HgCairoPainter::copyBar(int x1, int y1, int x2, int y2)
{
  mRendererBase.copy_bar(x1, y1, x2, y2, mPaintColor);
}

void HgCairoPainter::blendFromColor(
    const HgPainter* src, const litehtml::position* srcRect, int dx, int dy)
{
  const HgCairoPainter* cairoSrc = static_cast<const HgCairoPainter*>(src);

  if(srcRect) {
    cairo::rect_i cairoRect(
        srcRect->left(), srcRect->top(), srcRect->right(), srcRect->bottom());
    mRendererBase.blend_from_color(cairoSrc->getRendererBase().ren(),
        mPaintColor, &cairoRect, dx, dy, cairo::cover_mask);
  } else {
    mRendererBase.blend_from_color(cairoSrc->getRendererBase().ren(),
        mPaintColor, nullptr, dx, dy, cairo::cover_mask);
  }
}

void HgCairoPainter::copyFrom(
    const HgPainter* src, const litehtml::position* srcRect, int dx, int dy)
{
  const HgCairoPainter* cairoSrc = static_cast<const HgCairoPainter*>(src);
  if(srcRect) {
    cairo::rect_i cairoRect(
        srcRect->left(), srcRect->top(), srcRect->right(), srcRect->bottom());
    mRendererBase.copy_from(
        cairoSrc->getRendererBase().ren(), &cairoRect, dx, dy);
  } else {
    mRendererBase.copy_from(cairoSrc->getRendererBase().ren(), nullptr, dx, dy);
  }
}

}  // namespace hg
