#include <memory>
#include <string>
#include <vector>
#include <map>
#include "UserController.h"
#include "UserEntity.h"
#include "Response.h"
#include "HttpException.h"
#include "Json.h"

using namespace std;

shared_ptr<UserController> UserController::instance = nullptr;

shared_ptr<UserController> UserController::getInstance(shared_ptr<Repository> repository) {
    if (instance.get() == nullptr) {
        instance = std::make_shared<UserController>(UserController());
        instance->setRepository(repository);
        instance->setPrefix("/user");
    }
    return instance;
}

void UserController::createUser(int sockfd, const Request& request) {
    shared_ptr<Repository> repository = getRepository();
    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"username", "password", "email"});

    UserEntity user(queryString["username"], queryString["password"], queryString["email"]);
    repository->create(user);

    Json json = Json("id", user.getUuid())
        .add("username", user.getUsername())
        .add("email", user.getEmail());

    Response response(201, json);
    response.execute(sockfd);
}

#include <iostream>

void UserController::findOneUser(int sockfd, const Request& request) {
    shared_ptr<Repository> repository = getRepository();

    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"id"});

    vector<string> userString = repository->find(queryString["id"]);
    if (userString.size() == 0) {
        throw BadRequestException("User not found");
    }
    UserEntity user(userString[0], userString[1], userString[2], userString[3]);

    
    Json json = Json("id", user.getUuid())
        .add("username", user.getUsername())
        .add("email", user.getEmail());

    Response response(200, json);
    response.execute(sockfd);
}