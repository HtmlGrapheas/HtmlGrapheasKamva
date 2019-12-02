#ifndef CAIROWRAPPER_HPP
#define CAIROWRAPPER_HPP

#include <cairo/cairo.h>

class CairoHolder
{
public:
  CairoHolder() = delete;

  explicit CairoHolder(unsigned char* buffer,
      cairo_format_t colorFormat,
      unsigned int width,
      unsigned int height,
      int stride);
  ~CairoHolder();

  cairo_surface_t* getSurface() { return mSurface; }
  cairo_t* getContext() { return mContext; };

private:
  cairo_surface_t* mSurface;
  cairo_t* mContext;
};

#endif  // CAIROWRAPPER_HPP
