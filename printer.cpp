#include "printer.hpp"

#include "option.hpp"
#include "color.hpp"

namespace crepe {

void printer::notify_match(std::unique_ptr<match_result>&& m) {
  std::unique_lock<std::mutex> lock(contents_mtx);
  contents.emplace(tag::match, std::move(m));
  contents_cv.notify_one();
}

void printer::notify_finish() {
  std::unique_lock<std::mutex> lock(contents_mtx);
  contents.emplace(tag::finish);
  contents_cv.notify_one();
}

static void
print_line_number(std::ostream& os, match_type t, int n) {
  if (!opt.line_number) return;
  switch (t) {
  case match_type::exact:
    print_with_color(os, opt.line_color, n);
    break;
  case match_type::after:
    print_with_color(os, opt.line_color, '+');
    break;
  case match_type::before:
    print_with_color(os, opt.line_color, '-');
    break;
  }
  os << ":";
}

static void
print_file_name(std::ostream& os, std::string const& name) {
  if (!opt.group_result) return;
  print_with_color(os, opt.file_color, name);
  os << '\n';
}

static void
print_match(std::ostream& os, match_result const& m) {
  print_file_name(os, m.file_name);
  for (auto&& line: m.matches) {
    print_line_number(os, line.type, line.line_num);
    os << line.contents;
  }
  if (opt.group_result) os << '\n';
}

void printer::run() {
  for (;;) {
    {
      std::unique_lock<std::mutex> lock(contents_mtx);
      contents_cv.wait(lock, [this] { return !this->contents.empty(); });
      while (!contents.empty()) {
        auto&& top = contents.front();
        switch (top.t) {
        case tag::finish:
          return;
        case tag::match:
          print_match(os, *top.result);
          break;
        }
        contents.pop();
      }
    }
  }
}

} // namespace crepe
