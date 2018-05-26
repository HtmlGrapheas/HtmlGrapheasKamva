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

package ru.htmlgrapheas.htmlgrapheaskamva;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.Loader;
import android.support.v4.view.GestureDetectorCompat;
import android.support.v4.view.ViewCompat;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.View;
import android.widget.OverScroller;
import ru.htmlgrapheas.htmlgrapheaskamva.hgkamva_api.HgKamvaApiJni;
import ru.htmlgrapheas.htmlgrapheaskamva.hgkamva_api.codes.hgLitehtmlMediaType;

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

// Touch events:
// https://developer.android.com/training/gestures/detector.html
// https://developer.android.com/training/gestures/scroll.html
// https://developer.android.com/training/gestures/scale.html
// https://developer.android.com/shareables/training/InteractiveChart.zip

public class HtmlGrapheasView
    extends View
    implements GestureDetector.OnGestureListener,
               GestureDetector.OnDoubleTapListener,
               ScaleGestureDetector.OnScaleGestureListener,
               LoaderManager.LoaderCallbacks<Bitmap>
{
  // TODO: order private and protected members.

  private GestureDetectorCompat mGestureDetector;
  private ScaleGestureDetector  mScaleGestureDetector;
  private OverScroller          mScroller;

  private static final Bitmap.Config BITMAP_CONFIG = Bitmap.Config.ARGB_8888;

  private Bitmap mBitmap;
  private long   mHgHtmlRenderer;

  private int mBackgroundColor = Color.argb(255, 255, 255, 255);

  private int mHtmlWidth         = 0;
  private int mHtmlHeight        = 0;
  private int mVisibleHtmlWidth  = 0;
  private int mVisibleHtmlHeight = 0;

  private int mHtmlX    = 0;
  private int mHtmlY    = 0;
  private int mNewHtmlX = 0;
  private int mNewHtmlY = 0;

  private boolean mWaitBitmap = false;

  private OnHtmlParamsChangeListener mOnHtmlParamsChangeListener;


  public HtmlGrapheasView(Context context)
  {
    super(context);

    mGestureDetector = new GestureDetectorCompat(context, this);
    mGestureDetector.setOnDoubleTapListener(this);
    mScaleGestureDetector = new ScaleGestureDetector(context, this);
    mScroller = new OverScroller(context);

    // Using example of destructor for HgHtmlRenderer.
    // if(mHgHtmlRenderer != 0) {
    //   HgKamvaApiJni.hgDeleteHtmlRenderer(mHgHtmlRenderer);
    //   mHgHtmlRenderer = 0;
    // }

    // Here mBitmap is used as converter
    // from Java Bitmap.Config to NDK ANDROID_BITMAP_FORMAT_*.
    mBitmap = Bitmap.createBitmap(1, 1, BITMAP_CONFIG);
    mHgHtmlRenderer = HgKamvaApiJni.hgNewHtmlRenderer(mBitmap);
    mBitmap = null;
    initHgContainer();
  }

  @Override
  protected void finalize()
      throws Throwable
  {
    if (mHgHtmlRenderer != 0) {
      HgKamvaApiJni.hgDeleteHtmlRenderer(mHgHtmlRenderer);
      mHgHtmlRenderer = 0;
    }

    super.finalize();
  }

  protected void setNewHtmlX(int x)
  {
    if (x < 0) {
      mNewHtmlX = 0;
    } else {
      int maxX = mHtmlWidth - mVisibleHtmlWidth;
      if (x > maxX) {
        mNewHtmlX = maxX;
      } else {
        mNewHtmlX = x;
      }
    }
  }

  protected void setNewHtmlY(int y)
  {
    if (y < 0) {
      mNewHtmlY = 0;
    } else {
      int maxY = mHtmlHeight - mVisibleHtmlHeight;
      if (y > maxY) {
        mNewHtmlY = maxY;
      } else {
        mNewHtmlY = y;
      }
    }
  }

  // TODO: move it to util.
  private int bitmapConfigToColorBits(Bitmap.Config config)
  {
    switch (BITMAP_CONFIG) {
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
    HgKamvaApiJni.hgContainer_setFontTextCacheSize(mHgHtmlRenderer, 10000);

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

    // TODO: createHtmlDocumentFromUtf8() to loader.
    HgKamvaApiJni.hgHtmlRenderer_createHtmlDocumentFromUtf8(
        mHgHtmlRenderer, htmlText);
    return true;
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

  // View interface.

  @Override
  protected void onSizeChanged(int w, int h, int oldw, int oldh)
  {
    resetBitmapParams(w, h);
  }

  @Override
  protected void onDraw(Canvas canvas)
  {
    if (mBitmap == null) {
      mVisibleHtmlWidth = canvas.getWidth();
      mVisibleHtmlHeight = canvas.getHeight();
      mHtmlX = mNewHtmlX;
      mHtmlY = mNewHtmlY;
      runDrawOnBitmapLoader();
    }

    if (mBitmap != null) {
      canvas.drawBitmap(mBitmap, 0, 0, null);
    }

    // TODO: for mBackgroundColor
    if (!mWaitBitmap && (/*backgroundColor != mBackgroundColor ||*/
        mNewHtmlX != mHtmlX || mNewHtmlY != mHtmlY
            || canvas.getWidth() != mVisibleHtmlWidth
            || canvas.getHeight() != mVisibleHtmlHeight)) {

      //mBackgroundColor = backgroundColor;
      mVisibleHtmlWidth = canvas.getWidth();
      mVisibleHtmlHeight = canvas.getHeight();
      mHtmlX = mNewHtmlX;
      mHtmlY = mNewHtmlY;

      if (mOnHtmlParamsChangeListener != null) {
        mOnHtmlParamsChangeListener.onHtmlParamsChange(mHgHtmlRenderer,
            mBackgroundColor, mVisibleHtmlWidth, mVisibleHtmlHeight, mNewHtmlX,
            mNewHtmlY);
        mWaitBitmap = true;
      }
    }
  }

  @Override
  public void computeScroll()
  {
    super.computeScroll();

    boolean needsInvalidate = false;

    // The scroller isn't finished, meaning a fling or programmatic pan
    // operation is currently active.
    if (mScroller.computeScrollOffset()) {
      // This part is used only for fling, for scroll it can not be used.
      mNewHtmlX = mScroller.getCurrX();
      mNewHtmlY = mScroller.getCurrY();
      //float velocity = getCurrVelocityCompat(mScroller);
      needsInvalidate = true;
    }

    if (needsInvalidate) {
      ViewCompat.postInvalidateOnAnimation(this);
    }
  }

  @Override
  public boolean onTouchEvent(MotionEvent event)
  {
    boolean ret = mScaleGestureDetector.onTouchEvent(event);
    ret = mGestureDetector.onTouchEvent(event) || ret;
    return ret || super.onTouchEvent(event);
  }

  // GestureDetector.OnGestureListener interface.

  @Override
  public boolean onDown(MotionEvent e)
  {
    mScroller.forceFinished(true);
    ViewCompat.postInvalidateOnAnimation(this);
    return true;
  }

  @Override
  public void onShowPress(MotionEvent e)
  {

  }

  @Override
  public boolean onSingleTapUp(MotionEvent e)
  {
    return false;
  }

  @Override
  public void onLongPress(MotionEvent e)
  {

  }

  @Override
  public boolean onScroll(
      MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
  {
    mScroller.forceFinished(true);

    setNewHtmlX(mNewHtmlX + (int) distanceX);
    setNewHtmlY(mNewHtmlY + (int) distanceY);

    ViewCompat.postInvalidateOnAnimation(this);
    return true;
  }

  @Override
  public boolean onFling(
      MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)
  {
    // Before flinging, aborts the current animation.
    mScroller.forceFinished(true);

    // Begins the animation
    mScroller.fling(
        // Current scroll position
        mHtmlX, mHtmlY,
        //
        (int) -velocityX, (int) -velocityY,
        // Minimum and maximum scroll positions. The minimum scroll
        // position is generally zero and the maximum scroll position
        // is generally the content size less the screen size. So if the
        // content width is 1000 pixels and the screen width is 200
        // pixels, the maximum scroll offset should be 800 pixels.
        0, mHtmlWidth - mVisibleHtmlWidth, 0, mHtmlHeight - mVisibleHtmlHeight,
        // The edges of the content. This comes into play when using
        // the EdgeEffect class to draw "glow" overlays.
        0, 0);

    // Invalidates to trigger computeScroll()
    ViewCompat.postInvalidateOnAnimation(this);
    return true;
  }

  // GestureDetector.OnDoubleTapListener interface.

  @Override
  public boolean onSingleTapConfirmed(MotionEvent e)
  {
    return false;
  }

  @Override
  public boolean onDoubleTap(MotionEvent e)
  {
//    // TODO:
//    ViewCompat.postInvalidateOnAnimation(this);
//    return true;
    return false;
  }

  @Override
  public boolean onDoubleTapEvent(MotionEvent e)
  {
    return false;
  }

  // ScaleGestureDetector.OnScaleGestureListener interface.

  @Override
  public boolean onScaleBegin(ScaleGestureDetector detector)
  {
    return false;
  }

  @Override
  public boolean onScale(ScaleGestureDetector detector)
  {
    return false;
  }

  @Override
  public void onScaleEnd(ScaleGestureDetector detector)
  {

  }

  // Utils.

  @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
  public static float getCurrVelocityCompat(OverScroller overScroller)
  {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
      return overScroller.getCurrVelocity();
    } else {
      return 0;
    }
  }

  // LoaderManager.LoaderCallbacks interface.

  // TODO: move it to util.
  // https://stackoverflow.com/a/32973351
  private AppCompatActivity getActivity()
  {
    Context context = getContext();
    while (context instanceof ContextWrapper) {
      if (context instanceof AppCompatActivity) {
        return (AppCompatActivity) context;
      }
      context = ((ContextWrapper) context).getBaseContext();
    }
    return null;
  }

  private void runDrawOnBitmapLoader()
  {
    AppCompatActivity activity = getActivity();
    if (activity == null) {
      return;
    }
    LoaderManager lm = getActivity().getSupportLoaderManager();
    int loaderId = 1231;
    Loader loader = lm.getLoader(loaderId);

    if (mOnHtmlParamsChangeListener == null && loader != null
        && loader instanceof DrawOnBitmapLoader) {
      DrawOnBitmapLoader bitmapLoader = (DrawOnBitmapLoader) loader;
      setOnHtmlParamsChangeListener(bitmapLoader);
      // Set new params to loader here, not in constructor, loader is exist.
      resetBitmapParams(mVisibleHtmlWidth, mVisibleHtmlHeight);
    }

    if (null != loader && loader.isStarted()) {
      lm.restartLoader(loaderId, null, this);
    } else {
      lm.initLoader(loaderId, null, this);
    }
  }

  @NonNull
  @Override
  public Loader<Bitmap> onCreateLoader(
      int id,
      @Nullable
          Bundle args)
  {
    DrawOnBitmapLoader loader =
        new DrawOnBitmapLoader(getContext(), mHgHtmlRenderer, BITMAP_CONFIG,
            mBackgroundColor, mVisibleHtmlWidth, mVisibleHtmlHeight, mNewHtmlX,
            mNewHtmlY);
    setOnHtmlParamsChangeListener(loader);
    return loader;
  }

  @Override
  public void onLoadFinished(
      @NonNull
          Loader<Bitmap> loader, Bitmap bitmap)
  {
    if (!bitmap.isRecycled()) {
      mBitmap = bitmap.copy(BITMAP_CONFIG, false);
      mWaitBitmap = false;
      mHtmlWidth = HgKamvaApiJni.hgHtmlDocument_width(mHgHtmlRenderer);
      mHtmlHeight = HgKamvaApiJni.hgHtmlDocument_height(mHgHtmlRenderer);
      invalidate();
    }
  }

  @Override
  public void onLoaderReset(
      @NonNull
          Loader<Bitmap> loader)
  {
    setOnHtmlParamsChangeListener(null);
    resetBitmapParams(mVisibleHtmlWidth, mVisibleHtmlHeight);
    invalidate();
  }

  private void resetBitmapParams(int width, int height)
  {
    mBitmap = null;
    mVisibleHtmlWidth = width;
    mVisibleHtmlHeight = height;
    mHtmlWidth = 0;
    mHtmlHeight = 0;

    if (mOnHtmlParamsChangeListener != null) {
      mOnHtmlParamsChangeListener.onHtmlParamsChange(mHgHtmlRenderer,
          mBackgroundColor, mVisibleHtmlWidth, mVisibleHtmlHeight, mNewHtmlX,
          mNewHtmlY);
      mWaitBitmap = true;
    }
  }

  public void setOnHtmlParamsChangeListener(
      OnHtmlParamsChangeListener onHtmlParamsChangeListener)
  {
    mOnHtmlParamsChangeListener = onHtmlParamsChangeListener;
  }

  interface OnHtmlParamsChangeListener
  {
    void onHtmlParamsChange(
        long hgHtmlRenderer, int backgroundColor, int width, int height,
        int htmlX, int htmlY);
  }
}
