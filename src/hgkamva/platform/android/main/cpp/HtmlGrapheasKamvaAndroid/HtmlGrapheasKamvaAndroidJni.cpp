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

#include <android/bitmap.h>
#include <jni.h>

#include <iostream>

#include "HtmlGrapheasKamvaAndroid.h"


// https://developer.android.com/training/articles/perf-jni.html#faq_ULE
// http://stackoverflow.com/a/2480564
// http://stackoverflow.com/a/17643762
// if your native file is compiled as C++ you would need extern "C" {...}
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_ru_feographia_htmlgrapheaskamva_HtmlGrapheasKamvaAndroidJni_drawIntoBitmap(
    JNIEnv* env, jclass type, jobject bitmap)
{
  // https://github.com/AndroidDeveloperLB/AndroidJniBitmapOperations
  // http://stackoverflow.com/questions/18250951/jni-bitmap-operations-for-helping-to-avoid-oom-when-using-large-images
  // http://stackoverflow.com/questions/17900732/how-to-cache-bitmaps-into-native-memory

  AndroidBitmapInfo bitmapInfo;
  AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);

  int ret;
  void* p_pixels;
  if((ret = AndroidBitmap_lockPixels(env, bitmap, &p_pixels)) < 0) {
    //LOGE("AndroidBitmap_lockPixels() failed! Error=%d", ret);
    return;
  }

  hg::HtmlGrapheasKamvaAndroid::drawIntoBitmap(p_pixels, bitmapInfo.width,
      bitmapInfo.height, bitmapInfo.stride, bitmapInfo.format);

  AndroidBitmap_unlockPixels(env, bitmap);
}

#ifdef __cplusplus
}
#endif
