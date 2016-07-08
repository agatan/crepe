#include "option.hpp"

#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <boost/program_options.hpp>

namespace crepe {

namespace popt = boost::program_options;

crepe_option opt;

static bool is_stdin_redirect() {
  return !isatty(STDIN_FILENO);
}

static bool is_stdout_redirect() {
  return !isatty(STDOUT_FILENO);
}

static void set_defaults() {
  opt.is_stdin_redirect = is_stdin_redirect();
  opt.is_stdout_redirect = is_stdout_redirect();
  opt.color = !opt.is_stdout_redirect;
  opt.group_result = !opt.is_stdout_redirect && !opt.is_stdin_redirect;
  opt.line_number = !opt.is_stdin_redirect;
}

void init_option(int argc, char* const* argv) {
  set_defaults();

  static int flag;
  static struct option longopts[] = {
    { "version", no_argument, NULL, 'v' },
    { "help", no_argument, NULL, 'h' },
    { "no-line-number", no_argument, &flag, 1 },
    { "no-group", no_argument, &flag, 2 },
    { "no-color", no_argument, &flag, 3 }
  };

  int ch;
  while ((ch = getopt_long(argc, argv, "hv", longopts, NULL)) != -1) {
    switch (ch) {
      case 0:
        // long only options
        switch (flag) {
        case 1:
          opt.line_number = false;
          break;
        case 2:
          opt.group_result = false;
          break;
        case 3:
          opt.color = false;
          break;
        }
        break;
      case 'h':
        usage();
        std::exit(0);
      case 'v':
        version();
        std::exit(0);
      case '?':
      default:
        usage();
        std::exit(1);
    }
  }

  if (argc == optind) {
    usage();
    std::exit(1);
  }

  opt.pattern = argv[optind++];

  while (optind < argc) {
    opt.paths.emplace_back(argv[optind++]);
  }
}

void usage() {
  puts("Usage: crepe");
  puts("    -h, --help:            show this message");
  puts("    -v, --version:         print crepe version");
  puts("    --no-line-number:      disable line number print");
  puts("    --no-group:            disable group output");
  puts("    --no-color:            disable colorful output");
}

void version() {
  puts("crete: version 0.0.1");
}

} // namespace crepe
