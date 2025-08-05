#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include "user.h"

class Server 
{
public:
    Server(int port); 
    ~Server(); 

    void start(); 
    void stop(); 

private:
    int server_id; 
    int port; 
    std::vector<User*> users; 
    std::mutex users_mutex; 

    void acceptClients(); 
    void handleClient(User* user); 
    void broadcast(const std::string& message, User* sender); 
    int get_user_count(); 
};