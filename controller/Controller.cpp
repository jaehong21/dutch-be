#include <string>
#include <vector>
#include <map>
#include "Controller.h"
#include "Request.h"
#include "HttpException.h"


using namespace std;

void Controller::setRepository(shared_ptr<Repository> repository) { this->repository = repository; };
shared_ptr<Repository> Controller::getRepository() const { return repository; };

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

void Controller::setPrefix(string prefix) { this->prefix = prefix; };
string Controller::getPrefix() const { return this->prefix; };

bool Controller::startWith(string prefix, string str) {
    return str.substr(0, prefix.size()) == prefix;
}