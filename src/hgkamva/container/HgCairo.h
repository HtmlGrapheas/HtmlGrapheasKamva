#ifndef CAIROWRAPPER_HPP
#define CAIROWRAPPER_HPP

#include <memory>
#include <utility>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cairo/cairo.h>

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
  using FontExtentsPtr = std::shared_ptr<cairo_font_extents_t>;
  using TextExtentsPtr = std::shared_ptr<cairo_text_extents_t>;
  using GlyphVector = std::vector<cairo_glyph_t>;
  using GlyphVectorPtr = std::shared_ptr<GlyphVector>;

  struct Color
  {
    explicit Color()
        : mRed{0}
        , mGreen{0}
        , mBlue{0}
        , mAlpha{1}
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

    double mRed;
    double mGreen;
    double mBlue;
    double mAlpha;
  };

  HgCairo() = delete;

  // TODO: Copy/move constructors/operators.
  explicit HgCairo(unsigned char* buffer,
      cairo_format_t colorFormat,
      unsigned int width,
      unsigned int height,
      int stride);
  ~HgCairo();

  //  cairo_surface_t* getSurface() { return mSurface; }
  //  cairo_t* getContext() { return mContext; };

  void clear(const Color& color = Color{});

  static FontFacePtr getFontFace(FT_Face ftFace, int ftLoadFlags);
  ScaledFontPtr getScaledFont(const FontFacePtr fontFace, const int pixelSize);
  static cairo_font_extents_t getScaledFontExtents(ScaledFontPtr scaledFont);
  void showGlyphs(const GlyphVector& glyphs,
      const ScaledFontPtr scaledFont,
      const double x,
      const double y,
      const cairo_text_extents_t& extents,
      const Color& color);
  double xHeight(const ScaledFontPtr scaledFont);

private:
  template <typename StatusFunc, typename... Args>
  static void checkStatus(StatusFunc statusFunc, Args&&... args);

  template <typename Ptr>
  static void checkPtrStatus(Ptr ptr);

  SurfacePtr mSurface;
  ContextPtr mContext;
};

// static
template <typename StatusFunc, typename... Args>
inline void HgCairo::checkStatus(StatusFunc statusFunc, Args&&... args)
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
inline void HgCairo::checkPtrStatus(Ptr ptr)
{
  if(!ptr) {
    std::string msg = "Cairo's status: ";
    msg += cairo_status_to_string(CAIRO_STATUS_NULL_POINTER);
    throw std::logic_error(msg);
  }
}

}  // namespace hg

#endif  // CAIROWRAPPER_HPP
