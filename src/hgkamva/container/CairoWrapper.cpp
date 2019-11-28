#include "hgkamva/container/CairoWrapper.h"

#include <stdexcept>

CairoWrapper::CairoWrapper(
    unsigned char* buffer, unsigned int width, unsigned int height, int stride)
{
  // TODO: format
  mSurface = cairo_image_surface_create_for_data(
      buffer, CAIRO_FORMAT_ARGB32 /*TODO*/, width, height, stride);
  if(cairo_surface_status(mSurface) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(mSurface);
    throw std::logic_error("cairo_surface_status() != CAIRO_STATUS_SUCCESS");
  }
  mContext = cairo_create(mSurface);
  if(cairo_status(mContext) != CAIRO_STATUS_SUCCESS) {
    cairo_destroy(mContext);
    cairo_surface_destroy(mSurface);
    throw std::logic_error("cairo_status() != CAIRO_STATUS_SUCCESS");
  }
}

CairoWrapper::~CairoWrapper()
{
  cairo_destroy(mContext);
  cairo_surface_destroy(mSurface);
}
