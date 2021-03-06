#ifndef CAIROWRAPPER_HPP
#define CAIROWRAPPER_HPP

#include <memory>
#include <utility>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cairo/cairo.h>

#include "litehtml.h"

namespace hg
{
class HgCairo;

using HgCairoPtr = std::shared_ptr<HgCairo>;

class HgCairo
{
public:
  using SurfacePtr = std::shared_ptr<cairo_surface_t>;
  using ContextPtr = std::shared_ptr<cairo_t>;
  using FontFacePtr = std::shared_ptr<cairo_font_face_t>;
  using ScaledFontPtr = std::shared_ptr<cairo_scaled_font_t>;
  using FontOptionsPtr = std::shared_ptr<cairo_font_options_t>;
  using FontExtentsPtr = std::shared_ptr<cairo_font_extents_t>;
  using TextExtentsPtr = std::shared_ptr<cairo_text_extents_t>;
  using GlyphVector = std::vector<cairo_glyph_t>;
  using GlyphVectorPtr = std::shared_ptr<GlyphVector>;

  struct Color
  {
    explicit Color()
        : mRed{0.0}
        , mGreen{0.0}
        , mBlue{0.0}
        , mAlpha{1.0}
    {
    }

    explicit Color(const double red,
        const double green,
        const double blue,
        const double alpha)
        : mRed{red}
        , mGreen{green}
        , mBlue{blue}
        , mAlpha{alpha}
    {
    }

    explicit Color(const litehtml::web_color& webColor)
        : mRed{webColor.red / 255.0}
        , mGreen{webColor.green / 255.0}
        , mBlue{webColor.blue / 255.0}
        , mAlpha{webColor.alpha / 255.0}
    {
    }

    double mRed;
    double mGreen;
    double mBlue;
    double mAlpha;
  };

  HgCairo() = delete;

  // TODO: Copy/move constructors/operators.
  explicit HgCairo(unsigned char* buffer,
      const cairo_format_t colorFormat,
      const unsigned int width,
      const unsigned int height,
      const int stride);
  ~HgCairo();

  void save();
  void restore();
  void clip(
      const double x, const double y, const double width, const double height);

  void clear(const Color& color = Color{});
  void drawLine(const double x1,
      const double y1,
      const double x2,
      const double y2,
      const double width,
      const Color& color = Color{});
  void showGlyphs(const GlyphVector& glyphs,
      const ScaledFontPtr scaledFont,
      const double x,
      const double y,
      const cairo_text_extents_t& extents,
      const Color& color);

  static int formatBitsPerPixel(cairo_format_t format);
  static ScaledFontPtr getScaledFont(
      const FT_Face ftFace, const int ftLoadFlags, const int pixelSize);
  static double xHeight(const ScaledFontPtr scaledFont);

  void rasterCopy(int diffX, int diffY);

private:
  template <typename StatusFunc, typename... Args>
  static void checkStatus(const StatusFunc statusFunc, Args&&... args);

  template <typename Ptr>
  static void checkPtrStatus(const Ptr ptr);

  ContextPtr mContext;
};

// static
template <typename StatusFunc, typename... Args>
inline void HgCairo::checkStatus(const StatusFunc statusFunc, Args&&... args)
{
  cairo_status_t status = statusFunc(std::forward<Args>(args)...);
  if(status != CAIRO_STATUS_SUCCESS) {
    std::string msg = "Cairo's status: ";
    msg += cairo_status_to_string(status);
    throw std::logic_error(msg);
  }
}

// static
template <typename Ptr>
inline void HgCairo::checkPtrStatus(const Ptr ptr)
{
  if(!ptr) {
    std::string msg = "Cairo's status: ";
    msg += cairo_status_to_string(CAIRO_STATUS_NULL_POINTER);
    throw std::logic_error(msg);
  }
}

}  // namespace hg

#endif  // CAIROWRAPPER_HPP
