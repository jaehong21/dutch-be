#pragma once

#include <string>
#include "Entity.h"

class User : public Entity {
public:
    User(std::string username, std::string password, std::string email);
    User(std::string uuid, std::string username, std::string password, std::string email);
    virtual ~User() {}

    std::string getUuid() const;
    std::string getUsername() const;
    std::string getEmail() const;

    std::vector<std::string> toString() override;

private:
    std::string uuid;
    std::string username;
    std::string password;
    std::string email;
};