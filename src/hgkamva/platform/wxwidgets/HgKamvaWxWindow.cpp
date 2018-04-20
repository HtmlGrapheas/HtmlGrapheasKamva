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

// Based on the code from:
// http://mrl.nyu.edu/~ajsecord/downloads/wxAGG-1.1.tgz

/**
 * @author Adrian Secord
 * @link http://mrl.nyu.edu/~ajsecord
 * @link http://mrl.nyu.edu/~ajsecord/downloads/wxAGG-1.1.tgz
 */

#include "hgkamva/platform/wxwidgets/HgKamvaWxWindow.h"

#include <cassert>
#include <string>

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "litehtml.h"

#include "hgkamva/util/FileUtil.h"

namespace hg
{
BEGIN_EVENT_TABLE(HgKamvaWxWindow, wxWindow)
EVT_SIZE(HgKamvaWxWindow::onSize)
EVT_PAINT(HgKamvaWxWindow::onPaint)
EVT_ERASE_BACKGROUND(HgKamvaWxWindow::onEraseBackground)
END_EVENT_TABLE()

HgKamvaWxWindow::HgKamvaWxWindow(wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style)
    : wxScrolledCanvas(parent, id, pos, size, style, wxT("HgKamvaWxWindow"))
    , mBitmap(nullptr)
    , mScrollX(0)
    , mScrollY(0)
    , mNewScrollX(0)
    , mNewScrollY(0)
{
  mHgHtmlRenderer = newHgHtmlRenderer(mPixFmtId);
  initHgContainer();

  // This part makes the scrollbars show up.
  SetScrollRate(1, 1);
  // Ask the sizer about the needed size.
  FitInside();
}

HgKamvaWxWindow::~HgKamvaWxWindow()
{
  mMemoryDC.SelectObject(wxNullBitmap);
  deleteHgHtmlRenderer(mHgHtmlRenderer);
}

void HgKamvaWxWindow::initHgContainer()
{
  wxFileName exeFile(wxStandardPaths::Get().GetExecutablePath());

  wxFileName exeDir;
  exeDir.AssignDir(exeFile.GetPath());

  wxFileName dataDir(exeDir);
  dataDir.AppendDir("data");

  wxFileName fontDir(exeDir);
  fontDir.AppendDir("fonts");

  wxFileName masterCssFile(dataDir);
  masterCssFile.SetFullName("master.css");

  wxFileName htmlFile(dataDir);
  htmlFile.SetFullName("test.html");

  // Load font config.
  wxFileName fontConfFile(fontDir);
  fontConfFile.SetFullName("fonts.conf");

  std::string fontConfig =
      hg::FileUtil::readFile(fontConfFile.GetFullPath().ToStdString());
  assert(fontConfig.size());

  bool loadedFontConfig = hgContainer_parseAndLoadFontConfigFromMemory(
      mHgHtmlRenderer, fontConfig.c_str(), true);
  assert(loadedFontConfig);

  // Set fonts.
  bool addedFontDir =
      hgContainer_addFontDir(mHgHtmlRenderer, fontDir.GetPath().c_str());
  assert(addedFontDir);

  hgContainer_setDefaultFontName(mHgHtmlRenderer, "Tinos");
  hgContainer_setDefaultFontSize(mHgHtmlRenderer, 24);

  // Set device parameters.
  hgContainer_setDeviceDpiX(mHgHtmlRenderer, 96);
  hgContainer_setDeviceDpiY(mHgHtmlRenderer, 96);
  hgContainer_setDeviceMonochromeBits(mHgHtmlRenderer, 0);
  hgContainer_setDeviceColorBits(mHgHtmlRenderer, 8);
  hgContainer_setDeviceColorIndex(mHgHtmlRenderer, 256);
  hgContainer_setDeviceMediaType(
      mHgHtmlRenderer, hgLitehtmlMediaType::media_type_screen);

  std::string masterCss =
      FileUtil::readFile(masterCssFile.GetFullPath().ToStdString());
  assert(masterCss.size());

  hgHtmlContext_loadMasterStylesheet(mHgHtmlRenderer, masterCss.c_str());

  std::string htmlText =
      FileUtil::readFile(htmlFile.GetFullPath().ToStdString());
  assert(htmlText.size());

  hgHtmlRenderer_createHtmlDocumentFromUtf8(mHgHtmlRenderer, htmlText.c_str());
}

void HgKamvaWxWindow::renderHtml(const int width, const int height)
{
  // Render HTML document.
  hgHtmlRenderer_renderHtml(mHgHtmlRenderer, width, height);
  SetVirtualSize(hgHtmlDocument_width(mHgHtmlRenderer),
      hgHtmlDocument_height(mHgHtmlRenderer));
}

void HgKamvaWxWindow::drawHtml(const int width, const int height)
{
  if(mBitmap && mBitmap->GetWidth() == width && mBitmap->GetHeight() == height
      && mScrollX == mNewScrollX && mScrollY == mNewScrollY) {
    return;
  }

  mScrollX = mNewScrollX;
  mScrollY = mNewScrollY;

  mMemoryDC.SelectObject(wxNullBitmap);

  mBitmap = std::shared_ptr<wxBitmap>(
      new wxBitmap(width, height, PixelFormat::wxWidgetsType::BitsPerPixel));
  mMemoryDC.SelectObject(*mBitmap);

  // Attach the AGG rendering buffer to the bitmap
  // and call the user draw() code.

  // Draw the bitmap.
  // Get raw access to the wxWidgets bitmap -- this locks the pixels and
  // unlocks on destruction.
  PixelData pixels(*mBitmap);
  assert(pixels);

  // This cast looks like it is ignoring byte-ordering, but the
  // pixel format already explicitly handles that.
  assert(pixels.GetPixels().IsOk());

  wxAlphaPixelFormat::ChannelType* pData =
      reinterpret_cast<wxAlphaPixelFormat::ChannelType*>(
          &pixels.GetPixels().Data());

  // wxWidgets always returns a pointer to the first row of pixels, whether
  // that is stored at the beginning of the buffer (stride > 0) or at the
  // end of the buffer (stride < 0).  AGG always wants a pointer to the
  // beginning of the buffer, no matter what the stride.  (AGG does handle
  // negative strides correctly.)
  // Upshot: if the stride is negative, rewind the pointer from the end of
  // the buffer to the beginning.
  const int stride = pixels.GetRowStride();
  if(stride < 0) {
    pData += (pixels.GetHeight() - 1) * stride;
  }

  int frameWidth = pixels.GetWidth();
  int frameHeight = pixels.GetHeight();

  // Draw HTML document.
  hgHtmlRenderer_setBackgroundColor(mHgHtmlRenderer, 255, 255, 255);
  hgHtmlRenderer_drawHtml(mHgHtmlRenderer, pData, frameWidth, frameHeight,
      stride, mNewScrollX, mNewScrollY);

  // Request a full redraw of the window.
  Refresh(false);
}

void HgKamvaWxWindow::onSize(wxSizeEvent& event)
{
  const wxSize size = GetClientSize();
  renderHtml(size.GetWidth(), size.GetHeight());
}

void HgKamvaWxWindow::onPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  DoPrepareDC(dc);

  int width, height;
  dc.GetSize(&width, &height);

  // GetScrollPos() do not work on Windows with native scroll bars.
  GetViewStart(&mNewScrollX, &mNewScrollY);

  drawHtml(width, height);

  // Iterate over regions needing repainting.
  wxRegionIterator regions(GetUpdateRegion());
  wxRect rect;
  while(regions) {
    rect = regions.GetRect();
    int xd = rect.x + mNewScrollX;
    int yd = rect.y + mNewScrollY;
    int xs = rect.x;
    int ys = rect.y;
    dc.Blit(xd, yd, rect.width, rect.height, &mMemoryDC, xs, ys);
    ++regions;
  }
}

void HgKamvaWxWindow::onEraseBackground(wxEraseEvent& WXUNUSED(event))
{
  // Do nothing to "avoid flashing in MSW"  Grr.
}

}  // namespace hg
