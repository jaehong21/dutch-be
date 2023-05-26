#include <memory>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include "AccountController.h"
#include "User.h"
#include "Account.h"
#include "Response.h"
#include "HttpException.h"
#include "Json.h"

using std::string, std::vector, std::map, std::shared_ptr;

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
    map<string, string> query = request.getQueryString();
    validQueryString(request, {"uuid", "balance"});

    int balance;
    try {
        balance = stoi(query["balance"]);
    } catch (const std::exception& e) {
        throw BadRequestException("Balance must be integer");
    }

    vector<string> accountString = this->accountRepository->find(query["uuid"]);
    if (accountString.size() == 0) {
        throw BadRequestException("Account not found");
    }
    vector<string> userString = this->userRepository->find(query["uuid"]);
    if (userString.size() == 0) {
        throw BadRequestException("User not found");
    }

    auto newAccount = UserAccount(
        make_shared<User>(userString[0], userString[1], userString[2], userString[3]), balance);

    this->accountRepository->update(query["uuid"], newAccount);

    Json json = Json()
        .add("uuid", newAccount.getUuid())
        .add("balance", newAccount.getMoney());

    Response response(201, json);
    response.execute(sockfd);
}