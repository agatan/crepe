#include "walker.hpp"

#include <boost/filesystem.hpp>

namespace crepe {

namespace fs = boost::filesystem;

void walker::run(char const* base) {
  fs::path path(base);
  if (!fs::is_directory(path)) {
    m.notify_file(path.string());
  } else {
    for (auto&& entry: fs::recursive_directory_iterator(path)) {
      if (!fs::is_directory(entry)) {
        m.notify_file(entry.path().string());
      }
    }
  }
  m.notify_finish();
}

} // namespace crepe