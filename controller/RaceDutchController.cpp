#include "RaceDutchController.h"
#include "Account.h"
#include "Dutch.h"
#include "HttpException.h"
#include "Request.h"
#include "User.h"
#include "Utils.h"
#include <memory>

using std::string, std::vector, std::map, std::shared_ptr;

shared_ptr<RaceDutchController> RaceDutchController::instance = nullptr;

shared_ptr<RaceDutchController> RaceDutchController::getInstance(
    shared_ptr<Repository> userRepository, shared_ptr<Repository> accountRepository,
    shared_ptr<Repository> dutchRepository, shared_ptr<Repository> ledgerRepository) {
    if (instance.get() == nullptr) {
        instance = std::make_shared<RaceDutchController>(RaceDutchController());
        instance->userRepository = userRepository;
        instance->accountRepository = accountRepository;
        instance->dutchRepository = dutchRepository;
        instance->ledgerRepository = ledgerRepository;
    }
    return instance;
}

void RaceDutchController::findOneRaceDutch(int sockfd, const Request &request) {
    map<string, string> query = request.getQueryString();
    validQueryString(request, {"dutch_uuid"});

    // uuid, type, owner, targetBalance
    vector<string> dutchString = this->dutchRepository->find(query["dutch_uuid"]);
    if (dutchString.size() < 4)
        throw BadRequestException("Dutch not found");
    if (dutchString[1] != "race")
        throw BadRequestException("Dutch is not normal");

    // uuid, dutch_uuid, user_uuid, amount, send_at
    vector<vector<string>> ledgerStringList = this->ledgerRepository->findAll();
    vector<string> userUuidList, sendUserUuidList;
    for (auto const &ledgerString : ledgerStringList) {
        // if ledger is not related to this dutch
        if (ledgerString[1] != dutchString[0])
            continue;

        userUuidList.push_back(ledgerString[2]);
        if (stoi(ledgerString[4]) > 0) { // send_at > 0 means the user has paid
            sendUserUuidList.push_back(ledgerString[2]);
        }
    }

    auto owner = this->getUser(dutchString[2]);
    auto dutch = std::make_shared<NormalDutch>(dutchString[0], stoi(dutchString[3]), owner,
                                               this->getUserList(userUuidList),
                                               this->getUserList(sendUserUuidList));

    // uuid, type, balance
    vector<string> dutchAccountString = this->accountRepository->find(dutchString[0]);

    vector<string> userNameList, sendUserNameList;
    for (auto const &uuid : userUuidList) {
        auto user = this->getUser(uuid);
        userNameList.push_back(user->getUsername());
    }
    for (auto const &uuid : sendUserUuidList) {
        auto user = this->getUser(uuid);
        sendUserNameList.push_back(user->getUsername());
    }

    auto json = Json()
                    .add("dutch_uuid", dutch->getUuid())
                    .add("type", dutchString[1])
                    .add("owner", dutch->getOwner()->getUuid())
                    .add("owner_name", dutch->getOwner()->getUsername())
                    .add("current_balance", dutchAccountString[2])
                    .add("target_balance", dutch->getTargetBalance())
                    .add("user_list", userUuidList)
                    .add("user_name_list", userNameList)
                    .add("send_user_list", sendUserUuidList)
                    .add("send_user_name_list", sendUserNameList);

    auto response = Response(200, json);
    response.execute(sockfd);
}

shared_ptr<User> RaceDutchController::getUser(const string &uuid) {
    vector<string> userString = this->userRepository->find(uuid);
    if (userString.size() < 4)
        throw BadRequestException("User not found");

    auto user = std::make_shared<User>(userString[0], userString[1], userString[2], userString[3]);
    return user;
}

vector<shared_ptr<User>> RaceDutchController::getUserList(const vector<string> &uuidList) {
    vector<shared_ptr<User>> userList;
    for (const string &uuid : uuidList) {
        auto user = this->getUser(uuid);
        userList.push_back(user);
    }
    return userList;
}
