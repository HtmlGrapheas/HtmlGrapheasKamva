/*****************************************************************************
 * Project:  HtmlGrapheas
 * Purpose:  HTML text editor library
 * Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *   Copyright (c) 2017 NikitaFeodonit
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

#ifndef WX_AGG_HGA_WINDOW_H
#define WX_AGG_HGA_WINDOW_H

#include "AGGWindow.h"

namespace GUI
{
// TODO: use code from AGGWindow to exclude agg headers from export.

/// An example of an application-specific subclass of AGGWindow,
/// in this case, a window that draws the lion example from AGG.
class HtmlGrapheasKamvaWx : public AGGWindow
{
public:
  /// Initialize a lion window.
  /// Defaults are taken from AGGWindow::AGGWindow(), see that
  /// documentation for more information.
  HtmlGrapheasKamvaWx(wxWindow* parent,
      wxWindowID id = wxID_ANY,
      const wxPoint& pos = wxDefaultPosition,
      const wxSize& size = wxDefaultSize,
      long style = wxTAB_TRAVERSAL);

  /// Clean up any resources held.
  virtual ~HtmlGrapheasKamvaWx();

  /// Draw the bitmap with the lion.
  virtual void draw();

protected:
};
}

#endif
