#pragma once

#include <string>
#include <vector>
#include <map>

class Request {
public:
    Request(const std::string& request);
    ~Request();

    std::map<std::string, std::string> getQueryString() const;
    std::string getMethod() const;
    std::string getRequestPath() const;
    std::string getPath() const;
    std::string getHeader(std::string headerKey) const;
    std::string getData() const;

private:
    // whole request bytes to string
    std::string data;
    std::string method;
    std::string requestPath;
};