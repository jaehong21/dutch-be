#include <memory>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include "AccountController.h"
#include "Account.h"
#include "User.h"
#include "Response.h"
#include "HttpException.h"
#include "Json.h"

using namespace std;

shared_ptr<AccountController> AccountController::instance = nullptr;

shared_ptr<AccountController> AccountController::getInstance(
    shared_ptr<Repository> userRepository, shared_ptr<Repository> accountRepository) {
    if (instance.get() == nullptr) {
        instance = std::make_shared<AccountController>(AccountController());
        instance->userRepository = userRepository;
        instance->accountRepository = accountRepository;
    }
    return instance;
}

void AccountController::updateUserAccount(int sockfd, const Request& request) {
    map<string, string> queryString = request.getQueryString();
    validQueryString(request, {"uuid", "money"});

    int money;
    try {
        money = stoi(queryString["money"]);
    } catch (const std::exception& e) {
        throw BadRequestException("Money must be integer");
    }

    vector<string> accountString = this->accountRepository->find(queryString["uuid"]);
    if (accountString.size() == 0) {
        throw BadRequestException("Account not found");
    }
    vector<string> userString = this->userRepository->find(queryString["uuid"]);
    if (userString.size() == 0) {
        throw BadRequestException("User not found");
    }

    auto newAccount = UserAccount(
        make_shared<User>(userString[0], userString[1], userString[2], userString[3]), money);

    this->accountRepository->update(queryString["uuid"], newAccount);

    Json json = Json()
        .add("uuid", newAccount.getUuid())
        .add("money", newAccount.getMoney());

    Response response(201, json);
    response.execute(sockfd);
}