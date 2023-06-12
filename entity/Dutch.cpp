#include "Dutch.h"
#include <ctime>
#include <memory>
#include <string>
#include <vector>

using std::string, std::vector, std::shared_ptr;

Dutch::Dutch(int targetBalance, shared_ptr<User> owner, vector<shared_ptr<User>> userList)
    : uuid(Entity::generateUuidV4()), targetBalance(targetBalance), owner(owner),
      userList(userList), sendUserList(vector<shared_ptr<User>>()) {}
Dutch::Dutch(string uuid, int targetBalance, shared_ptr<User> owner,
             vector<shared_ptr<User>> userList, vector<shared_ptr<User>> sendUserList)
    : uuid(uuid), targetBalance(targetBalance), owner(owner), userList(userList),
      sendUserList(sendUserList) {}

string Dutch::getUuid() const { return uuid; }
int Dutch::getTargetBalance() const { return targetBalance; }
shared_ptr<User> Dutch::getOwner() const { return owner; }
vector<shared_ptr<User>> Dutch::getUserList() const { return userList; }
vector<shared_ptr<User>> Dutch::getSendUserList() const { return sendUserList; }

NormalDutch::NormalDutch(int targetBalance, shared_ptr<User> owner,
                         vector<shared_ptr<User>> userList)
    : Dutch(targetBalance, owner, userList) {}
NormalDutch::NormalDutch(string uuid, int targetBalance, shared_ptr<User> owner,
                         vector<shared_ptr<User>> userList, vector<shared_ptr<User>> sendUserList)
    : Dutch(uuid, targetBalance, owner, userList, sendUserList) {}

vector<string> NormalDutch::toString() {
    vector<string> dutchString;
    dutchString.push_back(this->getUuid());
    dutchString.push_back("normal");
    dutchString.push_back(this->getOwner()->getUuid());
    dutchString.push_back(std::to_string(this->getTargetBalance()));

    return dutchString;
}

RaceDutch::RaceDutch(int targetBalance, shared_ptr<User> owner, vector<shared_ptr<User>> userList)
    : Dutch(targetBalance, owner, userList) {}
RaceDutch::RaceDutch(string uuid, int targetBalance, shared_ptr<User> owner,
                     vector<shared_ptr<User>> userList, vector<shared_ptr<User>> sendUserList)
    : Dutch(uuid, targetBalance, owner, userList, sendUserList) {}

vector<string> RaceDutch::toString() {
    vector<string> dutchString;
    dutchString.push_back(this->getUuid());
    dutchString.push_back("race");
    dutchString.push_back(this->getOwner()->getUuid());
    dutchString.push_back(std::to_string(this->getTargetBalance()));

    return dutchString;
}

Ledger::Ledger(string dutchUuid, string userUuid, int amount)
    : uuid(Entity::generateUuidV4()), dutchUuid(dutchUuid), userUuid(userUuid), amount(amount),
      sendTime(-1) {}
Ledger::Ledger(string uuid, string dutchUuid, string userUuid, int amount, long sendTime)
    : uuid(uuid), dutchUuid(dutchUuid), userUuid(userUuid), amount(amount), sendTime(sendTime) {}

long Ledger::getTimeNow() {
    std::time_t t = std::time(nullptr);
    return static_cast<long>(t);
}

string Ledger::getUuid() const { return uuid; }
string Ledger::getDutchUuid() const { return dutchUuid; }
string Ledger::getUserUuid() const { return userUuid; }

vector<string> Ledger::toString() {
    vector<string> ledgerString;
    ledgerString.push_back(this->getUuid());
    ledgerString.push_back(this->getDutchUuid());
    ledgerString.push_back(this->getUserUuid());
    ledgerString.push_back(std::to_string(this->amount));
    ledgerString.push_back(std::to_string(this->sendTime));
    return ledgerString;
}