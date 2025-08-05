#include <iostream>
#include <string>
#include <cassert>
#include "database.h"

int main() {
    std::cout << "=== Comprehensive Database Test Suite ===" << std::endl;
    
    std::string dbPath = "data/chat.db";
    
    // Test 1: Database initialization
    std::cout << "\n1. Testing database initialization..." << std::endl;
    bool initResult = init_db(dbPath);
    if (initResult) {
        std::cout << "✓ Database initialized successfully" << std::endl;
    } else {
        std::cout << "✗ Database initialization failed" << std::endl;
        return 1;
    }
    
    // Test 2: User operations
    std::cout << "\n2. Testing user operations..." << std::endl;
    
    // Add users
    bool user1Result = register_user(dbPath, "alice", "password123");
    bool user2Result = register_user(dbPath, "bob", "password456");
    bool user3Result = register_user(dbPath, "charlie", "password789");
    
    if (user1Result && user2Result && user3Result) {
        std::cout << "✓ Users added successfully" << std::endl;
    } else {
        std::cout << "✗ User addition failed" << std::endl;
        return 1;
    }
    
    // Test duplicate user
    bool duplicateUserResult = register_user(dbPath, "alice", "password123");
    if (!duplicateUserResult) {
        std::cout << "✓ Duplicate user correctly rejected" << std::endl;
    } else {
        std::cout << "✗ Duplicate user should have been rejected" << std::endl;
    }
    
    // Test user authentication
    bool authResult = authenticate_user(dbPath, "alice", "password123");
    if (authResult) {
        std::cout << "✓ User authentication successful" << std::endl;
    } else {
        std::cout << "✗ User authentication failed" << std::endl;
    }
    
    // Test wrong password
    bool wrongAuthResult = authenticate_user(dbPath, "alice", "wrongpassword");
    if (!wrongAuthResult) {
        std::cout << "✓ Wrong password correctly rejected" << std::endl;
    } else {
        std::cout << "✗ Wrong password should have been rejected" << std::endl;
    }
    
    // Get user IDs
    int userId1 = get_user_id(dbPath, "alice");
    int userId2 = get_user_id(dbPath, "bob");
    int userId3 = get_user_id(dbPath, "charlie");
    
    std::cout << "✓ User IDs retrieved: " << userId1 << ", " << userId2 << ", " << userId3 << std::endl;
    
    // Test 3: Group operations
    std::cout << "\n3. Testing group operations..." << std::endl;
    
    // Add groups
    bool group1Result = create_group(dbPath, "general");
    bool group2Result = create_group(dbPath, "random");
    
    if (group1Result && group2Result) {
        std::cout << "✓ Groups added successfully" << std::endl;
    } else {
        std::cout << "✗ Group addition failed" << std::endl;
        return 1;
    }
    
    // Test duplicate group
    bool duplicateGroupResult = create_group(dbPath, "general");
    if (!duplicateGroupResult) {
        std::cout << "✓ Duplicate group correctly rejected" << std::endl;
    } else {
        std::cout << "✗ Duplicate group should have been rejected" << std::endl;
    }
    
    // Add users to groups
    bool addUser1ToGroup1 = add_user_to_group(dbPath, userId1, 1);
    bool addUser2ToGroup1 = add_user_to_group(dbPath, userId2, 1);
    bool addUser3ToGroup2 = add_user_to_group(dbPath, userId3, 2);
    
    if (addUser1ToGroup1 && addUser2ToGroup1 && addUser3ToGroup2) {
        std::cout << "✓ Users added to groups successfully" << std::endl;
    } else {
        std::cout << "✗ Adding users to groups failed" << std::endl;
    }
    
    // Test 4: Message operations
    std::cout << "\n4. Testing message operations..." << std::endl;
    
    // Add messages
    bool msg1Result = save_message(dbPath, userId1, 1, "Hello everyone!");
    bool msg2Result = save_message(dbPath, userId2, 1, "Hi Alice!");
    bool msg3Result = save_message(dbPath, userId3, 2, "Random message");
    
    if (msg1Result && msg2Result && msg3Result) {
        std::cout << "✓ Messages added successfully" << std::endl;
    } else {
        std::cout << "✗ Message addition failed" << std::endl;
        return 1;
    }
    
    // Test 5: Retrieve data
    std::cout << "\n5. Testing data retrieval..." << std::endl;
    
    // Get group messages
    auto messages = get_group_messages(dbPath, 1, 10);
    if (messages.size() >= 2) {
        std::cout << "✓ Retrieved " << messages.size() << " messages for group 1" << std::endl;
        for (const auto& msg : messages) {
            std::cout << "  - " << msg.first << ": " << msg.second << std::endl;
        }
    } else {
        std::cout << "✗ Failed to retrieve messages" << std::endl;
    }
    
    // Get user groups
    auto userGroups = get_user_groups(dbPath, userId1);
    if (userGroups.size() >= 1) {
        std::cout << "✓ User " << userId1 << " is in " << userGroups.size() << " groups" << std::endl;
    } else {
        std::cout << "✗ Failed to retrieve user groups" << std::endl;
    }
    
    // Get group members
    auto groupMembers = get_group_members(dbPath, 1);
    if (groupMembers.size() >= 2) {
        std::cout << "✓ Group 1 has " << groupMembers.size() << " members" << std::endl;
    } else {
        std::cout << "✗ Failed to retrieve group members" << std::endl;
    }
    
    // Test 6: Cleanup operations
    std::cout << "\n6. Testing cleanup operations..." << std::endl;
    
    // Remove user from group
    bool removeUserResult = remove_user_from_group(dbPath, userId3, 2);
    if (removeUserResult) {
        std::cout << "✓ User removed from group successfully" << std::endl;
    } else {
        std::cout << "✗ Failed to remove user from group" << std::endl;
    }
    
    // Verify removal
    auto updatedGroupMembers = get_group_members(dbPath, 2);
    if (updatedGroupMembers.empty()) {
        std::cout << "✓ Group 2 is now empty" << std::endl;
    } else {
        std::cout << "✗ Group 2 should be empty" << std::endl;
    }
    
    std::cout << "\n=== All tests completed successfully! ===" << std::endl;
    return 0;
} 