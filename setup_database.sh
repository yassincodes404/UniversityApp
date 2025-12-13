#!/bin/bash
# Setup database from WSL - connects to MySQL on Windows

set -e

# Get Windows host IP
WINDOWS_HOST=$(cat /etc/resolv.conf 2>/dev/null | grep nameserver | awk '{print $2}' || echo "localhost")

MYSQL_USER="root"
MYSQL_PASS="yaseentarek"
MYSQL_DB="nctu_sis"

PROJECT_DIR="/mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net"

echo "=== NCTU SIS Database Setup ==="
echo "Connecting to MySQL on Windows host: $WINDOWS_HOST"
echo ""

# Try different connection methods
connect_mysql() {
    local host=$1
    echo "Trying to connect to MySQL at $host..."
    
    mysql -h "$host" -u "$MYSQL_USER" -p"$MYSQL_PASS" -e "SELECT 1;" 2>/dev/null && {
        echo "✓ Connection successful to $host"
        return 0
    } || {
        echo "✗ Connection failed to $host"
        return 1
    }
}

# Try Windows host IP first
if ! connect_mysql "$WINDOWS_HOST"; then
    # Try localhost
    if ! connect_mysql "localhost"; then
        echo ""
        echo "ERROR: Cannot connect to MySQL on Windows"
        echo ""
        echo "Please ensure:"
        echo "1. MySQL is running on Windows"
        echo "2. MySQL allows remote connections"
        echo ""
        echo "To fix, run on Windows (in PowerShell as Admin):"
        echo "  mysql -u root -pyaseentarek < setup_mysql_windows.sql"
        echo ""
        echo "Or manually in MySQL:"
        echo "  CREATE USER 'root'@'%' IDENTIFIED BY 'yaseentarek';"
        echo "  GRANT ALL PRIVILEGES ON *.* TO 'root'@'%';"
        echo "  FLUSH PRIVILEGES;"
        exit 1
    else
        WINDOWS_HOST="localhost"
    fi
fi

cd "$PROJECT_DIR"

echo ""
echo "Creating database and tables..."
mysql -h "$WINDOWS_HOST" -u "$MYSQL_USER" -p"$MYSQL_PASS" < db/mysql_create.sql 2>/dev/null && {
    echo "✓ Database created successfully!"
} || {
    echo "⚠ Database might already exist (this is OK)"
}

echo ""
echo "Seeding test data..."
mysql -h "$WINDOWS_HOST" -u "$MYSQL_USER" -p"$MYSQL_PASS" "$MYSQL_DB" < db/seed_data.sql 2>/dev/null && {
    echo "✓ Test data seeded successfully!"
} || {
    echo "⚠ Seeding might have failed (data might already exist)"
}

echo ""
echo "=== Database Setup Complete ==="
echo ""
echo "Connection details for the application:"
echo "  Host: $WINDOWS_HOST"
echo "  User: $MYSQL_USER"
echo "  Password: $MYSQL_PASS"
echo "  Database: $MYSQL_DB"

