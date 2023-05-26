#pragma once

#include <memory>
#include "Controller.h"
#include "Request.h"

class DutchController : public Controller {
public:
    static std::shared_ptr<DutchController> getInstance(
        std::shared_ptr<Repository> userRepository, 
        std::shared_ptr<Repository> accountRepository,
        std::shared_ptr<Repository> dutchRepository,
        std::shared_ptr<Repository> ledgerRepository);
    virtual ~DutchController() {}

    void createNormalDutch(int sockfd, const Request& request);

private:
    static std::shared_ptr<DutchController> instance;
    DutchController() {}

    std::shared_ptr<Repository> userRepository;
    std::shared_ptr<Repository> accountRepository;
    std::shared_ptr<Repository> dutchRepository;
    std::shared_ptr<Repository> ledgerRepository;
};