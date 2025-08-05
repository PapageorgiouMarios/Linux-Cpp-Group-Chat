#pragma once
#include <string>
#include <ctime>
#include "user.h"

class Message 
{
public:
    Message(User* sender, const std::string& content, int group_id, bool isFile = false); 
    Message();
    ~Message(); 

    int message_id;
    User* sender; 
    int group_id;  // Which group this message belongs to
    std::string content; 
    std::string file_path;  // Path to file if it's a file message
    bool isFile; 
    time_t timestamp; 
    
    // Helper methods
    bool is_text_message() const;
    bool is_file_message() const;
    std::string get_sender_name() const;
};