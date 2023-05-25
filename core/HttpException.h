#pragma once

#include <string>
#include <exception>

class HttpException : public std::exception {
public:
    HttpException(int statusCode, std::string msg) : statusCode(statusCode), msg(msg) {}
    virtual const char* what() const throw() { return msg.c_str(); }
    int getStatusCode() const { return statusCode; }

private:
    int statusCode;
    std::string msg;
};

class BadRequestException : public HttpException {
public:
    BadRequestException(std::string msg) : HttpException(400, msg) {}
};

class UnauthorizedException : public HttpException {
public:
    UnauthorizedException(std::string msg) : HttpException(401, msg) {}
};

class ForbiddenException : public HttpException {
public:
    ForbiddenException(std::string msg) : HttpException(403, msg) {}
};

class NotFoundException : public HttpException {
public:
    NotFoundException(std::string msg) : HttpException(404, msg) {}
};

class InternalServerErrorException : public HttpException {
public:
    InternalServerErrorException(std::string msg) : HttpException(500, msg) {}
};
