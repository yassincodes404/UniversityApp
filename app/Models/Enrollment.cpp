#include "Enrollment.h"

QString Enrollment::statusString() const
{
    switch (status) {
        case EnrollmentStatus::Registered: return "registered";
        case EnrollmentStatus::Dropped: return "dropped";
        case EnrollmentStatus::Pending: return "pending";
        default: return "pending";
    }
}

EnrollmentStatus Enrollment::statusFromString(const QString& str)
{
    if (str == "registered") return EnrollmentStatus::Registered;
    if (str == "dropped") return EnrollmentStatus::Dropped;
    return EnrollmentStatus::Pending;
}

