# NCTU Student Information System (SIS)

A comprehensive desktop Student Information System for New Cairo Technological University (NCTU), built with Qt6 and C++17, using SQLite as the database.

## Features

### Multi-Role System
- **Admin Portal**: Full CRUD operations for students, courses, programs, and departments. Manage enrollments, grades, payments, and news.
- **Student Portal**: View profile, registered courses, grades, attendance, payments, and announcements.
- **Doctor Portal**: View assigned courses, enrolled students, enter grades, and mark attendance.

### Core Functionality
- User authentication with password hashing
- Student enrollment and course registration workflow
- Grade management with letter grade calculation
- Attendance tracking with automatic barred status
- Payment records and receipts
- News and announcements system
- PMD (Pass/Medical/Delayed) records handling
- Exam scheduling and results

## Requirements

- Qt6 (Core, Widgets, Sql)
- CMake 3.16 or higher
- C++17 compatible compiler
- SQLite3 (included with Qt)

## Building the Project

### Linux/WSL

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make

# Run
./NCTU_SIS
```

### Windows

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake (adjust generator if needed)
cmake .. -G "MinGW Makefiles"

# Build
cmake --build .

# Run
.\NCTU_SIS.exe
```

## Database Setup

The database is automatically initialized on first run. The application will:
1. Create the SQLite database file in the application data directory
2. Execute the schema script (`db/sqlite_create.sql`)
3. Load seed data (`db/seed_data.sql`) if available

### Manual Database Initialization

If you need to manually initialize the database:

```bash
sqlite3 nctu_sis.db < db/sqlite_create.sql
sqlite3 nctu_sis.db < db/seed_data.sql
```

## Default Login Credentials

### Admin
- **Username**: `admin@nctu.edu.eg`
- **Password**: `admin123`

### Doctor
- **Username**: `ahmed.mohamed@nctu.edu.eg`
- **Password**: `doctor123`

### Student
- **Username**: `STU2024001`
- **Password**: `student123`

**Note**: These are test credentials. Change passwords in production!

## Project Structure

```
NCTU_SIS/
├── app/                        # Application source
│   ├── main.cpp               # Entry point
│   ├── AppMainWindow/         # Main window controller
│   ├── Login/                 # Login UI and controller
│   ├── AdminWindow/           # Admin portal
│   ├── StudentWindow/         # Student portal
│   ├── DoctorWindow/          # Doctor portal
│   ├── Models/                # Data models
│   ├── DAL/                   # Data Access Layer (repositories)
│   ├── Services/              # Business logic services
│   └── Utils/                 # Utility classes
├── db/                        # Database scripts
│   ├── sqlite_create.sql     # Database schema
│   └── seed_data.sql         # Test data
├── CMakeLists.txt            # Build configuration
└── README.md                 # This file
```

## Architecture

The application follows a layered architecture:

1. **Models**: Plain C++ classes representing database entities
2. **DAL (Data Access Layer)**: Repository classes for database operations
3. **Services**: Business logic layer (authentication, grading, attendance, PMD)
4. **UI**: Qt Widgets-based user interface

## Key Components

### DatabaseManager
Singleton class managing SQLite database connection and transactions.

### Repositories
- `StudentRepo`: Student CRUD operations
- `CourseRepo`: Course management
- `EnrollmentRepo`: Enrollment workflow
- `GradeRepo`: Grade management
- `AttendanceRepo`: Attendance tracking
- `PaymentRepo`: Payment records
- `NewsRepo`: News and announcements
- `DoctorRepo`: Doctor management
- `AdminRepo`: Admin management

### Services
- `AuthService`: Authentication and password management
- `GradingService`: Grade calculations and letter grade conversion
- `AttendanceService`: Attendance percentage and barred status
- `PMDService`: PMD record management

## Development Notes

### Adding New Features

1. **New Entity**: Create model class in `Models/`, repository in `DAL/`
2. **Business Logic**: Add service methods in `Services/`
3. **UI**: Add widgets/windows as needed

### Database Migrations

For schema changes, update `db/sqlite_create.sql` and handle migration in `DatabaseManager::initialize()`.

## Troubleshooting

### Database Connection Issues
- Ensure SQLite driver is available: `QSqlDatabase::drivers()` should include "QSQLITE"
- Check file permissions for database directory

### Build Issues
- Ensure Qt6 is properly installed and CMake can find it
- Set `CMAKE_PREFIX_PATH` if Qt is in non-standard location:
  ```bash
  cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt6
  ```

## License

This project is developed for New Cairo Technological University.

## Contributing

This is an internal project. For contributions, contact the development team.

