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

#ifndef HG_RENDERER_H
#define HG_RENDERER_H

#include <functional>

#include "litehtml.h"

namespace hg
{
class HgRenderer
{
public:
  using BlendHLineFunc = std::function<void(int x1,
      int y,
      int x2,
      const litehtml::web_color& color,
      unsigned char cover)>;

  using CopyHLineFunc = std::function<void(
      int x1, int y, int x2, const litehtml::web_color& color)>;

  explicit HgRenderer() {}
  virtual ~HgRenderer() {}

  void setBlendHLineFunc(BlendHLineFunc blendHLineFunc)
  {
    mBlendHLineFunc = blendHLineFunc;
  }

  void blendHLine(int x1,
      int y,
      int x2,
      const litehtml::web_color& color,
      unsigned char cover)
  {
    return mBlendHLineFunc(x1, y, x2, color, cover);
  }

  void setCopyHLineFunc(CopyHLineFunc copyHLineFunc)
  {
    mCopyHLineFunc = copyHLineFunc;
  }

  void copyHLine(int x1, int y, int x2, const litehtml::web_color& color)
  {
    return mCopyHLineFunc(x1, y, x2, color);
  }

private:
  BlendHLineFunc mBlendHLineFunc;
  CopyHLineFunc mCopyHLineFunc;
};  // class HgRenderer

}  // namespace hg

#endif  // HG_RENDERER_H
