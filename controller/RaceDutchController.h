#pragma once

#include "Controller.h"
#include "Dutch.h"
#include "Request.h"
#include "User.h"
#include <memory>
#include <string>
#include <vector>

class RaceDutchController : public Controller {
  public:
    static std::shared_ptr<RaceDutchController> getInstance(
        std::shared_ptr<Repository> userRepository, std::shared_ptr<Repository> accountRepository,
        std::shared_ptr<Repository> dutchRepository, std::shared_ptr<Repository> ledgerRepository);
    virtual ~RaceDutchController() {}

    void createRaceDutch(int sockfd, const Request &request);
    void payRaceDutch(int sockfd, const Request &request);
    // void doneRaceDutch(int sockfd, const Request &request);

  private:
    static std::shared_ptr<RaceDutchController> instance;
    RaceDutchController() {}

    std::shared_ptr<User> getUser(const std::string &uuid);
    std::vector<std::shared_ptr<User>> getUserList(const std::vector<std::string> &uuidList);

    std::shared_ptr<Repository> userRepository;
    std::shared_ptr<Repository> accountRepository;
    std::shared_ptr<Repository> dutchRepository;
    std::shared_ptr<Repository> ledgerRepository;
};