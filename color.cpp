#include "color.hpp"

#include "option.hpp"

namespace crepe {

std::ostream& color(std::ostream& os, unsigned int c) {
  if (opt.color)
    return os << ESCAPE << c << 'm';
  else
    return os;
}

} // namespace crepe
