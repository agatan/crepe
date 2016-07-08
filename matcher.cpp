#include "matcher.hpp"
#include "file.hpp"

#include <fstream>
#include <memory>
#include <stdio.h>

#include "match_result.hpp"

namespace crepe {

void matcher::notify_file(std::string const& name, FILE* fp) {
  std::unique_lock<std::mutex> lock(file_queue_mtx);
  file_queue.emplace(tag::file, name, fp);
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
          process_file(std::move(top.filename), top.fp);
        }
        file_queue.pop();
      }
    }
  }
}

void matcher::process_file(std::string&& filename, FILE* fp) {
  if (!fp) {
    // TODO: raise or notify error.
    return;
  }
  int linum = 1;
  char* line = NULL;
  size_t len;
  auto result = std::make_unique<match_result>(std::move(filename));
  while (getline(&line, &len, fp) != -1) {
    std::string l(line);
    if (l.find(needle) != std::string::npos) {
      result->add_match(match_type::exact, linum, std::move(l));
    }
    linum++;
  }
  if (line) free(line);
  fclose(fp); // TODO: error handling
  if (!result->matches.empty()) p.notify_match(std::move(result));
}

} // namespace crepe
