#ifndef OPTION_HPP_
#define OPTION_HPP_

#include <string>
#include <vector>

namespace crepe {

struct crepe_option {
  bool is_stdin_redirect;
  bool is_stdout_redirect;
  bool color;
  bool group_result;
  bool line_number;

  std::string pattern;
  std::vector<std::string> paths;
};

extern crepe_option opt;

void init_option(int argc, char* const* argv);

void usage();
void version();

} // namespace crepe

#endif
