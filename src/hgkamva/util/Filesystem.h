#ifndef HG_FILESYSTEM_H
#define HG_FILESYSTEM_H

#ifdef USE_BOOST
#include <boost/filesystem.hpp>
#else  // #ifndef USE_BOOST
#include <filesystem>
#endif  // #ifdef USE_BOOST

namespace hg
{
#ifdef USE_BOOST
namespace filesystem = boost::filesystem;
#else  // #ifndef USE_BOOST
namespace filesystem = std::filesystem;
#endif  // #ifdef USE_BOOST
}  // namespace hg

#endif  // HG_FILESYSTEM_H
