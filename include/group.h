#pragma once
#include <string>
#include <vector>
#include <ctime>

class Group 
{
public:

    Group(int group_id, const std::string& name); // Constructor
    Group(); // Default Constructor
    ~Group(); // Destructor

    int group_id; // Primary key of group
    std::string name;
    std::vector<int> member_ids; 
    std::vector<int> message_ids;  // Message IDs in this group
    time_t created_at;
    time_t updated_at;
    
    // Member Management
    void add_member(int user_id);
    void remove_member(int user_id);
    bool is_member(int user_id) const;
    bool has_member(int user_id) const;

    // Message Management
    void add_message(int message_id);
    void remove_message(int message_id);
    bool has_message(int message_id) const;
    int get_message_count() const;
    int get_member_count() const;
}; 