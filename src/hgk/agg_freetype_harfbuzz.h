/*****************************************************************************
 * Project:  HtmlGrapheas
 * Purpose:  HTML text editor library
 * Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *   Copyright (c) 2017 NikitaFeodonit
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

// Based on the code from:
// https://github.com/lxnt/ex-sdl-freetype-harfbuzz/blob/master/ex-sdl-freetype-harfbuzz.c

#ifndef AGG_FREETYPE_HARFBUZZ_H
#define AGG_FREETYPE_HARFBUZZ_H

#include <string>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb-ft.h>
#include <hb.h>

#include "agg_renderer_base.h"

/* google this */
#ifndef unlikely
#define unlikely
#endif

template <class PixelFormat>
struct agg_spanner_baton_t
{
  agg::renderer_base<PixelFormat>* rbase;
  int glyph_x;
  int glyph_y;

  /* sizing part */
  int min_span_x;
  int max_span_x;
  int min_y;
  int max_y;
};

template <class PixelFormat>
void agg_spanner_blend(int y, int count, const FT_Span* spans, void* user)
{
  typedef typename agg::renderer_base<PixelFormat>::color_type rbase_color_type;

  agg_spanner_baton_t<PixelFormat>* baton =
      static_cast<agg_spanner_baton_t<PixelFormat>*>(user);

  rbase_color_type color;
  color.clear();
  color.opacity(1.0);
  color.r = 128;
  color.g = 128;
  color.b = 128;

  for (int i = 0; i < count; ++i) {
    baton->rbase->blend_hline(baton->glyph_x + spans[i].x, baton->glyph_y - y,
        baton->glyph_x + spans[i].x + spans[i].len - 1, color,
        spans[i].coverage);
  }
}

/*  This spanner is for obtaining exact bounding box for the string.
    Unfortunately this can't be done without rendering it (or pretending to).
    After this runs, we get min and max values of coordinates used.
*/
template <class PixelFormat>
void agg_spanner_sizer(int y, int count, const FT_Span* spans, void* user)
{
  agg_spanner_baton_t<PixelFormat>* baton =
      static_cast<agg_spanner_baton_t<PixelFormat>*>(user);

  if (y < baton->min_y)
    baton->min_y = y;
  if (y > baton->max_y)
    baton->max_y = y;
  for (int i = 0; i < count; i++) {
    if (spans[i].x + spans[i].len > baton->max_span_x)
      baton->max_span_x = spans[i].x + spans[i].len;
    if (spans[i].x < baton->min_span_x)
      baton->min_span_x = spans[i].x;
  }
}

void ftfdump(FT_Face ftf)
{
  for (int i = 0; i < ftf->num_charmaps; i++) {
    printf("%d: %s %s %c%c%c%c plat=%hu id=%hu\n", i, ftf->family_name,
        ftf->style_name, ftf->charmaps[i]->encoding >> 24,
        (ftf->charmaps[i]->encoding >> 16) & 0xff,
        (ftf->charmaps[i]->encoding >> 8) & 0xff,
        (ftf->charmaps[i]->encoding) & 0xff, ftf->charmaps[i]->platform_id,
        ftf->charmaps[i]->encoding_id);
  }
}

/*  See http://www.microsoft.com/typography/otspec/name.htm
    for a list of some possible platform-encoding pairs.
    We're interested in 0-3 aka 3-1 - UCS-2.
    Otherwise, fail. If a font has some unicode map, but lacks
    UCS-2 - it is a broken or irrelevant font. What exactly
    Freetype will select on face load (it promises most wide
    unicode, and if that will be slower that UCS-2 - left as
    an excercise to check. */
int force_ucs2_charmap(FT_Face ftf)
{
  for (int i = 0; i < ftf->num_charmaps; i++)
    if (((ftf->charmaps[i]->platform_id == 0)
            && (ftf->charmaps[i]->encoding_id == 3))
        || ((ftf->charmaps[i]->platform_id == 3)
               && (ftf->charmaps[i]->encoding_id == 1)))
      return (FT_Set_Charmap(ftf, ftf->charmaps[i]));
  return (-1);
}

template <class PixelFormat>
void agg_ft_hb_draw(agg::renderer_base<PixelFormat>& rbase)
{
#ifdef ANDROID
  std::string fontPath = "/mnt/sdcard/fonts/";
#else
  std::string fontPath = "fonts/";
#endif

  typedef typename agg::renderer_base<PixelFormat>::color_type rbase_color_type;

  constexpr int NUM_EXAMPLES = 3;

  /* tranlations courtesy of google */
  const char* texts[NUM_EXAMPLES] = {
      "Ленивый рыжий кот", "كسول الزنجبيل القط", "懶惰的姜貓",
  };

  const int text_directions[NUM_EXAMPLES] = {
      HB_DIRECTION_LTR, HB_DIRECTION_RTL, HB_DIRECTION_TTB,
  };

  /* XXX: These are not correct, though it doesn't seem to break anything
   *      regardless of their value. */
  const char* languages[NUM_EXAMPLES] = {
      "en", "ar", "ch",
  };

  const hb_script_t scripts[NUM_EXAMPLES] = {
      HB_SCRIPT_LATIN, HB_SCRIPT_ARABIC, HB_SCRIPT_HAN,
  };

  enum
  {
    ENGLISH = 0,
    ARABIC,
    CHINESE
  };

  int ptSize = 50 * 64;
  int device_hdpi = 72;
  int device_vdpi = 72;

  std::string enFont = fontPath + "DejaVuSerif.ttf";
  std::string arFont = fontPath + "amiri-0.104/amiri-regular.ttf";
  std::string chFont = fontPath + "fireflysung-1.3.0/fireflysung.ttf";

  /* Init freetype */
  FT_Library ft_library;
  assert(!FT_Init_FreeType(&ft_library));

  /* Load our fonts */
  FT_Face ft_face[NUM_EXAMPLES];
  assert(!FT_New_Face(ft_library, enFont.c_str(), 0, &ft_face[ENGLISH]));
  assert(
      !FT_Set_Char_Size(ft_face[ENGLISH], 0, ptSize, device_hdpi, device_vdpi));
  ftfdump(ft_face[ENGLISH]); /* wonderful world of encodings ... */
  force_ucs2_charmap(ft_face[ENGLISH]); /* which we ignore. */

  assert(!FT_New_Face(ft_library, arFont.c_str(), 0, &ft_face[ARABIC]));
  assert(
      !FT_Set_Char_Size(ft_face[ARABIC], 0, ptSize, device_hdpi, device_vdpi));
  ftfdump(ft_face[ARABIC]);
  force_ucs2_charmap(ft_face[ARABIC]);

  assert(!FT_New_Face(ft_library, chFont.c_str(), 0, &ft_face[CHINESE]));
  assert(
      !FT_Set_Char_Size(ft_face[CHINESE], 0, ptSize, device_hdpi, device_vdpi));
  ftfdump(ft_face[CHINESE]);
  force_ucs2_charmap(ft_face[CHINESE]);

  /* Get our harfbuzz font structs */
  hb_font_t* hb_ft_font[NUM_EXAMPLES];
  hb_ft_font[ENGLISH] = hb_ft_font_create(ft_face[ENGLISH], NULL);
  hb_ft_font[ARABIC] = hb_ft_font_create(ft_face[ARABIC], NULL);
  hb_ft_font[CHINESE] = hb_ft_font_create(ft_face[CHINESE], NULL);

  /* Setup our AGG window */
  int width = rbase.width();
  //int height = rbase.height();

  /* Create a buffer for harfbuzz to use */
  hb_buffer_t* buf = hb_buffer_create();

  for (int i = 0; i < NUM_EXAMPLES; ++i) {
    /* or LTR */
    hb_buffer_set_direction(buf, (hb_direction_t) text_directions[i]);
    hb_buffer_set_script(buf, scripts[i]); /* see hb-unicode.h */
    hb_buffer_set_language(
        buf, hb_language_from_string(languages[i], strlen(languages[i])));

    /* Layout the text */
    hb_buffer_add_utf8(buf, texts[i], strlen(texts[i]), 0, strlen(texts[i]));
    hb_shape(hb_ft_font[i], buf, NULL, 0);

    unsigned int glyph_count;
    hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
    hb_glyph_position_t* glyph_pos =
        hb_buffer_get_glyph_positions(buf, &glyph_count);

    /* set up rendering via spanners */
    agg_spanner_baton_t<PixelFormat> stuffbaton;

    FT_Raster_Params ftr_params;
    ftr_params.target = 0;
    ftr_params.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
    ftr_params.user = &stuffbaton;
    ftr_params.black_spans = 0;
    ftr_params.bit_set = 0;
    ftr_params.bit_test = 0;

    /* Calculate string bounding box in pixels */
    ftr_params.gray_spans = agg_spanner_sizer<PixelFormat>;

    /* See
               http://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html */

    /* largest coordinate a pixel has been set at,
               or the pen was advanced to. */
    int max_x = INT_MIN;
    /* smallest coordinate a pixel has been set at,
               or the pen was advanced to. */
    int min_x = INT_MAX;
    /* this is max topside bearing along the string. */
    int max_y = INT_MIN;
    /* this is max value of (height - topbearing) along the string. */
    int min_y = INT_MAX;
    /*  Naturally, the above comments swap their meaning between
                horizontal and vertical scripts, since the pen changes the axis
                it is advanced along. However, their differences still make up
                the bounding box for the string. Also note that all this is
                in FT coordinate system where y axis points upwards.
             */

    int sizer_x = 0;
    int sizer_y = 0; /* in FT coordinate system. */

    FT_Error fterr;
    for (unsigned j = 0; j < glyph_count; ++j) {
      if ((fterr = FT_Load_Glyph(ft_face[i], glyph_info[j].codepoint, 0))) {
        printf("load %08x failed fterr=%d.\n", glyph_info[j].codepoint, fterr);
      } else {
        if (ft_face[i]->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
          printf("glyph->format = %4s\n", (char*) &ft_face[i]->glyph->format);
        } else {
          int gx = sizer_x + (glyph_pos[j].x_offset / 64);
          /* note how the sign differs from the rendering pass */
          int gy = sizer_y + (glyph_pos[j].y_offset / 64);

          stuffbaton.min_span_x = INT_MAX;
          stuffbaton.max_span_x = INT_MIN;
          stuffbaton.min_y = INT_MAX;
          stuffbaton.max_y = INT_MIN;

          if ((fterr = FT_Outline_Render(
                   ft_library, &ft_face[i]->glyph->outline, &ftr_params)))
            printf("FT_Outline_Render() failed err=%d\n", fterr);

          if (stuffbaton.min_span_x != INT_MAX) {
            /* Update values if the spanner was actually called. */
            if (min_x > stuffbaton.min_span_x + gx)
              min_x = stuffbaton.min_span_x + gx;

            if (max_x < stuffbaton.max_span_x + gx)
              max_x = stuffbaton.max_span_x + gx;

            if (min_y > stuffbaton.min_y + gy)
              min_y = stuffbaton.min_y + gy;

            if (max_y < stuffbaton.max_y + gy)
              max_y = stuffbaton.max_y + gy;
          } else {
            /* The spanner wasn't called at all - an empty glyph,
                           like space. */
            if (min_x > gx)
              min_x = gx;
            if (max_x < gx)
              max_x = gx;
            if (min_y > gy)
              min_y = gy;
            if (max_y < gy)
              max_y = gy;
          }
        }
      }

      sizer_x += glyph_pos[j].x_advance / 64;
      /* note how the sign differs from the rendering pass */
      sizer_y += glyph_pos[j].y_advance / 64;
    }
    /* Still have to take into account last glyph's advance. Or not? */
    if (min_x > sizer_x)
      min_x = sizer_x;
    if (max_x < sizer_x)
      max_x = sizer_x;
    if (min_y > sizer_y)
      min_y = sizer_y;
    if (max_y < sizer_y)
      max_y = sizer_y;

    /* The bounding box */
    int bbox_w = max_x - min_x;
    int bbox_h = max_y - min_y;

    /* Two offsets below position the bounding box with respect
               to the 'origin', which is sort of origin of string's first glyph.

                baseline_offset - offset perpendecular to the baseline
                                  to the topmost (horizontal),
                                  or leftmost (vertical) pixel drawn.

                baseline_shift  - offset along the baseline, from the first
                                  drawn glyph's origin to the leftmost
                                  (horizontal), or topmost (vertical) pixel drawn.

                Thus those offsets allow positioning the bounding box to fit
                the rendered string, as they are in fact offsets from the point
                given to the renderer, to the top left corner of the bounding box.

                NB: baseline is defined as y==0 for horizontal and x==0 for
                vertical scripts.
                (0,0) here is where the first glyph's origin ended up after
                shaping, not taking into account glyph_pos[0].xy_offset
                (yeah, my head hurts too).
            */

    int baseline_offset;
    int baseline_shift;

    if (HB_DIRECTION_IS_HORIZONTAL(hb_buffer_get_direction(buf))) {
      baseline_offset = max_y;
      baseline_shift = min_x;
    }
    if (HB_DIRECTION_IS_VERTICAL(hb_buffer_get_direction(buf))) {
      baseline_offset = min_x;
      baseline_shift = max_y;
    }

    printf(
        "ex %d string min_x=%d max_x=%d min_y=%d max_y=%d bbox %dx%d boffs "
        "%d,%d\n",
        i, min_x, max_x, min_y, max_y, bbox_w, bbox_h, baseline_offset,
        baseline_shift);

    /* The pen/baseline start coordinates in window coordinate system
                - with those text placement in the window is controlled.
                - note that for RTL scripts pen still goes LTR */
    int x = 0, y = 50 + i * 75;
    /* left justify */
    if (i == ENGLISH) {
      x = 20;
    }
    /* right justify */
    if (i == ARABIC) {
      x = width - bbox_w - 20;
    }
    /* center, and for TTB script h_advance is half-width. */
    if (i == CHINESE) {
      x = width / 2 - bbox_w / 2;
    }

    /* Draw baseline and the bounding box */
    /* The below is complicated since we simultaneously
               convert to the window coordinate system. */
    int left, right, top, bottom;
    rbase_color_type color;
    color.opacity(1.0);

    if (HB_DIRECTION_IS_HORIZONTAL(hb_buffer_get_direction(buf))) {
      /* bounding box in window coordinates without offsets */
      left = x;
      right = x + bbox_w;
      top = y - bbox_h;
      bottom = y;

      /* apply offsets */
      left += baseline_shift;
      right += baseline_shift;
      top -= baseline_offset - bbox_h;
      bottom -= baseline_offset - bbox_h;

      /* draw the baseline */
      color.r = 0;
      color.g = 255;
      color.b = 0;
      rbase.copy_hline(x, y, x + bbox_w, color);
    }

    if (HB_DIRECTION_IS_VERTICAL(hb_buffer_get_direction(buf))) {
      left = x;
      right = x + bbox_w;
      top = y;
      bottom = y + bbox_h;

      left += baseline_offset;
      right += baseline_offset;
      top -= baseline_shift;
      bottom -= baseline_shift;

      color.r = 0;
      color.g = 255;
      color.b = 0;
      rbase.copy_vline(x, y, y + bbox_h, color);
    }
    printf("ex %d origin %d,%d bbox l=%d r=%d t=%d b=%d\n", i, x, y, left,
        right, top, bottom);

    /* +1/-1 are for the bbox borders be the next pixel
               outside the bbox itself */
    color.r = 255;
    color.g = 0;
    color.b = 0;
    rbase.copy_hline(left - 1, top - 1, right + 1, color);
    rbase.copy_hline(left - 1, bottom + 1, right + 1, color);
    rbase.copy_vline(left - 1, top - 1, bottom + 1, color);
    rbase.copy_vline(right + 1, top - 1, bottom + 1, color);

    /* set rendering spanner */
    ftr_params.gray_spans = agg_spanner_blend<PixelFormat>;

    /* initialize rendering part of the baton */
    stuffbaton.rbase = &rbase;

    /* render */
    for (unsigned j = 0; j < glyph_count; ++j) {
      if ((fterr = FT_Load_Glyph(ft_face[i], glyph_info[j].codepoint, 0))) {
        printf("load %08x failed fterr=%d.\n", glyph_info[j].codepoint, fterr);
      } else {
        if (ft_face[i]->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
          printf("glyph->format = %4s\n", (char*) &ft_face[i]->glyph->format);
        } else {
          stuffbaton.glyph_x = x + (glyph_pos[j].x_offset / 64);
          stuffbaton.glyph_y = y - (glyph_pos[j].y_offset / 64);

          if ((fterr = FT_Outline_Render(
                   ft_library, &ft_face[i]->glyph->outline, &ftr_params)))
            printf("FT_Outline_Render() failed err=%d\n", fterr);
        }
      }

      x += glyph_pos[j].x_advance / 64;
      y -= glyph_pos[j].y_advance / 64;
    }

    /* clean up the buffer, but don't kill it just yet */
    hb_buffer_clear_contents(buf);
  }

  /* Cleanup */
  hb_buffer_destroy(buf);
  for (int i = 0; i < NUM_EXAMPLES; ++i)
    hb_font_destroy(hb_ft_font[i]);

  FT_Done_FreeType(ft_library);
}

#endif /* AGG_FREETYPE_HARFBUZZ_H */
