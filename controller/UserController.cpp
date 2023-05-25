#include <memory>
#include <string>
#include <vector>
#include <map>
#include "UserController.h"
#include "User.h"
#include "Account.h"
#include "Response.h"
#include "HttpException.h"
#include "Json.h"

using namespace std;

shared_ptr<UserController> UserController::instance = nullptr;

shared_ptr<UserController> UserController::getInstance(
    shared_ptr<Repository> userRepository, shared_ptr<Repository> accountRepository) {
    if (instance.get() == nullptr) {
        instance = std::make_shared<UserController>(UserController());
        instance->userRepository = userRepository;
        instance->accountRepository = accountRepository;
    }
    return instance;
}

void UserController::createUser(int sockfd, const Request& request) {
    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"username", "password", "email"});

    User user(queryString["username"], queryString["password"], queryString["email"]);
    this->userRepository->create(user);

    UserAccount account(make_shared<User>(user), 0);
    this->accountRepository->create(account);

    Json json = Json()
        .add("uuid", user.getUuid())
        .add("username", user.getUsername())
        .add("email", user.getEmail());

    Response response(201, json);
    response.execute(sockfd);
}

void UserController::updateUser(int sockfd, const Request& request) {
    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"uuid", "username"});

    vector<string> userString = this->userRepository->find(queryString["uuid"]);
    if (userString.size() == 0) {
        throw BadRequestException("User not found");
    }
    auto user = User(userString[0], userString[1], userString[2], userString[3]);
    auto newUser = User(user.getUuid(), queryString["username"], userString[2], user.getEmail());

    this->userRepository->update(user.getUuid(), newUser);

    Json json = Json()
        .add("uuid", newUser.getUuid())
        .add("username", newUser.getUsername())
        .add("email", newUser.getEmail());

    Response response(201, json);
    response.execute(sockfd);
}

void UserController::findOneUser(int sockfd, const Request& request) {
    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"uuid"});

    vector<string> userString = this->userRepository->find(queryString["uuid"]);
    if (userString.size() == 0) {
        throw BadRequestException("User not found");
    }
    auto user = User(userString[0], userString[1], userString[2], userString[3]);

    
    Json json = Json()
        .add("uuid", user.getUuid())
        .add("username", user.getUsername())
        .add("email", user.getEmail());

    Response response(200, json);
    response.execute(sockfd);
}

void UserController::findAllUser(int sockfd, const Request& request) {
    vector<vector<string>> usersString = this->userRepository->findAll();
    vector<string> userList;
    for (auto userString : usersString) {
        userList.push_back(userString[0]);
    }

    Json json = Json()
        .add("user_list", userList);

    Response response(200, json);
    response.execute(sockfd);
}