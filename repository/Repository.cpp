#pragma once

#include "Entity.h"
#include <string>
#include <vector>

class Repository {
  public:
    virtual ~Repository() {}

    virtual void create(Entity &entity) = 0;
    virtual void update(std::string uuid, Entity &newEntity) = 0;
    virtual void remove(std::string uuid) = 0;
    virtual std::vector<std::string> find(std::string uuid) const = 0;
    virtual std::vector<std::vector<std::string>> findAll() const = 0;

  private:
    std::string tableName;
};