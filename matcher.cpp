#include "matcher.hpp"

#include <fstream>
#include <iostream>

namespace crepe {

void matcher::run(std::string const& filename) {
  std::fstream fin;
  fin.open(filename);
  if (!fin) {
    // TODO: throw error
    std::cerr << "unable to open file: " << filename << std::endl;
    std::exit(1);
  }
  std::string line;
  int linum = 1;
  while (std::getline(fin, line)) {
    if (line.find(needle) != std::string::npos) {
      p.notify(linum, line);
    }
    linum++;
  }
  p.notify_finish();
}

} // namespace crepe
