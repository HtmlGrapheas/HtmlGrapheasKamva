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

#include <hgkamva/container/HgPainter.h>
#include "agg_renderer_base.h"


namespace hg
{
template <typename PixelFormat>
class HgAggPainter : public HgPainter
{
private:
  // The AGG base renderer.
  using RendererBase = agg::renderer_base<PixelFormat>;

  using RendererColor = typename RendererBase::color_type;

public:
  explicit HgAggPainter();
  virtual ~HgAggPainter() = default;

  // HgRenderer interface.
  virtual void attach(unsigned char* buffer,
      unsigned int width,
      unsigned int height,
      int stride) override;
  virtual void setRendererColor(const litehtml::web_color& color) override;
  virtual void clear() override;
  virtual void blendHLine(int x1, int y, int x2, unsigned char cover) override;
  virtual void copyHLine(int x1, int y, int x2) override;

private:
  agg::rendering_buffer mRenderingBuffer;
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
inline void HgAggPainter<PixelFormat>::HgAggPainter::blendHLine(
    int x1, int y, int x2, unsigned char cover)
{
  mRendererBase.blend_hline(x1, y, x2, mRendererColor, cover);
}

template <typename PixelFormat>
inline void HgAggPainter<PixelFormat>::HgAggPainter::copyHLine(
    int x1, int y, int x2)
{
  mRendererBase.copy_hline(x1, y, x2, mRendererColor);
}

}  // namespace hg

#endif  // HG_AGG_PAINTER_H
