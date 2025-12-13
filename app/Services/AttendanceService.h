#ifndef ATTENDANCESERVICE_H
#define ATTENDANCESERVICE_H

#include <QString>

class AttendanceService
{
public:
    static double calculateAttendancePercentage(int studentId, int sectionId);
    static bool isBarred(int studentId, int sectionId);
    static void checkAndBarStudents(int sectionId);
};

#endif // ATTENDANCESERVICE_H

