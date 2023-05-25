#pragma once

#include <string>
#include <vector>

class Entity {
public:
    virtual ~Entity() {};
    virtual std::vector<std::string> toString() = 0;
    static std::string generateUuidV4();
};