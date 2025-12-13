#!/bin/bash
# Setup script for NCTU SIS in WSL environment

set -e

echo "=== NCTU SIS WSL Setup ==="
echo ""

# Get Windows host IP
WINDOWS_HOST=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}')
echo "Windows host IP: $WINDOWS_HOST"

# Install MySQL client if not installed
if ! command -v mysql &> /dev/null; then
    echo "Installing MySQL client..."
    sudo apt-get update
    sudo apt-get install -y mysql-client
else
    echo "MySQL client already installed"
fi

# Test MySQL connection
echo ""
echo "Testing MySQL connection to Windows MySQL server..."
mysql -h "$WINDOWS_HOST" -u root -pyaseentarek -e "SELECT VERSION();" 2>/dev/null && {
    echo "✓ MySQL connection successful!"
} || {
    echo "✗ MySQL connection failed. Trying localhost..."
    mysql -h localhost -u root -pyaseentarek -e "SELECT VERSION();" 2>/dev/null && {
        echo "✓ MySQL connection via localhost successful!"
        WINDOWS_HOST="localhost"
    } || {
        echo "✗ Connection failed. Please check:"
        echo "  1. MySQL is running on Windows"
        echo "  2. MySQL allows remote connections"
        echo "  3. Firewall settings"
        exit 1
    }
}

# Get project directory
PROJECT_DIR="/mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net"
cd "$PROJECT_DIR"

# Create database
echo ""
echo "Creating database and tables..."
mysql -h "$WINDOWS_HOST" -u root -pyaseentarek < db/mysql_create.sql && {
    echo "✓ Database created successfully!"
} || {
    echo "✗ Database creation failed (might already exist)"
}

# Seed data
echo ""
echo "Seeding test data..."
mysql -h "$WINDOWS_HOST" -u root -pyaseentarek < db/seed_data.sql && {
    echo "✓ Test data seeded successfully!"
} || {
    echo "✗ Seeding failed (might already exist)"
}

# Install Qt and build tools
echo ""
echo "Checking Qt installation..."
if ! command -v qmake &> /dev/null && ! command -v cmake &> /dev/null; then
    echo "Installing build tools..."
    sudo apt-get install -y build-essential cmake qt6-base-dev qt6-base-dev-tools libqt6sql6-mysql
else
    echo "Build tools already installed"
fi

# Check CMake
if ! command -v cmake &> /dev/null; then
    echo "Installing CMake..."
    sudo apt-get install -y cmake
fi

echo ""
echo "=== Setup Complete ==="
echo ""
echo "To build the project, run:"
echo "  cd $PROJECT_DIR"
echo "  mkdir -p build && cd build"
echo "  cmake .."
echo "  cmake --build ."
echo ""
echo "MySQL connection details:"
echo "  Host: $WINDOWS_HOST"
echo "  User: root"
echo "  Password: yaseentarek"
echo "  Database: nctu_sis"

