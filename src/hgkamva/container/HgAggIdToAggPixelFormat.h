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

#ifndef HG_AGG_ID_TO_AGG_PIXEL_FORMAT_H
#define HG_AGG_ID_TO_AGG_PIXEL_FORMAT_H

#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_rgb_packed.h"

#include "hgkamva/hg_kamva_codes.h"

namespace
{
/// Given a pixel format ID, return the AGG format that matches.
/// The actual template specializations that follow give the actual types,
/// and using a combination of parameters that are not listed will give
/// a compile-time error.
template <enum hgPixelFormatId id>
struct wxWidgetsToAGGHelper
{
  //empty
};

/// 16-bit RGB
template <>
struct wxWidgetsToAGGHelper<hgPixelFormatId::RGB565>
{
  using format = agg::pixfmt_rgb565;
};

/// 24-bit RGB
template <>
struct wxWidgetsToAGGHelper<hgPixelFormatId::RGB24>
{
  using format = agg::pixfmt_rgb24;
};

/// 24-bit BGR
template <>
struct wxWidgetsToAGGHelper<hgPixelFormatId::BGR24>
{
  using format = agg::pixfmt_bgr24;
};

/// 32-bit RGBA
template <>
struct wxWidgetsToAGGHelper<hgPixelFormatId::RGBA32>
{
  using format = agg::pixfmt_rgba32;
};

/// 32-bit BGRA
template <>
struct wxWidgetsToAGGHelper<hgPixelFormatId::BGRA32>
{
  using format = agg::pixfmt_bgra32;
};

/// 32-bit ARGB
template <>
struct wxWidgetsToAGGHelper<hgPixelFormatId::ARGB32>
{
  using format = agg::pixfmt_argb32;
};

/// 32-bit ABGR
template <>
struct wxWidgetsToAGGHelper<hgPixelFormatId::ABGR32>
{
  using format = agg::pixfmt_abgr32;
};

}  // namespace

namespace hg
{
/// Convert between a pixel format ID and an AGG pixel format class.
/// Usage examples:
/// PixelFormatConvertor<wxNativePixelFormat>::AGGType or
/// PixelFormatConvertor<wxAlphaPixelFormat>::AGGType.
template <enum hgPixelFormatId id>
class HgAggIdToAggPixelFormat
{
public:
  // Break out the wxWidgets parameters and feed to the helper class.
  using AGGType = typename wxWidgetsToAGGHelper<id>::format;
};  // class PixelFormatConvertor

}  // namespace hg

#endif  // HG_AGG_ID_TO_AGG_PIXEL_FORMAT_H
