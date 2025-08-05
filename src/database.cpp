#include "../include/database.h"
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <ctime>

// Helper function to execute SQL and handle errors
bool execute_sql(sqlite3* db, const std::string& sql, const std::string& error_msg) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << error_msg << ": " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool init_db(const std::string& db_name) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Create Users table
    const char* users_sql = "CREATE TABLE IF NOT EXISTS Users ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "username TEXT UNIQUE NOT NULL,"
                           "password TEXT NOT NULL,"
                           "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
                           ");";

    // Create Groups table
    const char* groups_sql = "CREATE TABLE IF NOT EXISTS Groups ("
                            "group_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "name TEXT NOT NULL UNIQUE,"
                            "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                            "updated_at DATETIME DEFAULT CURRENT_TIMESTAMP"
                            ");";

    // Create GroupMembers table (many-to-many relationship)
    const char* group_members_sql = "CREATE TABLE IF NOT EXISTS GroupMembers ("
                                   "group_id INTEGER,"
                                   "user_id INTEGER,"
                                   "joined_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                                   "PRIMARY KEY (group_id, user_id),"
                                   "FOREIGN KEY (group_id) REFERENCES Groups(group_id),"
                                   "FOREIGN KEY (user_id) REFERENCES Users(id)"
                                   ");";

    // Create Messages table with group_id relationship
    const char* messages_sql = "CREATE TABLE IF NOT EXISTS Messages ("
                              "message_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "sender_id INTEGER,"
                              "group_id INTEGER NOT NULL,"
                              "text TEXT,"
                              "file_path TEXT,"
                              "sent_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                              "FOREIGN KEY (sender_id) REFERENCES Users(id),"
                              "FOREIGN KEY (group_id) REFERENCES Groups(group_id)"
                              ");";

    // Execute all table creation statements
    if (!execute_sql(db, users_sql, "Failed to create Users table")) {
        sqlite3_close(db);
        return false;
    }

    if (!execute_sql(db, groups_sql, "Failed to create Groups table")) {
        sqlite3_close(db);
        return false;
    }

    if (!execute_sql(db, group_members_sql, "Failed to create GroupMembers table")) {
        sqlite3_close(db);
        return false;
    }

    if (!execute_sql(db, messages_sql, "Failed to create Messages table")) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    std::cout << "Database initialized successfully!" << std::endl;
    return true;
}

// User management functions
bool register_user(const std::string& db_name, const std::string& username, const std::string& password) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const std::string sql = "INSERT INTO Users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to register user: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::cout << "User registered successfully!" << std::endl;
    return true;
}

bool authenticate_user(const std::string& db_name, const std::string& username, const std::string& password) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const std::string sql = "SELECT id FROM Users WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    bool authenticated = (rc == SQLITE_ROW);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return authenticated;
}

int get_user_id(const std::string& db_name, const std::string& username) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    const std::string sql = "SELECT id FROM Users WHERE username = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    int user_id = -1;
    if (rc == SQLITE_ROW) {
        user_id = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return user_id;
}

std::string get_username(const std::string& db_name, int user_id) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    const std::string sql = "SELECT username FROM Users WHERE id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, user_id);

    rc = sqlite3_step(stmt);
    std::string username = "";
    if (rc == SQLITE_ROW) {
        username = (const char*)sqlite3_column_text(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return username;
}

// Group management functions
bool create_group(const std::string& db_name, const std::string& group_name) {
    // Input validation
    if (group_name.empty()) {
        std::cerr << "Group name cannot be empty" << std::endl;
        return false;
    }
    
    if (group_name.length() > 100) { // Reasonable limit
        std::cerr << "Group name too long (max 100 characters)" << std::endl;
        return false;
    }
    
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Create the group (let SQLite handle the auto-increment)
    const std::string create_sql = "INSERT INTO Groups (name) VALUES (?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, create_sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, group_name.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    
    bool success = false;
    if (rc == SQLITE_CONSTRAINT) {
        std::cerr << "Group '" << group_name << "' already exists" << std::endl;
        success = false;
    } else if (rc == SQLITE_DONE) {
        std::cout << "Group created successfully!" << std::endl;
        success = true;
    } else {
        std::cerr << "Failed to create group: " << sqlite3_errmsg(db) << std::endl;
        success = false;
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return success;
}

bool add_user_to_group(const std::string& db_name, int user_id, int group_id) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const std::string sql = "INSERT INTO GroupMembers (group_id, user_id) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_int(stmt, 2, user_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to add user to group" << std::endl;
        return false;
    }

    return true;
}

bool remove_user_from_group(const std::string& db_name, int user_id, int group_id) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const std::string sql = "DELETE FROM GroupMembers WHERE group_id = ? AND user_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_int(stmt, 2, user_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to remove user from group" << std::endl;
        return false;
    }

    return true;
}

std::vector<int> get_user_groups(const std::string& db_name, int user_id) {
    std::vector<int> groups;
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return groups;
    }

    const std::string sql = "SELECT group_id FROM GroupMembers WHERE user_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return groups;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        groups.push_back(sqlite3_column_int(stmt, 0));
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return groups;
}

std::vector<int> get_group_members(const std::string& db_name, int group_id) {
    std::vector<int> members;
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return members;
    }

    const std::string sql = "SELECT user_id FROM GroupMembers WHERE group_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return members;
    }

    sqlite3_bind_int(stmt, 1, group_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        members.push_back(sqlite3_column_int(stmt, 0));
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return members;
}

std::string get_group_name(const std::string& db_name, int group_id) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    const std::string sql = "SELECT name FROM Groups WHERE group_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, group_id);

    rc = sqlite3_step(stmt);
    std::string group_name = "";
    if (rc == SQLITE_ROW) {
        group_name = (const char*)sqlite3_column_text(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return group_name;
}

// Message management functions
bool save_message(const std::string& db_name, int sender_id, int group_id, const std::string& text, const std::string& file_path) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const std::string sql = "INSERT INTO Messages (sender_id, group_id, text, file_path) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, sender_id);
    sqlite3_bind_int(stmt, 2, group_id);
    sqlite3_bind_text(stmt, 3, text.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, file_path.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to save message" << std::endl;
        return false;
    }

    return true;
}

std::vector<std::pair<std::string, std::string>> get_group_messages(const std::string& db_name, int group_id, int limit) {
    std::vector<std::pair<std::string, std::string>> messages;
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return messages;
    }

    const std::string sql = "SELECT u.username, m.text FROM Messages m "
                           "JOIN Users u ON m.sender_id = u.id "
                           "WHERE m.group_id = ? "
                           "ORDER BY m.sent_at DESC "
                           "LIMIT ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return messages;
    }

    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_int(stmt, 2, limit);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string username = (const char*)sqlite3_column_text(stmt, 0);
        std::string text = (const char*)sqlite3_column_text(stmt, 1);
        messages.push_back({username, text});
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return messages;
}

// Group-Message relationship functions
std::vector<int> get_group_message_ids(const std::string& db_name, int group_id) {
    std::vector<int> message_ids;
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return message_ids;
    }

    const std::string sql = "SELECT message_id FROM Messages WHERE group_id = ? ORDER BY sent_at DESC;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return message_ids;
    }

    sqlite3_bind_int(stmt, 1, group_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        message_ids.push_back(sqlite3_column_int(stmt, 0));
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return message_ids;
}

bool add_message_to_group(const std::string& db_name, int message_id, int group_id) {
    // This function is mainly for consistency - messages are automatically added to groups
    // when they are saved via save_message()
    return true;
}

bool remove_message_from_group(const std::string& db_name, int message_id, int group_id) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const std::string sql = "DELETE FROM Messages WHERE message_id = ? AND group_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, message_id);
    sqlite3_bind_int(stmt, 2, group_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to remove message from group" << std::endl;
        return false;
    }

    return true;
}

int get_message_group_id(const std::string& db_name, int message_id) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    const std::string sql = "SELECT group_id FROM Messages WHERE message_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_int(stmt, 1, message_id);

    rc = sqlite3_step(stmt);
    int group_id = -1;
    if (rc == SQLITE_ROW) {
        group_id = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return group_id;
}

int get_message_count_in_group(const std::string& db_name, int group_id) {
    sqlite3* db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    const std::string sql = "SELECT COUNT(*) FROM Messages WHERE group_id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_int(stmt, 1, group_id);

    rc = sqlite3_step(stmt);
    int count = 0;
    if (rc == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return count;
} 