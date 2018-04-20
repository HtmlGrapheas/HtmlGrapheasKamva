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

#ifndef HG_KAMVA_COMMON_H
#define HG_KAMVA_COMMON_H

#ifndef HG_KAMVA_SHARED
#   define HG_KAMVA_EXTERN
#else
#   ifdef _WIN32
#    ifdef HG_KAMVA_EXPORTS
#      ifdef __GNUC__
#        define HG_KAMVA_EXTERN __attribute__((dllexport))
#      else
#        define HG_KAMVA_EXTERN __declspec(dllexport)
#      endif
#    else
#      ifdef __GNUC__
#        define HG_KAMVA_EXTERN __attribute__((dllimport))
#      else
#        define HG_KAMVA_EXTERN __declspec(dllimport)
#      endif
#    endif
#   else
#     if __GNUC__ >= 4
#       define HG_KAMVA_EXTERN __attribute__((visibility("default")))
#     else
#       define HG_KAMVA_EXTERN
#     endif
#   endif
#endif

/* TODO: need '!defined __ANDROID__' ? */
/* #if defined __cplusplus && !defined __ANDROID__ */
#if defined __cplusplus
#define HG_KAMVA_EXTERNC extern "C" HG_KAMVA_EXTERN
#else
#define HG_KAMVA_EXTERNC HG_KAMVA_EXTERN
#endif

#endif  /* HG_KAMVA_COMMON_H */
