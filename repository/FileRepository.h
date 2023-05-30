#pragma once

#include "Repository.h"
#include <mutex>
#include <string>
#include <vector>

class FileRepository : public Repository {
  public:
    FileRepository(std::string fileName);
    virtual ~FileRepository() {}

    void create(Entity &entity) override;
    void update(std::string uuid, Entity &newEntity) override;
    void remove(std::string uuid) override;
    std::vector<std::string> find(std::string uuid) const override;
    std::vector<std::vector<std::string>> findAll() const override;

  private:
    std::string tableName;
    mutable std::mutex mtx;

    std::ofstream getOutputFile();
    std::ifstream getInputFile() const;
};