#ifndef PRINTER_HPP_
#define PRINTER_HPP_

#include <string>
#include <queue>
#include <memory>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "match_result.hpp"

namespace crepe {

class printer {
public:
  explicit printer(std::ostream& os) : os(os) {}

  void notify_match(std::unique_ptr<match_result>&&);

  void notify_finish();

  void run();

private:
  enum class tag {
    match,
    finish,
  };

  struct task {
    tag t;
    std::unique_ptr<match_result> result;

    task(tag t, std::unique_ptr<match_result>&& m) : t(t), result(std::move(m)) {}
    explicit task(tag t) : t(t) {}
  };

  std::ostream& os;

  std::mutex contents_mtx;
  std::condition_variable contents_cv;
  std::queue<task> contents;

  bool is_first_file = true;
};

} // namespace crepe

#endif // PRINTER_HPP_
