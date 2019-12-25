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
//template <>
//struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 24, 0, 1, 2, -1>
//{
//};


/// 24-bit BGR
//template <>
//struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 24, 2, 1, 0, -1>
//{
//};

/// 32-bit RGB, alpha unused but stored as ARGB.
//template <>
//struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 1, 2, 3, -1>
//{
//};

/// 32-bit RGB, alpha unused but stored as RGBA.
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 0, 1, 2, -1>
{
  static const hgColorFormat colorFormat = HG_FORMAT_RGB24;
};

/// 32-bit BGR, alpha unused but stored as ABGR.
//template <>
//struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 3, 2, 1, -1>
//{
//};

/// 32-bit BGR, alpha unused but stored as BGRA.
//template <>
//struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 2, 1, 0, -1>
//{
//};

/// 32-bit RGBA
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 0, 1, 2, 3>
{
  static const hgColorFormat colorFormat = HG_FORMAT_ARGB32;
};

/// 32-bit BGRA
//template <>
//struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 2, 1, 0, 3>
//{
//};

/// 32-bit ARGB
template <>
struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 1, 2, 3, 0>
{
  // TODO: use cairo_quartz_surface_create() or similar for macOS
  static const hgColorFormat colorFormat = HG_FORMAT_ARGB32;
};

/// 32-bit ABGR
//template <>
//struct wxWidgetsToHgHtmlRendererHelper<unsigned char, 32, 3, 2, 1, 0>
//{
//};

}  // namespace

namespace hg
{
/// Convert between a wxWidgets pixel format class and a Cairo's color format.
/// Usage examples:
/// hgColorFormat colorFormat = wxPixelFormatToId<wxNativePixelFormat>::colorFormat
/// or
/// hgColorFormat colorFormat = wxPixelFormatToId<wxAlphaPixelFormat>::colorFormat.
template <typename wxWidgetsPixelFormat>
class wxPixelFormatToHgHtmlRenderer
// TODO: rename to wxPixelFormatToCairoColorFormat
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

  static const hgColorFormat colorFormat = HelperType::colorFormat;
  static const int bitsPerPixel = wxWidgetsPixelFormat::BitsPerPixel;
};  // class wxPixelFormatToHgHtmlRenderer

}  // namespace hg

#endif  // WX_PIXEL_FORMAT_TO_HG_HTML_RENDERER_H
