#include "matcher.hpp"
#include "file.hpp"

#include <fstream>
#include <stdio.h>

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
  FILE* fp = fopen(filename.c_str(), "rb");
  if (!fp) {
    // TODO: raise or notify error.
    return;
  }
  auto ft = detect_filetype(fp, filename.c_str());
  if (ft == filetype::binary) {
    // Skip binary file.
    return;
  }
  p.notify_file(std::move(filename));
  int linum = 1;
  char* line = NULL;
  size_t len;
  while (getline(&line, &len, fp) != -1) {
    std::string l(line);
    if (l.find(needle) != std::string::npos) {
      p.notify(linum, std::move(l));
    }
    linum++;
  }
}

} // namespace crepe
