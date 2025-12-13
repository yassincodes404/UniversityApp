#include "Course.h"

QString Course::semesterString() const
{
    return semester == Semester::Fall ? "Fall" : "Spring";
}

QString Course::courseTypeString() const
{
    switch (courseType) {
        case CourseType::Theory: return "Theory";
        case CourseType::Practical: return "Practical";
        case CourseType::Mixed: return "Mixed";
        default: return "Theory";
    }
}

Semester Course::semesterFromString(const QString& str)
{
    if (str == "Spring") return Semester::Spring;
    return Semester::Fall;
}

CourseType Course::courseTypeFromString(const QString& str)
{
    if (str == "Practical") return CourseType::Practical;
    if (str == "Mixed") return CourseType::Mixed;
    return CourseType::Theory;
}

