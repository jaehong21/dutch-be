#include "FileRepository.h"
#include "Entity.h"
#include "HttpException.h"
#include <cstdio>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

using std::string, std::vector;

FileRepository::FileRepository(string fileName) : tableName(fileName) {
    std::ifstream file(fileName);
    if (!file.good()) {
        std::ofstream newFile(fileName);
        newFile.close();
    }
}

void FileRepository::create(Entity &entity) {
    std::lock_guard<std::mutex> guard(this->mtx);
    std::ofstream out = this->getOutputFile();

    vector<string> entityString = entity.toString();
    for (const string &str : entityString) {
        out << str;
        // If this is not the last string in the vector, output a comma separator
        if (&str != &entityString.back()) {
            out << ",";
        }
    }
    // Output a newline character to separate the entity from the next one
    out << std::endl;
    out.close();
}

void FileRepository::update(string uuid, Entity &entity) {
    std::lock_guard<std::mutex> guard(this->mtx);

    // Temporary file
    string tempFileName = Entity::generateUuidV4() + ".txt";
    std::ofstream tempFile(tempFileName);
    std::ifstream inputFile = this->getInputFile();

    if (!tempFile.is_open() || !inputFile.is_open()) {
        throw InternalServerErrorException("File not found");
    }

    string line;
    // Go through each line in the input file
    while (getline(inputFile, line)) {
        std::stringstream ss(line);
        string token;

        // Get the uuid from the line
        getline(ss, token, ',');

        // If the line starts with the provided uuid, update the line
        if (token == uuid) {
            vector<string> entityString = entity.toString();
            for (const string &str : entityString) {
                tempFile << str;

                // If this is not the last string in the vector, output a comma separator
                if (&str != &entityString.back()) {
                    tempFile << ",";
                }
            }
        } else {
            // If the line does not start with the provided uuid, just write it to the temp file
            tempFile << line;
        }
        tempFile << std::endl;
    }

    // Close the input and temp files
    inputFile.close();
    tempFile.close();

    // Remove the original file and rename the temp file to the original file's name
    remove(this->tableName.c_str());
    rename(tempFileName.c_str(), this->tableName.c_str());
}

void FileRepository::remove(string uuid) {}

vector<string> FileRepository::find(string uuid) const {
    std::lock_guard<std::mutex> guard(this->mtx);
    std::ifstream in = this->getInputFile();

    bool found = false;
    vector<string> result;
    string line;

    // Split the line using comma as the separator
    while (!found && getline(in, line)) {
        std::stringstream ss(line);
        string token;

        result.clear();
        while (getline(ss, token, ',')) {
            if (token == uuid) {
                found = true;
            }
            // Add each token to the results vector
            result.push_back(token);
        }
    }

    in.close();
    return found ? result : vector<string>();
}

vector<vector<string>> FileRepository::findAll() const {
    std::lock_guard<std::mutex> guard(this->mtx);
    std::ifstream in = this->getInputFile();

    vector<vector<string>> result;
    string line;

    // Split the line using comma as the separator
    while (getline(in, line)) {
        std::stringstream ss(line);
        string token;
        vector<string> entityString;

        while (getline(ss, token, ',')) {
            // Add each token to the results vector
            entityString.push_back(token);
        }
        result.push_back(entityString);
    }

    in.close();
    return result;
}

std::ofstream FileRepository::getOutputFile() {
    std::ofstream outputFile(this->tableName, std::ios::app);
    if (!outputFile.is_open()) {
        throw InternalServerErrorException("File not found");
    } else {
        return outputFile;
    }
}

std::ifstream FileRepository::getInputFile() const {
    std::ifstream inputFile(this->tableName);
    if (!inputFile.is_open()) {
        throw InternalServerErrorException("File not found");
    } else {
        return inputFile;
    }
}