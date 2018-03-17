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

#include "AggContainer.h"

#include <cassert>

#include "HgFont.h"

namespace hg
{
AggContainer::AggContainer()
{
  mHgFont = std::shared_ptr<HgFont>(new HgFont());
}

AggContainer::~AggContainer()
{
}

litehtml::uint_ptr AggContainer::create_font(const litehtml::tchar_t* faceNames,
    int pixelSize,
    int weight,
    litehtml::font_style fontStyle,
    unsigned int decoration,
    litehtml::font_metrics* fm)
{
  mHgFont->createFtFace(faceNames, pixelSize, weight, fontStyle);


//  agg_font* ret = 0;
//
//  if(fm && fontFace) {
//    cairo_save(m_temp_cr);
//
//    cairo_set_font_face(m_temp_cr, fontFace);
//    cairo_set_font_size(m_temp_cr, pixelSize);
//    cairo_font_extents_t ext;
//    cairo_font_extents(m_temp_cr, &ext);
//
//    cairo_text_extents_t tex;
//    cairo_text_extents(m_temp_cr, "x", &tex);
//
//    fm->ascent = (int) ext.ascent;
//    fm->descent = (int) ext.descent;
//    fm->height = (int) (ext.ascent + ext.descent);
//    fm->x_height = (int) tex.height;
//
//    cairo_restore(m_temp_cr);
//
//    ret = new agg_font;
//    ret->font = fontFace;
//    ret->size = pixelSize;
//    ret->strikeout =
//        (decoration & litehtml::font_decoration_linethrough) ? true : false;
//    ret->underline =
//        (decoration & litehtml::font_decoration_underline) ? true : false;
//  }
//
//  return static_cast<litehtml::uint_ptr>(hgFont);

  return nullptr;
}

void AggContainer::delete_font(litehtml::uint_ptr hFont)
{
  mHgFont->destroyFtFace();
}

int AggContainer::text_width(
    const litehtml::tchar_t* text, litehtml::uint_ptr hFont)
{
  return 0;
}

void AggContainer::draw_text(litehtml::uint_ptr hdc,
    const litehtml::tchar_t* text,
    litehtml::uint_ptr hFont,
    litehtml::web_color color,
    const litehtml::position& pos)
{
}

int AggContainer::pt_to_px(int pt)
{
  return 0;
}

int AggContainer::get_default_font_size() const
{
  return 0;
}

const litehtml::tchar_t* AggContainer::get_default_font_name() const
{
  return "Times New Roman";
}

void AggContainer::draw_list_marker(
    litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
{
}

void AggContainer::load_image(const litehtml::tchar_t* src,
    const litehtml::tchar_t* baseurl,
    bool redraw_on_ready)
{
}

void AggContainer::get_image_size(const litehtml::tchar_t* src,
    const litehtml::tchar_t* baseurl,
    litehtml::size& sz)
{
}

void AggContainer::draw_background(
    litehtml::uint_ptr hdc, const litehtml::background_paint& bg)
{
}

void AggContainer::draw_borders(litehtml::uint_ptr hdc,
    const litehtml::borders& borders,
    const litehtml::position& draw_pos,
    bool root)
{
}

// hw
void AggContainer::set_caption(const litehtml::tchar_t* caption)
{
}

// hw
void AggContainer::set_base_url(const litehtml::tchar_t* base_url)
{
}

void AggContainer::link(const std::shared_ptr<litehtml::document>& ptr,
    const litehtml::element::ptr& el)
{
}

// hw
void AggContainer::on_anchor_click(
    const litehtml::tchar_t* url, const litehtml::element::ptr& el)
{
}

// hw
void AggContainer::set_cursor(const litehtml::tchar_t* cursor)
{
}

void AggContainer::transform_text(
    litehtml::tstring& text, litehtml::text_transform tt)
{
}

// hw
void AggContainer::import_css(litehtml::tstring& text,
    const litehtml::tstring& url,
    litehtml::tstring& baseurl)
{
}

void AggContainer::set_clip(const litehtml::position& pos,
    const litehtml::border_radiuses& bdr_radius,
    bool valid_x,
    bool valid_y)
{
}

void AggContainer::del_clip()
{
}

// hw
void AggContainer::get_client_rect(litehtml::position& client) const
{
}

std::shared_ptr<litehtml::element> AggContainer::create_element(
    const litehtml::tchar_t* tag_name,
    const litehtml::string_map& attributes,
    const std::shared_ptr<litehtml::document>& doc)
{
  return nullptr;
}

void AggContainer::get_media_features(litehtml::media_features& media) const
{
}

void AggContainer::get_language(
    litehtml::tstring& language, litehtml::tstring& culture) const
{
  language = _t("en");
  culture = _t("");
}

//virtual litehtml::tstring AggContainer::resolve_color(
//    const litehtml::tstring& color) const
//{
//  return litehtml::tstring();
//}

}  // namespace hg
