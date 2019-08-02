#include "CommandLine.hpp"

#include <stdexcept>
#include <utility>
#include <iostream>
#include <iomanip>

#define CHAR_TO_STRING(a)\
  std::string(1, (a))

typedef std::pair<std::string, unsigned> arg_type;

CommandLine::CommandLine(int argc, char **argv) {
  if (argc < 1)
    std::runtime_error("ERROR: has no arguments");

  executable_name = argv[0];

  for (int i = 1; i < argc; i++)
    arguments.push_back(argv[i]);

  usage_str = "";
}

void CommandLine::AddSingleUseArgs(const argument *args, const int num_args) {
  for (int i = 0; i < num_args; i++) {
    AddSingleUseArg(args[i]);
  }
}

void CommandLine::AddSingleUseArg(argument arg) {
  if (arg.short_flag != 0)
    flag_map.insert(arg_type(CHAR_TO_STRING(arg.short_flag), other_args.size()));

  flag_map.insert(arg_type(arg.long_flag, other_args.size()));
  other_args.push_back(arg);
}

bool flag_in_map(std::map<std::string, unsigned> map, std::string flag) {
  return map.find(flag) != map.end();
}

void CommandLine::PrintHelpSection() {
  std::cout << executable_name << " [options] " << usage_str << std::endl;
  for (const argument &a : other_args) {
    std::cout << "    ";
    if (a.short_flag)
      std::cout << "-" << a.short_flag << " ";
    else
      std::cout << "   ";

        
    std::cout << std::left << "--";
    std::cout.width(13);

    std::cout << std::left << a.long_flag;

    std::cout << a.help_section << std::endl;
  }
}

void CommandLine::SetUsage(std::string usage_string) {
  usage_str = usage_string;
}

bool CommandLine::ParseArguments() {
  for (const std::string &s : arguments) {
    if (s == "--help" || s == "-h") {
      PrintHelpSection();
    }
  }
  for (unsigned i = 0; i < arguments.size(); i++) {
    if (arguments[i].size() > 0 && arguments[i].at(0) == '-') {
      if (arguments[i].size() > 0 && arguments[i].at(1) == '-') {
        if (flag_in_map(flag_map, arguments[i].substr(2))) {
          argument a = other_args[flag_map[arguments[i].substr(2)]];

          if (a.is_set) {
            errors += "Argument: " + arguments[i] + " has already been set\n";
            return false;
          }
          if (a.requires_extra_arg) {
            if (i + 1 < arguments.size()) {
              other_args[flag_map[arguments[i].substr(2)]].is_set = true;
              other_args[flag_map[arguments[i].substr(2)]].set_value =
                arguments[i + 1];
              i++;
            }
            else {
              errors += "Flag: " + arguments[i] + " has already been set\n";
              return false;
            }
          }
          else {
            other_args[flag_map[arguments[i].substr(2)]].is_set = true;
          }
        }
        else {
          errors += "Flag: " + arguments[i] + " doesn't exist in this program\n";
        }
      }
      else {
        if (flag_in_map(flag_map, arguments[i].substr(1))) {
          argument a = other_args[flag_map[arguments[i].substr(1)]];
          if (a.is_set) {
            errors += "Argument: " + arguments[i] + " has already been set\n";
            return false;
          }
          if (a.requires_extra_arg) {
            if (i + 1 < arguments.size()) {
              other_args[flag_map[arguments[i].substr(1)]].is_set = true;
              other_args[flag_map[arguments[i].substr(1)]].set_value =
                arguments[i + 1];
              i++;
            }
            else {
              errors += "Flag: " + arguments[i] + " has already been set\n";
              return false;
            }
          }
          else {
            other_args[flag_map[arguments[i].substr(1)]].is_set = true;
          }
        }
        else {
          errors += "Flag: " + arguments[i] + " doesn't exist in this program\n";
        }
      }
    }
    else {
      default_arg_types.push_back(arguments[i]);
    }
  }

  return true;
}

std::string CommandLine::Errors() {
  return errors;
}

std::vector<std::string> CommandLine::GetDefaultTypeArgs() {
  return default_arg_types;
}

std::vector<argument> CommandLine::GetRemainingArguments() {
  return other_args;
}
