#include "Validators.h"
#include <QRegularExpression>

bool Validators::isValidEmail(const QString& email)
{
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email).hasMatch();
}

bool Validators::isValidPhone(const QString& phone)
{
    QRegularExpression regex(R"(^01[0-9]{9}$)");
    return regex.match(phone).hasMatch();
}

bool Validators::isValidStudentCode(const QString& code)
{
    return code.length() >= 6 && code.length() <= 32;
}

bool Validators::isValidGrade(double grade)
{
    return grade >= 0.0 && grade <= 100.0;
}

