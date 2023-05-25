#pragma once

// #include <memory>
#include <string>
#include <vector>
#include "Repository.h"

class FileRepository : public Repository {
public:
    // static std::shared_ptr<FileRepository> getInstance(std::string fileName);
    FileRepository(std::string fileName);
    virtual ~FileRepository() {}

    void create(Entity& entity) override;
    void update(std::string uuid, Entity& newEntity) override;
    void remove(std::string uuid) override;
    std::vector<std::string> find(std::string uuid) const override;
    std::vector<std::vector<std::string>> findAll() const override;

private:
    // static std::shared_ptr<FileRepository> instance;
    std::string tableName;

    std::ofstream getOutputFile();
    std::ifstream getInputFile() const;
};