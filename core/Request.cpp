#include <string>
#include <vector>
#include "Request.h"

using namespace std;

Request::Request(const string& request) : data(request) {
    // Split request string by newline character
    vector<string> contents = splitByNewline(request);
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
    vector<string> queryStrings = splitString(queryStringStr, "&");

    for (const auto& query : queryStrings) {
        size_t pos = query.find('=');
        if (pos != string::npos) {
            string key = query.substr(0, pos);
            string value = query.substr(pos + 1);
            result.insert(pair<string, string>(key, value));
        }
    }

    return result;
}

string Request::getHeader(string headerKey) const {
    vector<string> contents = splitByNewline(this->data);
    // Parse request method, host, and user agent
    for (const auto& line : contents) {
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

vector<string> Request::splitByNewline(const string& text) const {
    return splitString(text, "\n");
};

vector<string> Request::splitString(const string& text, const string seperator) const {
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
string Request::getBody() const { return body; }