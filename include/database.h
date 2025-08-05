#pragma once
#include <string>
#include <vector>

// Database initialization
bool init_db(const std::string& db_name);

// User management
bool register_user(const std::string& db_name, const std::string& username, const std::string& password);
bool authenticate_user(const std::string& db_name, const std::string& username, const std::string& password);
int get_user_id(const std::string& db_name, const std::string& username);
std::string get_username(const std::string& db_name, int user_id);

// Group management
bool create_group(const std::string& db_name, const std::string& group_name);
bool add_user_to_group(const std::string& db_name, int user_id, int group_id);
bool remove_user_from_group(const std::string& db_name, int user_id, int group_id);
std::vector<int> get_user_groups(const std::string& db_name, int user_id);
std::vector<int> get_group_members(const std::string& db_name, int group_id);
std::string get_group_name(const std::string& db_name, int group_id);

// Message management
bool save_message(const std::string& db_name, int sender_id, int group_id, const std::string& text, const std::string& file_path = "");
std::vector<std::pair<std::string, std::string>> get_group_messages(const std::string& db_name, int group_id, int limit = 50);

// Group-Message relationship functions
std::vector<int> get_group_message_ids(const std::string& db_name, int group_id);
bool add_message_to_group(const std::string& db_name, int message_id, int group_id);
bool remove_message_from_group(const std::string& db_name, int message_id, int group_id);
int get_message_group_id(const std::string& db_name, int message_id);
int get_message_count_in_group(const std::string& db_name, int group_id);