#pragma once

#include "Repository.h"
#include "Request.h"
#include "Response.h"
#include <memory>
#include <string>
#include <vector>

class Controller {
  public:
    // Controller(Repository* repository);
    virtual ~Controller(){};

    static bool handleRequest(std::string method, std::string path, const Request &request);
    bool validQueryString(const Request &request, std::vector<std::string> keys) const;
    static bool startWith(std::string prefix, std::string str);
};