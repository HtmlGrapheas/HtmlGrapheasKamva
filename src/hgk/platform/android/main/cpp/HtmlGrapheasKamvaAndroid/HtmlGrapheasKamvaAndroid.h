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

#ifndef HTMLGRAPHEASKAMVAANDROID_H
#define HTMLGRAPHEASKAMVAANDROID_H

#include <stdint.h>

namespace fgr
{
class HtmlGrapheasKamvaAndroid
{
public:
  static bool drawIntoBitmap(void* p_pixels,
      uint32_t width,
      uint32_t height,
      uint32_t stride,
      uint32_t format);

private:
  template <class PixelFormat>
  static bool drawIntoBitmap_pixfmt(
      void* p_pixels, uint32_t width, uint32_t height, uint32_t stride);
};
}

#endif  // HTMLGRAPHEASKAMVAANDROID_H
