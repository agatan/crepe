#include <thread>
#include <string>
#include <fstream>
#include <iostream>

#include "printer.hpp"
#include "matcher.hpp"
#include "walker.hpp"
#include "option.hpp"

int main(int argc, char **argv) {
  crepe::init_option(argc, argv);

  crepe::printer p(std::cout);
  crepe::matcher m(std::move(crepe::opt.pattern), p);
  crepe::walker w(m);

  std::thread print_thread([&] { p.run(); });
  std::thread match_thread([&] { m.run(); });
  std::thread walk_thread([&] { w.run(); });

  print_thread.join();
  match_thread.join();
  walk_thread.join();

  return 0;
}
