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

package ru.htmlgrapheas.htmlgrapheaskamva.hgkamva_api.codes;

public final class hgPixelFormatId
{
  public final static int UNKNOWN = 0;
  public final static int RGB565 = UNKNOWN + 1; // 16-bit RGB.
  public final static int RGB24  = RGB565 + 1; // 24-bit RGB.
  public final static int BGR24  = RGB24 + 1; // 24-bit BGR.
  public final static int RGBA32 = BGR24 + 1; // 32-bit RGBA.
  public final static int BGRA32 = RGBA32 + 1; // 32-bit BGRA.
  public final static int ARGB32 = BGRA32 + 1; // 32-bit ARGB.
  public final static int ABGR32 = ARGB32 + 1; // 32-bit ABGR.
}
