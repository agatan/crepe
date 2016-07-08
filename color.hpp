#ifndef COLOR_HPP_
#define COLOR_HPP_

#include <iostream>

namespace crepe {

enum class color : unsigned int {
  gray = 90u,
  red = 91u,
  green = 92u,
  yellow = 93u,
  blue = 94u,
  purple = 95u,
  cyan = 96u,
  none = 0u,
};

enum class brightness {
  light,
  dark,
};

#define ESCAPE "\033["

std::ostream& color(std::ostream& os, unsigned int c);

template <typename T>
std::ostream& print_with_color(std::ostream& os, enum color c, T&& t) {
  color(os, static_cast<unsigned int>(c));
  os << std::forward<T>(t);
  return color(os, static_cast<unsigned int>(color::none));
}

} // namespace crepe

#endif
