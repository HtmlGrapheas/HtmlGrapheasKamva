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

#include "hgkamva/util/FileUtil.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iterator>
#include <vector>

namespace hg
{
// Writing the buffer to a .PPM file, assuming it has RGB-structure, one byte
// per color component.
// http://www.antigrain.com/doc/basic_renderers/basic_renderers.agdoc.html
// static
bool FileUtil::writePpmFile(const unsigned char* buf,
    unsigned width,
    unsigned height,
    unsigned bytePerPixel,
    const char* file_name)
{
  FILE* fd = fopen(file_name, "wb");
  if(fd) {
    fprintf(fd, "P6 %d %d 255 ", width, height);

    unsigned size = width * height * bytePerPixel;
    for (int i = 0; i < size; i += bytePerPixel) {
      fwrite(buf + i, 1, 3, fd);
    }

    fclose(fd);
    return true;
  }
  return false;
}

//https://stackoverflow.com/a/37575457
// static
bool FileUtil::compareFiles(
    const std::string& filePath1, const std::string& filePath2)
{
  std::ifstream f1(filePath1, std::ifstream::binary | std::ifstream::ate);
  std::ifstream f2(filePath2, std::ifstream::binary | std::ifstream::ate);

  if(f1.fail() || f2.fail()) {
    return false;  // File problem.
  }

  if(f1.tellg() != f2.tellg()) {
    return false;  // Size mismatch.
  }

  // Seek back to beginning and use std::equal to compare contents.
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
      std::istreambuf_iterator<char>(),
      std::istreambuf_iterator<char>(f2.rdbuf()));
}

// https://stackoverflow.com/a/525103
// https://stackoverflow.com/a/43009155
// static
std::string FileUtil::readFile(const std::string& fileName)
{
  std::ifstream ifs(
      fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

  std::ifstream::pos_type fileSize = ifs.tellg();
  if(fileSize < 0)
    return std::string();

  ifs.seekg(0, std::ios::beg);
  std::vector<char> bytes(fileSize);
  ifs.read(&bytes[0], fileSize);

  return std::string(&bytes[0], fileSize);
}

}  // namespace hg
