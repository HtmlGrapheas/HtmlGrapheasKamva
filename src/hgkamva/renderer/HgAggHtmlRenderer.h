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

#ifndef HG_AGG_HTML_RENDERER_H
#define HG_AGG_HTML_RENDERER_H

#include "hgkamva/container/HgAggPainter.h"
#include "hgkamva/renderer/HgHtmlRenderer.h"

namespace hg
{
template <typename PixelFormat>
class HgAggHtmlRenderer : public HgHtmlRenderer
{
public:
  explicit HgAggHtmlRenderer();
  virtual ~HgAggHtmlRenderer() = default;

private:
  HgAggPainter<PixelFormat> mHgAggPainter;
};

template <typename PixelFormat>
HgAggHtmlRenderer<PixelFormat>::HgAggHtmlRenderer()
    : HgHtmlRenderer(&mHgAggPainter)
{
}

}  // namespace hg

#endif  // HG_AGG_HTML_RENDERER_H
