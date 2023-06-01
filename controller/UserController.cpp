#include "UserController.h"
#include "Account.h"
#include "HttpException.h"
#include "Json.h"
#include "Response.h"
#include "User.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

using std::string, std::vector, std::map, std::shared_ptr;

#define INITIAL_BALANCE 1000

shared_ptr<UserController> UserController::instance = nullptr;

shared_ptr<UserController> UserController::getInstance(shared_ptr<Repository> userRepository,
                                                       shared_ptr<Repository> accountRepository) {
    if (instance.get() == nullptr) {
        instance = std::make_shared<UserController>(UserController());
        instance->userRepository = userRepository;
        instance->accountRepository = accountRepository;
    }
    return instance;
}

void UserController::loginUser(int sockfd, const Request &request) {
    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"username", "password"});

    // uuid, username, password, email
    vector<vector<string>> userStringList = this->userRepository->findAll();
    bool found = false;
    for (auto userString : userStringList) {
        if (userString[1] == queryString["username"]) {
            found = true;
            if (userString[2] == queryString["password"]) {
                auto json = Json()
                                .add("uuid", userString[0])
                                .add("username", userString[1])
                                .add("email", userString[3]);
                auto response = Response(201, json);
                response.execute(sockfd);
                return;
            } else {
                throw UnauthorizedException("Wrong password");
            }
        }
    }

    throw BadRequestException("User not found");
}

void UserController::createUser(int sockfd, const Request &request) {
    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"username", "password", "email"});

    auto user = User(queryString["username"], queryString["password"], queryString["email"]);
    this->userRepository->create(user);

    auto account = UserAccount(std::make_shared<User>(user), INITIAL_BALANCE);
    this->accountRepository->create(account);

    auto json = Json()
                    .add("uuid", user.getUuid())
                    .add("username", user.getUsername())
                    .add("email", user.getEmail());

    Response response(201, json);
    response.execute(sockfd);
}

void UserController::updateUser(int sockfd, const Request &request) {
    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"uuid", "username"});

    vector<string> userString = this->userRepository->find(queryString["uuid"]);
    if (userString.size() == 0)
        throw BadRequestException("User not found");

    auto user = User(userString[0], userString[1], userString[2], userString[3]);
    auto newUser = User(user.getUuid(), queryString["username"], userString[2], user.getEmail());

    this->userRepository->update(user.getUuid(), newUser);

    auto json = Json()
                    .add("uuid", newUser.getUuid())
                    .add("username", newUser.getUsername())
                    .add("email", newUser.getEmail());

    auto response = Response(201, json);
    response.execute(sockfd);
}

void UserController::findOneUser(int sockfd, const Request &request) {
    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"uuid"});

    vector<string> userString = this->userRepository->find(queryString["uuid"]);
    if (userString.size() == 0)
        throw BadRequestException("User not found");

    auto user = User(userString[0], userString[1], userString[2], userString[3]);

    auto json = Json()
                    .add("uuid", user.getUuid())
                    .add("username", user.getUsername())
                    .add("email", user.getEmail());

    auto response = Response(200, json);
    response.execute(sockfd);
}

void UserController::findAllUser(int sockfd, const Request &request) {
    vector<vector<string>> usersString = this->userRepository->findAll();
    vector<string> userList;
    for (auto userString : usersString) {
        userList.push_back(userString[0]);
    }

    auto json = Json().add("user_list", userList);

    auto response = Response(200, json);
    response.execute(sockfd);
}