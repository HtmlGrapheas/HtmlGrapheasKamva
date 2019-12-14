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

#ifndef HG_HTML_RENDERER_H
#define HG_HTML_RENDERER_H

#include <memory>
#include <string>

#include "litehtml.h"

#include "hgkamva/container/HgCairo.h"
#include "hgkamva/container/HgContainer.h"

namespace hg
{
class HgHtmlRenderer
{
public:
  // TODO: Copy/move constructors/operators.
  explicit HgHtmlRenderer();
  virtual ~HgHtmlRenderer() = default;

  void createHtmlDocumentFromUtf8(const std::string& htmlText);
  int renderHtml(int width, int height);
  virtual void drawHtml(unsigned char* buffer,
      const cairo_format_t colorFormat,
      const int width,
      const int height,
      const int stride,
      const int htmlX,
      const int htmlY);

  void setBackgroundColor(const litehtml::web_color& color);

  HgContainerPtr getHgContainer();
  std::shared_ptr<litehtml::context> getHtmlContext();
  litehtml::document::ptr getHtmlDocument();

private:
  litehtml::web_color mBackgroundColor;

  HgContainerPtr mHgContainer;
  std::shared_ptr<litehtml::context> mHtmlContext;
  litehtml::document::ptr mHtmlDocument;

  HgCairoPtr mCairo;

  unsigned char* mBuffer;
  int mBufferWidth;
  int mBufferHeight;
  int mBufferStride;
  int mHtmlX;
  int mHtmlY;
};

inline void HgHtmlRenderer::setBackgroundColor(const litehtml::web_color& color)
{
  mBackgroundColor = color;
}

inline HgContainerPtr HgHtmlRenderer::getHgContainer()
{
  return mHgContainer;
}
inline std::shared_ptr<litehtml::context> HgHtmlRenderer::getHtmlContext()
{
  return mHtmlContext;
}

inline litehtml::document::ptr HgHtmlRenderer::getHtmlDocument()
{
  return mHtmlDocument;
}

}  // namespace hg

#endif  // HG_HTML_RENDERER_H
