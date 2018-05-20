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

#ifndef HG_KAMVA_API_H
#define HG_KAMVA_API_H

#include "hgkamva/hg_kamva_common.h"

#include "hgkamva/hg_kamva_codes.h"

typedef void* HgHtmlRendererPtr;
typedef unsigned char HgBool;
typedef unsigned char HgByte;

HG_KAMVA_EXTERNC int hgPixelFormatIdToColorBits(enum hgPixelFormatId pixFmtId);

HG_KAMVA_EXTERNC HgHtmlRendererPtr hgNewHtmlRendererRgb24();
HG_KAMVA_EXTERNC HgHtmlRendererPtr hgNewHtmlRendererBgr24();
HG_KAMVA_EXTERNC HgHtmlRendererPtr hgNewHtmlRendererRgba32();
HG_KAMVA_EXTERNC HgHtmlRendererPtr hgNewHtmlRendererBgra32();
HG_KAMVA_EXTERNC HgHtmlRendererPtr hgNewHtmlRendererArgb32();
HG_KAMVA_EXTERNC HgHtmlRendererPtr hgNewHtmlRendererAbgr32();

HG_KAMVA_EXTERNC void hgDeleteHtmlRenderer(HgHtmlRendererPtr renderer);

HG_KAMVA_EXTERNC void hgHtmlRenderer_createHtmlDocumentFromUtf8(
    HgHtmlRendererPtr renderer, const char* htmlText);
HG_KAMVA_EXTERNC int hgHtmlRenderer_renderHtml(
    HgHtmlRendererPtr renderer, int width, int height);
HG_KAMVA_EXTERNC void hgHtmlRenderer_drawHtml(HgHtmlRendererPtr renderer,
    unsigned char* buffer,
    int width,
    int height,
    int stride,
    int htmlX,
    int htmlY);
HG_KAMVA_EXTERNC void hgHtmlRenderer_setBackgroundColor(
    HgHtmlRendererPtr renderer,
    HgByte red,
    HgByte green,
    HgByte blue,
    HgByte alpha = 255);

HG_KAMVA_EXTERNC HgBool hgContainer_parseAndLoadFontConfigFromMemory(
    HgHtmlRendererPtr renderer, const char* fontConfig, HgBool complain);
HG_KAMVA_EXTERNC HgBool hgContainer_addFontDir(
    HgHtmlRendererPtr renderer, const char* dirPath);
HG_KAMVA_EXTERNC void hgContainer_setFontTextCacheSize(
    HgHtmlRendererPtr renderer, int size);
HG_KAMVA_EXTERNC void hgContainer_setDefaultFontName(
    HgHtmlRendererPtr renderer, const char* name);
HG_KAMVA_EXTERNC void hgContainer_setDefaultFontSize(
    HgHtmlRendererPtr renderer, int size);

HG_KAMVA_EXTERNC void hgContainer_setDeviceWidth(
    HgHtmlRendererPtr renderer, int width);
HG_KAMVA_EXTERNC void hgContainer_setDeviceHeight(
    HgHtmlRendererPtr renderer, double height);
HG_KAMVA_EXTERNC void hgContainer_setDeviceDpiX(
    HgHtmlRendererPtr renderer, double dpi);
HG_KAMVA_EXTERNC void hgContainer_setDeviceDpiY(
    HgHtmlRendererPtr renderer, double dpi);
HG_KAMVA_EXTERNC void hgContainer_setDisplayAreaWidth(
    HgHtmlRendererPtr renderer, int width);
HG_KAMVA_EXTERNC void hgContainer_setDisplayAreaHeight(
    HgHtmlRendererPtr renderer, int height);

HG_KAMVA_EXTERNC void hgContainer_setDeviceMonochromeBits(
    HgHtmlRendererPtr renderer, int bits);
HG_KAMVA_EXTERNC void hgContainer_setDeviceColorBits(
    HgHtmlRendererPtr renderer, int bits);
HG_KAMVA_EXTERNC void hgContainer_setDeviceColorIndex(
    HgHtmlRendererPtr renderer, int colorIndex);
HG_KAMVA_EXTERNC void hgContainer_setDeviceMediaType(
    HgHtmlRendererPtr renderer, hgLitehtmlMediaType type);
HG_KAMVA_EXTERNC int hgContainer_ptToPx(HgHtmlRendererPtr renderer, int pt);

HG_KAMVA_EXTERNC void hgHtmlContext_loadMasterStylesheet(
    HgHtmlRendererPtr renderer, const char* str);

HG_KAMVA_EXTERNC int hgHtmlDocument_width(HgHtmlRendererPtr renderer);
HG_KAMVA_EXTERNC int hgHtmlDocument_height(HgHtmlRendererPtr renderer);

#endif /* HG_KAMVA_API_H */
