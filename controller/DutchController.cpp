#include <memory>
#include "DutchController.h"
#include "User.h"
#include "Account.h"
#include "Dutch.h"
#include "Utils.h"
#include "Request.h"
#include "HttpException.h"

using std::string, std::vector, std::map, std::shared_ptr;

shared_ptr<DutchController> DutchController::instance = nullptr;

shared_ptr<DutchController> DutchController::getInstance(
    shared_ptr<Repository> userRepository, 
    shared_ptr<Repository> accountRepository,
    shared_ptr<Repository> dutchRepository,
    shared_ptr<Repository> ledgerRepository) {
    if (instance.get() == nullptr) {
        instance = std::make_shared<DutchController>(DutchController());
        instance->userRepository = userRepository;
        instance->accountRepository = accountRepository;
        instance->dutchRepository = dutchRepository;
        instance->ledgerRepository = ledgerRepository;
    }
    return instance;
}

void DutchController::createNormalDutch(int sockfd, const Request& request) {
    map<string, string> query = request.getQueryString();
    validQueryString(request, {"owner", "target_balance", "user_list"});

    int targetBalance;
    try {
        targetBalance = stoi(query["target_balance"]);
    } catch (const std::exception& e) {
        throw BadRequestException("Target balance must be integer");
    }

    vector<string> userUuidList = Utils::splitStringBySeperator(query["user_list"], ",");
    if (userUuidList.size() < 1) throw BadRequestException("User list must have at least 1 users");

    vector<string> userString = this->userRepository->find(query["owner"]);
    if (userString.size() < 4) throw BadRequestException("User not found");
    auto owner = User(userString[0], userString[1], userString[2], userString[3]);
    
    vector<shared_ptr<User>> userList;
    for (auto userUuid : userUuidList) {
        vector<string> userString = this->userRepository->find(userUuid);
        if (userString.size() < 4) throw BadRequestException("User not found");
        userList.push_back(std::make_shared<User>(userString[0], userString[1], userString[2], userString[3]));
    }

    auto newDutch = NormalDutch(targetBalance, std::make_shared<User>(owner), userList);
    this->dutchRepository->create(newDutch);

    int eachBalance = targetBalance / userList.size();
    for (auto user : userList) {
        auto newLedger = Ledger(std::make_shared<NormalDutch>(newDutch), user, eachBalance);
        this->ledgerRepository->create(newLedger);
    }

    auto json = Json()
        .add("uuid", newDutch.getUuid())
        .add("type", "normal")
        .add("owner", newDutch.getOwner()->getUuid())
        .add("target_balance", newDutch.getTargetBalance())
        .add("user_list", userUuidList);

    Response response(201, json);
    response.execute(sockfd);
}