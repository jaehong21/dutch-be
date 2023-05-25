#include <string>
#include "UserEntity.h"

using namespace std;

UserEntity::UserEntity(string username, string password, string email)
    : uuid(Entity::generateUuidV4()), username(username), password(password), email(email) {}
    
UserEntity::UserEntity(string uuid, string username, string password, string email)
    : uuid(uuid), username(username), password(password), email(email) {}

vector<string> UserEntity::toString() {
    vector<string> userString;
    userString.push_back(uuid);
    userString.push_back(username);
    userString.push_back(password);
    userString.push_back(email);
    return userString;
}

UserEntity UserEntity::toEntity(string entityString) {
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
    
    return UserEntity(userString[0], userString[1], userString[2], userString[3]);
}

string UserEntity::getUuid() { return uuid; }
string UserEntity::getUsername() { return username; }
string UserEntity::getEmail() { return email; }