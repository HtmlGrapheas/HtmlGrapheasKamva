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

#include "hgkamva/container/HgContainer.h"
#include "hgkamva/container/HgPainter.h"

namespace hg
{
class HgHtmlRenderer
{
public:
  explicit HgHtmlRenderer(HgPainter* painter);
  virtual ~HgHtmlRenderer() = default;

  void createHtmlDocumentFromUtf8(const std::string& htmlText);
  int renderHtml(int width, int height);
  virtual void drawHtml(unsigned char* buffer,
      int width,
      int height,
      int stride,
      int htmlX,
      int htmlY);

  void setBackgroundColor(const litehtml::web_color& color);

  std::shared_ptr<HgContainer> getHgContainer();
  std::shared_ptr<litehtml::context> getHtmlContext();
  litehtml::document::ptr getHtmlDocument();

private:
  HgPainter* mHgPainter;
  litehtml::web_color mBackgroundColor;

  std::shared_ptr<HgContainer> mHgContainer;
  std::shared_ptr<litehtml::context> mHtmlContext;
  litehtml::document::ptr mHtmlDocument;

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

inline std::shared_ptr<HgContainer> HgHtmlRenderer::getHgContainer()
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
