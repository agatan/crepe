#ifndef MATCHER_HPP_
#define MATCHER_HPP_

#include <string>
#include <mutex>
#include <condition_variable>

#include "printer.hpp"

namespace crepe {

class matcher {
public:
  explicit matcher(std::string&& needle, printer& p)
     : needle(std::move(needle)), p(p) {}

  void notify_file(std::string const&);
  void notify_finish();

  void run();

private:
  enum class tag {
    file,
    finish,
  };

  struct task {
    tag t;
    std::string filename;

    task(tag t, std::string&& n) : t(t), filename(std::move(n)) {}
    task(tag t, std::string const& n) : t(t), filename(n) {}
    explicit task(tag t) : t(t) {}
  };

  std::string needle;
  printer& p;

  std::mutex file_queue_mtx;
  std::condition_variable file_queue_cv;
  std::queue<task> file_queue;

  void process_file(std::string&& filename);
};

} // namespace crepe

#endif // MATCHER_HPP_
