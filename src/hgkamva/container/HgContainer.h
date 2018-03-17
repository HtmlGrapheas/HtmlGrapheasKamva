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

#include <memory>

#include "litehtml.h"

namespace hg
{
class HgFont;

class HgContainer : public litehtml::document_container
{
public:
  HgContainer();
  virtual ~HgContainer();

  // litehtml::document_container interface.
  virtual litehtml::uint_ptr create_font(const litehtml::tchar_t* faceName,
      int size,
      int weight,
      litehtml::font_style italic,
      unsigned int decoration,
      litehtml::font_metrics* fm) override;
  virtual void delete_font(litehtml::uint_ptr hFont) override;
  virtual int text_width(
      const litehtml::tchar_t* text, litehtml::uint_ptr hFont) override;
  virtual void draw_text(litehtml::uint_ptr hdc,
      const litehtml::tchar_t* text,
      litehtml::uint_ptr hFont,
      litehtml::web_color color,
      const litehtml::position& pos) override;
  virtual int pt_to_px(int pt) override;
  virtual int get_default_font_size() const override;
  virtual const litehtml::tchar_t* get_default_font_name() const override;
  virtual void draw_list_marker(
      litehtml::uint_ptr hdc, const litehtml::list_marker& marker) override;
  virtual void load_image(const litehtml::tchar_t* src,
      const litehtml::tchar_t* baseurl,
      bool redraw_on_ready) override;
  virtual void get_image_size(const litehtml::tchar_t* src,
      const litehtml::tchar_t* baseurl,
      litehtml::size& sz) override;
  virtual void draw_background(
      litehtml::uint_ptr hdc, const litehtml::background_paint& bg) override;
  virtual void draw_borders(litehtml::uint_ptr hdc,
      const litehtml::borders& borders,
      const litehtml::position& draw_pos,
      bool root) override;

  virtual void set_caption(const litehtml::tchar_t* caption) override;
  virtual void set_base_url(const litehtml::tchar_t* base_url) override;
  virtual void link(const std::shared_ptr<litehtml::document>& ptr,
      const litehtml::element::ptr& el) override;
  virtual void on_anchor_click(
      const litehtml::tchar_t* url, const litehtml::element::ptr& el) override;
  virtual void set_cursor(const litehtml::tchar_t* cursor) override;
  virtual void transform_text(
      litehtml::tstring& text, litehtml::text_transform tt) override;
  virtual void import_css(litehtml::tstring& text,
      const litehtml::tstring& url,
      litehtml::tstring& baseurl) override;
  virtual void set_clip(const litehtml::position& pos,
      const litehtml::border_radiuses& bdr_radius,
      bool valid_x,
      bool valid_y) override;
  virtual void del_clip() override;
  virtual void get_client_rect(litehtml::position& client) const override;
  virtual std::shared_ptr<litehtml::element> create_element(
      const litehtml::tchar_t* tag_name,
      const litehtml::string_map& attributes,
      const std::shared_ptr<litehtml::document>& doc) override;

  virtual void get_media_features(
      litehtml::media_features& media) const override;
  virtual void get_language(
      litehtml::tstring& language, litehtml::tstring& culture) const override;
  //  virtual litehtml::tstring resolve_color(
  //      const litehtml::tstring& color) const override;

private:
  std::shared_ptr<HgFont> mHgFont;
}; // class HgContainer

}  // namespace hg
