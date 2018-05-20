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

#ifndef HTMLGRAPHEASKAMVAWX_H
#define HTMLGRAPHEASKAMVAWX_H

#include <functional>
#include <memory>

#include <wx/bitmap.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/rawbmp.h>
#include <wx/scrolwin.h>
#include <wx/windowid.h>

#include "hgkamva/hg_kamva_api.h"
#include "hgkamva/platform/wxwidgets/wxPixelFormatToHgHtmlRenderer.h"

namespace hg
{
/// A simple widget that displays a bitmap that AGG can draw on.
/// It reallocates the bitmap so that it always is the current size of the
/// entire panel and calls the virtual method draw() to draw to the bitmap.
/// This should be useable anywhere a wxWindow can be, e.g. in actual windows,
/// buttons, etc.
class HgKamvaWxWindow : public wxScrolledCanvas
{
private:
  using PixelFormat = wxPixelFormatToHgHtmlRenderer<wxNativePixelFormat>;
  using PixelData = wxPixelData<wxBitmap, PixelFormat::wxWidgetsType>;

public:
  /// Create an HtmlGrapheasKamvaWx.
  /// Defaults are taken from wxWindow::wxWindow(), see that documentation
  /// for more information.
  explicit HgKamvaWxWindow(wxWindow* parent,
      wxWindowID id = wxID_ANY,
      const wxPoint& pos = wxDefaultPosition,
      const wxSize& size = wxDefaultSize,
      long style = wxTAB_TRAVERSAL);

  /// Clean up resources held.
  virtual ~HgKamvaWxWindow();

protected:
  void initHgContainer();

  /// Create the bitmap given the current size.
  void setBitmap(const int width, const int height);

  void drawOnBitmap();

  void drawHtml(unsigned char* buffer,
      const int width,
      const int height,
      const int stride);

  /// Resize the bitmap to match the window.
  void onSize(wxSizeEvent& event);

  /// Paint the bitmap onto the panel.
  void onPaint(wxPaintEvent& event);

  /// Handle the erase-background event.
  void onEraseBackground(wxEraseEvent& event);

private:
  wxMemoryDC mMemoryDC;  ///< Memory "device context" for drawing the bitmap
  std::shared_ptr<wxBitmap> mBitmap;  ///< wxWidgets bitmap for AGG to draw into

  const enum hgPixelFormatId mPixFmtId = PixelFormat::id;
  std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc =
      PixelFormat::hgNewHtmlRendererFunc;
  HgHtmlRendererPtr mHgHtmlRenderer;

  int mVisibleHtmlWidth;
  int mVisibleHtmlHeight;
  int mHtmlX;
  int mHtmlY;
  int mNewHtmlX;
  int mNewHtmlY;

  DECLARE_EVENT_TABLE()  /// Allocate wxWidgets storage for event handlers
};  // class HtmlGrapheasKamvaWx

}  // namespace hg

#endif  // HTMLGRAPHEASKAMVAWX_H
