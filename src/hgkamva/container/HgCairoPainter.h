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

#ifndef HG_CAIRO_PAINTER_H
#define HG_CAIRO_PAINTER_H

// TODO: remove unneeded
#include <cstdlib>
#include <memory>
#include <vector>

#include "hgkamva/container/CairoWrapper.h"
#include "hgkamva/container/HgPainter.h"

namespace hg
{
class HgCairoPainter : public HgPainter
{
private:
  using Buffer = std::vector<unsigned char>;
  using BufferPtr = std::shared_ptr<Buffer>;
  using CairoWrapperPtr = std::shared_ptr<CairoWrapper>;
//  using RenderingBuffer = cairo::rendering_buffer;
//  using RendererBase = cairo::renderer_base;
//  using RendererColor = typename RendererBase::color_type;

public:
  explicit HgCairoPainter();
  virtual ~HgCairoPainter() = default;

private:
  void attachNewBuffer(unsigned int width, unsigned int height, int stride);
//  const RendererBase& getRendererBase() const;

public:
  // HgPainter interface.
  virtual HgPainterPtr newHgCachePainter(
      unsigned int width, unsigned int height) override;
  virtual void attach(unsigned char* buffer,
      unsigned int width,
      unsigned int height,
      int stride) override;
  virtual void setPaintColor(const litehtml::web_color& color) override;
  virtual bool clipBox(int x1, int y1, int x2, int y2) override;
  virtual void resetClipping(bool visibility) override;
  virtual void clear() override;
  virtual void blendHLine(int x1, int y, int x2, unsigned char cover) override;
  virtual void copyHLine(int x1, int y, int x2) override;
  virtual void copyBar(int x1, int y1, int x2, int y2) override;
  virtual void blendFromColor(const HgPainter* src,
      const litehtml::position* srcRect,
      int dx = 0,
      int dy = 0) override;
  virtual void copyFrom(const HgPainter* src,
      const litehtml::position* srcRect,
      int dx = 0,
      int dy = 0) override;

private:
  BufferPtr mBuffer;
  CairoWrapperPtr mCairoWrapper;

//  RenderingBuffer mRenderingBuffer;
//  PixelFormat mPixelFormat;
//  RendererBase mRendererBase;
//  RendererColor mPaintColor;
};  // class HgCairoPainter

}  // namespace hg

#endif  // HG_CAIRO_PAINTER_H
