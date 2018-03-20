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

#include "hgkamva/container/HgFont.h"

namespace hg
{
HgFont::HgFont(FT_Library ftLibrary)
    : mFtLibrary(ftLibrary)
    , mFtFace(nullptr)
    , mHbFont(nullptr)
    , mPixelSize(0)
    , mStrikeout(false)
    , mUnderline(false)
{
  mHbBuffer = hb_buffer_create();
}

HgFont::~HgFont()
{
  destroyFtFace();
  hb_buffer_destroy(mHbBuffer);
}

bool HgFont::createFtFace(const std::string& fontFilePath, int pixelSize)
{
  bool ret = destroyFtFace()
      && (FT_New_Face(mFtLibrary, fontFilePath.c_str(), 0, &mFtFace)
                 == FT_Err_Ok)
      && (FT_Set_Pixel_Sizes(mFtFace, 0, pixelSize) == FT_Err_Ok)
      && (mHbFont = hb_ft_font_create(mFtFace, NULL));
  if(!ret) {
    destroyFtFace();
  }
  return ret;
}

bool HgFont::destroyFtFace()
{
  if(mHbFont) {
    hb_font_destroy(mHbFont);
    mHbFont = nullptr;
  }
  if(mFtFace) {
    FT_Done_Face(mFtFace);
    mFtFace = nullptr;
  }
  return true;
}

FT_F26Dot6 HgFont::xHeight() const
{
  if(!mFtFace || FT_Load_Char(mFtFace, 'x', 0) != FT_Err_Ok) {
    return 0;
  }
  return mFtFace->glyph->metrics.height;
}

}  // namespace hg
