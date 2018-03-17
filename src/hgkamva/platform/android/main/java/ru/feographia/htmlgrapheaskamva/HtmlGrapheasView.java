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
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.view.View;


public class HtmlGrapheasView
    extends View
{
  Bitmap mBitmap;

  public HtmlGrapheasView(Context context)
  {
    super(context);
  }

  @Override
  protected void onSizeChanged(
      int w, int h, int oldw, int oldh)
  {
    mBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
//    mBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.RGB_565);
  }

  @Override
  protected void onDraw(Canvas canvas)
  {
    HtmlGrapheasKamvaAndroidJni.drawIntoBitmap(mBitmap);
    canvas.drawBitmap(mBitmap, 0, 0, null);
  }
}
