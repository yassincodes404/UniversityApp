cmake_minimum_required(VERSION 3.16)
project(NCTU_SIS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 or Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql)

# Enable automatic MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/app)

# Source files
set(SOURCES
    app/main.cpp
    app/DAL/DatabaseManager.cpp
    app/DAL/StudentRepo.cpp
    app/DAL/CourseRepo.cpp
    app/DAL/EnrollmentRepo.cpp
    app/Models/Student.cpp
    app/Models/Course.cpp
    app/Models/Enrollment.cpp
    app/Services/AuthService.cpp
    app/Services/GradingService.cpp
    app/Services/AttendanceService.cpp
    app/Utils/PasswordHasher.cpp
    app/Login/LoginController.cpp
    app/AdminWindow/AdminWindow.cpp
    app/StudentWindow/StudentWindow.cpp
    app/DoctorWindow/DoctorWindow.cpp
)

# UI files (if using Qt Designer .ui files, add them here)
# set(UI_FILES
#     app/Login/LoginStudent.ui
#     app/AdminWindow/AdminWindow.ui
# )

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Qt libraries
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Windows specific
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Install rules (optional)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

