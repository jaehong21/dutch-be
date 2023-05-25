#include <memory>
#include "User.h"
#include "Account.h"

using namespace std;

void Account::setMoney(int money) { this->money = money; }
int Account::getMoney() const { return money; }

UserAccount::UserAccount(shared_ptr<User> owner, int money) 
    : Account(money), owner(owner) {
    this->uuid = owner->getUuid();
}

void UserAccount::setOwner(shared_ptr<User> owner) { this->owner = owner; }
shared_ptr<User> UserAccount::getOwner() const { return owner; }
string UserAccount::getUuid() const { return uuid; }

vector<string> UserAccount::toString() {
    vector<string> accountString;
    accountString.push_back(uuid);
    accountString.push_back(to_string(this->getMoney()));
    return accountString;
}