#pragma once

#include <string>
#include <vector>

class Utils {
  public:
    static std::vector<std::string> splitStringBySeperator(const std::string &text,
                                                           const std::string seperator);
};
