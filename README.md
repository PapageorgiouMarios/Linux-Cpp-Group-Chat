# Linux C++ Group Chat

Multi-user chat application built with C++ threads and SQLite, designed for Linux environments.

## 🏗️ Project Architecture

### Core Components

- **Database Layer**: SQLite-based data persistence with comprehensive user, group, and message management
- **Server Layer**: C++ server implementation for handling client connections and message routing
- **Client Layer**: Chat client for real-time messaging

## 🚀 Phase 1: Database Implementation ✅

In this first phase, we successfully implemented and tested a complete database layer:

#### ✅ Database Features Implemented

1. **User Management**
2. **Group Management**
3. **Message Management**
4. **Data Integrity**

## 🐳 Docker Integration

You can run the database tests inside a containerized environment using Docker.

```bash
docker-compose -f docker/docker-compose.yml run --rm database-test