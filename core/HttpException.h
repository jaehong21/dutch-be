#pragma once

#include <string>
#include <exception>

class HttpException : public std::exception {
public:
    HttpException(int statusCode, std::string msg) : statusCode(statusCode), msg(msg) {}
    virtual ~HttpException() {}
    virtual const char* what() const throw() { return msg.c_str(); }
    int getStatusCode() const { return statusCode; }

private:
    int statusCode;
    std::string msg;
};

class BadRequestException : public HttpException {
public:
    virtual ~BadRequestException() {}
    BadRequestException(std::string msg) : HttpException(400, msg) {}
};

class UnauthorizedException : public HttpException {
public:
    virtual ~UnauthorizedException() {}
    UnauthorizedException(std::string msg) : HttpException(401, msg) {}
};

class ForbiddenException : public HttpException {
public:
    virtual ~ForbiddenException() {}
    ForbiddenException(std::string msg) : HttpException(403, msg) {}
};

class NotFoundException : public HttpException {
public:
    virtual ~NotFoundException() {}
    NotFoundException(std::string msg) : HttpException(404, msg) {}
};

class InternalServerErrorException : public HttpException {
public:
    virtual ~InternalServerErrorException() {}
    InternalServerErrorException(std::string msg) : HttpException(500, msg) {}
};
