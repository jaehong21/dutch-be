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

Ledger::Ledger(shared_ptr<Dutch> dutch, shared_ptr<User> user, int amount)
    : dutch(dutch), user(user), amount(amount), sendTime(-1) {}
Ledger::Ledger(shared_ptr<Dutch> dutch, shared_ptr<User> user, int amount, long sendTime)
    : dutch(dutch), user(user), amount(amount), sendTime(sendTime) {}

void Ledger::setSendTime() {
    std::time_t t = std::time(nullptr);
    this->sendTime = static_cast<long>(t);
}

vector<string> Ledger::toString() {
    vector<string> ledgerString;
    ledgerString.push_back(this->dutch->getUuid());
    ledgerString.push_back(this->user->getUuid());
    ledgerString.push_back(std::to_string(this->amount));
    ledgerString.push_back(std::to_string(this->sendTime));
    return ledgerString;
}