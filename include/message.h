#pragma once
#include <string>
#include <ctime>
#include "user.h"

class Message 
{
public:

    Message(User* sender, const std::string& content, int group_id, bool isFile = false); // Constructor 
    Message(); // Default Construtor
    ~Message(); // Destructor

    int message_id; // Primary key for messages
    User* sender; 
    int group_id;  // Which group this message belongs to
    std::string content; 
    std::string file_path;  // Path to file if it's a file message
    bool isFile; // Check if the message contains a file 
    time_t timestamp; 
    
    // Helper methods
    bool is_text_message() const;
    bool is_file_message() const;
    std::string get_sender_name() const;
};