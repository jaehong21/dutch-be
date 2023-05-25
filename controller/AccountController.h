#pragma once

#include <memory>
#include "Controller.h"
#include "Request.h"

class AccountController : public Controller {
public:
    static std::shared_ptr<AccountController> getInstance(
        std::shared_ptr<Repository> userRepository, std::shared_ptr<Repository> accountRepository);
    virtual ~AccountController() {}

    void updateUserAccount(int sockfd, const Request& request);

private:
    static std::shared_ptr<AccountController> instance;
    AccountController() {}

    std::shared_ptr<Repository> userRepository;
    std::shared_ptr<Repository> accountRepository;
};