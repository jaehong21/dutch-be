#pragma once

#include <string>
#include "Entity.h"

class User : public Entity {
public:
    User(std::string username, std::string password, std::string email);
    User(std::string uuid, std::string username, std::string password, std::string email);
    virtual ~User() {};
    
    std::vector<std::string> toString() override;
    User toEntity(std::string entityString);

    std::string getUuid();
    std::string getUsername();
    std::string getEmail();

private:
    std::string uuid;
    std::string username;
    std::string password;
    std::string email;
};