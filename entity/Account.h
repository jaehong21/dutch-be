#pragma once

#include <memory>
#include <string>
#include "Entity.h"
#include "User.h"

class Account : public Entity {
public:
    Account(int money) : money(money) {}
    virtual ~Account() {}
    void setMoney(int money);
    int getMoney() const;

private:
    int money;
};

class UserAccount: public Account {
public:
    UserAccount(std::shared_ptr<User> owner, int money);
    virtual ~UserAccount() {}
    void setOwner(std::shared_ptr<User> owner);
    std::shared_ptr<User> getOwner() const;
    std::string getUuid() const;

    std::vector<std::string> toString() override;

private:
    std::string uuid;
    std::shared_ptr<User> owner;
};