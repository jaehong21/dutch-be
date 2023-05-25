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
    
    std::shared_ptr<Repository> getRepository() const;

    void setPrefix(std::string prefix);
    std::string getPrefix() const;

    static bool handleRequest(std::string method, std::string path, const Request& request);
    bool validQueryString(const Request& request, std::vector<std::string> keys) const;
    static bool startWith(std::string prefix, std::string str);

protected:
    void setRepository(std::shared_ptr<Repository> repository);

private:
    std::string prefix;
    std::shared_ptr<Repository> repository;
};