#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Repository.h"
#include "Response.h"
#include "Request.h"

class Controller {
public:
    // Controller(Repository* repository);
    virtual ~Controller() {};

    static bool handleRequest(std::string method, std::string path, const Request& request);
    bool validQueryString(const Request& request, std::vector<std::string> keys) const;
    static bool startWith(std::string prefix, std::string str);
};