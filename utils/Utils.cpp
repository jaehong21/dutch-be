#include "Utils.h"
#include <string>
#include <vector>

using std::string, std::vector;

vector<string> Utils::splitStringBySeperator(const string &text, const string seperator) {
    vector<string> lines;
    string::size_type pos = 0, prev = 0;
    while ((pos = text.find(seperator, prev)) != string::npos) {
        lines.push_back(text.substr(prev, pos - prev));
        prev = pos + 1;
    }
    if (prev < text.length()) {
        lines.push_back(text.substr(prev));
    }
    return lines;
};
