#include <string>
#include "Json.h"

using namespace std;

Json::Json() {
    jsonString = "";
}
Json::Json(string key, string value) {
    add(key, value);
}
Json::Json(string key, int value) {
    add(key, value);
}

Json& Json::add(string key, string value) {
    jsonString += "\"" + key + "\": \"" + value + "\"" + ",";
    return *this;
}

Json& Json::add(string key, int value) {
    jsonString += "\"" + key + "\": " + to_string(value) + ",";
    return *this;
}

Json& Json::add(string key, vector<string> value) {
    jsonString += "\"" + key + "\": [";
    for (int i = 0; i < value.size(); ++i) {
        jsonString += "\"" + value[i] + "\"";
        if (i != value.size() - 1) {
            jsonString += ", ";
        }
    }
    jsonString += "],";
    return *this;
}

string Json::getJsonString() { 
    jsonString.pop_back(); // remove last comma
    jsonString = "{" + jsonString + "}";

    return jsonString; 
}