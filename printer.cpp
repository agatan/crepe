#include "printer.hpp"

namespace crepe {

void printer::notify_file(std::string&& name) {
  std::unique_lock<std::mutex> lock(contents_mtx);
  contents.emplace(tag::filename, std::move(name));
  contents_cv.notify_one();
}

void printer::notify(std::string&& s) {
  std::unique_lock<std::mutex> lock(contents_mtx);
  contents.emplace(tag::match, std::move(s));
  contents_cv.notify_one();
}

void printer::notify(int linum, std::string const& s) {
  notify(linum, std::string{s});
}

void printer::notify(int linum, std::string&& s) {
  std::unique_lock<std::mutex> lock(contents_mtx);
  contents.emplace(tag::match, std::to_string(linum) + ": " + std::move(s));
  contents_cv.notify_one();
}

void printer::notify_finish() {
  std::unique_lock<std::mutex> lock(contents_mtx);
  contents.emplace(tag::finish);
  contents_cv.notify_one();
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
          os << top.contents << '\n';
          break;
        case tag::filename:
          if (!is_first_file) {
            os << '\n';
          } else {
            is_first_file = false;
          }
          os << top.contents << '\n';
        }
        contents.pop();
      }
    }
  }
}

} // namespace crepe
