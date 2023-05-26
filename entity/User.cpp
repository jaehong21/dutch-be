#include <string>
#include <vector>
#include "User.h"

using std::string, std::vector;

User::User(string username, string password, string email)
    : uuid(Entity::generateUuidV4()), username(username), password(password), email(email) {}
    
User::User(string uuid, string username, string password, string email)
    : uuid(uuid), username(username), password(password), email(email) {}

vector<string> User::toString() {
    vector<string> userString;
    userString.push_back(uuid);
    userString.push_back(username);
    userString.push_back(password);
    userString.push_back(email);
    return userString;
}

User User::toEntity(string entityString) {
    vector<string> userString;
    string delimiter = ",";
    size_t pos = 0;
    string token;
    while ((pos = entityString.find(delimiter)) != string::npos) {
        token = entityString.substr(0, pos);
        userString.push_back(token);
        entityString.erase(0, pos + delimiter.length());
    }
    userString.push_back(entityString);
    
    return User(userString[0], userString[1], userString[2], userString[3]);
}

string User::getUuid() const { return uuid; }
string User::getUsername() const { return username; }
string User::getEmail() const { return email; }