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

#include "hgkamva/container/HgFontLibrary.h"
#include "hgkamva/container/HgFont.h"

namespace hg
{
HgContainer::HgContainer(void)
{
  // TODO: fonts can be different in one text!
  mHgFontLibrary = std::shared_ptr<HgFontLibrary>(new HgFontLibrary());
  mHgFont = std::shared_ptr<HgFont>(new HgFont(mHgFontLibrary->ftLibrary()));
}

HgContainer::~HgContainer(void)
{
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
    return nullptr;
  }

  uint_least8_t result;
  std::string filePath =
      mHgFontLibrary->getFontFilePath(faceName, size, weight, italic, &result);

  if(HgFontLibrary::FontMatches::allMatched != result) {
    return nullptr;
  }
  if(filePath.size() == 0) {
    return nullptr;
  }
  if(!mHgFont->createFtFace(filePath, size)) {
    return nullptr;
  }

  // Note: for font metric precision (in particular for TTF) see
  // https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_Size_Metrics
  FT_Size ftSize = mHgFont->ftFace()->size;
  fm->ascent = HgFont::f26Dot6ToInt(ftSize->metrics.ascender);
  fm->descent = HgFont::f26Dot6ToInt(ftSize->metrics.descender);
  fm->height = HgFont::f26Dot6ToInt(ftSize->metrics.height);
  fm->x_height = HgFont::f26Dot6ToInt(mHgFont->xHeight());

  mHgFont->setPixelSize(size);
  mHgFont->setStrikeout(decoration & litehtml::font_decoration_linethrough);
  mHgFont->setUnderline(decoration & litehtml::font_decoration_underline);

  return static_cast<litehtml::uint_ptr>(&mHgFont);
}

void HgContainer::delete_font(litehtml::uint_ptr hFont)
{
  mHgFont->destroyFtFace();
}

int HgContainer::text_width(
    const litehtml::tchar_t* text, litehtml::uint_ptr hFont)
{
  return 0;
}

void HgContainer::draw_text(litehtml::uint_ptr hdc,
    const litehtml::tchar_t* text,
    litehtml::uint_ptr hFont,
    litehtml::web_color color,
    const litehtml::position& pos)
{
}

int HgContainer::pt_to_px(int pt)
{
  return 0;
}

int HgContainer::get_default_font_size() const
{
  return 0;
}

const litehtml::tchar_t* HgContainer::get_default_font_name() const
{
	// TODO: set by constructor's parameter.
  return "Tinos";
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
}

void HgContainer::get_language(
    litehtml::tstring& language, litehtml::tstring& culture) const
{
  language = _t("en");
  culture = _t("");
}

//virtual litehtml::tstring HgContainer::resolve_color(
//    const litehtml::tstring& color) const
//{
//  return litehtml::tstring();
//}

}  // namespace hg
