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

#ifndef HG_AGG_PAINTER_H
#define HG_AGG_PAINTER_H

#include <memory>
#include <vector>

#include "agg_basics.h"
#include "agg_renderer_base.h"

#include "hgkamva/container/HgPainter.h"

namespace hg
{
template <typename PixelFormat>
class HgAggPainter : public HgPainter
{
private:
  using Buffer = std::vector<unsigned char>;
  using BufferPtr = std::shared_ptr<Buffer>;
  using RenderingBuffer = agg::rendering_buffer;
  using RendererBase = agg::renderer_base<PixelFormat>;
  using RendererColor = typename RendererBase::color_type;

public:
  explicit HgAggPainter();
  virtual ~HgAggPainter() = default;

private:
  void attachNewBuffer(unsigned int width, unsigned int height, int stride);
  const RendererBase& getRendererBase() const;

public:
  // HgRenderer interface.
  virtual HgPainterPtr newHgCachePainter(
      unsigned int width, unsigned int height) override;
  virtual void attach(unsigned char* buffer,
      unsigned int width,
      unsigned int height,
      int stride) override;
  virtual void setRendererColor(const litehtml::web_color& color) override;
  virtual void clear() override;
  virtual void blendHLine(int x1, int y, int x2, unsigned char cover) override;
  virtual void copyHLine(int x1, int y, int x2) override;
  virtual void blendFromColor(const HgPainter* src,
      const litehtml::web_color& color,
      int dx = 0,
      int dy = 0) override;
  virtual void copyFrom(const HgPainter* src, int dx = 0, int dy = 0) override;

private:
  BufferPtr mBuffer;
  RenderingBuffer mRenderingBuffer;
  PixelFormat mPixelFormat;
  RendererBase mRendererBase;
  RendererColor mRendererColor;
};  // class HgAggRenderer

template <typename PixelFormat>
HgAggPainter<PixelFormat>::HgAggPainter()
    : HgPainter()
    , mPixelFormat(mRenderingBuffer)
{
}

template <typename PixelFormat>
inline void HgAggPainter<PixelFormat>::attachNewBuffer(
    unsigned int width, unsigned int height, int stride)
{
  // stride == width * bytesPerPixel
  mBuffer = std::make_shared<Buffer>(stride * height);
  attach(mBuffer->data(), width, height, stride);
}

template <typename PixelFormat>
inline const typename HgAggPainter<PixelFormat>::RendererBase&
HgAggPainter<PixelFormat>::getRendererBase() const
{
  return mRendererBase;
}

template <typename PixelFormat>
inline HgPainterPtr HgAggPainter<PixelFormat>::newHgCachePainter(
    unsigned int width, unsigned int height)
{
  int bytesPerPixel = mRenderingBuffer.stride() / mRenderingBuffer.width();
  int stride = width * bytesPerPixel;
  HgAggPainter* aggPainter = new HgAggPainter();
  aggPainter->attachNewBuffer(width, height, stride);
  return std::shared_ptr<HgPainter>(aggPainter);
}

template <typename PixelFormat>
inline void HgAggPainter<PixelFormat>::attach(
    unsigned char* buffer, unsigned int width, unsigned int height, int stride)
{
  mRenderingBuffer.attach(buffer, width, height, stride);
  mRendererBase.attach(mPixelFormat);
}

template <typename PixelFormat>
inline void HgAggPainter<PixelFormat>::setRendererColor(
    const litehtml::web_color& color)
{
  mRendererColor.clear();
  mRendererColor.opacity(color.alpha / 255.0);
  mRendererColor.r = color.red;
  mRendererColor.g = color.green;
  mRendererColor.b = color.blue;
}

template <typename PixelFormat>
inline void HgAggPainter<PixelFormat>::clear()
{
  mRendererBase.clear(mRendererColor);
}

template <typename PixelFormat>
inline void HgAggPainter<PixelFormat>::blendHLine(
    int x1, int y, int x2, unsigned char cover)
{
  mRendererBase.blend_hline(x1, y, x2, mRendererColor, cover);
}

template <typename PixelFormat>
inline void HgAggPainter<PixelFormat>::copyHLine(int x1, int y, int x2)
{
  mRendererBase.copy_hline(x1, y, x2, mRendererColor);
}

template <typename PixelFormat>
inline void HgAggPainter<PixelFormat>::blendFromColor(
    const HgPainter* src, const litehtml::web_color& color, int dx, int dy)
{
  const HgAggPainter* aggSrc = static_cast<const HgAggPainter*>(src);

  RendererColor aggColor;
  aggColor.clear();
  aggColor.opacity(color.alpha / 255.0);
  aggColor.r = color.red;
  aggColor.g = color.green;
  aggColor.b = color.blue;

  mRendererBase.blend_from_color(aggSrc->getRendererBase().ren(), aggColor,
      nullptr, dx, dy, agg::cover_mask);
}

template <typename PixelFormat>
inline void HgAggPainter<PixelFormat>::copyFrom(
    const HgPainter* src, int dx, int dy)
{
  const HgAggPainter* aggSrc = static_cast<const HgAggPainter*>(src);
  mRendererBase.copy_from(aggSrc->getRendererBase().ren(), nullptr, dx, dy);
}

}  // namespace hg

#endif  // HG_AGG_PAINTER_H
