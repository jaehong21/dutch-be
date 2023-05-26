#pragma once

#include <memory>
#include <string>
#include "Entity.h"
#include "User.h"

class Account : public Entity {
public:
    Account(std::string uuid, std::shared_ptr<User> owner, int balance) 
        : uuid(uuid), owner(owner), balance(balance) {}
    virtual ~Account() {}
    std::shared_ptr<User> getOwner() const;
    std::string getUuid() const;
    void setMoney(int balance);
    int getMoney() const;

private:
    std::string uuid;
    std::shared_ptr<User> owner;
    int balance;
};

class UserAccount : public Account {
public:
    UserAccount(std::shared_ptr<User> owner, int balance);
    virtual ~UserAccount() {}

    std::vector<std::string> toString() override;
};

class DutchAccount : public Account {
public: 
    DutchAccount(std::shared_ptr<User> owner, int balance);
    virtual ~DutchAccount() {}

    std::vector<std::string> toString() override;
};