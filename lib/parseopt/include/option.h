#ifndef __PARSEOPT_H
#define __PARSEOPT_H

#include <string>
#include <vector>
#include <iostream>

#define OPTEND 0
#define ENONE 1
#define EUNKNOWN 2
#define EFAILURE 3

extern std::vector<std::string> optparams;
extern int optindex;

class option {
  private:
    char short_option;
    int parameters_count;
    std::string long_option;

  public:
    option();
    option(char short_option, int parameters_count = 0, std::string long_option = "");
    char getShortOption() const;
    int getParametersCount() const;
    std::string getLongOption() const;
    void setShortOption(char short_option);
    void setParametersCount(int parameters_count);
    void setLongOption(std::string long_option);
    bool isEmpty() const;
    bool operator==(const option &other);
    bool operator==(const char &other);
    bool operator==(const std::string &other);
    bool operator!=(const option &other);
    bool operator!=(const char &other);
    bool operator!=(const std::string &other);
    friend std::ostream &operator<<(std::ostream &output, const option &operand) {
      output << operand.getShortOption() << " ";
      if(!operand.getLongOption().empty()) {
        output << operand.getLongOption() << " ";
      }
      output << operand.getParametersCount() << std::endl;

      return output;
    }
};

option parseopt(const int argc, const char * const * argv, std::vector<option> options);

#endif // __PARSEOPT_H
