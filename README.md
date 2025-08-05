# 🗨️ Linux C++ Group Chat
![C++](https://img.shields.io/badge/C%2B%2B-blue.svg?logo=c%2B%2B&logoColor=white)
![SQLite](https://img.shields.io/badge/SQLite-003B57.svg?logo=sqlite&logoColor=white)
![Docker](https://img.shields.io/badge/Docker-2496ED.svg?logo=docker&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624.svg?logo=linux&logoColor=black)
---
Our goal is to create a multi-user chat application built with C++ threads and SQLite, designed for Linux environments 🐧💬

## 🏗️ Project Architecture

### 🧱 Core Components

- **🗂️ Database Layer**: SQLite-based data persistence with comprehensive user, group, and message management  
- **🖥️ Server Layer**: C++ server implementation for handling client connections and message routing  
- **💻 Client Layer**: Multiple clients for real-time messaging  

---

## 🚀 Phase 1: Database Implementation ✅

In this first phase, we successfully implemented and tested a complete database layer 🧪📦:

### ✅ Database Features Implemented

1. 👤 **User Management**  
2. 👥 **Group Management**  
3. 💬 **Message Management**  
4. 🛡️ **Data Integrity**

---

## 🐳 Docker Integration

You can run the database tests inside a containerized environment using Docker 🧪🐳:

```bash
docker-compose -f docker/docker-compose.yml run --rm database-test
