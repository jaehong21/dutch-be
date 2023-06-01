#include "AccountController.h"
#include "Account.h"
#include "HttpException.h"
#include "Json.h"
#include "Response.h"
#include "User.h"
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <vector>

using std::string, std::vector, std::map, std::shared_ptr;

shared_ptr<AccountController> AccountController::instance = nullptr;

shared_ptr<AccountController>
AccountController::getInstance(shared_ptr<Repository> userRepository,
                               shared_ptr<Repository> accountRepository) {
    if (instance.get() == nullptr) {
        instance = std::make_shared<AccountController>(AccountController());
        instance->userRepository = userRepository;
        instance->accountRepository = accountRepository;
    }
    return instance;
}

void AccountController::findOneUserAccount(int sockfd, const Request &request) {
    map<string, string> query = request.getQueryString();
    validQueryString(request, {"uuid"});

    // uuid, type, balance
    vector<string> accountString = this->accountRepository->find(query["uuid"]);
    if (accountString.size() < 3)
        throw BadRequestException("Account not found");
    if (accountString[1] != "user")
        throw BadRequestException("Account is not user account");

    auto json = Json()
                    .add("uuid", accountString[0])
                    .add("type", accountString[1])
                    .add("balance", accountString[2]);

    auto response = Response(200, json);
    response.execute(sockfd);
}

void AccountController::updateUserAccount(int sockfd, const Request &request) {
    map<string, string> query = request.getQueryString();
    validQueryString(request, {"uuid", "balance"});

    int balance;
    try {
        balance = stoi(query["balance"]);
    } catch (const std::exception &e) {
        throw BadRequestException("Balance must be integer");
    }

    vector<string> accountString = this->accountRepository->find(query["uuid"]);
    if (accountString.size() == 0)
        throw BadRequestException("Account not found");

    vector<string> userString = this->userRepository->find(query["uuid"]);
    if (userString.size() < 4)
        throw BadRequestException("User not found");

    auto newAccount = UserAccount(
        std::make_shared<User>(userString[0], userString[1], userString[2], userString[3]),
        balance);

    this->accountRepository->update(query["uuid"], newAccount);

    auto json = Json().add("uuid", newAccount.getUuid()).add("balance", newAccount.getBalance());

    auto response = Response(201, json);
    response.execute(sockfd);
}