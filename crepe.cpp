#include <thread>
#include <string>
#include <fstream>
#include <iostream>

#include "printer.hpp"
#include "matcher.hpp"

struct match {
  std::string contents;
  int linum;
};

std::ostream& operator<<(std::ostream& os, match const& m) {
  return os << m.linum << ": " << m.contents;
}

bool make_match(int linum, std::string const& line, std::string const& needle, match& m) {
  if (line.find(needle) == std::string::npos) {
    return false;
  }
  m.contents = line;
  m.linum = linum;
  return true;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    return 1;
  }
  std::string needle = argv[1];
  auto filename = argv[2];

  crepe::printer p(std::cout);
  crepe::matcher m(std::move(needle), p);

  std::thread t([&]{ p.run(); });
  m.run(filename);

  t.join();

  return 0;
}
