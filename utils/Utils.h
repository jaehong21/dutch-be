#pragma once

#include <map>
#include <string>
#include <vector>

class Utils {
  public:
    static std::vector<std::string> splitStringBySeparator(const std::string &text,
                                                           const std::string seperator);
    static std::string getKeyWithLowestValue(const std::map<std::string, long> &myMap);
};
