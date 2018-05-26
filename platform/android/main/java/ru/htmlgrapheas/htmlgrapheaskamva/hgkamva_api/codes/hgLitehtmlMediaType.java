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

public final class hgLitehtmlMediaType
{
  public final static int media_type_none       = 0;
  public final static int media_type_all        = media_type_none + 1;
  public final static int media_type_screen     = media_type_all + 1;
  public final static int media_type_print      = media_type_screen + 1;
  public final static int media_type_braille    = media_type_print + 1;
  public final static int media_type_embossed   = media_type_braille + 1;
  public final static int media_type_handheld   = media_type_embossed + 1;
  public final static int media_type_projection = media_type_handheld + 1;
  public final static int media_type_speech     = media_type_projection + 1;
  public final static int media_type_tty        = media_type_speech + 1;
  public final static int media_type_tv         = media_type_tty + 1;
}
