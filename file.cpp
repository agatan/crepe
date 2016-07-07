#include "file.hpp"

#include <cstring>

namespace crepe {

static constexpr std::size_t buf_size_for_ft_detect = 512;

filetype detect_filetype(std::FILE* fp, char const* name) {
  char buf[buf_size_for_ft_detect];
  auto sz = std::fread(buf, sizeof(char), buf_size_for_ft_detect, fp);
  if (sz <= 0) {
    return filetype::binary;
  }
  if (std::fseek(fp, 0, SEEK_SET) != 0) {
    // TODO: error handling
    return filetype::binary;
  }

  unsigned char const* bufc = (unsigned char const*) buf;

  if (sz >= 3 && bufc[0] == 0xEF && bufc[1] == 0xBB && bufc[2] == 0xBF) {
    // UTF-8 BOM
    return filetype::utf8;
  }
  if (sz >= 5 && std::strncmp(buf, "%PDF-", 5) == 0) {
    // pdf file
    return filetype::binary;
  }

  int invalid = 0, utf8 = 0;
  for (int i = 0; i < sz; i++) {
    unsigned char c = bufc[i];
    if (c == 0x00) {
      // NULL character may not be in text file.
      return filetype::binary;
    }
    if (c == 0x09 || c == 0x0A || c == 0x0D || (0x20 <= c && c < 0x7E)) {
      // ascii character. skip it.
      continue;
    }
    if (i + 1 >= sz) {
      // last character won't be multibyte character.
      break;
    }

    unsigned char c2 = bufc[++i];
    if (0xC2 <= c && c <= 0xDF && 0x80 <= c2 && c2 <= 0xBF) {
      // UTF-8 2-byte character.
      utf8++;
      continue;
    }

    if (i + 1 >= sz) {
      break;
    }
    unsigned char c3 = bufc[++i];
    if (c == 0xE0 && 0xA0 <= c2 && c2 <= 0xBF && 0x80 <= c3 && c3 <= 0xBF) {
      utf8++;
      continue;
    }
    if (((0xE1 <= c && c <= 0xEC) || c == 0xEE || c == 0xEF) &&
        (0x80 <= c2 && c2 <= 0xBF) && (0x80 <= c3 && c3 <= 0xBF)) {
      utf8++;
      continue;
    }
    if (c == 0xED && 0x80 <= c2 && c2 <= 0x9F && 0x80 <= c3 && c3 <= 0xBF) {
      utf8++;
      continue;
    }

    invalid++;
  }
  if (invalid * 100 / sz > 10) {
    return filetype::binary;
  }
  return filetype::utf8;
}

} // namespace crepe
