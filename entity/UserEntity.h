#pragma once

#include <string>
#include "Entity.h"

class UserEntity : public Entity {
public:
    UserEntity(std::string username, std::string password, std::string email);
    UserEntity(std::string uuid, std::string username, std::string password, std::string email);
    virtual ~UserEntity() {};
    
    std::vector<std::string> toString() override;
    UserEntity toEntity(std::string entityString);

    std::string getUuid();
    std::string getUsername();
    std::string getEmail();

private:
    std::string uuid;
    std::string username;
    std::string password;
    std::string email;
};