#include "walker.hpp"

#include <stdio.h>
#include <boost/filesystem.hpp>

#include "option.hpp"
#include "file.hpp"

namespace crepe {

namespace fs = boost::filesystem;

void walker::run() {
  if (opt.paths.empty()) {
    // from stdin
    m.notify_file("", stdin);
    m.notify_finish();
    return;
  }
  for (auto&& base : opt.paths) {
    fs::path path(std::move(base));
    if (!fs::is_directory(path)) {
      process_file(path.string());
    } else {
      for (auto&& entry: fs::recursive_directory_iterator(path)) {
        if (!fs::is_directory(entry)) {
          process_file(entry.path().string());
        }
      }
    }
  }
  m.notify_finish();
}

void walker::process_file(std::string const& p) {
  FILE* fp = fopen(p.c_str(), "rb");
  if (!fp) {
    // TODO: error notification
    // skip it.
    return;
  }
  auto ft = detect_filetype(fp, p.c_str());
  if (ft == filetype::binary) {
    // Skip binary file.
    return;
  }
  m.notify_file(p, fp);
}

} // namespace crepe
