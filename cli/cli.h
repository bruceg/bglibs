#ifndef CLI__H__
#define CLI__H__

enum cli_option_type {
  CLI_FLAG,
  CLI_COUNTER,
  CLI_INTEGER,
  CLI_UINTEGER,
  CLI_STRING,
  CLI_STRINGLIST,
};
typedef enum cli_option_type cli_option_type;

struct cli_option
{
  char ch;
  const char* name;
  cli_option_type type;
  int flag_value;
  void* dataptr;
  const char* helpstr;
  const char* defaultstr;
};
typedef struct cli_option cli_option;

struct cli_stringlist
{
  const char* string;
  const struct cli_option* option;
  struct cli_stringlist* next;
};
typedef struct cli_stringlist cli_stringlist;

/* The following are required from the CLI program */
extern const char cli_help_prefix[];
extern const char cli_help_suffix[];
extern const char cli_args_usage[];
extern const int cli_args_min;
extern const int cli_args_max;
extern cli_option cli_options[];
extern int cli_main(int argc, char** argv);

/* The following are provided to the CLI program */
extern const char* argv0;
extern const char* argv0base;
extern const char* argv0dir;
extern void usage(int exit_value, const char* errorstr);

#endif
