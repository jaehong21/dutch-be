#pragma once

#include <memory>
#include "Controller.h"
#include "Request.h"

class UserController : public Controller {
public:
    static std::shared_ptr<UserController> getInstance(
        std::shared_ptr<Repository> userRepository, std::shared_ptr<Repository> accountRepository);
    virtual ~UserController() { 
        // as we are using shared_ptr, we don't need to delete instance
        // delete instance; 
    };

    void createUser(int sockfd, const Request& request);
    void findOneUser(int sockfd, const Request& request);

private:
    static std::shared_ptr<UserController> instance;
    UserController() {};

    std::shared_ptr<Repository> userRepository;
    std::shared_ptr<Repository> accountRepository;
};