#ifndef PRINTER_HPP_
#define PRINTER_HPP_

#include <string>
#include <queue>
#include <iostream>
#include <mutex>
#include <condition_variable>

namespace crepe {

class printer {
public:
  explicit printer(std::ostream& os) : os(os) {}

  void notify_file(char const*);

  void notify(std::string&&);
  void notify(int linum, std::string const&);
  void notify(int linum, std::string&&);

  void notify_finish();

  void run();

private:
  enum class tag {
    print,
    finish,
  };

  struct task {
    tag t;
    std::string contents;

    task(tag t, std::string&& s) : t(t), contents(std::move(s)) {}
    explicit task(tag t) : t(t) {}
  };

  std::ostream& os;

  std::mutex contents_mtx;
  std::condition_variable contents_cv;
  std::queue<task> contents;
};

} // namespace crepe

#endif // PRINTER_HPP_