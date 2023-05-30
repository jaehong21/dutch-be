#include "DutchController.h"
#include "Account.h"
#include "Dutch.h"
#include "HttpException.h"
#include "Request.h"
#include "User.h"
#include "Utils.h"
#include <memory>

using std::string, std::vector, std::map, std::shared_ptr;

shared_ptr<DutchController> DutchController::instance = nullptr;

shared_ptr<DutchController> DutchController::getInstance(shared_ptr<Repository> userRepository,
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

void DutchController::createNormalDutch(int sockfd, const Request &request) {
    map<string, string> query = request.getQueryString();
    validQueryString(request, {"owner", "target_balance", "user_list"});

    int targetBalance;
    try {
        targetBalance = stoi(query["target_balance"]);
    } catch (const std::exception &e) {
        throw BadRequestException("Target balance must be integer");
    }

    vector<string> userUuidList = Utils::splitStringBySeperator(query["user_list"], ",");
    if (userUuidList.size() < 1)
        throw BadRequestException("User list must have at least 1 users");

    vector<string> userString = this->userRepository->find(query["owner"]);
    if (userString.size() < 4)
        throw BadRequestException("User not found");
    auto owner = User(userString[0], userString[1], userString[2], userString[3]);

    vector<shared_ptr<User>> userList;
    for (auto userUuid : userUuidList) {
        vector<string> userString = this->userRepository->find(userUuid);
        if (userString.size() < 4)
            throw BadRequestException("User not found");
        userList.push_back(
            std::make_shared<User>(userString[0], userString[1], userString[2], userString[3]));
    }

    auto newDutch = NormalDutch(targetBalance, std::make_shared<User>(owner), userList);
    this->dutchRepository->create(newDutch);

    auto dutchAccount =
        DutchAccount(std::make_shared<NormalDutch>(newDutch), std::make_shared<User>(owner), 0);
    this->accountRepository->create(dutchAccount);

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

#include <iostream>
void DutchController::payNormalDutch(int sockfd, const Request &request) {
    map<string, string> query = request.getQueryString();
    validQueryString(request, {"dutch_uuid", "user_uuid"});

    vector<string> dutchString = this->dutchRepository->find(query["dutch_uuid"]);
    if (dutchString.size() < 4)
        throw BadRequestException("Dutch not found");

    vector<string> userString = this->userRepository->find(query["user_uuid"]);
    if (userString.size() < 4)
        throw BadRequestException("User not found");
    auto payer = User(userString[0], userString[1], userString[2], userString[3]);

    vector<string> ownerString = this->userRepository->find(dutchString[2]);
    auto owner = User(ownerString[0], ownerString[1], ownerString[2], ownerString[3]);

    vector<vector<string>> ledgerStringList = this->ledgerRepository->findAll();
    for (auto ledgerString : ledgerStringList) {
        for (auto s : ledgerString) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }

    auto json = Json().add("msg", "OK");

    Response response(201, json);
    response.execute(sockfd);
}