#include "hgkamva/container/CairoHolder.h"

#include <stdexcept>

CairoHolder::CairoHolder(unsigned char* buffer,
    cairo_format_t colorFormat,
    unsigned int width,
    unsigned int height,
    int stride)
{
  mSurface = cairo_image_surface_create_for_data(
      buffer, colorFormat, width, height, stride);

  if(cairo_surface_status(mSurface) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(mSurface);
    throw std::logic_error("cairo_surface_status() != CAIRO_STATUS_SUCCESS: "
        + std::to_string(cairo_surface_status(mSurface)));
  }

  mContext = cairo_create(mSurface);

  if(cairo_status(mContext) != CAIRO_STATUS_SUCCESS) {
    cairo_destroy(mContext);
    cairo_surface_destroy(mSurface);
    throw std::logic_error("cairo_status() != CAIRO_STATUS_SUCCESS");
  }
}

CairoHolder::~CairoHolder()
{
  cairo_destroy(mContext);
  cairo_surface_destroy(mSurface);
}
