#include "Response.h"
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using std::string;

Response::Response(int statusCode, Json body) : statusCode(statusCode), body(body.getJsonString()) {
    this->contentType = "application/json";
}

string Response::getResponse() const {
    string response = "HTTP/1.1 " + std::to_string(statusCode) + " " +
                      getStatusMessage(statusCode) + "\r\n" + "Content-Type: " + contentType +
                      "\r\n" + "Access-Control-Allow-Origin: *\r\n" +
                      "Content-Length: " + std::to_string(body.length()) + "\r\n" + "\r\n" + body;

    return response;
}

void Response::execute(int sockfd) {
    string response = getResponse();
    send(sockfd, response.c_str(), response.size(), 0);
    close(sockfd);
}

string Response::getStatusMessage(int statusCode) const {
    switch (statusCode) {
    case 200:
        return "OK";
    case 201:
        return "Created";
    case 204:
        return "No Content";
    case 400:
        return "Bad Request";
    case 401:
        return "Unauthorized";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 500:
        return "Internal Server Error";
    default:
        return "Unknown";
    }
}
