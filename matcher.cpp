#include "matcher.hpp"

#include <fstream>
#include <iostream>

namespace crepe {

void matcher::notify_file(std::string const& name) {
  std::unique_lock<std::mutex> lock(file_queue_mtx);
  file_queue.emplace(tag::file, name);
  file_queue_cv.notify_one();
}

void matcher::notify_finish() {
  std::unique_lock<std::mutex> lock(file_queue_mtx);
  file_queue.emplace(tag::finish);
  file_queue_cv.notify_one();
}

void matcher::run() {
  for (;;) {
    {
      std::unique_lock<std::mutex> lock(file_queue_mtx);
      file_queue_cv.wait(lock, [this] { return !this->file_queue.empty(); });
      while (!file_queue.empty()) {
        auto&& top = file_queue.front();
        switch (top.t) {
        case tag::finish:
          p.notify_finish();
          return;
        case tag::file:
          process_file(std::move(top.filename));
        }
        file_queue.pop();
      }
    }
  }
}

void matcher::process_file(std::string&& filename) {
  std::fstream fin;
  fin.open(filename);
  if (!fin) {
    // TODO: throw error
    std::cerr << "unable to open file: " << filename << std::endl;
    std::exit(1);
  }
  p.notify_file(std::move(filename));
  std::string line;
  int linum = 1;
  while (std::getline(fin, line)) {
    if (line.find(needle) != std::string::npos) {
      p.notify(linum, line);
    }
    linum++;
  }
}

} // namespace crepe
