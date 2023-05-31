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
    Ledger(std::string dutchUuid, std::string userUuid, int amount);
    Ledger(std::string uuid, std::string dutchUuid, std::string userUuid, int amount,
           long sendTime);
    virtual ~Ledger() {}

    static long getTimeNow();
    std::string getUuid() const;
    std::string getDutchUuid() const;
    std::string getUserUuid() const;
    std::vector<std::string> toString() override;

  private:
    std::string uuid;
    std::string dutchUuid;
    std::string userUuid;
    int amount;
    long sendTime;
};