#!/bin/bash

echo "=== Simple Database Test ==="

# Create data directory if it doesn't exist
mkdir -p data

# Build and run the Docker container with simple test
echo "Building and running Docker container with simple test..."
cd docker
docker-compose run --rm database-test ./build/test_db_simple
cd ..

echo "=== Simple test completed! ===" 