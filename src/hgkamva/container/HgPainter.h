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

#ifndef HG_PAINTER_H
#define HG_PAINTER_H

#include "litehtml.h"

namespace hg
{
class HgPainter
{
public:
  explicit HgPainter();
  virtual ~HgPainter() = default;

  virtual void attach(unsigned char* buffer,
      unsigned int width,
      unsigned int height,
      int stride) = 0;
  virtual void setRendererColor(const litehtml::web_color& color) = 0;
  virtual void clear() = 0;
  virtual void blendHLine(int x1, int y, int x2, unsigned char cover) = 0;
  virtual void copyHLine(int x1, int y, int x2) = 0;
};  // class HgRenderer

}  // namespace hg

#endif  // HG_PAINTER_H
