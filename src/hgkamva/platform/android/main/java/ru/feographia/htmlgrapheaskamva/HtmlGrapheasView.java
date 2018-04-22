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
import android.util.DisplayMetrics;
import android.view.View;
import ru.feographia.htmlgrapheaskamva.hgkamva_api.HgKamvaApiJni;
import ru.feographia.htmlgrapheaskamva.hgkamva_api.codes.hgLitehtmlMediaType;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;


public class HtmlGrapheasView
    extends View
{
  private static final Bitmap.Config BITMAP_CONFIG = Bitmap.Config.ARGB_8888;
//  private static final Bitmap.Config BITMAP_CONFIG = Bitmap.Config.RGB_565;

  private Bitmap mBitmap;
  private long   mHgHtmlRenderer;

  private int mHtmlWidth    = 0;
  private int mHtmlHeight   = 0;
  private int mVisibleHtmlWidth  = 0;
  private int mVisibleHtmlHeight = 0;
  
  private int mHtmlX             = 0;
  private int mHtmlY             = 0;
  private int mNewHtmlX          = 0;
  private int mNewHtmlY          = 0;


  public HtmlGrapheasView(Context context)
  {
    super(context);

    // Using example of destructor for HgHtmlRenderer.
    // if(mHgHtmlRenderer != 0) {
    //   HgKamvaApiJni.hgDeleteHtmlRenderer(mHgHtmlRenderer);
    // }

    // Here mBitmap is used as converter
    // from Java Bitmap.Config to NDK ANDROID_BITMAP_FORMAT_*.
    mBitmap = Bitmap.createBitmap(1, 1, BITMAP_CONFIG);
    mHgHtmlRenderer = HgKamvaApiJni.hgNewHtmlRenderer(mBitmap);
    initHgContainer();
  }

  // TODO: move it to util.
  private int bitmapConfigToColorBits(Bitmap.Config config)
  {
    switch (BITMAP_CONFIG) {
      case RGB_565:
        return 5;
      case ARGB_8888:
        return 8;
      default:
        return 0;
    }
  }

  private boolean initHgContainer()
  {
    // TODO: move it to MainApplication
    if (!initAppData()) {
      return false;
    }

    int colorBits = bitmapConfigToColorBits(BITMAP_CONFIG);
    if (colorBits == 0) {
      return false;
    }

    // Set device parameters.
    DisplayMetrics metrics = getResources().getDisplayMetrics();
    HgKamvaApiJni.hgContainer_setDeviceDpiX(mHgHtmlRenderer, metrics.xdpi);
    HgKamvaApiJni.hgContainer_setDeviceDpiY(mHgHtmlRenderer, metrics.ydpi);
    HgKamvaApiJni.hgContainer_setDeviceMonochromeBits(mHgHtmlRenderer, 0);
    HgKamvaApiJni.hgContainer_setDeviceColorBits(mHgHtmlRenderer, colorBits);
    HgKamvaApiJni.hgContainer_setDeviceColorIndex(mHgHtmlRenderer, 0);
    HgKamvaApiJni.hgContainer_setDeviceMediaType(
        mHgHtmlRenderer, hgLitehtmlMediaType.media_type_screen);

    File appDir = getAppDir();
    String dataDirName = "data";
    String fontDirName = "fonts";
    File assetsDir = new File(appDir, "assets");
    File dataDir = new File(assetsDir, dataDirName);
    File fontDir = new File(assetsDir, fontDirName);

    File fontConfigFile = new File(fontDir, "fonts.conf");
    String fontConfig = readFromFile(fontConfigFile);
    if (TextUtils.isEmpty(fontConfig)) {
      return false;
    }

    boolean loadedFontConfig =
        HgKamvaApiJni.hgContainer_parseAndLoadFontConfigFromMemory(
            mHgHtmlRenderer, fontConfig, true);
    if (!loadedFontConfig) {
      return false;
    }

    // Set fonts.
    boolean addedFontDir = HgKamvaApiJni.hgContainer_addFontDir(mHgHtmlRenderer,
        fontDir.getPath());
    if (!addedFontDir) {
      return false;
    }

    // Set font params. Must be after hgContainer_setDeviceDpiY().
    int fontSizePx = HgKamvaApiJni.hgContainer_ptToPx(mHgHtmlRenderer, 10);
    HgKamvaApiJni.hgContainer_setDefaultFontSize(mHgHtmlRenderer, fontSizePx);
    HgKamvaApiJni.hgContainer_setDefaultFontName(mHgHtmlRenderer, "Tinos");

    File masterCssFile = new File(dataDir, "master.css");
    String masterCss = readFromFile(masterCssFile);
    if (TextUtils.isEmpty(masterCss)) {
      return false;
    }

    HgKamvaApiJni.hgHtmlContext_loadMasterStylesheet(
        mHgHtmlRenderer, masterCss);

    File htmlTextFile = new File(dataDir, "test.html");
    String htmlText = readFromFile(htmlTextFile);
    if (TextUtils.isEmpty(htmlText)) {
      return false;
    }

    HgKamvaApiJni.hgHtmlRenderer_createHtmlDocumentFromUtf8(
        mHgHtmlRenderer, htmlText);
    return true;
  }

  private void setBitmap(int width, int height)
  {
    if (mBitmap != null && mBitmap.getWidth() == width
        && mBitmap.getHeight() == height) {
      return;
    }
    mBitmap = Bitmap.createBitmap(width, height, BITMAP_CONFIG);
  }

  private void drawOnBitmap()
  {
    drawHtml(mBitmap.getWidth(), mBitmap.getHeight());
  }

  private void drawHtml(int width, int height)
  {
    if (mBitmap == null || (width == mVisibleHtmlWidth
        && height == mVisibleHtmlHeight && mNewHtmlX == mHtmlX
        && mNewHtmlY == mHtmlY)) {
      return;
    }

    mVisibleHtmlWidth = width;
    mVisibleHtmlHeight = height;
    mHtmlX = mNewHtmlX;
    mHtmlY = mNewHtmlY;

    // Draw HTML document.
    HgKamvaApiJni.hgHtmlRenderer_setBackgroundColor(
        mHgHtmlRenderer, (short) 255, (short) 255, (short) 255, (short) 255);
    HgKamvaApiJni.hgHtmlRenderer_drawHtml(
        mHgHtmlRenderer, mBitmap, mNewHtmlX, mNewHtmlY);
  }

  @Override
  protected void onSizeChanged(int width, int height, int oldw, int oldh)
  {
    int htmlWidth = HgKamvaApiJni.hgHtmlDocument_width(mHgHtmlRenderer);

    if (htmlWidth != width) {
      // Render HTML document.
      HgKamvaApiJni.hgHtmlRenderer_renderHtml(mHgHtmlRenderer, width, height);

      mHtmlWidth = HgKamvaApiJni.hgHtmlDocument_width(mHgHtmlRenderer);
      mHtmlHeight = HgKamvaApiJni.hgHtmlDocument_height(mHgHtmlRenderer);
    }
  }

  @Override
  protected void onDraw(Canvas canvas)
  {
    setBitmap(canvas.getWidth(), canvas.getHeight());
    drawOnBitmap();
    canvas.drawBitmap(mBitmap, 0, 0, null);
  }

  // TODO: move it to MainApplication
  private boolean initAppData()
  {
    File appDir = getAppDir();
    String dataDirName = "data";
    String fontDirName = "fonts";
    File assetsDir = new File(appDir, "assets");
    File dataDir = new File(assetsDir, dataDirName);
    File fontDir = new File(assetsDir, fontDirName);

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
  public File getAppDir()
  {
    String appDirName = null;
    File appDir = getContext().getExternalFilesDir(appDirName);
    if (appDir == null) {
      appDir = new File(getContext().getFilesDir(), appDirName);
    }
    return appDir;
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

  // TODO: move it to util
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

  // TODO: move it to util
  public static String readFromFile(File filePath)
  {
    try {
      FileInputStream inputStream = new FileInputStream(filePath);
      InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
      BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
      StringBuilder stringBuilder = new StringBuilder();
      String line;
      while ((line = bufferedReader.readLine()) != null) {
        stringBuilder.append(line);
      }
      inputStream.close();
      return stringBuilder.toString();
    } catch (IOException e) {
      e.printStackTrace();
      return "";
    }
  }
}
