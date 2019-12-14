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

#include "hgkamva/renderer/HgHtmlRenderer.h"

#include <cassert>
#include <cstdlib>

//#include <chrono>
//#include <iostream>

namespace hg
{
HgHtmlRenderer::HgHtmlRenderer()
    : mBackgroundColor(255, 255, 255)
    , mHgContainer{std::make_shared<HgContainer>()}
    , mHtmlContext{std::make_shared<litehtml::context>()}
    , mHtmlDocument(nullptr)
    , mBuffer(nullptr)
    , mBufferWidth(0)
    , mBufferHeight(0)
    , mBufferStride(0)
    , mHtmlX(0)
    , mHtmlY(0)
{
}

void HgHtmlRenderer::createHtmlDocumentFromUtf8(const std::string& htmlText)
{
  mHtmlDocument = litehtml::document::createFromUTF8(
      htmlText.c_str(), mHgContainer.get(), mHtmlContext.get());
}

int HgHtmlRenderer::renderHtml(int width, int height)
{
  mHgContainer->setDeviceWidth(width);
  mHgContainer->setDeviceHeight(height);
  mHgContainer->setDisplayAreaWidth(width);
  mHgContainer->setDisplayAreaHeight(height);

  // Render HTML document.
  int bestWidth = mHtmlDocument->render(width);
  assert(bestWidth != 0);
  return bestWidth;
}

void HgHtmlRenderer::drawHtml(unsigned char* buffer,
    const cairo_format_t colorFormat,
    const int width,
    const int height,
    const int stride,
    const int htmlX,
    const int htmlY)
{
  // https://stackoverflow.com/a/18685338
  //auto start = std::chrono::steady_clock::now();

  if(buffer != mBuffer) {
    mCairo =
        std::make_shared<HgCairo>(buffer, colorFormat, width, height, stride);
  }
  litehtml::uint_ptr hdcCairo = reinterpret_cast<litehtml::uint_ptr>(&mCairo);

  bool fullDraw = buffer != mBuffer || width != mBufferWidth
      || height != mBufferHeight || stride != mBufferStride
      || abs(mHtmlX - htmlX) >= width || abs(mHtmlY - htmlY) >= height;

  if(fullDraw) {
    mCairo->save();
    mCairo->clear(HgCairo::Color{mBackgroundColor});
    litehtml::position testClip(0, 0, width, height);
    mHtmlDocument->draw(hdcCairo, -htmlX, -htmlY, &testClip);
    mCairo->restore();

  } else {
    int diffX = htmlX - mHtmlX;
    int diffY = htmlY - mHtmlY;

    mCairo->rasterCopy(diffX, diffY);

    int x1 = 0;
    int y1 = 0;
    int x2 = width - 1;
    int y2 = height - 1;

    if(diffX != 0) {
      if(diffX > 0) {
        x1 = x2 - diffX + 1;
      } else {
        x2 = -diffX - 1;  // x2 = x1 - diffX - 1;
      }
      if(x1 != x2) {
        int clipWidth = x2 - x1 + 1;
        int clipHeight = y2 - y1 + 1;
        mCairo->save();
        mCairo->clip(x1, y1, clipWidth, clipHeight);
        mCairo->clear(HgCairo::Color{mBackgroundColor});
        litehtml::position textClip(x1, y1, clipWidth, clipHeight);
        mHtmlDocument->draw(hdcCairo, -htmlX, -htmlY, &textClip);
        mCairo->restore();
      }
    }

    if(diffY != 0) {
      if(width - 1 == x2 && 0 != x1) {
        x2 = x1 - 1;
        x1 = 0;
      } else if(0 == x1 && width - 1 != x2) {
        x1 = x2 + 1;
        x2 = width - 1;
      }

      if(diffY > 0) {
        y1 = y2 - diffY + 1;
      } else {
        y2 = -diffY - 1;  // y2 = y1 - diffY - 1;
      }
      if(y1 != y2) {
        int clipWidth = x2 - x1 + 1;
        int clipHeight = y2 - y1 + 1;
        mCairo->save();
        mCairo->clip(x1, y1, clipWidth, clipHeight);
        mCairo->clear(HgCairo::Color{mBackgroundColor});
        litehtml::position textClip(x1, y1, clipWidth, clipHeight);
        mHtmlDocument->draw(hdcCairo, -htmlX, -htmlY, &textClip);
        mCairo->restore();
      }
    }
  }

  if(buffer != mBuffer) {
    mBuffer = buffer;
  }
  if(width != mBufferWidth) {
    mBufferWidth = width;
  }
  if(height != mBufferHeight) {
    mBufferHeight = height;
  }
  if(stride != mBufferStride) {
    mBufferStride = stride;
  }
  if(htmlX != mHtmlX) {
    mHtmlX = htmlX;
  }
  if(htmlY != mHtmlY) {
    mHtmlY = htmlY;
  }

  //auto finish = std::chrono::steady_clock::now();
  //auto elapsed =
  //    std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
  //std::cout << "HgHtmlRenderer::drawHtml: " << elapsed.count() << "\n";
}

}  // namespace hg
