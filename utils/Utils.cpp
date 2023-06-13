#include "Utils.h"
#include <limits>
#include <string>
#include <vector>

using std::string, std::vector, std::map;

vector<string> Utils::splitStringBySeparator(const string &text, const string separator) {
    vector<string> lines;
    string::size_type pos = 0, prev = 0;
    while ((pos = text.find(separator, prev)) != string::npos) {
        lines.push_back(text.substr(prev, pos - prev));
        prev = pos + 1;
    }
    if (prev < text.length()) {
        lines.push_back(text.substr(prev));
    }
    return lines;
};

string Utils::getKeyWithLowestValue(const map<string, long> &myMap) {
    long lowestValue =
        std::numeric_limits<long>::max(); // Set initial lowest value to maximum possible value
    string keyWithLowestValue;

    for (const auto &pair : myMap) {
        if (pair.second < lowestValue) {
            lowestValue = pair.second;
            keyWithLowestValue = pair.first;
        }
    }

    return keyWithLowestValue;
}