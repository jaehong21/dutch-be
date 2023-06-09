#pragma once

#include "Controller.h"
#include "Dutch.h"
#include "Request.h"
#include "User.h"
#include <memory>
#include <string>
#include <vector>

class NormalDutchController : public Controller {
  public:
    static std::shared_ptr<NormalDutchController> getInstance(
        std::shared_ptr<Repository> userRepository, std::shared_ptr<Repository> accountRepository,
        std::shared_ptr<Repository> dutchRepository, std::shared_ptr<Repository> ledgerRepository);
    virtual ~NormalDutchController() {}

    void findOneDutch(int sockfd, const Request &request);
    void findAllDutch(int sockfd, const Request &request);
    void createNormalDutch(int sockfd, const Request &request);
    void payNormalDutch(int sockfd, const Request &request);
    void doneNormalDutch(int sockfd, const Request &request);

  private:
    static std::shared_ptr<NormalDutchController> instance;
    NormalDutchController() {}

    std::shared_ptr<User> getUser(const std::string &uuid);
    std::vector<std::shared_ptr<User>> getUserList(const std::vector<std::string> &uuidList);

    std::shared_ptr<Repository> userRepository;
    std::shared_ptr<Repository> accountRepository;
    std::shared_ptr<Repository> dutchRepository;
    std::shared_ptr<Repository> ledgerRepository;
};