#ifndef WALKER_HPP_
#define WALKER_HPP_

#include "matcher.hpp"

namespace crepe {

class walker {
public:
  explicit walker(matcher& m): m(m) {}

  void run(char const* base);

private:
  matcher& m;
};

} // namespace crepe

#endif // WALKER_HPP_
