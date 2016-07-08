#include "printer.hpp"

#include "option.hpp"

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
    os << n << ":";
    break;
  case match_type::after:
    os << "+:";
    break;
  case match_type::before:
    os << "-:";
    break;
  }
}

static void
print_match(std::ostream& os, match_result const& m) {
  os << m.file_name << '\n';
  for (auto&& line: m.matches) {
    print_line_number(os, line.type, line.line_num);
    os << line.contents;
  }
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
          os << '\n';
          break;
        }
        contents.pop();
      }
    }
  }
}

} // namespace crepe
