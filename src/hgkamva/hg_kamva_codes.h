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

#ifndef HG_KAMVA_CODES_H
#define HG_KAMVA_CODES_H

typedef enum _hgColorFormat
{
  HG_FORMAT_INVALID = -1,
  HG_FORMAT_ARGB32 = 0,
  HG_FORMAT_RGB24 = 1,
  HG_FORMAT_A8 = 2,
  HG_FORMAT_A1 = 3,
  HG_FORMAT_RGB16_565 = 4,
  HG_FORMAT_RGB30 = 5,
  HG_FORMAT_RGB96F = 6,
  HG_FORMAT_RGBA128F = 7
} hgColorFormat;  // == cairo_format_t

enum hgLitehtmlMediaType
{
  media_type_none,
  media_type_all,
  media_type_screen,
  media_type_print,
  media_type_braille,
  media_type_embossed,
  media_type_handheld,
  media_type_projection,
  media_type_speech,
  media_type_tty,
  media_type_tv,
};

#endif /* HG_KAMVA_CODES_H */
