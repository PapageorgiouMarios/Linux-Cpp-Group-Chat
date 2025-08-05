#pragma once
#include <string>
#include <vector>
#include <ctime>

class Group 
{
public:
    Group(int group_id, const std::string& name);
    Group();
    ~Group(); 

    int group_id;
    std::string name;
    std::vector<int> member_ids; 
    std::vector<int> message_ids;  // Message IDs in this group
    time_t created_at;
    time_t updated_at;
    
    void add_member(int user_id);
    void remove_member(int user_id);
    bool is_member(int user_id) const;
    bool has_member(int user_id) const;
    void add_message(int message_id);
    void remove_message(int message_id);
    bool has_message(int message_id) const;
    int get_message_count() const;
    int get_member_count() const;
}; 