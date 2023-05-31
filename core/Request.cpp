#include "Request.h"
#include "Utils.cpp"
#include <map>
#include <string>
#include <vector>

using std::string, std::vector, std::map;

Request::Request(const string &request) : data(request) {
    // Split request string by newline character
    vector<string> contents = Utils::splitStringBySeperator(request, "\n");
    string firstLine = contents[0];
    size_t pos1 = firstLine.find(' ');
    size_t pos2 = firstLine.find(' ', pos1 + 1);

    this->method = firstLine.substr(0, pos1);
    this->requestPath = firstLine.substr(pos1 + 1, pos2 - pos1 - 1);
}

Request::~Request() {}

map<string, string> Request::getQueryString() const {
    map<string, string> result;
    string queryStringStr = this->requestPath.substr(this->requestPath.find('?') + 1);
    vector<string> queryStrings = Utils::splitStringBySeperator(queryStringStr, "&");

    for (const auto &query : queryStrings) {
        size_t pos = query.find('=');
        if (pos != string::npos) {
            string key = query.substr(0, pos);
            string value = query.substr(pos + 1);
            result.insert(std::pair<string, string>(key, value));
        }
    }

    return result;
}

string Request::getHeader(string headerKey) const {
    vector<string> contents = Utils::splitStringBySeperator(this->data, "\n");
    // Parse request method, host, and user agent
    for (const auto &line : contents) {
        string::size_type pos = line.find(": ");
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 2);
            if (key == headerKey) {
                return value;
            }
        }
    }
    return "";
}

string Request::getData() const { return data; }
string Request::getMethod() const { return method; }
string Request::getRequestPath() const { return requestPath; }
string Request::getPath() const {
    string path = this->requestPath;
    size_t pos = path.find('?');
    if (pos != string::npos) {
        path = path.substr(0, pos);
    }
    return path;
}