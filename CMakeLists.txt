cmake_minimum_required(VERSION 3.16)
project(NCTU_SIS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 or Qt5 components
find_package(Qt6 QUIET COMPONENTS Core Widgets Sql)
if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Sql)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

# Enable Qt MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/app)

# Source files
set(SOURCES
    app/main.cpp
    app/AppMainWindow/MainWindow.cpp
    app/Login/LoginController.cpp
    app/AdminWindow/AdminWindow.cpp
    app/StudentWindow/StudentWindow.cpp
    app/DoctorWindow/DoctorWindow.cpp
    app/Models/Student.cpp
    app/Models/Course.cpp
    app/Models/Doctor.cpp
    app/Models/Enrollment.cpp
    app/Models/Grade.cpp
    app/Models/Attendance.cpp
    app/Models/Payment.cpp
    app/Models/News.cpp
    app/Models/Exam.cpp
    app/DAL/DatabaseManager.cpp
    app/DAL/StudentRepo.cpp
    app/DAL/CourseRepo.cpp
    app/DAL/EnrollmentRepo.cpp
    app/DAL/GradeRepo.cpp
    app/DAL/AttendanceRepo.cpp
    app/DAL/PaymentRepo.cpp
    app/DAL/NewsRepo.cpp
    app/DAL/ExamRepo.cpp
    app/DAL/DoctorRepo.cpp
    app/DAL/AdminRepo.cpp
    app/Services/AuthService.cpp
    app/Services/GradingService.cpp
    app/Services/AttendanceService.cpp
    app/Services/PMDService.cpp
    app/Utils/PasswordHasher.cpp
    app/Utils/Validators.cpp
)

# Header files
set(HEADERS
    app/AppMainWindow/MainWindow.h
    app/Login/LoginController.h
    app/AdminWindow/AdminWindow.h
    app/StudentWindow/StudentWindow.h
    app/DoctorWindow/DoctorWindow.h
    app/Models/Student.h
    app/Models/Course.h
    app/Models/Doctor.h
    app/Models/Enrollment.h
    app/Models/Grade.h
    app/Models/Attendance.h
    app/Models/Payment.h
    app/Models/News.h
    app/Models/Exam.h
    app/DAL/DatabaseManager.h
    app/DAL/StudentRepo.h
    app/DAL/CourseRepo.h
    app/DAL/EnrollmentRepo.h
    app/DAL/GradeRepo.h
    app/DAL/AttendanceRepo.h
    app/DAL/PaymentRepo.h
    app/DAL/NewsRepo.h
    app/DAL/ExamRepo.h
    app/DAL/DoctorRepo.h
    app/DAL/AdminRepo.h
    app/Services/AuthService.h
    app/Services/GradingService.h
    app/Services/AttendanceService.h
    app/Services/PMDService.h
    app/Utils/PasswordHasher.h
    app/Utils/Validators.h
)

# Create executable
# Note: UI is built programmatically, no .ui files needed
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Qt libraries
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(${PROJECT_NAME}
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
    )
else()
    target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Widgets
        Qt5::Sql
    )
endif()

# Note: Database will be created automatically in application data directory

