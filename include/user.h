#pragma once
#include <string>
#include <ctime>

class User 
{
public:
    User(int sock); 
    ~User(); 

    int socket; 
    std::string username;
    std::string password;
    bool is_active; 
    time_t created_at;
};