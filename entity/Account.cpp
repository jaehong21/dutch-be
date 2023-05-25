#include <memory>
#include <string>
#include "User.h"
#include "Account.h"

using namespace std;

void Account::setMoney(int money) { this->money = money; }
int Account::getMoney() const { return money; }
shared_ptr<User> Account::getOwner() const { return owner; }
string Account::getUuid() const { return uuid; }

UserAccount::UserAccount(shared_ptr<User> owner, int money)
    : Account(owner->getUuid(), owner, money) {}

vector<string> UserAccount::toString() {
    vector<string> accountString;
    accountString.push_back(this->getUuid());
    accountString.push_back("user");
    accountString.push_back(to_string(this->getMoney()));
    return accountString;
}

DutchAccount::DutchAccount(shared_ptr<User> owner, int money)
    : Account(owner->getUuid(), owner, money) {}

vector<string> DutchAccount::toString() {
    vector<string> accountString;
    accountString.push_back(this->getUuid());
    accountString.push_back("dutch");
    accountString.push_back(to_string(this->getMoney()));
    return accountString;
}