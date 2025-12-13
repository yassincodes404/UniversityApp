-- SQL script to configure MySQL on Windows for WSL access
-- Run this in MySQL on Windows: mysql -u root -p < setup_mysql_windows.sql

-- Allow root to connect from any host (for WSL)
CREATE USER IF NOT EXISTS 'root'@'%' IDENTIFIED BY 'yaseentarek';
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;

-- Also ensure localhost access
CREATE USER IF NOT EXISTS 'root'@'localhost' IDENTIFIED BY 'yaseentarek';
GRANT ALL PRIVILEGES ON *.* TO 'root'@'localhost' WITH GRANT OPTION;

FLUSH PRIVILEGES;

-- Show current users
SELECT User, Host FROM mysql.user WHERE User = 'root';

