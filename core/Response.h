#pragma once

#include <string>
#include <vector>
#include "Json.h"

class Response {
public:
    Response(int statusCode, Json body);
    ~Response() {}

    void execute(int sockfd);
    std::string getResponse() const;

private:
    int statusCode;
    std::string contentType;
    std::string body;

    std::string getStatusMessage(int statusCode) const;
};