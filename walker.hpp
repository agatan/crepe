#ifndef WALKER_HPP_
#define WALKER_HPP_

#include "matcher.hpp"

namespace crepe {

class walker {
public:
  explicit walker(matcher& m): m(m) {}

  void run();

private:
  matcher& m;

  void process_file(std::string const&);
};

} // namespace crepe

#endif // WALKER_HPP_
