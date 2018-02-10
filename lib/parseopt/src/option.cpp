#include "../include/option.h"

std::vector<std::string> optparams;
int optindex = 1;

option::option() {
  this->setShortOption(EUNKNOWN);
  this->setParametersCount(0);
  this->setLongOption("");
}

option::option(char short_option, int parameters_count, std::string long_option) {
  this->setShortOption(short_option);
  this->setParametersCount(parameters_count);
  this->setLongOption(long_option);
}

char option::getShortOption() const {
  return this->short_option;
}

int option::getParametersCount() const {
  return this->parameters_count;
}

std::string option::getLongOption() const {
  return this->long_option;
}

void option::setShortOption(char short_option) {
  this->short_option = short_option;
}

void option::setParametersCount(int parameters_count) {
  this->parameters_count = parameters_count;
}

void option::setLongOption(std::string long_option) {
  this->long_option = long_option;
}

bool option::isEmpty() const {
  return (this->short_option == EUNKNOWN && this->long_option.empty());
}

bool option::operator==(const option &other) {
  return (this->short_option == other.short_option || this->long_option == other.long_option);
}

bool option::operator==(const char &other) {
  return this->short_option == other;
}

bool option::operator==(const std::string &other) {
  return this->long_option == other;
}

bool option::operator!=(const option &other) {
  return !this->operator==(other);
}

bool option::operator!=(const char &other) {
  return !this->operator==(other);
}

bool option::operator!=(const std::string &other) {
  return !this->operator==(other);
}

option parseopt(const int argc, const char * const * argv, std::vector<option> options) {
  if(argc < 2) return option(ENONE);

  optparams.clear();

  while(optindex < argc && argv[optindex][0] != '-' && argv[optindex][0] != '/') {
    optindex++;
  }

  if(optindex >= argc) {
    optindex = 1;
    return option(OPTEND);
  }

  std::string long_argument(argv[optindex]);
  bool is_short = long_argument.size() == 2;
  char short_argument = argv[optindex][1];
  option current;
  std::vector<option>::iterator i;
  bool found = false;

  for(i = options.begin(); i != options.end(); i++) {
    current = *i;
    if((is_short && current == short_argument) || (current == long_argument)) {
      found = true;
      break;
    }
  }

  optindex++;

  if(!found) return option(EUNKNOWN);

  int shift = optindex;

  for(int j = shift; j < shift + current.getParametersCount(); j++) {
    optparams.push_back(std::string(argv[j]));
    optindex++;
  }

  return current;
}
