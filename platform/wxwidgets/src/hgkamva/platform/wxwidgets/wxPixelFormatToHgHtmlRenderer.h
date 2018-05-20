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

#ifndef WX_PIXEL_FORMAT_TO_HG_HTML_RENDERER_H
#define WX_PIXEL_FORMAT_TO_HG_HTML_RENDERER_H

#include <functional>

#include "hgkamva/hg_kamva_api.h"

namespace
{
/// Given a particular combination of channel type, bits per pixel and
/// channel indices, return the pixel format ID that matches.
/// The actual template specializations that follow give the actual types,
/// and using a combination of parameters that are not listed will give
/// a compile-time error.
template <typename Channel, int bitsPerPixel, int r, int g, int b, int a>
struct wxWidgetsToHgHtmlRendererHelper
{
  //empty
};

/// 24-bit RGB
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 24, 0, 1, 2, -1>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::RGB24;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 24, 0, 1, 2, -1>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererRgb24;

/// 24-bit BGR
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 24, 2, 1, 0, -1>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::BGR24;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 24, 2, 1, 0, -1>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererBgr24;

/// 32-bit RGB, alpha unused but stored as ARGB.
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 1, 2, 3, -1>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::ARGB32;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 1, 2, 3, -1>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererArgb32;

/// 32-bit RGB, alpha unused but stored as RGBA.
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 0, 1, 2, -1>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::RGBA32;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 0, 1, 2, -1>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererRgba32;

/// 32-bit BGR, alpha unused but stored as ABGR.
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 3, 2, 1, -1>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::ABGR32;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 3, 2, 1, -1>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererAbgr32;

/// 32-bit BGR, alpha unused but stored as BGRA.
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 2, 1, 0, -1>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::BGRA32;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 2, 1, 0, -1>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererBgra32;

/// 32-bit RGBA
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 0, 1, 2, 3>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::RGBA32;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 0, 1, 2, 3>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererRgba32;

/// 32-bit BGRA
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 2, 1, 0, 3>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::BGRA32;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 2, 1, 0, 3>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererBgra32;

/// 32-bit ARGB
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 1, 2, 3, 0>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::ARGB32;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 1, 2, 3, 0>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererArgb32;

/// 32-bit ABGR
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 3, 2, 1, 0>
{
  static const enum hgPixelFormatId id = hgPixelFormatId::ABGR32;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};
const std::function<HgHtmlRendererPtr()>
    wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 3, 2, 1, 0>::
        hgNewHtmlRendererFunc = hgNewHtmlRendererAbgr32;

}  // namespace

namespace hg
{
/// Convert between a wxWidgets pixel format class and a pixel format ID.
/// Usage examples:
/// hgPixelFormatId pixFmtId = wxPixelFormatToId<wxNativePixelFormat>::id or
/// hgPixelFormatId pixFmtId = wxPixelFormatToId<wxAlphaPixelFormat>::id.
template <typename wxWidgetsPixelFormat>
class wxPixelFormatToHgHtmlRenderer
{
public:
  using wxWidgetsType = wxWidgetsPixelFormat;

  // Break out the wxWidgets parameters and feed to the helper class.
  using HelperType = wxWidgetsToHgHtmlRendererHelper<
      typename wxWidgetsPixelFormat::ChannelType,
      wxWidgetsPixelFormat::BitsPerPixel,
      wxWidgetsPixelFormat::RED,
      wxWidgetsPixelFormat::GREEN,
      wxWidgetsPixelFormat::BLUE,
      wxWidgetsPixelFormat::ALPHA>;

  static const enum hgPixelFormatId id = HelperType::id;
  static const std::function<HgHtmlRendererPtr()> hgNewHtmlRendererFunc;
};  // class wxPixelFormatToId

template <typename wxWidgetsPixelFormat>
const std::function<HgHtmlRendererPtr()>
    wxPixelFormatToHgHtmlRenderer<wxWidgetsPixelFormat>::hgNewHtmlRendererFunc =
        wxPixelFormatToHgHtmlRenderer<wxWidgetsPixelFormat>::HelperType::
            hgNewHtmlRendererFunc;

}  // namespace hg

#endif  // WX_PIXEL_FORMAT_TO_HG_HTML_RENDERER_H
