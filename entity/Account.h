#pragma once

#include "Dutch.h"
#include "Entity.h"
#include "User.h"
#include <memory>
#include <string>

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
    DutchAccount(std::shared_ptr<Dutch> dutch, std::shared_ptr<User> owner, int balance);
    virtual ~DutchAccount() {}

    std::shared_ptr<Dutch> getDutch() const;
    std::vector<std::string> toString() override;

  private:
    std::shared_ptr<Dutch> dutch;
};