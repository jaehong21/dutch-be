#pragma once

#include <string>
#include <vector>

class Utils {
  public:
    static std::vector<std::string> splitStringBySeperator(const std::string &text,
                                                           const std::string seperator) {
        std::vector<std::string> lines;
        std::string::size_type pos = 0, prev = 0;
        while ((pos = text.find(seperator, prev)) != std::string::npos) {
            lines.push_back(text.substr(prev, pos - prev));
            prev = pos + 1;
        }
        if (prev < text.length()) {
            lines.push_back(text.substr(prev));
        }
        return lines;
    };
};
