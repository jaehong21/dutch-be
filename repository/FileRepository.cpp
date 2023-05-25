#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include "FileRepository.h"
#include "Entity.h"

using namespace std;

shared_ptr<FileRepository> FileRepository::instance = nullptr;

shared_ptr<FileRepository> FileRepository::getInstance(string fileName) {
    if (instance.get() == nullptr) {
        instance = std::make_shared<FileRepository>(FileRepository());
        instance->tableName = fileName;
    }
    return instance;
}

void FileRepository::create(Entity& entity) {
    // Get an output file stream for the repository file
    ofstream out = this->getOutputFile();

    // Convert the entity to a vector of strings and output each string to the file
    vector<string> entityString = entity.toString();
    for (const string& str : entityString) {
        out << str;

        // If this is not the last string in the vector, output a comma separator
        if (&str != &entityString.back()) {
            out << ",";
        }
    }

    // Output a newline character to separate the entity from the next one
    out << endl;

    // Close the output file stream
    out.close();
}

void FileRepository::update(string uuid, Entity& entity) {
}

void FileRepository::remove(string uuid) {
    
}

vector<string> FileRepository::find(string uuid) const {
    ifstream in = this->getInputFile();

    bool found = false;
    vector<string> result;
    string line;

    // Split the line using comma as the separator
    while (!found && getline(in, line)) {  
        stringstream ss(line);
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
    ifstream in = this->getInputFile();

    vector<vector<string>> result;
    string line;

    // Split the line using comma as the separator
    while (getline(in, line)) { 
        stringstream ss(line);
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

ofstream FileRepository::getOutputFile() {
    ofstream outputFile(this->tableName, std::ios::app);
    if (!outputFile.is_open()) {
        
        throw "File not found";
    } else {
        return outputFile;
    }
}

ifstream FileRepository::getInputFile() const {
    ifstream inputFile(this->tableName);
    if (!inputFile.is_open()) {
        throw "File not found";
    } else {
        return inputFile;
    }
}