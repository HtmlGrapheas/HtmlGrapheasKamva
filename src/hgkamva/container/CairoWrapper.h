#ifndef CAIROWRAPPER_HPP
#define CAIROWRAPPER_HPP

#include <cairo.h>

class CairoWrapper
{
public:
  explicit CairoWrapper(unsigned char* buffer,
      unsigned int width,
      unsigned int height,
      int stride);
  ~CairoWrapper();

  cairo_surface_t* getSurface() { return mSurface; }
  cairo_t* getContext() { return mContext; };

private:
  cairo_surface_t* mSurface;
  cairo_t* mContext;
};

#endif  // CAIROWRAPPER_HPP
