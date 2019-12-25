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

#ifndef HG_FILEUTIL_H
#define HG_FILEUTIL_H

#include <string>

#include "hgkamva/util/Filesystem.h"

namespace hg
{
namespace util
{
bool writePpmFile(const unsigned char* buf,
    unsigned width,
    unsigned height,
    unsigned bytePerPixel,
    const hg::filesystem::path& fileName);

bool compareFiles(const hg::filesystem::path& filePath1,
    const hg::filesystem::path& filePath2);

std::string readFile(const hg::filesystem::path& fileName);

}  // namespace util
}  // namespace hg

#endif  // HG_FILEUTIL_H
