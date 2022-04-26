#include "flag_parser/flag_parser.h"
#include <iostream>
#include <getopt.h>

using namespace std;


void print_usage() {
  cout <<
      "Usage: mem-sim [options] filename\n"
      "\n"
      "Options:\n"
      "  -v, --verbose\n"
      "      Output information about every memory access.\n"
      "\n"
      "  -s, --strategy <FIFO | LRU>\n"
      "      The replacement strategy to use. One of FIFO or LRU.\n"
      "\n"
      "  -f, --max-frames <positive integer>\n"
      "      The maximum number of frames a process may be allocated.\n"
      "\n"
      "  -i, --file-verbose\n"
      "      Print process size and virtual addresses when reading in file.\n"
      "\n"
      "  -h --help\n"
      "      Display a help message about these flags and exit\n"
      "\n";
}


bool parse_flags(int argc, char** argv, FlagOptions& flags) {
    // Command-line flags accepted by this program.
    static struct option flag_options[] = {
        {"verbose",             no_argument,       0, 'v'},
        {"csv",                 no_argument,       0, 'c'},
        {"strategy",            required_argument, 0, 's'},
        {"max-frames",          required_argument, 0, 'f'},
        {"help",                no_argument,       0, 'h'},
        {"file-verbose",        no_argument,       0, 'i'},
        {0, 0, 0, 0}
    };

    int option_index;
    char flag_char;

    // Parse flags entered by the user.
    while (true) {
        flag_char =
            getopt_long(argc, argv, "-vcs:f:w:hi", flag_options, &option_index);

        // Detect the end of the options.
        if (flag_char == -1) {
            break;
        }

        switch (flag_char) {
            case 'v':
                if (!flags.csv) {
                    flags.verbose = true;
                }
                break;

            case 'c':
                flags.csv = true;
                flags.verbose = false;
                break;

            case 's':
                if (string(optarg) == "FIFO") {
                    flags.strategy = ReplacementStrategy::FIFO;
                } else if (string(optarg) == "LRU") {
                    flags.strategy = ReplacementStrategy::LRU;
                } else {
                    return false;
                }
                break;

            case 'f':
                flags.max_frames = atoi(optarg);

                if (flags.max_frames < 1) {
                    return false;
                }

                break;

            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
                break;

            case 'i':
                flags.file_verbose = true;
                break;

            case 1:
                flags.filename = optarg;
                break;

            default:
                return false;
        }
    }

    if (flags.filename == "") {
        return false;
    }

    return true;
}
