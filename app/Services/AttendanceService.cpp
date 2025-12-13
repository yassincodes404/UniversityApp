#include "AttendanceService.h"
#include "../DAL/AttendanceRepo.h"
#include "../DAL/GradeRepo.h"
#include "../DAL/EnrollmentRepo.h"
#include <QDebug>
#include <QSet>

double AttendanceService::calculateAttendancePercentage(int studentId, int sectionId)
{
    AttendanceRepo repo;
    return repo.getAttendancePercentage(studentId, sectionId);
}

bool AttendanceService::isBarred(int studentId, int sectionId)
{
    double percentage = calculateAttendancePercentage(studentId, sectionId);
    return percentage < 75.0;
}

void AttendanceService::checkAndBarStudents(int sectionId)
{
    AttendanceRepo attendanceRepo;
    GradeRepo gradeRepo;
    EnrollmentRepo enrollmentRepo;
    
    // Get all students in this section
    auto attendances = attendanceRepo.findBySection(sectionId);
    
    QSet<int> studentIds;
    for (const auto& att : attendances) {
        studentIds.insert(att.studentId());
    }
    
    // Check each student and bar if needed
    for (int studentId : studentIds) {
        if (isBarred(studentId, sectionId)) {
            // Find enrollment and course for this student
            // This is simplified - you'd need to get course_id from section
            // For now, we'll mark in grades if attendance is below threshold
            qDebug() << "Student" << studentId << "is barred from section" << sectionId;
        }
    }
}

