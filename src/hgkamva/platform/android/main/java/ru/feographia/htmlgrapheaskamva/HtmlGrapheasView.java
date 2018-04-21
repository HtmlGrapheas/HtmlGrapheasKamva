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

package ru.feographia.htmlgrapheaskamva;

import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.text.TextUtils;
import android.view.View;
import ru.feographia.htmlgrapheaskamva.hgkamva_api.HgKamvaApiJni;
import ru.feographia.htmlgrapheaskamva.hgkamva_api.codes.hgLitehtmlMediaType;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;


public class HtmlGrapheasView
    extends View
{
  private static final Bitmap.Config BITMAP_CONFIG = Bitmap.Config.ARGB_8888;
//  private static final Bitmap.Config BITMAP_CONFIG = Bitmap.Config.RGB_565;

  private Bitmap mBitmap;
  private long   mHgHtmlRenderer;

  int mScrollX = 0;
  int mScrollY = 0;
  int mNewScrollX = 0;
  int mNewScrollY = 0;

  public HtmlGrapheasView(Context context)
  {
    super(context);

    // Using example of destructor for HgHtmlRenderer.
    // if(mHgHtmlRenderer != 0) {
    //   HgKamvaApiJni.deleteHgHtmlRenderer(mHgHtmlRenderer);
    // }

    // Here mBitmap is used as converter
    // from Java Bitmap.Config to NDK ANDROID_BITMAP_FORMAT_*.
    mBitmap = Bitmap.createBitmap(1, 1, BITMAP_CONFIG);
    mHgHtmlRenderer = HgKamvaApiJni.newHgHtmlRenderer(mBitmap);
    initHgContainer();
  }

  private void initHgContainer()
  {
    // TODO: move it to MainApplication
    initAppData();

    String fontConfig = "TODO: fontConfig";
    if (TextUtils.isEmpty(fontConfig)) {
      // TODO: TextUtils.isEmpty(fontConfig)
    }

    boolean loadedFontConfig =
        HgKamvaApiJni.hgContainer_parseAndLoadFontConfigFromMemory(
            mHgHtmlRenderer, fontConfig, true);
    if (!loadedFontConfig) {
      // TODO: !loadedFontConfig
    }

    // Set fonts.
    boolean addedFontDir =
        HgKamvaApiJni.hgContainer_addFontDir(mHgHtmlRenderer, "TODO: fontDir");
    if (!addedFontDir) {
      // TODO: !addedFontDir
    }

    HgKamvaApiJni.hgContainer_setDefaultFontName(mHgHtmlRenderer, "Tinos");
    HgKamvaApiJni.hgContainer_setDefaultFontSize(mHgHtmlRenderer, 24);

    // Set device parameters.
    HgKamvaApiJni.hgContainer_setDeviceDpiX(mHgHtmlRenderer, 96);
    HgKamvaApiJni.hgContainer_setDeviceDpiY(mHgHtmlRenderer, 96);
    HgKamvaApiJni.hgContainer_setDeviceMonochromeBits(mHgHtmlRenderer, 0);
    HgKamvaApiJni.hgContainer_setDeviceColorBits(mHgHtmlRenderer, 8);
    HgKamvaApiJni.hgContainer_setDeviceColorIndex(mHgHtmlRenderer, 256);
    HgKamvaApiJni.hgContainer_setDeviceMediaType(
        mHgHtmlRenderer, hgLitehtmlMediaType.media_type_screen);

    String masterCss = "TODO: masterCss";
    if (TextUtils.isEmpty(masterCss)) {
      // TODO: TextUtils.isEmpty(masterCss)
    }

    HgKamvaApiJni.hgHtmlContext_loadMasterStylesheet(
        mHgHtmlRenderer, masterCss);

    String htmlText = "TODO: htmlText";
    if (TextUtils.isEmpty(htmlText)) {
      // TODO: TextUtils.isEmpty(htmlText)
    }

    HgKamvaApiJni.hgHtmlRenderer_createHtmlDocumentFromUtf8(
        mHgHtmlRenderer, htmlText);
  }

  private void renderHtml(int width, int height)
  {
    // Render HTML document.
    HgKamvaApiJni.hgHtmlRenderer_renderHtml(mHgHtmlRenderer, width, height);
    int htmlWidth = HgKamvaApiJni.hgHtmlDocument_width(mHgHtmlRenderer);
    int htmlHeight = HgKamvaApiJni.hgHtmlDocument_height(mHgHtmlRenderer);
  }

  private void drawHtml(int width, int height)
  {
    if (mBitmap != null && mBitmap.getWidth() == width
        && mBitmap.getHeight() == height && mScrollX == mNewScrollX
        && mScrollY == mNewScrollY) {
      return;
    }

    mScrollX = mNewScrollX;
    mScrollY = mNewScrollY;

    mBitmap = Bitmap.createBitmap(width, height, BITMAP_CONFIG);

    // Draw HTML document.
    HgKamvaApiJni.hgHtmlRenderer_setBackgroundColor(
        mHgHtmlRenderer, (short) 255, (short) 255, (short) 255, (short) 255);
    HgKamvaApiJni.hgHtmlRenderer_drawHtml(
        mHgHtmlRenderer, mBitmap, mNewScrollX, mNewScrollY);
  }

  @Override
  protected void onSizeChanged(
      int w, int h, int oldw, int oldh)
  {
    renderHtml(w, h);
  }

  @Override
  protected void onDraw(Canvas canvas)
  {
    drawHtml(canvas.getWidth(), canvas.getHeight());
    canvas.drawBitmap(mBitmap, 0, 0, null);
  }

  // TODO: move it to MainApplication
  private boolean initAppData()
  {
    String appDirName = null;
    File appDir = getContext().getExternalFilesDir(appDirName);
    if (appDir == null) {
      appDir = new File(getContext().getFilesDir(), appDirName);
    }

    String dataDirName = "data";
    String fontDirName = "fonts";
    File dataDir = new File(appDir, dataDirName);
    File fontDir = new File(appDir, fontDirName);

    if (dataDir.exists() && dataDir.isDirectory() && fontDir.exists() && fontDir
        .isDirectory()) {
      return true;
    }

    String apkFilePath = getApkPath();
    if (apkFilePath == null) {
      return false;
    }
    File apkFile = new File(getApkPath());

    try {
      unzip(apkFile, appDir, "assets/" + dataDirName);
      unzip(apkFile, appDir, "assets/" + fontDirName);
    } catch (IOException e) {
      e.printStackTrace();
      return false;
    }
    return true;
  }

  // TODO: move it to MainApplication
  public String getApkPath()
  {
    try {
      return getContext().getPackageManager()
          .getApplicationInfo(getContext().getPackageName(), 0).sourceDir;
    } catch (PackageManager.NameNotFoundException e) {
      e.printStackTrace();
      return null;
    }
  }

  // TODO: move it to MainApplication
  public static void unzip(
      File zipFile, File targetDirectory, String extractedDirName)
      throws IOException
  {
    ZipInputStream zis = new ZipInputStream(
        new BufferedInputStream(new FileInputStream(zipFile)));
    try {
      ZipEntry ze;
      int count;
      byte[] buffer = new byte[8192];
      while ((ze = zis.getNextEntry()) != null) {
        File file = new File(targetDirectory, ze.getName());
        File dir = ze.isDirectory() ? file : file.getParentFile();

        if (extractedDirName != null && !dir.getPath()
            .contains(extractedDirName)) {
          continue;
        }

        if (!dir.isDirectory() && !dir.mkdirs()) {
          throw new FileNotFoundException(
              "Failed to ensure directory: " + dir.getAbsolutePath());
        }
        if (ze.isDirectory()) { continue; }
        FileOutputStream fout = new FileOutputStream(file);
        try {
          while ((count = zis.read(buffer)) != -1) {
            fout.write(buffer, 0, count);
          }
        } finally {
          fout.close();
        }
        // If time should be restored as well.
        //long time = ze.getTime();
        //if (time > 0) { file.setLastModified(time); }
      }
    } finally {
      zis.close();
    }
  }
}
