#pragma once

#include "Entity.h"
#include "User.h"
#include <memory>
#include <vector>

class Dutch : public Entity {
  public:
    Dutch(int targetBalance, std::shared_ptr<User> owner,
          std::vector<std::shared_ptr<User>> userList);
    Dutch(std::string uuid, int targetBalance, std::shared_ptr<User> owner,
          std::vector<std::shared_ptr<User>> userList,
          std::vector<std::shared_ptr<User>> sendUserList);
    virtual ~Dutch() {}

    std::string getUuid() const;
    int getTargetBalance() const;
    std::shared_ptr<User> getOwner() const;
    std::vector<std::shared_ptr<User>> getUserList() const;
    std::vector<std::shared_ptr<User>> getSendUserList() const;

  private:
    std::string uuid;
    int targetBalance;
    std::shared_ptr<User> owner;
    std::vector<std::shared_ptr<User>> userList;
    std::vector<std::shared_ptr<User>> sendUserList;
};

class NormalDutch : public Dutch {
  public:
    NormalDutch(int targetBalance, std::shared_ptr<User> owner,
                std::vector<std::shared_ptr<User>> userList);
    NormalDutch(std::string uuid, int targetBalance, std::shared_ptr<User> owner,
                std::vector<std::shared_ptr<User>> userList,
                std::vector<std::shared_ptr<User>> sendUserList);
    virtual ~NormalDutch() {}

    std::vector<std::string> toString() override;
};

class Ledger : public Entity {
  public:
    Ledger(std::shared_ptr<Dutch> dutch, std::shared_ptr<User> user, int amount);
    Ledger(std::shared_ptr<Dutch> dutch, std::shared_ptr<User> user, int amount, long sendTime);
    virtual ~Ledger() {}

    void setSendTime();
    std::vector<std::string> toString() override;

  private:
    std::shared_ptr<Dutch> dutch;
    std::shared_ptr<User> user;
    int amount;
    long sendTime;
};