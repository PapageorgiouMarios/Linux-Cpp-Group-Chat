#include <iostream>
#include <string>
#include "database.h"

int main() {
    std::cout << "=== Simple Database Test ===" << std::endl;
    
    std::string dbPath = "data/chat.db";
    
    // Test database initialization
    std::cout << "Initializing database..." << std::endl;
    if (!init_db(dbPath)) {
        std::cout << "Failed to initialize database" << std::endl;
        return 1;
    }
    std::cout << "Database initialized successfully" << std::endl;
    
    // Test adding a user
    std::cout << "Adding test user..." << std::endl;
    bool userResult = register_user(dbPath, "testuser", "testpass");
    if (!userResult) {
        std::cout << "Failed to add user" << std::endl;
        return 1;
    }
    std::cout << "User added successfully" << std::endl;
    
    // Get user ID
    int userId = get_user_id(dbPath, "testuser");
    std::cout << "User ID: " << userId << std::endl;
    
    // Test adding a group
    std::cout << "Adding test group..." << std::endl;
    bool groupResult = create_group(dbPath, "testgroup");
    if (!groupResult) {
        std::cout << "Failed to add group" << std::endl;
        return 1;
    }
    std::cout << "Group added successfully" << std::endl;
    
    // Add user to group
    bool addToGroupResult = add_user_to_group(dbPath, userId, 1);
    if (!addToGroupResult) {
        std::cout << "Failed to add user to group" << std::endl;
        return 1;
    }
    std::cout << "User added to group successfully" << std::endl;
    
    // Test adding a message
    std::cout << "Adding test message..." << std::endl;
    bool msgResult = save_message(dbPath, userId, 1, "Hello, this is a test message!");
    if (!msgResult) {
        std::cout << "Failed to add message" << std::endl;
        return 1;
    }
    std::cout << "Message added successfully" << std::endl;
    
    // Test retrieving data
    std::cout << "Retrieving test data..." << std::endl;
    auto messages = get_group_messages(dbPath, 1, 10);
    auto userGroups = get_user_groups(dbPath, userId);
    auto groupMembers = get_group_members(dbPath, 1);
    
    std::cout << "Retrieved " << messages.size() << " messages" << std::endl;
    std::cout << "User is in " << userGroups.size() << " groups" << std::endl;
    std::cout << "Group has " << groupMembers.size() << " members" << std::endl;
    
    // Test user authentication
    std::cout << "Testing user authentication..." << std::endl;
    bool authResult = authenticate_user(dbPath, "testuser", "testpass");
    if (authResult) {
        std::cout << "User authentication successful" << std::endl;
    } else {
        std::cout << "User authentication failed" << std::endl;
        return 1;
    }
    
    // Test wrong password
    bool wrongAuthResult = authenticate_user(dbPath, "testuser", "wrongpass");
    if (!wrongAuthResult) {
        std::cout << "Wrong password correctly rejected" << std::endl;
    } else {
        std::cout << "Wrong password should have been rejected" << std::endl;
        return 1;
    }
    
    std::cout << "\n=== Simple Test Completed Successfully! ===" << std::endl;
    std::cout << "✓ Basic database operations working" << std::endl;
    std::cout << "✓ SQLite integration functional" << std::endl;
    
    return 0;
} 