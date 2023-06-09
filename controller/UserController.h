#pragma once

#include "Controller.h"
#include "Request.h"
#include <memory>

class UserController : public Controller {
  public:
    static std::shared_ptr<UserController>
    getInstance(std::shared_ptr<Repository> userRepository,
                std::shared_ptr<Repository> accountRepository);
    virtual ~UserController() {}

    void loginUser(int sockfd, const Request &request);
    void createUser(int sockfd, const Request &request);
    void updateUser(int sockfd, const Request &request);
    void findOneUser(int sockfd, const Request &request);
    void findAllUser(int sockfd, const Request &request);

  private:
    static std::shared_ptr<UserController> instance;
    UserController() {}

    std::shared_ptr<Repository> userRepository;
    std::shared_ptr<Repository> accountRepository;
};