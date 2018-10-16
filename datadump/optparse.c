#include "proto.h"

int parse_options (char ** options, unsigned count) {
  int done_options = 0;
  int mode = MODE_INTERACTIVE;
  unsigned current;
  for (current = 0; current < count; current ++) {
    if (!options[current]) break;
    if (done_options || (*(options[current]) != '-')) {
      if (repository_path) {
        command_line_filenames = realloc(command_line_filenames, sizeof(char *) * (command_line_filename_count + 1));
        command_line_filenames[command_line_filename_count ++] = options[current];
      } else
        repository_path = options[current];
    } else if (!strcmp(options[current], "--"))
      done_options = 1;
    else if (!strcmp(options[current], "-a")) {
      if (mode) multiple_execution_mode_error(mode);
      mode = auto_execution_mode(get_option_argument(options, count, &current));
    } else if (!strcmp(options[current], "-c"))
      parse_configuration_line(get_option_argument(options, count, &current));
    else if (!strcmp(options[current], "-l")) {
      if (global_symbol_table) error_exit(1, "symbols already loaded");
      preload_symbols(get_option_argument(options, count, &current));
    } else if (!strcmp(options[current], "-s")) {
      if (mode) multiple_execution_mode_error(mode);
      global_script_path = get_option_argument(options, count, &current);
      mode = MODE_AUTO_SCRIPT;
    } else if (!(strcmp(options[current], "-h") && strcmp(options[current], "-?"))) {
      print_help();
      exit(1);
    } else if (!strcmp(options[current], "-v")) {
      print_version();
      exit(1);
    } else
      error_exit(1, "unknown option: %s", options[current]);
  }
  return mode;
}

const char * get_option_argument (char ** options, unsigned count, unsigned * current) {
  if ((*current + 1) >= count) error_exit(1, "option %s requires an argument", options[*current]);
  return options[++ (*current)];
}

void multiple_execution_mode_error (int previous_mode) {
  error_exit(1, "conflicting execution modes given (previous mode was %s)", ((previous_mode >= 0) && (previous_mode <= 5)) ?
                previous_mode[(const char * []) {"interactive", "-a 8", "-a 16", "-a ptr", "-a 32", "-s"}] : "unknown");
}

int auto_execution_mode (const char * mode_string) {
  // we could parse the number, but to keep it simple, just do a string compare -- nobody should be doing -a 0x20 anyway
  if (!strcmp(mode_string, "8"))
    return MODE_AUTO_DATA_8;
  else if (!strcmp(mode_string, "16"))
    return MODE_AUTO_DATA_16;
  else if (!strcmp(mode_string, "32"))
    return MODE_AUTO_DATA_32;
  else if (!strcmp(mode_string, "ptr"))
    return MODE_AUTO_DATA_PTR;
  error_exit(1, "unknown execution mode: -a %s", mode_string);
}

void print_help (void) {
  print_version();
  fputs("ddump is released to the public domain. No copyright is claimed, and no\n"
        "warranty, implied or otherwise, is given with the program.\n"
        "\n"
        "Usage: ddump [-a <mode> | -s <script>] [-c <config>] [-l <symfile>]\n"
        "             [--] <repository path> <file> [<file> [<file> ...]]\n"
        " -a <mode>:    runs the program in automatic dumping mode, as if an equivalent\n"
        "               'data' command had been used in interactive mode. Valid values\n"
        "               for <mode> are 8, 16, 32, and ptr.\n"
        " -c <config>:  sets a setting prior to running the program. <config> must be a\n"
        "               comma-separated list of name=value pairs; valid settings and\n"
        "               values are the same as in interactive mode.\n"
        " -l <symfile>: loads a symfile prior to dumping, as if the 'loadsym' command had\n"
        "               been used in interactive mode.\n"
        " -s <script>:  runs the program in automatic scripting mode, applying the given\n"
        "               script to every block referenced by an .incbin directive.\n"
        "Note that -- may be used to terminate the option list and parse all remaining\n"
        "command-line arguments as filenames. Also, -h or -? print this help, and -v\n"
        "will print version information.\n"
        "In interactive mode, only one file may be given. If two filenames are given, the\n"
        "second will be taken as the output file (instead of working in place); passing\n"
        "three or more in interactive mode (i.e., not -a or -s) is an error.\n", stderr);
}
