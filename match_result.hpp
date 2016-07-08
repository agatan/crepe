#ifndef MATCH_HPP_
#define MATCH_HPP_

#include <string>

namespace crepe {

enum class match_type {
  exact,
  after,
  before,
};

struct match_line {
  match_type type;
  int line_num;
  std::string contents;

  match_line(match_type t, int line_num, std::string&& c)
    : type(t), line_num(line_num), contents(std::move(c)) {}
};

struct match_result {
  std::string file_name;
  std::vector<match_line> matches;

  explicit match_result(std::string&& fname) : file_name(std::move(fname)) {}

  void add_match(match_line&& l) {
    matches.emplace_back(std::move(l));
  }

  void add_match(match_type t, int linum, std::string&& c) {
    matches.emplace_back(t, linum, std::move(c));
  }
};

} // namespace crepe

#endif
