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

#include "hgkamva/container/HgHtmlRenderer.h"

#include <cassert>

#include "hgkamva/container/HgPaint.h"

namespace hg
{
HgHtmlRenderer::HgHtmlRenderer(HgPaint* painter)
    : mHgPainter(painter)
    , mBackgroundColor(255, 255, 255)
    , mHtmlDocument(nullptr)
{
  mHgContainer = std::make_shared<HgContainer>();
  mHtmlContext = std::make_shared<litehtml::context>();
}

void HgHtmlRenderer::createHtmlDocumentFromUtf8(const std::string& htmlText)
{
  mHtmlDocument = litehtml::document::createFromUTF8(
      htmlText.c_str(), mHgContainer.get(), mHtmlContext.get());
}

int HgHtmlRenderer::renderHtml(int width, int height)
{
  if(mHtmlDocument && mHtmlDocument->width() == width) {
    return 0;
  }

  mHgContainer->setDeviceWidth(width);
  mHgContainer->setDeviceHeight(height);
  mHgContainer->setDisplayAreaWidth(width);
  mHgContainer->setDisplayAreaHeight(height);

  // Render HTML document.
  int bestWidth = mHtmlDocument->render(width);
  assert(bestWidth);
  return bestWidth;
}

void HgHtmlRenderer::drawHtml(unsigned char* buffer,
    unsigned int width,
    unsigned int height,
    int stride,
    int scrollX,
    int scrollY)
{
  mHgPainter->attach(buffer, width, height, stride);
  mHgPainter->setRendererColor(mBackgroundColor);
  mHgPainter->clear();

  // Draw HTML document.
  litehtml::uint_ptr hdc = reinterpret_cast<litehtml::uint_ptr>(mHgPainter);
  litehtml::position clip(0, 0, width, height);
  mHtmlDocument->draw(hdc, -scrollX, -scrollY, &clip);
}

}  // namespace hg
