#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Repository.h"

class FileRepository : public Repository {
public:
    static std::shared_ptr<FileRepository> getInstance(std::string fileName);
    virtual ~FileRepository() { 
        /// as we are using shared_ptr, we don't need to delete instance
        // delete instance; 
    };

    void create(Entity& entity) override;
    void update(std::string uuid, Entity& newEntity) override;
    void remove(std::string uuid) override;
    std::vector<std::string> find(std::string uuid) const override;
    std::vector<std::vector<std::string>> findAll() const override;

private:
    static std::shared_ptr<FileRepository> instance;
    FileRepository() {};
    std::string tableName;

    std::ofstream getOutputFile();
    std::ifstream getInputFile() const;
};