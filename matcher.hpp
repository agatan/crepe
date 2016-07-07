#ifndef MATCHER_HPP_
#define MATCHER_HPP_

#include <string>

#include "printer.hpp"

namespace crepe {

class matcher {
public:
  explicit matcher(std::string&& needle, printer& p)
     : needle(std::move(needle)), p(p) {}

  void run(std::string const& filename);

private:
  std::string needle;
  printer& p;
};

} // namespace crepe

#endif // MATCHER_HPP_
