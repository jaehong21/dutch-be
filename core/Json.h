#pragma once

#include <string>
#include <vector>

class Json {
  public:
    Json();
    ~Json() {}

    Json &add(std::string key, std::string value);
    Json &add(std::string key, int value);
    Json &add(std::string key, std::vector<std::string> value);
    std::string getJsonString();

  private:
    std::string jsonString;
};