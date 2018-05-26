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

#include "hgkamva/container/HgContainer.h"

#include "hgkamva/container/HgFont.h"
#include "hgkamva/container/HgFontLibrary.h"
#include "hgkamva/container/HgPainter.h"

namespace hg
{
HgContainer::HgContainer()
    : mFontDefaultName("Times New Roman")
    , mDefaultFontSize(16)
    , mFontTextCacheSize(1000)
    , mDeviceWidth(320)
    , mDeviceHeight(240)
    , mDeviceDpiX(96)
    , mDeviceDpiY(96)
    , mDisplayAreaWidth(320)
    , mDisplayAreaHeight(240)
    , mDeviceMonochromeBits(0)
    , mDeviceColorBits(8)
    , mDeviceColorIndex(256)
    , mDeviceMediaType(litehtml::media_type_screen)
{
  mHgFontLibrary = std::shared_ptr<HgFontLibrary>(new HgFontLibrary());
}

bool HgContainer::parseAndLoadFontConfigFromMemory(
    const std::string& fontConfig, bool complain)
{
  return mHgFontLibrary->parseAndLoadConfigFromMemory(fontConfig, complain);
}

bool HgContainer::addFontDir(const std::string& dirPath)
{
  return mHgFontLibrary->addFontDir(dirPath);
}

litehtml::uint_ptr HgContainer::create_font(const litehtml::tchar_t* faceName,
    int size,
    int weight,
    litehtml::font_style italic,
    unsigned int decoration,
    litehtml::font_metrics* fm)
{
  if(!fm) {
    return 0;
  }

  HgFont* hgFont = new HgFont(mHgFontLibrary->ftLibrary(), mFontTextCacheSize);

  uint_least8_t result;
  std::string filePath =
      mHgFontLibrary->getFontFilePath(faceName, size, weight, italic, &result);

  if(HgFontLibrary::FontMatches::allMatched != result) {
    return 0;
  }
  if(filePath.size() == 0) {
    return 0;
  }
  if(!hgFont->createFtFace(filePath, size)) {
    return 0;
  }

  // Note: for font metric precision (in particular for TTF) see
  // https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_Size_Metrics
  FT_Size ftSize = hgFont->ftFace()->size;
  fm->ascent = HgFont::f26Dot6ToInt(ftSize->metrics.ascender);
  fm->descent = HgFont::f26Dot6ToInt(ftSize->metrics.descender);
  fm->height = HgFont::f26Dot6ToInt(ftSize->metrics.height);
  fm->x_height = HgFont::f26Dot6ToInt(hgFont->xHeight());
  if(italic == litehtml::fontStyleItalic || decoration) {
    fm->draw_spaces = true;
  } else {
    fm->draw_spaces = false;
  }

  hgFont->setPixelSize(size);
  hgFont->setStrikeout(decoration & litehtml::font_decoration_linethrough);
  hgFont->setUnderline(decoration & litehtml::font_decoration_underline);

  return reinterpret_cast<litehtml::uint_ptr>(hgFont);
}

void HgContainer::delete_font(litehtml::uint_ptr hFont)
{
  HgFont* hgFont = reinterpret_cast<HgFont*>(hFont);
  if(hgFont) {
    hgFont->destroyFtFace();
    delete hgFont;
  }
}

int HgContainer::text_width(
    const litehtml::tchar_t* text, litehtml::uint_ptr hFont)
{
  HgFont* hgFont = reinterpret_cast<HgFont*>(hFont);
  if(!hgFont) {
    return 0;
  }

  hgFont->clearBuffer();

  // TODO: set Direction, Script and Language through HgContainer's methods.
  hgFont->setDirection(HB_DIRECTION_LTR);
  hgFont->setScript(HB_SCRIPT_LATIN);
  hgFont->setLanguage("eng");

  return hgFont->getBbox(text)->mBboxW;
}

void HgContainer::draw_text(litehtml::uint_ptr hdc,
    const litehtml::tchar_t* text,
    litehtml::uint_ptr hFont,
    litehtml::web_color color,
    const litehtml::position& pos)
{
  HgPainter* hgPainter = reinterpret_cast<HgPainter*>(hdc);
  if(!hgPainter) {
    return;
  }

  if(!text) {
    return;
  }

  HgFont* hgFont = reinterpret_cast<HgFont*>(hFont);
  if(!hgFont) {
    return;
  }

  FT_Size ftSize = hgFont->ftFace()->size;
  int descent = hg::HgFont::f26Dot6ToInt(ftSize->metrics.descender);
  int x = pos.left();
  int y = pos.bottom() + descent;

  hgFont->clearBuffer();

  // TODO: set Direction, Script and Language through HgContainer's methods.
  hgFont->setDirection(HB_DIRECTION_LTR);
  hgFont->setScript(HB_SCRIPT_LATIN);
  hgFont->setLanguage("eng");

  hgFont->drawText(text, hgPainter, x, y, color);

  if(hgFont->underline() || hgFont->strikeout()) {
    int tw = 0;
    tw = text_width(text, hFont);
    hgPainter->setPaintColor(color);

    if(hgFont->underline()) {
      // TODO: set line width by font's height.
      // TODO: set line position by font's parameters.
      //hgPainter->copyHLine(x, y + 1.5, x + tw, color);
      hgPainter->copyHLine(x, y + 3, x + tw);
    }

    if(hgFont->strikeout()) {
      // TODO: set line width by font's height.
      //int lnY = y - HgFont::f26Dot6ToInt(hgFont->xHeight()) / 2.0;
      //hgPainter->copyHLine(x, lnY - 0.5, x + tw, color);
      int lnY = y - HgFont::f26Dot6ToInt(hgFont->xHeight()) / 2;
      hgPainter->copyHLine(x, lnY, x + tw);
    }
  }
}

int HgContainer::pt_to_px(int pt)
{
  return static_cast<int>(static_cast<double>(pt) * mDeviceDpiY / 72.0);
}

int HgContainer::get_default_font_size() const
{
  return mDefaultFontSize;
}

const litehtml::tchar_t* HgContainer::get_default_font_name() const
{
  return mFontDefaultName.c_str();
}

void HgContainer::draw_list_marker(
    litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
{
}

void HgContainer::load_image(const litehtml::tchar_t* src,
    const litehtml::tchar_t* baseurl,
    bool redraw_on_ready)
{
}

void HgContainer::get_image_size(const litehtml::tchar_t* src,
    const litehtml::tchar_t* baseurl,
    litehtml::size& sz)
{
}

void HgContainer::draw_background(
    litehtml::uint_ptr hdc, const litehtml::background_paint& bg)
{
}

void HgContainer::draw_borders(litehtml::uint_ptr hdc,
    const litehtml::borders& borders,
    const litehtml::position& draw_pos,
    bool root)
{
}

void HgContainer::set_caption(const litehtml::tchar_t* caption)
{
}

void HgContainer::set_base_url(const litehtml::tchar_t* base_url)
{
}

void HgContainer::link(const std::shared_ptr<litehtml::document>& ptr,
    const litehtml::element::ptr& el)
{
}

void HgContainer::on_anchor_click(
    const litehtml::tchar_t* url, const litehtml::element::ptr& el)
{
}

void HgContainer::set_cursor(const litehtml::tchar_t* cursor)
{
}

void HgContainer::transform_text(
    litehtml::tstring& text, litehtml::text_transform tt)
{
}

void HgContainer::import_css(litehtml::tstring& text,
    const litehtml::tstring& url,
    litehtml::tstring& baseurl)
{
}

void HgContainer::set_clip(const litehtml::position& pos,
    const litehtml::border_radiuses& bdr_radius,
    bool valid_x,
    bool valid_y)
{
}

void HgContainer::del_clip()
{
}

void HgContainer::get_client_rect(litehtml::position& client) const
{
  client.width = mDisplayAreaWidth;
  client.height = mDisplayAreaHeight;
}

std::shared_ptr<litehtml::element> HgContainer::create_element(
    const litehtml::tchar_t* tag_name,
    const litehtml::string_map& attributes,
    const std::shared_ptr<litehtml::document>& doc)
{
  return nullptr;
}

void HgContainer::get_media_features(litehtml::media_features& media) const
{
  litehtml::position clientRect;
  get_client_rect(clientRect);

  media.type = mDeviceMediaType;
  media.width = clientRect.width;
  media.height = clientRect.height;
  media.color = mDeviceColorBits;
  media.color_index = mDeviceColorIndex;
  media.monochrome = mDeviceMonochromeBits;
  media.resolution = mDeviceDpiX;
  media.device_width = mDeviceWidth;
  media.device_height = mDeviceHeight;
}

void HgContainer::get_language(
    litehtml::tstring& language, litehtml::tstring& culture) const
{
  language = _t("en");
  culture = _t("");
}

//litehtml::tstring HgContainer::resolve_color(
//    const litehtml::tstring& color) const
//{
//  return litehtml::tstring();
//}

}  // namespace hg
