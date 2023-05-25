#pragma once

#include <memory>
#include <string>
#include "Entity.h"
#include "User.h"

class Account : public Entity {
public:
    Account(std::string uuid, std::shared_ptr<User> owner, int money) 
        : uuid(uuid), owner(owner), money(money) {}
    virtual ~Account() {}
    std::shared_ptr<User> getOwner() const;
    std::string getUuid() const;
    void setMoney(int money);
    int getMoney() const;

private:
    std::string uuid;
    std::shared_ptr<User> owner;
    int money;
};

class UserAccount : public Account {
public:
    UserAccount(std::shared_ptr<User> owner, int money);
    virtual ~UserAccount() {}

    std::vector<std::string> toString() override;
};

class DutchAccount : public Account {
public: 
    DutchAccount(std::shared_ptr<User> owner, int money);
    virtual ~DutchAccount() {}

    std::vector<std::string> toString() override;
};