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

namespace hg
{
class FileUtil
{
public:
  static bool writePpmFile(const unsigned char* buf,
      unsigned width,
      unsigned height,
      unsigned bytePerPixel,
      const char* file_name);

  static bool compareFiles(
      const std::string& filePath1, const std::string& filePath2);

  static std::string readFile(const std::string& fileName);
};

}  // namespace hg

#endif  // HG_FILEUTIL_H
