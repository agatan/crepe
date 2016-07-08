#ifndef OPTION_HPP_
#define OPTION_HPP_

#include <string>
#include <vector>

#include "color.hpp"

namespace crepe {

struct crepe_option {
  bool is_stdin_redirect;
  bool is_stdout_redirect;
  bool color;
  bool group_result;
  bool line_number;

  enum color file_color;
  enum color line_color;

  std::string pattern;
  std::vector<std::string> paths;
};

extern crepe_option opt;

void init_option(int argc, char* const* argv);

void usage();
void version();

} // namespace crepe

#endif
