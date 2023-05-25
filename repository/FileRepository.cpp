#include <memory>
#include <mutex>
#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "FileRepository.h"
#include "Entity.h"

using namespace std;

FileRepository::FileRepository(string fileName) : tableName(fileName) {}

void FileRepository::create(Entity& entity) {
    std::lock_guard<std::mutex> guard(this->mtx);
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
    std::lock_guard<std::mutex> guard(this->mtx);

    // Temporary file
    string tempFileName = Entity::generateUuidV4() + ".txt";
    ofstream tempFile(tempFileName);
    ifstream inputFile = this->getInputFile();

    if (!tempFile.is_open() || !inputFile.is_open()) {
        throw "File not found";
    }

    string line;

    // Go through each line in the input file
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string token;

        // Get the uuid from the line
        getline(ss, token, ',');

        // If the line starts with the provided uuid, update the line
        if (token == uuid) {
            vector<string> entityString = entity.toString();
            for (const string& str : entityString) {
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
        tempFile << endl;
    }

    // Close the input and temp files
    inputFile.close();
    tempFile.close();

    // Remove the original file and rename the temp file to the original file's name
    remove(this->tableName.c_str());
    rename(tempFileName.c_str(), this->tableName.c_str());
}

void FileRepository::remove(string uuid) {
}

vector<string> FileRepository::find(string uuid) const {
    std::lock_guard<std::mutex> guard(this->mtx);
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
    std::lock_guard<std::mutex> guard(this->mtx);
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