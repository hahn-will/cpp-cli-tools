#ifndef COMMAND_LINE_HPP__
#define COMMAND_LINE_HPP__

#include <vector>
#include <string>
#include <map>

typedef struct argument {
  char short_flag;
  bool requires_extra_arg;
  bool is_set;
  std::string long_flag;
  std::string help_section;
  std::string set_value;
} argument;

class CommandLine {
  public:
    CommandLine(int, char **);
    void AddSingleUseArg(argument);
    void SetUsage(std::string);
    bool ParseArguments();
    std::string Errors();
    std::vector<std::string> GetDefaultTypeArgs();
    std::vector<argument> GetRemainingArguments();
  private:
    std::string errors;
    std::string usage_str;
    std::string executable_name;
    std::vector<std::string> default_arg_types;
    std::vector<std::string> arguments;
    std::vector<argument> other_args;
    std::map<std::string, unsigned> flag_map;

    void PrintHelpSection();
};

#endif
