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

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.support.v4.content.AsyncTaskLoader;
import ru.htmlgrapheas.htmlgrapheaskamva.hgkamva_api.HgKamvaApiJni;


// See example in
// http://developer.android.com/reference/android/content/AsyncTaskLoader.html
public class DrawOnBitmapLoader
    extends AsyncTaskLoader<Bitmap>
    implements HtmlGrapheasView.OnHtmlParamsChangeListener
{
  private long          mHgHtmlRenderer;
  private Bitmap.Config mBitmapConfig;

  private int mBackgroundColor;
  private int mWidth;
  private int mHeight;
  private int mHtmlX;
  private int mHtmlY;

  private Bitmap  mBitmap            = null;
  private boolean mBitmapSizeChanged = true;

  public DrawOnBitmapLoader(
      Context context, long hgHtmlRenderer, Bitmap.Config config,
      int backgroundColor, int width, int height, int htmlX, int htmlY)
  {
    super(context);

    mHgHtmlRenderer = hgHtmlRenderer;
    mBitmapConfig = config;

    mBackgroundColor = backgroundColor;
    mWidth = width;
    mHeight = height;
    mHtmlX = htmlX;
    mHtmlY = htmlY;
  }

  private void renderHtmlToBitmap()
  {
    HgKamvaApiJni.hgContainer_setDeviceWidth(mHgHtmlRenderer, mWidth);
    HgKamvaApiJni.hgContainer_setDeviceHeight(mHgHtmlRenderer, mHeight);
    HgKamvaApiJni.hgContainer_setDisplayAreaWidth(mHgHtmlRenderer, mWidth);
    HgKamvaApiJni.hgContainer_setDisplayAreaHeight(mHgHtmlRenderer, mHeight);

    int htmlWidth = HgKamvaApiJni.hgHtmlDocument_width(mHgHtmlRenderer);
    if (htmlWidth != mWidth) {
      // Render HTML document.
      HgKamvaApiJni.hgHtmlRenderer_renderHtml(mHgHtmlRenderer, mWidth, mHeight);
    }

    short red = (short) Color.red(mBackgroundColor);
    short green = (short) Color.green(mBackgroundColor);
    short blue = (short) Color.blue(mBackgroundColor);
    short alpha = (short) Color.alpha(mBackgroundColor);

    // Draw HTML document.
    HgKamvaApiJni.hgHtmlRenderer_setBackgroundColor(
        mHgHtmlRenderer, red, green, blue, alpha);
    HgKamvaApiJni.hgHtmlRenderer_drawHtml(
        mHgHtmlRenderer, mBitmap, mHtmlX, mHtmlY);
  }

  /**
   * This is where the bulk of our work is done.
   * This function is called in a background thread
   * and should generate a new set of data to be published by the loader.
   */
  @Override
  public Bitmap loadInBackground()
  {
    if (mBitmapSizeChanged && mBitmap != null) {
      onReleaseResources(mBitmap);
      mBitmap = null;
    }

    if (mBitmap == null) {
      mBitmap = Bitmap.createBitmap(mWidth, mHeight, mBitmapConfig);
    }

    renderHtmlToBitmap();
    return mBitmap;
  }

  /**
   * Called when there is new data to deliver to the client.
   * The super class will take care of delivering it;
   * the implementation here just adds a little more logic.
   */
  @Override
  public void deliverResult(Bitmap bitmap)
  {
    if (isReset()) {
      // An async query came in while the loader is stopped.
      // We don't need the result.
      if (bitmap != null) {
        onReleaseResources(bitmap);
        bitmap = null;
      }
    }

    Bitmap oldBitmap = null;
    if (bitmap != mBitmap) {
      oldBitmap = mBitmap;
      mBitmap = bitmap;
    }

    if (isStarted()) {
      // If the Loader is currently started,
      // we can immediately deliver its results.
      super.deliverResult(bitmap);
    }

    // At this point we can release the resources
    // associated with 'oldBitmap' if needed;
    // now that the new result is delivered we know that it is no longer in use.
    if (oldBitmap != null) {
      onReleaseResources(oldBitmap);
    }
  }

  /**
   * Handles a request to start the Loader.
   */
  @Override
  protected void onStartLoading()
  {
    if (!mBitmapSizeChanged && mBitmap != null) {
      // If we currently have a result available, deliver it
      // immediately.
      deliverResult(mBitmap);
    } else if (mBitmap != null) {
      onReleaseResources(mBitmap);
      mBitmap = null;
    }

    // Start watching for changes in the bitmap.
//    if (null != mHtmlGrapheasView) {
//      mHtmlGrapheasView.onHtmlParamsChange(this);
//    }

    if (takeContentChanged() || mBitmap == null) {
      // If the data has changed since the last time it was loaded
      // or is not currently available, start a load.
      forceLoad();
    }
  }

  /**
   * Handles a request to stop the Loader.
   */
  @Override
  protected void onStopLoading()
  {
    // Attempt to cancel the current load task if possible.
    cancelLoad();
  }

  /**
   * Handles a request to cancel a load.
   */
  @Override
  public void onCanceled(Bitmap bitmap)
  {
    super.onCanceled(bitmap);

    if (bitmap == mBitmap) {
      mBitmap = null;
    }

    // At this point we can release the resources
    // associated with 'bitmap' if needed.
    onReleaseResources(bitmap);
  }

  /**
   * Handles a request to completely reset the Loader.
   */
  @Override
  protected void onReset()
  {
    super.onReset();

    // Ensure the loader is stopped
    onStopLoading();

    // At this point we can release the resources
    // associated with 'mBitmap' if needed.
    if (mBitmap != null) {
      onReleaseResources(mBitmap);
      mBitmap = null;
    }

    // Stop monitoring for changes.
//    if (null != mHtmlGrapheasView) {
//      mHtmlGrapheasView.onHtmlParamsChange(null);
//    }
  }

  /**
   * Helper function to take care of releasing resources
   * associated with an actively loaded data set.
   */
  protected void onReleaseResources(Bitmap bitmap)
  {
    if (null != bitmap) {
      bitmap.recycle();
    }
  }

  // HtmlGrapheasView.OnHtmlParamsChangeListener interface.

  @Override
  public void onHtmlParamsChange(
      long hgHtmlRenderer, int backgroundColor, int width, int height,
      int htmlX, int htmlY)
  {
    mBitmapSizeChanged = mBitmap != null && (mBitmap.getWidth() != width
        || mBitmap.getHeight() != height);

    mHgHtmlRenderer = hgHtmlRenderer;
    mBackgroundColor = backgroundColor;
    mWidth = width;
    mHeight = height;
    mHtmlX = htmlX;
    mHtmlY = htmlY;

    onContentChanged();
  }
}
