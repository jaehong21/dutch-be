#include "User.h"
#include <string>
#include <vector>

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

string User::getUuid() const { return uuid; }
string User::getUsername() const { return username; }
string User::getEmail() const { return email; }