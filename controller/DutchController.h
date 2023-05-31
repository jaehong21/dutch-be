#pragma once

#include "Controller.h"
#include "Dutch.h"
#include "Request.h"
#include "User.h"
#include <memory>
#include <string>
#include <vector>

class DutchController : public Controller {
  public:
    static std::shared_ptr<DutchController> getInstance(
        std::shared_ptr<Repository> userRepository, std::shared_ptr<Repository> accountRepository,
        std::shared_ptr<Repository> dutchRepository, std::shared_ptr<Repository> ledgerRepository);
    virtual ~DutchController() {}

    void findOneNormalDutch(int sockfd, const Request &request);
    void createNormalDutch(int sockfd, const Request &request);
    void payNormalDutch(int sockfd, const Request &request);

  private:
    static std::shared_ptr<DutchController> instance;
    DutchController() {}

    std::shared_ptr<User> getUser(const std::string &uuid);
    std::vector<std::shared_ptr<User>> getUserList(const std::vector<std::string> &uuidList);

    std::shared_ptr<Repository> userRepository;
    std::shared_ptr<Repository> accountRepository;
    std::shared_ptr<Repository> dutchRepository;
    std::shared_ptr<Repository> ledgerRepository;
};