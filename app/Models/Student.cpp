#include "Student.h"

QString Student::statusString() const
{
    switch (status) {
        case StudentStatus::Active: return "active";
        case StudentStatus::Graduated: return "graduated";
        case StudentStatus::Suspended: return "suspended";
        default: return "active";
    }
}

QString Student::genderString() const
{
    switch (gender) {
        case Gender::Male: return "M";
        case Gender::Female: return "F";
        case Gender::Other: return "Other";
        default: return "Other";
    }
}

StudentStatus Student::statusFromString(const QString& str)
{
    if (str == "active") return StudentStatus::Active;
    if (str == "graduated") return StudentStatus::Graduated;
    if (str == "suspended") return StudentStatus::Suspended;
    return StudentStatus::Active;
}

Gender Student::genderFromString(const QString& str)
{
    if (str == "M" || str == "Male") return Gender::Male;
    if (str == "F" || str == "Female") return Gender::Female;
    return Gender::Other;
}

