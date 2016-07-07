#ifndef FILE_HPP_
#define FILE_HPP_

#include <cstdio>

namespace crepe {

enum class filetype {
  binary,
  utf8,
};

filetype detect_filetype(std::FILE* fp, char const* name);

} // namespace crepe

#endif
