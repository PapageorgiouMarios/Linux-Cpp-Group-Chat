cmake_minimum_required(VERSION 3.10)
project(CppLinuxChat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SQLite3
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

# Include directories
include_directories(${SQLITE3_INCLUDE_DIRS})
include_directories(include)

# Database test executable (comprehensive test suite)
add_executable(test_database src/test_database.cpp src/database.cpp)
target_link_libraries(test_database ${SQLITE3_LIBRARIES})
target_compile_options(test_database PRIVATE ${SQLITE3_CFLAGS_OTHER})

# Simple database test executable (basic functionality)
add_executable(test_db_simple src/test_db.cpp src/database.cpp)
target_link_libraries(test_db_simple ${SQLITE3_LIBRARIES})
target_compile_options(test_db_simple PRIVATE ${SQLITE3_CFLAGS_OTHER}) 