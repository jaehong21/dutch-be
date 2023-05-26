#include <string>
#include <vector>
#include <map>
#include "Controller.h"
#include "Request.h"
#include "HttpException.h"

using std::string, std::vector, std::map;

bool Controller::handleRequest(string method, string path, const Request& request) {
    return (request.getMethod() == method && startWith(path, request.getRequestPath()));
}

bool Controller::validQueryString(const Request& request, vector<string> keys) const {
    map<string, string> queryString = request.getQueryString();
    bool valid = true;
    for(auto& key : keys) {
        if(queryString.find(key) == queryString.end()) {
            valid = false;
            throw BadRequestException("Missing query string key: " + key);
            break;
        }
    } 

    return valid;
}

bool Controller::startWith(string prefix, string str) {
    return str.substr(0, prefix.size()) == prefix;
}