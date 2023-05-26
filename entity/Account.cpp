#include <memory>
#include <string>
#include "User.h"
#include "Account.h"

using std::string, std::vector, std::shared_ptr;

void Account::setMoney(int balance) { this->balance = balance; }
int Account::getMoney() const { return balance; }
shared_ptr<User> Account::getOwner() const { return owner; }
string Account::getUuid() const { return uuid; }

UserAccount::UserAccount(shared_ptr<User> owner, int balance)
    : Account(owner->getUuid(), owner, balance) {}

vector<string> UserAccount::toString() {
    vector<string> accountString;
    accountString.push_back(this->getUuid());
    accountString.push_back("user");
    accountString.push_back(std::to_string(this->getMoney()));
    return accountString;
}

DutchAccount::DutchAccount(shared_ptr<Dutch> dutch, shared_ptr<User> owner, int balance)
    : Account(dutch->getUuid(), owner, balance), dutch(dutch) {}

shared_ptr<Dutch> DutchAccount::getDutch() const { return dutch; }

vector<string> DutchAccount::toString() {
    vector<string> accountString;
    accountString.push_back(this->getUuid());
    accountString.push_back("dutch");
    accountString.push_back(std::to_string(this->getMoney()));
    return accountString;
}