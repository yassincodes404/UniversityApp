#ifndef COURSE_H
#define COURSE_H

#include <QString>

enum class CourseType {
    Theory,
    Practical,
    Mixed
};

enum class Semester {
    Fall,
    Spring
};

struct Course {
    int id;
    QString code;
    QString name;
    int creditHours;
    int programId;
    int year;
    Semester semester;
    CourseType courseType;
    QString description;
    QString prerequisites;
    
    Course() : id(0), creditHours(3), programId(0), year(1), 
               semester(Semester::Fall), courseType(CourseType::Theory) {}
    
    QString semesterString() const;
    QString courseTypeString() const;
    static Semester semesterFromString(const QString& str);
    static CourseType courseTypeFromString(const QString& str);
};

#endif // COURSE_H

