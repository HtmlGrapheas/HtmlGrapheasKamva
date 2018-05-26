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

#include <assert.h>

#include <iostream>

#include "hgkamva/hg_kamva_api.h"


// https://developer.android.com/training/articles/perf-jni.html#faq_ULE
// http://stackoverflow.com/a/2480564
// http://stackoverflow.com/a/17643762
// if your native file is compiled as C++ you would need extern "C" {...}
#ifdef __cplusplus
extern "C" {
#endif

inline void* voidpCast(jlong val)
{
  return reinterpret_cast<void*>(val);
}

JNIEXPORT jint JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgPixelFormatIdToColorBits(
    JNIEnv* env, jclass type, jint pixFmtId)
{
  return hgPixelFormatIdToColorBits(static_cast<hgPixelFormatId>(pixFmtId));
}

JNIEXPORT jlong JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgNewHtmlRenderer(
    JNIEnv* env, jclass type, jobject bitmap)
{
  AndroidBitmapInfo bitmapInfo;
  AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);

  enum hgPixelFormatId pixFmtId;
  switch(bitmapInfo.format) {
    case ANDROID_BITMAP_FORMAT_RGBA_8888: {
      return reinterpret_cast<jlong>(hgNewHtmlRendererRgba32());
    }
    default: {
      std::cout << "hgHtmlRenderer_drawHtml(), unsupported bitmap format\n";
      assert(false);
      return 0;
    }
  }
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgDeleteHtmlRenderer(
    JNIEnv* env, jclass type, jlong renderer)
{
  hgDeleteHtmlRenderer(voidpCast(renderer));
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgHtmlRenderer_1createHtmlDocumentFromUtf8(
    JNIEnv* env, jclass type, jlong renderer, jstring htmlText_)
{
  const char* htmlText = env->GetStringUTFChars(htmlText_, 0);
  hgHtmlRenderer_createHtmlDocumentFromUtf8(voidpCast(renderer), htmlText);
  env->ReleaseStringUTFChars(htmlText_, htmlText);
}

JNIEXPORT jint JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgHtmlRenderer_1renderHtml(
    JNIEnv* env, jclass type, jlong renderer, jint width, jint height)
{
  return hgHtmlRenderer_renderHtml(voidpCast(renderer), width, height);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgHtmlRenderer_1drawHtml(
    JNIEnv* env,
    jclass type,
    jlong renderer,
    jobject bitmap,
    jint htmlX,
    jint htmlY)
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

  hgHtmlRenderer_drawHtml(voidpCast(renderer),
      static_cast<unsigned char*>(p_pixels), bitmapInfo.width,
      bitmapInfo.height, bitmapInfo.stride, htmlX, htmlY);

  AndroidBitmap_unlockPixels(env, bitmap);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgHtmlRenderer_1setBackgroundColor(
    JNIEnv* env,
    jclass type,
    jlong renderer,
    jshort red,
    jshort green,
    jshort blue,
    jshort alpha)
{
  hgHtmlRenderer_setBackgroundColor(
      voidpCast(renderer), red, green, blue, alpha);
}

JNIEXPORT jboolean JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1parseAndLoadFontConfigFromMemory(
    JNIEnv* env,
    jclass type,
    jlong renderer,
    jstring fontConfig_,
    jboolean complain)
{
  const char* fontConfig = env->GetStringUTFChars(fontConfig_, 0);
  jboolean ret = hgContainer_parseAndLoadFontConfigFromMemory(
      voidpCast(renderer), fontConfig, complain);
  env->ReleaseStringUTFChars(fontConfig_, fontConfig);
  return ret;
}

JNIEXPORT jboolean JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1addFontDir(
    JNIEnv* env, jclass type, jlong renderer, jstring dirPath_)
{
  const char* dirPath = env->GetStringUTFChars(dirPath_, 0);
  jboolean ret = hgContainer_addFontDir(voidpCast(renderer), dirPath);
  env->ReleaseStringUTFChars(dirPath_, dirPath);
  return ret;
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setFontTextCacheSize(
    JNIEnv* env, jclass type, jlong renderer, jint size)
{
  hgContainer_setFontTextCacheSize(voidpCast(renderer), size);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDefaultFontName(
    JNIEnv* env, jclass type, jlong renderer, jstring name_)
{
  const char* name = env->GetStringUTFChars(name_, 0);
  hgContainer_setDefaultFontName(voidpCast(renderer), name);
  env->ReleaseStringUTFChars(name_, name);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDefaultFontSize(
    JNIEnv* env, jclass type, jlong renderer, jint size)
{
  hgContainer_setDefaultFontSize(voidpCast(renderer), size);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDeviceWidth(
    JNIEnv* env, jclass type, jlong renderer, jint width)
{
  hgContainer_setDeviceWidth(voidpCast(renderer), width);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDeviceHeight(
    JNIEnv* env, jclass type, jlong renderer, jdouble height)
{
  hgContainer_setDeviceHeight(voidpCast(renderer), height);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDeviceDpiX(
    JNIEnv* env, jclass type, jlong renderer, jdouble dpi)
{
  hgContainer_setDeviceDpiX(voidpCast(renderer), dpi);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDeviceDpiY(
    JNIEnv* env, jclass type, jlong renderer, jdouble dpi)
{
  hgContainer_setDeviceDpiY(voidpCast(renderer), dpi);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDisplayAreaWidth(
    JNIEnv* env, jclass type, jlong renderer, jint width)
{
  hgContainer_setDisplayAreaWidth(voidpCast(renderer), width);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDisplayAreaHeight(
    JNIEnv* env, jclass type, jlong renderer, jint height)
{
  hgContainer_setDisplayAreaHeight(voidpCast(renderer), height);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDeviceMonochromeBits(
    JNIEnv* env, jclass type, jlong renderer, jint bits)
{
  hgContainer_setDeviceMonochromeBits(voidpCast(renderer), bits);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDeviceColorBits(
    JNIEnv* env, jclass type, jlong renderer, jint bits)
{
  hgContainer_setDeviceColorBits(voidpCast(renderer), bits);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDeviceColorIndex(
    JNIEnv* env, jclass type, jlong renderer, jint colorIndex)
{
  hgContainer_setDeviceColorIndex(voidpCast(renderer), colorIndex);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1setDeviceMediaType(
    JNIEnv* env, jclass type_, jlong renderer, int type)
{
  hgContainer_setDeviceMediaType(
      voidpCast(renderer), static_cast<hgLitehtmlMediaType>(type));
}

JNIEXPORT jint JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgContainer_1ptToPx(
    JNIEnv* env, jclass type, jlong renderer, jint pt)
{
  return hgContainer_ptToPx(voidpCast(renderer), pt);
}

JNIEXPORT void JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgHtmlContext_1loadMasterStylesheet(
    JNIEnv* env, jclass type, jlong renderer, jstring str_)
{
  const char* str = env->GetStringUTFChars(str_, 0);
  hgHtmlContext_loadMasterStylesheet(voidpCast(renderer), str);
  env->ReleaseStringUTFChars(str_, str);
}

JNIEXPORT jint JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgHtmlDocument_1width(
    JNIEnv* env, jclass type, jlong renderer)
{
  return hgHtmlDocument_width(voidpCast(renderer));
}

JNIEXPORT jint JNICALL
Java_ru_htmlgrapheas_htmlgrapheaskamva_hgkamva_1api_HgKamvaApiJni_hgHtmlDocument_1height(
    JNIEnv* env, jclass type, jlong renderer)
{
  return hgHtmlDocument_height(voidpCast(renderer));
}

#ifdef __cplusplus
}
#endif
