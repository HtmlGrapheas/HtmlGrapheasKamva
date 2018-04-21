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

package ru.feographia.htmlgrapheaskamva.hgkamva_api;

import android.graphics.Bitmap;


public class HgKamvaApiJni
{
  public native static long newHgHtmlRenderer(Bitmap bitmap);

  public native static void deleteHgHtmlRenderer(long renderer);

  public native static void hgHtmlRenderer_createHtmlDocumentFromUtf8(
      long renderer, String htmlText);

  public native static int hgHtmlRenderer_renderHtml(
      long renderer, int width, int height);

  public native static void hgHtmlRenderer_drawHtml(
      long renderer, Bitmap bitmap, int scrollX, int scrollY);

  public native static void hgHtmlRenderer_setBackgroundColor(
      long renderer, short red, short green, short blue, short alpha);

  public native static boolean hgContainer_parseAndLoadFontConfigFromMemory(
      long renderer, String fontConfig, boolean complain);

  public native static boolean hgContainer_addFontDir(
      long renderer, String dirPath);

  public native static void hgContainer_setDefaultFontName(
      long renderer, String name);

  public native static void hgContainer_setDefaultFontSize(
      long renderer, int size);

  public native static void hgContainer_setDeviceWidth(
      long renderer, int width);

  public native static void hgContainer_setDeviceHeight(
      long renderer, double height);

  public native static void hgContainer_setDeviceDpiX(
      long renderer, double dpi);

  public native static void hgContainer_setDeviceDpiY(
      long renderer, double dpi);

  public native static void hgContainer_setDisplayAreaWidth(
      long renderer, int width);

  public native static void hgContainer_setDisplayAreaHeight(
      long renderer, int height);

  public native static void hgContainer_setDeviceMonochromeBits(
      long renderer, int bits);

  public native static void hgContainer_setDeviceColorBits(
      long renderer, int bits);

  public native static void hgContainer_setDeviceColorIndex(
      long renderer, int colorIndex);

  public native static void hgContainer_setDeviceMediaType(
      long renderer, int type);

  public native static void hgHtmlContext_loadMasterStylesheet(
      long renderer, String str);

  public native static int hgHtmlDocument_width(long renderer);

  public native static int hgHtmlDocument_height(long renderer);


  static {
    try {
      System.loadLibrary("htmlgrapheaskamvaandroid");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. \n" + e);
      System.exit(1);
    }
  }
}