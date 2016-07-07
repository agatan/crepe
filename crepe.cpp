#include <thread>
#include <string>
#include <fstream>
#include <iostream>

#include "printer.hpp"
#include "matcher.hpp"
#include "walker.hpp"

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
  crepe::walker w(m);

  std::thread print_thread([&] { p.run(); });
  std::thread match_thread([&] { m.run(); });
  std::thread walk_thread([&] { w.run(filename); });

  print_thread.join();
  match_thread.join();
  walk_thread.join();

  return 0;
}
