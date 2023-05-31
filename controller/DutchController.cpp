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

void DutchController::findOneNormalDutch(int sockfd, const Request &request) {
    map<string, string> query = request.getQueryString();
    validQueryString(request, {"uuid"});

    // uuid, type, owner, targetBalance
    vector<string> dutchString = this->dutchRepository->find(query["uuid"]);
    if (dutchString.size() < 4)
        throw BadRequestException("Dutch not found");
    if (dutchString[1] != "normal")
        throw BadRequestException("Dutch is not normal");

    // uuid, dutch_uuid, user_uuid, amount, send_at
    vector<vector<string>> ledgerStringList = this->ledgerRepository->findAll();
    vector<string> userUuidList, sendUserUuidList;
    for (auto const &ledgerString : ledgerStringList) {
        userUuidList.push_back(ledgerString[2]);
        if (stoi(ledgerString[4]) > 0) { // send_at > 0 means the user has paid
            sendUserUuidList.push_back(ledgerString[2]);
        }
    }

    auto owner = this->getUser(dutchString[2]);
    auto dutch = std::make_shared<NormalDutch>(dutchString[0], stoi(dutchString[3]), owner,
                                               this->getUserList(userUuidList),
                                               this->getUserList(sendUserUuidList));

    auto json = Json()
                    .add("dutch_uuid", dutch->getUuid())
                    .add("type", dutchString[1])
                    .add("owner", dutch->getOwner()->getUuid())
                    .add("target_balance", dutch->getTargetBalance())
                    .add("user_list", userUuidList)
                    .add("send_user_list", sendUserUuidList);

    auto response = Response(200, json);
    response.execute(sockfd);
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

    auto owner = this->getUser(query["owner"]);
    auto userList = this->getUserList(userUuidList);
    auto newDutch = NormalDutch(targetBalance, owner, userList);
    this->dutchRepository->create(newDutch);

    auto dutchAccount = DutchAccount(std::make_shared<NormalDutch>(newDutch), owner, 0);
    this->accountRepository->create(dutchAccount);

    // TODO: when it is not divided completely
    int eachBalance = targetBalance / userList.size();
    for (auto user : userList) {
        auto newLedger = Ledger(newDutch.getUuid(), user->getUuid(), eachBalance);
        this->ledgerRepository->create(newLedger);
    }

    auto json = Json()
                    .add("dutch_uuid", newDutch.getUuid())
                    .add("type", "normal")
                    .add("owner", newDutch.getOwner()->getUuid())
                    .add("target_balance", newDutch.getTargetBalance())
                    .add("user_list", userUuidList);

    auto response = Response(201, json);
    response.execute(sockfd);
}

#include <iostream>
void DutchController::payNormalDutch(int sockfd, const Request &request) {
    map<string, string> query = request.getQueryString();
    validQueryString(request, {"dutch_uuid", "user_uuid"});

    // uuid, type, owner, targetBalance
    vector<string> dutchString = this->dutchRepository->find(query["dutch_uuid"]);
    if (dutchString.size() < 4)
        throw BadRequestException("Dutch not found");

    auto payer = this->getUser(query["user_uuid"]);
    auto owner = this->getUser(dutchString[2]);

    // uuid, dutch_uuid, user_uuid, amount, send_at
    vector<vector<string>> ledgerStringList = this->ledgerRepository->findAll();
    vector<string> userUuidList, sendUserUuidList;
    int sendAmount;
    string payerLedgerUuid;
    for (auto const &ledgerString : ledgerStringList) {
        userUuidList.push_back(ledgerString[2]);
        // send_at > 0 means the user has paid
        if (stoi(ledgerString[4]) > 0) {
            sendUserUuidList.push_back(ledgerString[2]);
        }
        // find the ledger of payer
        if (ledgerString[2] == payer->getUuid()) {
            // set sendAmount to ledger amount
            sendAmount = stoi(ledgerString[3]);
            payerLedgerUuid = ledgerString[0];
        }
    }

    auto dutch = std::make_shared<NormalDutch>(dutchString[0], stoi(dutchString[3]), owner,
                                               this->getUserList(userUuidList),
                                               this->getUserList(sendUserUuidList));
    auto newLedger = Ledger(payerLedgerUuid, dutch->getUuid(), payer->getUuid(), sendAmount,
                            Ledger::getTimeNow());

    // uuid, type, balance
    vector<string> dutchAccountString = this->accountRepository->find(dutch->getUuid());
    vector<string> payerAccountString = this->accountRepository->find(payer->getUuid());
    auto newDutchAccount = DutchAccount(dutch, owner, stoi(dutchAccountString[2]) + sendAmount);
    auto newPayerAccount = UserAccount(payer, stoi(payerAccountString[2]) - sendAmount);

    if (stoi(payerAccountString[2]) - sendAmount < 0)
        throw BadRequestException("Not enough balance");

    // update account
    this->accountRepository->update(dutchAccountString[0], newDutchAccount);
    this->accountRepository->update(payerAccountString[0], newPayerAccount);
    // update ledger
    this->ledgerRepository->update(newLedger.getUuid(), newLedger);

    sendUserUuidList.push_back(payer->getUuid());
    auto json = Json()
                    .add("dutch_uuid", dutch->getUuid())
                    .add("target_balance", dutch->getTargetBalance())
                    .add("current_balance", newDutchAccount.getBalance())
                    .add("user_list", userUuidList)
                    .add("send_user_list", sendUserUuidList);

    auto response = Response(201, json);
    response.execute(sockfd);
}

shared_ptr<User> DutchController::getUser(const string &uuid) {
    vector<string> userString = this->userRepository->find(uuid);
    if (userString.size() < 4)
        throw BadRequestException("User not found");

    auto user = std::make_shared<User>(userString[0], userString[1], userString[2], userString[3]);
    return user;
}

vector<shared_ptr<User>> DutchController::getUserList(const vector<string> &uuidList) {
    vector<shared_ptr<User>> userList;
    for (const string &uuid : uuidList) {
        auto user = this->getUser(uuid);
        userList.push_back(user);
    }
    return userList;
}
